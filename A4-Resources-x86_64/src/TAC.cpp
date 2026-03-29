#include "TAC.hpp"

TAC_Operand::TAC_Operand(Type type) : type(type)
{
}

Variable_TAC_Operand::Variable_TAC_Operand(Type type, std::string *name, Scope *declaring_scope)
	: TAC_Operand(type), name(name), declaring_scope(declaring_scope)
{
}

std::string Variable_TAC_Operand::to_string() const
{
	return *name + "_";
}

Int_Const_TAC_Operand::Int_Const_TAC_Operand(int num)
	: TAC_Operand(Type::INT), ival(num) {}
std::string Int_Const_TAC_Operand::to_string() const
{
	return std::to_string(ival);
}

Float_Const_TAC_Operand::Float_Const_TAC_Operand(float num)
	: TAC_Operand(Type::FLOAT), fval(num) {}
std::string Float_Const_TAC_Operand::to_string() const
{
	std::ostringstream out;
	out << std::fixed << std::setprecision(2) << fval;
	return out.str();
}

String_Const_TAC_operand::String_Const_TAC_operand(char *_sval)
	: TAC_Operand(Type::STR), sval(_sval) {}
std::string String_Const_TAC_operand::to_string() const
{
	return sval;
}

int Temporary_TAC_Operand::tac_temp_count = 0;

Temporary_TAC_Operand::Temporary_TAC_Operand(Type type)
	: TAC_Operand(type), temp_number(tac_temp_count++)
{
}

std::string Temporary_TAC_Operand::to_string() const
{
	return "temp" + std::to_string(temp_number);
}

int Shared_Temporary_TAC_Operand::tac_stemp_count = 0;

Shared_Temporary_TAC_Operand::Shared_Temporary_TAC_Operand(Type type)
	: TAC_Operand(type), stemp_number(tac_stemp_count++)
{
}

std::string Shared_Temporary_TAC_Operand::to_string() const
{
	return "stemp" + std::to_string(stemp_number);
}

int TAC_Label::tac_label_count = 0;

TAC_Label::TAC_Label() : label_number(tac_label_count++)
{
}

std::string TAC_Label::to_string() const
{
	return "Label" + std::to_string(label_number);
}

Assignment_TAC_Statement::Assignment_TAC_Statement(TAC_Operand *lhs, Binary_Operator op, TAC_Operand *opd1, TAC_Operand *opd2)
	: TAC_Statement(), lhs(lhs), op(binary_to_tac(op)), opd1(opd1), opd2(opd2)
{
}

Assignment_TAC_Statement::Assignment_TAC_Statement(TAC_Operand *lhs, Unary_Operator op, TAC_Operand *opd1)
	: TAC_Statement(), lhs(lhs), op(unary_to_tac(op)), opd1(opd1), opd2(nullptr)
{
}

Assignment_TAC_Statement::Assignment_TAC_Statement(TAC_Operand *lhs, TAC_Operand *opd1)
	: TAC_Statement(), lhs(lhs), op(TAC_Operator::NOP), opd1(opd1), opd2(nullptr)
{
}

std::string Assignment_TAC_Statement::to_string() const
{
	if (!lhs)
	{
		throw_SemanticError("LHS must exist for an Assignment statement");
		return "";
	}

	if (op == TAC_Operator::NOP)
	{
		return lhs->to_string() + " = " + opd1->to_string();
	}
	else
	{
		if (opd2)
		{
			return lhs->to_string() + " = " + opd1->to_string() + " " + op_to_string(op) + " " + opd2->to_string();
		}
		else
		{
			return lhs->to_string() + " = " + op_to_string(op) + " " + opd1->to_string();
		}
	}
}

RTL_Statement::RTL_Statement(bool is_float) : is_float(is_float)
{
}

RTL_Code *Assignment_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	RTL_Code *rtl_code = new RTL_Code();

	if (lhs->type == Type::FLOAT && opd1->type == Type::FLOAT)
	{
		// Operand 1
		RTL_Register *reg_opd1 = reg_tracker->get_register(opd1);

		if (!reg_opd1)
		{
			reg_opd1 = reg_tracker->get_float_register();

			RTL_Statement *load_stmt;
			if (auto o = dynamic_cast<Float_Const_TAC_Operand *>(opd1))
			{
				load_stmt = new Load_Float_RTL_Statement(reg_opd1, o->fval);
			}
			else
			{
				load_stmt = new Load_RTL_Statement(reg_opd1, opd1, true);
			}

			rtl_code->append_statement(load_stmt);
		}

		// LHS
		RTL_Register *reg_lhs = nullptr;
		if (op != TAC_Operator::NOP)
		{
			reg_lhs = reg_tracker->get_float_register();
			reg_tracker->mark(lhs, reg_lhs);
		}

		// Operand 2 (may be nullptr)
		RTL_Register *reg_opd2 = reg_tracker->get_register(opd2);

		if (opd2 && !reg_opd2)
		{
			reg_opd2 = reg_tracker->get_float_register();

			RTL_Statement *load_stmt;
			if (auto o = dynamic_cast<Float_Const_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_Float_RTL_Statement(reg_opd2, o->fval);
			}
			else
			{
				load_stmt = new Load_RTL_Statement(reg_opd2, opd2, true);
			}

			rtl_code->append_statement(load_stmt);
		}

		if (op != TAC_Operator::NOP)
		{
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, tac_to_rtl(op), reg_opd1, reg_opd2, true);
			rtl_code->append_statement(compute_stmt);
		}

		// Store the lhs if it is a variable or a shared temporary
		if (dynamic_cast<Variable_TAC_Operand *>(lhs) || dynamic_cast<Shared_Temporary_TAC_Operand *>(lhs))
		{
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg_opd1, lhs, true); // changed from your code
			rtl_code->append_statement(store_stmt);
		}

		// Cleanup
		reg_tracker->free_register(opd1, reg_opd1);
		reg_tracker->free_register(opd2, reg_opd2);

		return rtl_code;
	}
	else if (lhs->type == Type::BOOL && opd1->type == Type::FLOAT)
	{
		// Operand 1
		RTL_Register *reg_opd1 = reg_tracker->get_register(opd1);

		if (!reg_opd1)
		{
			reg_opd1 = reg_tracker->get_float_register();

			RTL_Statement *load_stmt;
			if (auto o = dynamic_cast<Float_Const_TAC_Operand *>(opd1))
			{
				load_stmt = new Load_Float_RTL_Statement(reg_opd1, o->fval);
			}
			else
			{
				load_stmt = new Load_RTL_Statement(reg_opd1, opd1, true);
			}

			rtl_code->append_statement(load_stmt);
		}

		// LHS
		RTL_Register *reg_lhs = nullptr;
		// if (op != TAC_Operator::NOP)
		// {
		// 	reg_lhs = reg_tracker->get_int_register();
		// 	reg_tracker->mark(lhs, reg_lhs);
		// }

		// Operand 2 (may be nullptr)
		RTL_Register *reg_opd2 = reg_tracker->get_register(opd2);

		if (opd2 && !reg_opd2)
		{
			reg_opd2 = reg_tracker->get_float_register();

			RTL_Statement *load_stmt;
			if (auto o = dynamic_cast<Float_Const_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_Float_RTL_Statement(reg_opd2, o->fval);
			}
			else
			{
				load_stmt = new Load_RTL_Statement(reg_opd2, opd2, true);
			}

			rtl_code->append_statement(load_stmt);
		}

		if (op == TAC_Operator::LE || op == TAC_Operator::LT || op == TAC_Operator::EQ)
		{
			// reg for iload
			RTL_Register* iload_reg;
			iload_reg = reg_tracker->get_int_register();
			reg_lhs = reg_tracker->get_int_register();
			reg_tracker->mark(lhs,reg_lhs);
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, tac_to_rtl(op), reg_opd1, reg_opd2, true);
			Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(iload_reg,1);
			Move_RTL_Statement* move_stmt = new Move_RTL_Statement(reg_lhs,nullptr,false,false,false);
			Move_RTL_Statement* movt_stmt = new Move_RTL_Statement(reg_lhs,iload_reg,true,true,false);
			rtl_code->append_statement(compute_stmt);
			rtl_code->append_statement(iload_stmt);
			rtl_code->append_statement(move_stmt);
			rtl_code->append_statement(movt_stmt);
			reg_tracker->free_register(nullptr,iload_reg);
		}
		else if(op == TAC_Operator::GE || op == TAC_Operator::GT || op == TAC_Operator::NE){
			RTL_Register* iload_reg;
			iload_reg = reg_tracker->get_int_register();
			reg_lhs = reg_tracker->get_int_register();
			reg_tracker->mark(lhs,reg_lhs);
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, invert_op(tac_to_rtl(op)), reg_opd1, reg_opd2, true);
			Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(iload_reg,1);
			Move_RTL_Statement* move_stmt = new Move_RTL_Statement(reg_lhs,nullptr,false,false,false);
			Move_RTL_Statement* movf_stmt = new Move_RTL_Statement(reg_lhs,iload_reg,true,false,false);
			rtl_code->append_statement(compute_stmt);
			rtl_code->append_statement(iload_stmt);
			rtl_code->append_statement(move_stmt);
			rtl_code->append_statement(movf_stmt);
			reg_tracker->free_register(nullptr,iload_reg);
		}
		// I think the below code is never encountered
		// Store the lhs if it is a variable or a shared temporary
		if (dynamic_cast<Variable_TAC_Operand *>(lhs) || dynamic_cast<Shared_Temporary_TAC_Operand *>(lhs))
		{
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg_opd1, lhs); // changed from your code
			rtl_code->append_statement(store_stmt);
		}

		// Cleanup
		reg_tracker->free_register(opd1, reg_opd1);
		reg_tracker->free_register(opd2, reg_opd2);

		return rtl_code;
	}
	else
	{
		// Operand 1
		RTL_Register *reg_opd1 = reg_tracker->get_register(opd1);

		if (!reg_opd1)
		{
			reg_opd1 = reg_tracker->get_int_register();

			RTL_Statement *load_stmt;
			if (auto o = dynamic_cast<Int_Const_TAC_Operand *>(opd1))
			{
				load_stmt = new Load_Int_RTL_Statement(reg_opd1, o->ival);
			}
			else
			{
				load_stmt = new Load_RTL_Statement(reg_opd1, opd1);
			}

			rtl_code->append_statement(load_stmt);
		}

		// LHS
		RTL_Register *reg_lhs = nullptr;
		if (op != TAC_Operator::NOP)
		{
			reg_lhs = reg_tracker->get_int_register();
			reg_tracker->mark(lhs, reg_lhs);
		}

		// Operand 2 (may be nullptr)
		RTL_Register *reg_opd2 = reg_tracker->get_register(opd2);

		if (opd2 && !reg_opd2)
		{
			reg_opd2 = reg_tracker->get_int_register();

			RTL_Statement *load_stmt;
			if (auto o = dynamic_cast<Int_Const_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_Int_RTL_Statement(reg_opd2, o->ival);
			}
			else
			{
				load_stmt = new Load_RTL_Statement(reg_opd2, opd2);
			}

			rtl_code->append_statement(load_stmt);
		}

		if (op != TAC_Operator::NOP)
		{
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, tac_to_rtl(op), reg_opd1, reg_opd2);
			rtl_code->append_statement(compute_stmt);
		}

		// Store the lhs if it is a variable or a shared temporary
		if (dynamic_cast<Variable_TAC_Operand *>(lhs) || dynamic_cast<Shared_Temporary_TAC_Operand *>(lhs))
		{
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg_opd1, lhs); // changed from your code
			rtl_code->append_statement(store_stmt);
		}

		// Cleanup
		reg_tracker->free_register(opd1, reg_opd1);
		reg_tracker->free_register(opd2, reg_opd2);

		return rtl_code;
	}
}

Goto_TAC_Statement::Goto_TAC_Statement(TAC_Label *_label) : label(_label) {}
std::string Goto_TAC_Statement::to_string() const
{
	return "goto " + label->to_string();
}

RTL_Code *Goto_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	RTL_Code *rtl_code = new RTL_Code();

	Goto_RTL_Statement *goto_stmt = new Goto_RTL_Statement(label->label_number);
	rtl_code->append_statement(goto_stmt);

	return rtl_code;
}

If_Goto_TAC_Statement::If_Goto_TAC_Statement(TAC_Operand *_cond, TAC_Label *_label)
	: condition(_cond), label(_label)
{
}

RTL_Code *If_Goto_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	RTL_Code *rtl_code = new RTL_Code();

	RTL_Register *reg_condition = reg_tracker->get_register(condition);
	if (!reg_condition && (dynamic_cast<Variable_TAC_Operand *>(condition) || dynamic_cast<Shared_Temporary_TAC_Operand *>(condition)))
	{
		// This can happen for do-while
		reg_condition = reg_tracker->get_int_register();
		RTL_Statement *load_stmt = new Load_RTL_Statement(reg_condition, condition);
		rtl_code->append_statement(load_stmt);
	}

	If_Goto_RTL_Statement *if_goto_stmt = new If_Goto_RTL_Statement(reg_condition, label->label_number);
	rtl_code->append_statement(if_goto_stmt);

	// Cleanup
	reg_tracker->free_register(condition, reg_condition);

	return rtl_code;
}

std::string If_Goto_TAC_Statement::to_string() const
{
	return "if(" + condition->to_string() + ") goto " + label->to_string();
}

IO_TAC_Statement::IO_TAC_Statement(IO_Kind _kind, TAC_Operand *_opd)
	: kind(_kind), opd(_opd)
{
}

std::string IO_TAC_Statement::to_string() const
{
	if (kind == IO_Kind::READ)
	{
		return "read " + opd->to_string();
	}
	else
	{
		return "write " + opd->to_string();
	}
}

RTL_Code *IO_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	bool is_int = opd->type == Type::INT;
	bool is_float = opd->type == Type::FLOAT;
	bool is_str = opd->type == Type::STR;

	if (this->kind == IO_Kind::READ)
	{
		if (!is_int && !is_float)
		{
			throw_SemanticError("Expected to read either an int or a float");
		}

		RTL_Code *rtl_code = new RTL_Code();

		RTL_Register *reg = reg_tracker->get_register(RegisterTracker::PRIORITY_V0);
		int signal = is_float ? 7 : 5;
		Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg, signal);
		rtl_code->append_statement(iload_stmt);
		reg_tracker->free_register(nullptr, reg); // cleanup

		Read_RTL_Statement *read_stmt = new Read_RTL_Statement(is_float);
		rtl_code->append_statement(read_stmt);

		if (is_int)
		{
			reg = reg_tracker->get_register(RegisterTracker::PRIORITY_V0);
		}
		else
		{
			reg = reg_tracker->get_register(RegisterTracker::PRIORITY_F0);
		}

		RTL_Statement *store_stmt = new Store_RTL_Statement(reg, opd, is_float);
		rtl_code->append_statement(store_stmt);
		reg_tracker->free_register(nullptr, reg); // cleanup

		return rtl_code;
	}
	else if (kind == IO_Kind::WRITE)
	{
		if (!is_int && !is_float && !is_str)
		{
			throw_SemanticError("Expected to read either an int, a float or a string");
		}

		RTL_Code *rtl_code = new RTL_Code();

		RTL_Register *reg1 = reg_tracker->get_register(RegisterTracker::PRIORITY_V0);
		int signal = is_int ? 1 : (is_float ? 3 : 4);
		Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg1, signal);
		rtl_code->append_statement(iload_stmt);

		RTL_Register *reg2 = reg_tracker->get_register(is_float ? RegisterTracker::PRIORITY_F12 : RegisterTracker::PRIORITY_A0);
		// TODO: Could be either a move (if printing an expression) or a load (if printing a variable)
		if(dynamic_cast<Temporary_TAC_Operand* >(opd)){
			RTL_Register* reg_opd = reg_tracker->get_register(opd);
			if(!reg_opd){
				throw_SemanticError("This should not happen, temp must have a register before move");
			}
			Move_RTL_Statement *move_stmt = new Move_RTL_Statement(reg2,reg_opd,false,false,is_float);
			rtl_code->append_statement(move_stmt);
			reg_tracker->free_register(opd,reg_opd);
		}
		else{
			Load_RTL_Statement *load_stmt = new Load_RTL_Statement(reg2, opd, is_float);
			rtl_code->append_statement(load_stmt);
		}
		Write_RTL_Statement *write_stmt = new Write_RTL_Statement(is_float);
		rtl_code->append_statement(write_stmt);

		// Cleanup
		reg_tracker->free_register(nullptr, reg1);
		reg_tracker->free_register(nullptr, reg2);

		return rtl_code;
	}
	else
	{
		throw_SemanticError("Expected either a READ or a WRITE operation");
		return nullptr;
	}
}

Label_TAC_Statement::Label_TAC_Statement(TAC_Label *_label) : label(_label) {}
std::string Label_TAC_Statement::to_string() const
{
	return label->to_string() + ": ";
}

RTL_Code *Label_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	RTL_Code *rtl_code = new RTL_Code();

	Label_RTL_Statement *label_stmt = new Label_RTL_Statement(label->label_number);
	rtl_code->append_statement(label_stmt);

	return rtl_code;
}

Code::Code()
	: stmt_list(new std::list<TAC_Statement *>)
{
}

void Code::append_statement(TAC_Statement *s)
{
	if (s)
	{
		stmt_list->push_back(s);
	}
}

TAC_Statement *Code::pop_statement()
{
	if (!stmt_list || stmt_list->empty())
	{
		return nullptr;
	}

	TAC_Statement *stmt = stmt_list->back();
	stmt_list->pop_back();
	return stmt;
}

void Code::append_list(Code *c)
{
	if (c && c->stmt_list)
	{
		for (auto it = c->stmt_list->begin(); it != c->stmt_list->end(); ++it)
		{
			append_statement(*it);
		}
	}
}

std::string Code::to_string() const
{
	std::string result;
	for (auto it = stmt_list->begin(); it != stmt_list->end(); ++it)
	{
		if (*it)
		{
			result += (*it)->to_string() + "\n";
		}
	}
	return result;
}

RTL_Code::RTL_Code() : stmt_list(new std::list<RTL_Statement *>)
{
}

void RTL_Code::append_statement(RTL_Statement *rtl_statement)
{
	if (rtl_statement)
	{
		stmt_list->push_back(rtl_statement);
	}
}

void RTL_Code::append_list(RTL_Code *rtl_code)
{
	if (rtl_code && rtl_code->stmt_list)
	{
		for (auto it = rtl_code->stmt_list->begin(); it != rtl_code->stmt_list->end(); ++it)
		{
			append_statement(*it);
		}
	}
}

std::string Read_RTL_Statement::to_string() const{
	return "read";
}

std::string Write_RTL_Statement::to_string() const{
	return "write";
}

Load_Int_RTL_Statement::Load_Int_RTL_Statement(RTL_Register *reg, int ival)
	: RTL_Statement(false), reg(reg), ival(ival)
{
}

std::string Load_Int_RTL_Statement::to_string() const{
	std::string result;
	result = "iload:\t" + rtl_priority_to_register(reg->priority) + " <- " + std::to_string(ival);
	return result;
}

Load_Float_RTL_Statement::Load_Float_RTL_Statement(RTL_Register *reg, float fval)
	: RTL_Statement(true), reg(reg), fval(fval)
{
}

std::string Load_Float_RTL_Statement::to_string() const{
	std::string result;
	result = "iload.d:\t" + rtl_priority_to_register(reg->priority) + " <- ";
	std::ostringstream out;
	out << std::fixed << std::setprecision(2) << fval;
	result += out.str();
	return result;
}

Load_RTL_Statement::Load_RTL_Statement(RTL_Register *reg, TAC_Operand *var, bool is_float)
	: RTL_Statement(is_float), reg(reg), var(var)
{
	if (!dynamic_cast<Variable_TAC_Operand *>(var) && !dynamic_cast<Shared_Temporary_TAC_Operand *>(var))
	{
		throw_SemanticError("Expected to load either a variable or a shared temporary variable");
	}
}

std::string Load_RTL_Statement::to_string() const{
	if(!is_float){
		return "load:\t" + rtl_priority_to_register(reg->priority) + " <- " + var->to_string(); 
	}
	else{
		return "load.d:\t" + rtl_priority_to_register(reg->priority) + " <- " + var->to_string(); 
	}
}

Store_RTL_Statement::Store_RTL_Statement(RTL_Register *reg, TAC_Operand *var, bool is_float)
	: RTL_Statement(is_float), reg(reg), var(var)
{
	if (!dynamic_cast<Variable_TAC_Operand *>(var) && !dynamic_cast<Shared_Temporary_TAC_Operand *>(var))
	{
		throw_SemanticError("Expected to store either a variable or a shared temporary variable");
	}
}

std::string Store_RTL_Statement::to_string() const{
	if(!is_float){
		return "store:\t" + var->to_string() + " <- " + rtl_priority_to_register(reg->priority);
	}
	else{
		return "store.d:\t" + var->to_string() + " <- " + rtl_priority_to_register(reg->priority);
	}
}

Move_RTL_Statement::Move_RTL_Statement(RTL_Register* dest, RTL_Register* src, bool is_movtf, bool is_movt, bool is_float)
	: RTL_Statement(is_float), dest(dest),src(src),is_movtf(is_movtf),is_movt(is_movt)
{
}

std::string Move_RTL_Statement::to_string() const{
	std::string result;
	std::string dest_name = rtl_priority_to_register(dest->priority);
	std::string src_name = (src)? rtl_priority_to_register(src->priority) : "zero"; 
	if(is_movtf){
		if(is_movt){
			result = "movt:\t" + dest_name + " <- " + src_name + " , 0";
		}
		else{
			result = "movf:\t" + dest_name + " <- " + src_name + " , 0";
		}
	}
	else{
		if(!is_float){
			result = "move:\t" + dest_name + " <- " + src_name;
		}
		else{
			result = "move.d:\t" + dest_name + " <- " + src_name;
		}
		
	}
	return result;
}

Compute_RTL_Statement::Compute_RTL_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2 = nullptr, bool is_float)
	: RTL_Statement(is_float), lhs(lhs), op(op), opd1(opd1), opd2(opd2)
{
}

std::string Compute_RTL_Statement::to_string() const
{
	std::string result;

	std::string result_name = rtl_priority_to_register(lhs->priority);
	std::string opd1_name = rtl_priority_to_register(opd1->priority);
	std::string opd2_name = (opd2) ? rtl_priority_to_register(opd2->priority) : "";
	std::string op_name = (is_float)? op_float_to_string(op) : op_to_string(op);

	if (op == RTL_Operator::NEGATE || op == RTL_Operator::LOGICAL_NOT){
		result = op_name + ":\t" + result_name + " <- " + opd1_name;
	}
	else{
		result = op_name + ":\t" + result_name + " <- " + opd1_name + " , " + opd2_name;
	}

	return result;
}

Goto_RTL_Statement::Goto_RTL_Statement(int label_number)
	: RTL_Statement(false), label_number(label_number)
{
}

std::string Goto_RTL_Statement::to_string() const
{
	std::string result = "goto:\tLabel" + std::to_string(label_number);
	return result;
}

If_Goto_RTL_Statement::If_Goto_RTL_Statement(RTL_Register *predicate, int label_number)
	: RTL_Statement(false), predicate(predicate), label_number(label_number)
{
}

std::string If_Goto_RTL_Statement::to_string() const
{
	std::string result = "bgtz:\t" + rtl_priority_to_register(predicate->priority) + " , " + "Label" + std::to_string(label_number);
	return result;
}

Read_RTL_Statement::Read_RTL_Statement(bool is_float)
	: RTL_Statement(is_float)
{
}

std::string Read_RTL_Statement::to_string() const
{
	// TODO
	return "";
}

Write_RTL_Statement::Write_RTL_Statement(bool is_float)
	: RTL_Statement(is_float)
{
}

std::string Write_RTL_Statement::to_string() const
{
	// TODO
	return "";
}

Label_RTL_Statement::Label_RTL_Statement(int label_number)
	: RTL_Statement(false), label_number(label_number)
{
}
std::string Label_RTL_Statement::to_string() const
{
	std::string result = "Label" + std::to_string(label_number) + ":";
	return result;
}

RegisterTracker::RegisterTracker()
	: all_regs(), reg_map(), available_int_regs(), available_float_regs()
{
	// v0 to s7
	for (int i = 1; i <= 19; i++)
	{
		RTL_Register *reg = new RTL_Register(i);
		all_regs[i] = reg;
		available_int_regs[i] = true;
	}

	// f2 to f30
	for (int i = 21; i <= 35; i++)
	{
		RTL_Register *reg = new RTL_Register(i);
		all_regs[i] = reg;
		available_float_regs[i] = true;
	}

	// a0
	RTL_Register *reg = new RTL_Register(PRIORITY_A0);
	all_regs[PRIORITY_A0] = reg;

	// v1
	reg = new RTL_Register(PRIORITY_V1);
	all_regs[PRIORITY_V1] = reg;

	// f0
	reg = new RTL_Register(PRIORITY_F0);
	all_regs[PRIORITY_F0] = reg;

	// The reserved float register is f12
	// TODO: Write a testcase which requires the use of f12 in something else, then use it for printing a float
}

RTL_Register *RegisterTracker::get_register(TAC_Operand *opd)
{
	if (!opd || reg_map.find(opd) == reg_map.end() || all_regs.find(reg_map[opd]) == all_regs.end())
	{
		return nullptr;
	}

	return all_regs[reg_map[opd]];
}

RTL_Register *RegisterTracker::get_register(int priority)
{
	if (all_regs.find(priority) != all_regs.end())
	{
		if (available_int_regs.find(priority) != available_int_regs.end() && available_int_regs[priority])
		{
			return all_regs[priority];
		}
		if (available_float_regs.find(priority) != available_float_regs.end() && available_float_regs[priority])
		{
			return all_regs[priority];
		}
	}
	throw_SemanticError("Expected to find unused register with priority " + std::to_string(priority));

	return all_regs[priority];
}

RTL_Register *RegisterTracker::get_int_register()
{
	for (auto it = available_int_regs.begin(); it != available_int_regs.end(); ++it)
	{
		if (it->second)
		{
			it->second = false;
			return all_regs[it->first];
		}
	}

	throw_SemanticError("Out of int registers!!!");
	return nullptr;
}

RTL_Register *RegisterTracker::get_float_register()
{
	for (auto it = available_float_regs.begin(); it != available_float_regs.end(); ++it)
	{
		if (it->second)
		{
			it->second = false;
			return all_regs[it->first];
		}
	}

	throw_SemanticError("Out of float registers!!!");
	return nullptr;
}

void RegisterTracker::mark(TAC_Operand *opd, RTL_Register *reg)
{
	if (opd)
	{
		reg_map[opd] = reg->priority;
	}
}

void RegisterTracker::free_register(TAC_Operand *opd, RTL_Register *reg)
{
	reg_map.erase(opd);

	if (reg)
	{
		int priority = reg->priority;

		if (available_int_regs.find(priority) != available_int_regs.end())
		{
			available_int_regs[priority] = true;
		}
		if (available_float_regs.find(priority) != available_float_regs.end())
		{
			available_float_regs[priority] = true;
		}
	}
}

RTL_Register::RTL_Register(int priority) : priority(priority)
{
}

Scope::Scope(Scope_Kind kind, Scope *parent_scope, Func_Signature *func_sig)
	: kind(kind), parent_scope(parent_scope), func_sig(func_sig), reg_tracker(new RegisterTracker())
{
}

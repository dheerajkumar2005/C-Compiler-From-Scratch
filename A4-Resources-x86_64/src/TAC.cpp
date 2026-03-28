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
			reg_tracker->reg_map[lhs] = reg_lhs;
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
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, tac_to_rtl(op), reg_opd1, reg_opd2);
			rtl_code->append_statement(compute_stmt);
		}

		// Store the lhs if it is a variable or a shared temporary
		if (dynamic_cast<Variable_TAC_Operand *>(lhs) || dynamic_cast<Shared_Temporary_TAC_Operand *>(lhs))
		{
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg_lhs, lhs, true);
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
		if (op != TAC_Operator::NOP)
		{
			reg_lhs = reg_tracker->get_int_register();
			reg_tracker->reg_map[lhs] = reg_lhs;
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
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, tac_to_rtl(op), reg_opd1, reg_opd2);
			rtl_code->append_statement(compute_stmt);
		}

		// Store the lhs if it is a variable or a shared temporary
		if (dynamic_cast<Variable_TAC_Operand *>(lhs) || dynamic_cast<Shared_Temporary_TAC_Operand *>(lhs))
		{
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg_lhs, lhs);
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
			reg_tracker->reg_map[lhs] = reg_lhs;
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
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg_lhs, lhs);
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

		RTL_Register *reg = reg_tracker->get_int_register();
		if (reg->priority != 1) // v0
		{
			throw_SemanticError("Expected v0 to be free rn");
		}
		int signal = is_float ? 7 : 5;
		Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg, signal);
		rtl_code->append_statement(iload_stmt);
		reg_tracker->free_register(nullptr, reg); // cleanup

		Read_RTL_Statement *read_stmt = new Read_RTL_Statement();
		rtl_code->append_statement(read_stmt);

		if (is_int)
		{
			// read from v0 and store
		}
		else
		{
			// read from f0 and store
		}

		// Store_RTL_Statement *store_stmt = new Store_RTL_Statement(reg, opd, is_float);
		// rtl_code->append_statement(store_stmt);

		return rtl_code;
	}
	else if (kind == IO_Kind::WRITE)
	{
		if (!is_int && !is_float && !is_str)
		{
			throw_SemanticError("Expected to read either an int, a float or a string");
		}

		RTL_Code *rtl_code = new RTL_Code();

		RTL_Register *reg = reg_tracker->get_int_register();
		if (reg->priority != 1) // v0
		{
			throw_SemanticError("Expected v0 to be free rn");
		}
		int signal = is_int ? 1 : (is_float ? 3 : 4);
		Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg, signal);
		rtl_code->append_statement(iload_stmt);

		reg = is_float ? reg_tracker->get_float_reserved_register() : reg_tracker->get_int_reserved_register();
		if (reg->priority != 100) // a0
		{
			throw_SemanticError("Expected a0(f12) to be free rn");
		}
		Load_RTL_Statement *load_stmt = new Load_RTL_Statement(reg, opd, is_float);
		rtl_code->append_statement(load_stmt);

		Write_RTL_Statement *write_stmt = new Write_RTL_Statement();
		rtl_code->append_statement(write_stmt);

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

Load_Int_RTL_Statement::Load_Int_RTL_Statement(RTL_Register *reg, int ival) : reg(reg), ival(ival)
{
}

Load_Float_RTL_Statement::Load_Float_RTL_Statement(RTL_Register *reg, float fval) : reg(reg), fval(fval)
{
}

Load_RTL_Statement::Load_RTL_Statement(RTL_Register *reg, TAC_Operand *var, bool is_float) : reg(reg), var(var), is_float(is_float)
{
	if (!dynamic_cast<Variable_TAC_Operand *>(var) && !dynamic_cast<Shared_Temporary_TAC_Operand *>(var))
	{
		throw_SemanticError("Expected to load either a variable or a shared temporary variable");
	}
}

Store_RTL_Statement::Store_RTL_Statement(RTL_Register *reg, TAC_Operand *var, bool is_float)
	: reg(reg), var(var), is_float(is_float)
{
	if (!dynamic_cast<Variable_TAC_Operand *>(var) && !dynamic_cast<Shared_Temporary_TAC_Operand *>(var))
	{
		throw_SemanticError("Expected to store either a variable or a shared temporary variable");
	}
}

Compute_RTL_Statement::Compute_RTL_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2)
	: lhs(lhs), op(op), opd1(opd1), opd2(opd2)
{
}

Goto_RTL_Statement::Goto_RTL_Statement(int label_number) : label_number(label_number)
{
}

std::string Goto_RTL_Statement::to_string() const{
	std::string result = "goto:\tLabel" + std::to_string(label_number);
	return result;
}

If_Goto_RTL_Statement::If_Goto_RTL_Statement(RTL_Register *predicate, int label_number)
	: predicate(predicate), label_number(label_number)
{
}

std::string If_Goto_RTL_Statement::to_string() const{
	std::string result = "bgtz:\t" + rtl_priority_to_register(predicate->priority) + " , " + "Label" + std::to_string(label_number);
	return result;
}

Label_RTL_Statement::Label_RTL_Statement(int label_number) : label_number(label_number)
{
}

RegisterTracker::RegisterTracker()
	: reg_map(), available_int_regs(), available_float_regs(), reserved_int_regs(), reserved_float_regs()
{
	available_int_regs[new RTL_Register(1)] = true;	 // v0
	available_int_regs[new RTL_Register(2)] = true;	 // t0
	available_int_regs[new RTL_Register(3)] = true;	 // t1
	available_int_regs[new RTL_Register(4)] = true;	 // t2
	available_int_regs[new RTL_Register(5)] = true;	 // t3
	available_int_regs[new RTL_Register(6)] = true;	 // t4
	available_int_regs[new RTL_Register(7)] = true;	 // t5
	available_int_regs[new RTL_Register(8)] = true;	 // t6
	available_int_regs[new RTL_Register(9)] = true;	 // t7
	available_int_regs[new RTL_Register(10)] = true; // t8
	available_int_regs[new RTL_Register(11)] = true; // t9
	available_int_regs[new RTL_Register(12)] = true; // s0
	available_int_regs[new RTL_Register(13)] = true; // s1
	available_int_regs[new RTL_Register(14)] = true; // s2
	available_int_regs[new RTL_Register(15)] = true; // s3
	available_int_regs[new RTL_Register(16)] = true; // s4
	available_int_regs[new RTL_Register(17)] = true; // s5
	available_int_regs[new RTL_Register(18)] = true; // s6
	available_int_regs[new RTL_Register(19)] = true; // s7

	reserved_int_regs[new RTL_Register(100)] = true; // a0

	available_float_regs[new RTL_Register(21)] = true; // f2
	available_float_regs[new RTL_Register(22)] = true; // f4
	available_float_regs[new RTL_Register(23)] = true; // f6
	available_float_regs[new RTL_Register(24)] = true; // f8
	available_float_regs[new RTL_Register(25)] = true; // f10
	available_float_regs[new RTL_Register(26)] = true;  // f12
	available_float_regs[new RTL_Register(27)] = true;  // f14
	available_float_regs[new RTL_Register(28)] = true;  // f16
	available_float_regs[new RTL_Register(29)] = true;  // f18
	available_float_regs[new RTL_Register(30)] = true; // f20
	available_float_regs[new RTL_Register(31)] = true; // f22
	available_float_regs[new RTL_Register(32)] = true; // f24
	available_float_regs[new RTL_Register(33)] = true; // f26
	available_float_regs[new RTL_Register(34)] = true; // f28
	available_float_regs[new RTL_Register(35)] = true; // f30

	// The reserved float register is f12
	// TODO: Write a testcase which requires the use of f12 in something else, then use it for printing a float
	reserved_float_regs[new RTL_Register(26)] = true; // f12
}

RTL_Register *RegisterTracker::get_register(TAC_Operand *opd)
{
	if (!opd || reg_map.find(opd) == reg_map.end() || !reg_map[opd])
	{
		return nullptr;
	}

	return reg_map[opd];
}

RTL_Register *RegisterTracker::get_int_register()
{
	RTL_Register *chosen_reg_ptr = nullptr;
	for (auto it = available_int_regs.begin(); it != available_int_regs.end(); ++it)
	{
		if (it->second)
		{
			chosen_reg_ptr = it->first;
		}
	}

	if (chosen_reg_ptr)
	{
		available_int_regs[chosen_reg_ptr] = false;
		return chosen_reg_ptr;
	}

	throw_SemanticError("Out of int registers!!!");
	return nullptr;
}

RTL_Register *RegisterTracker::get_int_reserved_register()
{
	RTL_Register *chosen_reg_ptr = nullptr;
	for (auto it = reserved_int_regs.begin(); it != reserved_int_regs.end(); ++it)
	{
		if (it->second)
		{
			chosen_reg_ptr = it->first;
		}
	}

	if (chosen_reg_ptr)
	{
		reserved_int_regs[chosen_reg_ptr] = false;
		return chosen_reg_ptr;
	}

	throw_SemanticError("Out of reserved int registers!");
	return nullptr;
}

RTL_Register *RegisterTracker::get_float_register()
{
	RTL_Register *chosen_reg_ptr = nullptr;
	for (auto it = available_float_regs.begin(); it != available_float_regs.end(); ++it)
	{
		if (it->second)
		{
			chosen_reg_ptr = it->first;
		}
	}

	if (chosen_reg_ptr)
	{
		available_int_regs[chosen_reg_ptr] = false;
		return chosen_reg_ptr;
	}

	throw_SemanticError("Out of float registers!!!");
	return nullptr;
}

RTL_Register *RegisterTracker::get_float_reserved_register()
{
	RTL_Register *chosen_reg_ptr = nullptr;
	for (auto it = reserved_float_regs.begin(); it != reserved_float_regs.end(); ++it)
	{
		if (it->second)
		{
			chosen_reg_ptr = it->first;
		}
	}

	if (chosen_reg_ptr)
	{
		reserved_float_regs[chosen_reg_ptr] = false;
		return chosen_reg_ptr;
	}

	throw_SemanticError("Out of reserved float registers!");
	return nullptr;
}

void RegisterTracker::free_register(TAC_Operand *opd, RTL_Register *reg)
{
	if (opd && reg_map.find(opd) != reg_map.end())
	{
		reg_map[opd] = nullptr;
	}
	if (reg && available_int_regs.find(reg) != available_int_regs.end())
	{
		available_int_regs[reg] = true;
	}
}

RTL_Register::RTL_Register(int priority) : priority(priority)
{
}

Scope::Scope(Scope_Kind kind, Scope *parent_scope, Func_Signature *func_sig)
	: kind(kind), parent_scope(parent_scope), func_sig(func_sig), reg_tracker(new RegisterTracker())
{
}

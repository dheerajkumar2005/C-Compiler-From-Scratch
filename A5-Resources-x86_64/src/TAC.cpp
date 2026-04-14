#include "TAC.hpp"

TAC_Statement::TAC_Statement(Scope *eval_scope)
	: eval_scope(eval_scope)
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

int String_Const_TAC_Operand::string_count = 0;

std::unordered_map<std::string, int> String_Const_TAC_Operand::s_map;

String_Const_TAC_Operand::String_Const_TAC_Operand(char *_sval)
	: TAC_Operand(Type::STR), sval(_sval)
{
	if (s_map.find(sval) == s_map.end())
	{
		string_label = string_count++;
		s_map[sval] = string_label;
	}
	else
	{
		string_label = s_map[sval];
	}
}
std::string String_Const_TAC_Operand::to_string() const
{
	return sval;
}

int Temporary_TAC_Operand::tac_temp_count = 0;

void Temporary_TAC_Operand::reset_temp_count(int cnt)
{
	tac_temp_count = cnt;
}

Temporary_TAC_Operand::Temporary_TAC_Operand(Type type)
	: TAC_Operand(type), temp_number(tac_temp_count++)
{
}

std::string Temporary_TAC_Operand::to_string() const
{
	return "temp" + std::to_string(temp_number);
}

int Shared_Temporary_TAC_Operand::tac_stemp_count = 0;

void Shared_Temporary_TAC_Operand::reset_stemp_count(int cnt)
{
	tac_stemp_count = cnt;
}

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

Assignment_TAC_Statement::Assignment_TAC_Statement(Scope *_eval_scope, TAC_Operand *lhs, Binary_Operator op, TAC_Operand *opd1, TAC_Operand *opd2)
	: TAC_Statement(_eval_scope), lhs(lhs), op(binary_to_tac(op)), opd1(opd1), opd2(opd2)
{
}

Assignment_TAC_Statement::Assignment_TAC_Statement(Scope *_eval_scope, TAC_Operand *lhs, Unary_Operator op, TAC_Operand *opd1)
	: TAC_Statement(_eval_scope), lhs(lhs), op(unary_to_tac(op)), opd1(opd1), opd2(nullptr)
{
}

Assignment_TAC_Statement::Assignment_TAC_Statement(Scope *_eval_scope, TAC_Operand *lhs, TAC_Operand *opd1)
	: TAC_Statement(_eval_scope), lhs(lhs), op(TAC_Operator::NOP), opd1(opd1), opd2(nullptr)
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

	// Register transfer after a call

	// Arithmetic operation over floats
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
			else if (dynamic_cast<Variable_TAC_Operand *>(opd1) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd1))
			{
				load_stmt = new Load_RTL_Statement(eval_scope, reg_opd1, opd1->to_string(), true);
			}
			else
			{
				// throw_SemanticError("Load operation failed");
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
			else if (dynamic_cast<Variable_TAC_Operand *>(opd2) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_RTL_Statement(eval_scope, reg_opd2, opd2->to_string(), true);
			}
			else
			{
				throw_SemanticError("Load operation failed");
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
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(eval_scope, reg_opd1, lhs->to_string(), true); // changed from your code
			rtl_code->append_statement(store_stmt);
		}

		// Cleanup
		reg_tracker->free_register(opd1, reg_opd1);
		reg_tracker->free_register(opd2, reg_opd2);

		return rtl_code;
	}
	// Relational operation over floats
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
			else if (dynamic_cast<Variable_TAC_Operand *>(opd1) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd1))
			{
				load_stmt = new Load_RTL_Statement(eval_scope, reg_opd1, opd1->to_string(), true);
			}
			else
			{
				throw_SemanticError("Load operation failed");
			}

			rtl_code->append_statement(load_stmt);
		}

		// LHS
		RTL_Register *reg_lhs = nullptr;

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
			else if (dynamic_cast<Variable_TAC_Operand *>(opd2) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_RTL_Statement(eval_scope, reg_opd2, opd2->to_string(), true);
			}
			else
			{
				throw_SemanticError("Load operation failed");
			}

			rtl_code->append_statement(load_stmt);
		}

		if (op == TAC_Operator::LE || op == TAC_Operator::LT || op == TAC_Operator::EQ)
		{
			// reg for int load
			RTL_Register *iload_reg;
			iload_reg = reg_tracker->get_int_register();
			reg_lhs = reg_tracker->get_int_register();
			reg_tracker->mark(lhs, reg_lhs);
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, tac_to_rtl(op), reg_opd1, reg_opd2, true);
			Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(iload_reg, 1);
			Move_RTL_Statement *move_stmt = new Move_RTL_Statement(reg_lhs, nullptr, false, false, false);
			Move_RTL_Statement *movt_stmt = new Move_RTL_Statement(reg_lhs, iload_reg, true, true, false);
			rtl_code->append_statement(compute_stmt);
			rtl_code->append_statement(iload_stmt);
			rtl_code->append_statement(move_stmt);
			rtl_code->append_statement(movt_stmt);
			reg_tracker->free_register(nullptr, iload_reg);
		}
		else if (op == TAC_Operator::GE || op == TAC_Operator::GT || op == TAC_Operator::NE)
		{
			RTL_Register *iload_reg;
			iload_reg = reg_tracker->get_int_register();
			reg_lhs = reg_tracker->get_int_register();
			reg_tracker->mark(lhs, reg_lhs);
			Compute_RTL_Statement *compute_stmt = new Compute_RTL_Statement(reg_lhs, invert_op(tac_to_rtl(op)), reg_opd1, reg_opd2, true);
			Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(iload_reg, 1);
			Move_RTL_Statement *move_stmt = new Move_RTL_Statement(reg_lhs, nullptr, false, false, false);
			Move_RTL_Statement *movf_stmt = new Move_RTL_Statement(reg_lhs, iload_reg, true, false, false);
			rtl_code->append_statement(compute_stmt);
			rtl_code->append_statement(iload_stmt);
			rtl_code->append_statement(move_stmt);
			rtl_code->append_statement(movf_stmt);
			reg_tracker->free_register(nullptr, iload_reg);
		}
		// I think the below code is never encountered
		// Store the lhs if it is a variable or a shared temporary
		if (dynamic_cast<Variable_TAC_Operand *>(lhs) || dynamic_cast<Shared_Temporary_TAC_Operand *>(lhs))
		{
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(eval_scope, reg_opd1, lhs->to_string()); // changed from your code
			rtl_code->append_statement(store_stmt);
		}

		// Cleanup
		reg_tracker->free_register(opd1, reg_opd1);
		reg_tracker->free_register(opd2, reg_opd2);

		return rtl_code;
	}
	// Arithmetic/relational/logical operation over ints/bools
	else
	{
		// Operand 1
		RTL_Register *reg_opd1 = reg_tracker->get_register(opd1);

		if (!reg_opd1)
		{
			reg_opd1 = reg_tracker->get_int_register();

			if (auto o = dynamic_cast<Int_Const_TAC_Operand *>(opd1))
			{
				rtl_code->append_statement(new Load_Int_RTL_Statement(reg_opd1, o->ival));
			}
			else if (auto o = dynamic_cast<String_Const_TAC_Operand *>(opd1))
			{
				rtl_code->append_statement(new Load_String_RTL_Statement(reg_opd1, o->sval, o->string_label));
			}
			else if (dynamic_cast<Variable_TAC_Operand *>(opd1) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd1))
			{
				rtl_code->append_statement(new Load_RTL_Statement(eval_scope, reg_opd1, opd1->to_string()));
			}
			else
			{
				// throw_SemanticError("Load operation failed");
			}
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
			else if (auto o = dynamic_cast<String_Const_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_String_RTL_Statement(reg_opd1, o->sval, o->string_label);
			}
			else if (dynamic_cast<Variable_TAC_Operand *>(opd2) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd2))
			{
				load_stmt = new Load_RTL_Statement(eval_scope, reg_opd2, opd2->to_string());
			}
			else
			{
				throw_SemanticError("Load operation failed");
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
			Store_RTL_Statement *store_stmt = new Store_RTL_Statement(eval_scope, reg_opd1, lhs->to_string()); // changed from your code
			rtl_code->append_statement(store_stmt);
		}

		// Cleanup
		reg_tracker->free_register(opd1, reg_opd1);
		reg_tracker->free_register(opd2, reg_opd2);

		return rtl_code;
	}
}

Goto_TAC_Statement::Goto_TAC_Statement(TAC_Label *_label)
	: TAC_Statement(), label(_label)
{
}

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

If_Goto_TAC_Statement::If_Goto_TAC_Statement(Scope *_eval_scope, TAC_Operand *_cond, TAC_Label *_label)
	: TAC_Statement(_eval_scope), condition(_cond), label(_label)
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
		RTL_Statement *load_stmt = new Load_RTL_Statement(eval_scope, reg_condition, condition->to_string());
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

IO_TAC_Statement::IO_TAC_Statement(Scope *_eval_scope, IO_Kind _kind, TAC_Operand *_opd)
	: TAC_Statement(_eval_scope), kind(_kind), opd(_opd)
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
		if (!dynamic_cast<Variable_TAC_Operand *>(opd))
		{
			throw_SemanticError("Expected to read into a variable");
		}

		RTL_Code *rtl_code = new RTL_Code();

		RTL_Register *reg = reg_tracker->get_register(PRIORITY_V0);
		int signal = is_float ? 7 : 5;
		Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg, signal);
		rtl_code->append_statement(iload_stmt);
		reg_tracker->free_register(nullptr, reg); // cleanup

		Read_RTL_Statement *read_stmt = new Read_RTL_Statement(is_float);
		rtl_code->append_statement(read_stmt);

		if (is_int)
		{
			reg = reg_tracker->get_register(PRIORITY_V0);
		}
		else
		{
			reg = reg_tracker->get_register(PRIORITY_F0);
		}

		RTL_Statement *store_stmt = new Store_RTL_Statement(eval_scope, reg, opd->to_string(), is_float);
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

		// TODO: If reg_opd occupies this, move it to some other temp reg
		if (!reg_tracker->available_int_regs[PRIORITY_V0])
		{
			RTL_Register *opd_src_reg = reg_tracker->get_register(opd);
			RTL_Register *opd_dest_reg = reg_tracker->get_int_register();

			rtl_code->append_statement(new Move_RTL_Statement(opd_dest_reg, opd_src_reg, false, false, is_float));

			reg_tracker->free_register(opd, opd_src_reg);
			reg_tracker->mark(opd, opd_dest_reg);
		}

		RTL_Register *reg1 = reg_tracker->get_register(PRIORITY_V0);
		int signal = is_int ? 1 : (is_float ? 3 : 4);
		Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg1, signal);
		rtl_code->append_statement(iload_stmt);

		RTL_Register *reg2 = reg_tracker->get_register(is_float ? PRIORITY_F12 : PRIORITY_A0);
		// TODO: Could be either a move (if printing an expression) or a load (if printing a variable)
		if (dynamic_cast<Temporary_TAC_Operand *>(opd))
		{
			RTL_Register *reg_opd = reg_tracker->get_register(opd);
			if (!reg_opd)
			{
				throw_SemanticError("This should not happen, temp must have a register before move");
			}
			Move_RTL_Statement *move_stmt = new Move_RTL_Statement(reg2, reg_opd, false, false, is_float);
			rtl_code->append_statement(move_stmt);
			reg_tracker->free_register(opd, reg_opd);
		}
		else if (auto o = dynamic_cast<Int_Const_TAC_Operand *>(opd))
		{
			Load_Int_RTL_Statement *iload_stmt = new Load_Int_RTL_Statement(reg2, o->ival);
			rtl_code->append_statement(iload_stmt);
		}
		else if (auto o = dynamic_cast<Float_Const_TAC_Operand *>(opd))
		{
			Load_Float_RTL_Statement *fload_stmt = new Load_Float_RTL_Statement(reg2, o->fval);
			rtl_code->append_statement(fload_stmt);
		}
		else if (auto o = dynamic_cast<String_Const_TAC_Operand *>(opd))
		{
			Load_String_RTL_Statement *sload_stmt = new Load_String_RTL_Statement(reg2, o->sval, o->string_label);
			rtl_code->append_statement(sload_stmt);
		}
		else if (dynamic_cast<Variable_TAC_Operand *>(opd) || dynamic_cast<Shared_Temporary_TAC_Operand *>(opd))
		{
			Load_RTL_Statement *load_stmt = new Load_RTL_Statement(eval_scope, reg2, opd->to_string(), is_float);
			rtl_code->append_statement(load_stmt);
		}
		else
		{
			throw_SemanticError("Write operation failed");
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

Label_TAC_Statement::Label_TAC_Statement(TAC_Label *_label)
	: TAC_Statement(), label(_label)
{
}

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

Return_TAC_Statement::Return_TAC_Statement(Scope *_eval_scope, Shared_Temporary_TAC_Operand *_return_stemp)
	: TAC_Statement(_eval_scope), return_stemp(_return_stemp)
{
}

std::string Return_TAC_Statement::to_string() const
{
	return "return " + return_stemp->to_string();
}

RTL_Code *Return_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	bool is_float = return_stemp->type == Type::FLOAT;
	RTL_Register *reg = reg_tracker->get_register(is_float ? PRIORITY_F0 : PRIORITY_V1);

	RTL_Code *rtl_code = new RTL_Code();
	rtl_code->append_statement(new Load_RTL_Statement(eval_scope, reg, return_stemp->to_string(), is_float));
	rtl_code->append_statement(new Return_RTL_Statement(reg, eval_scope->func_sig->name, is_float));
	return rtl_code;
}

Call_TAC_Statement::Call_TAC_Statement(Scope *_eval_scope, const std::string &name, const std::vector<TAC_Operand *> &args, TAC_Operand *lhs)
	: TAC_Statement(_eval_scope), func_name(name), args(args), lhs(lhs)
{
}

std::string Call_TAC_Statement::to_string() const
{
	std::string result;
	if (lhs)
	{
		result += lhs->to_string() + " = ";
	}
	result += func_name + "(";
	if (!args.empty())
	{
		for (int i = 0; i < args.size() - 1; i++)
		{
			result += args[i]->to_string() + ", ";
		}
		result += args.back()->to_string();
	}
	result += ")";
	return result;
}

RTL_Code *Call_TAC_Statement::to_rtl(RegisterTracker *reg_tracker) const
{
	RTL_Code *rtl_code = new RTL_Code();

	// Args go on the stack in reverse order
	// But we will generate the code in order
	// Look at simple-call.c for why this has to be done
	const int num_args = args.size();
	std::vector<RTL_Code *> arg_rtl_codes(num_args);

	for (int i = 0; i < num_args; i++)
	{
		RTL_Code *arg_rtl_code = new RTL_Code();

		TAC_Operand *opd = args[i];
		bool is_float = opd->type == Type::FLOAT;
		RTL_Register *reg;

		// For temps, can directly push
		// For literals/variables, need to iload/load and then push
		if (auto o = dynamic_cast<Int_Const_TAC_Operand *>(opd))
		{
			reg = reg_tracker->get_int_register();
			arg_rtl_code->append_statement(new Load_Int_RTL_Statement(reg, o->ival));
		}
		else if (auto o = dynamic_cast<Float_Const_TAC_Operand *>(opd))
		{
			reg = reg_tracker->get_float_register();
			arg_rtl_code->append_statement(new Load_Float_RTL_Statement(reg, o->fval));
		}
		else if (auto o = dynamic_cast<String_Const_TAC_Operand *>(opd))
		{
			reg = reg_tracker->get_int_register();
			arg_rtl_code->append_statement(new Load_String_RTL_Statement(reg, o->sval, o->string_label));
		}
		else if (dynamic_cast<Temporary_TAC_Operand *>(opd))
		{
			reg = reg_tracker->get_register(opd);
		}
		else
		{
			reg = is_float ? reg_tracker->get_float_register() : reg_tracker->get_int_register();
			arg_rtl_code->append_statement(new Load_RTL_Statement(eval_scope, reg, opd->to_string(), is_float));
		}

		arg_rtl_code->append_statement(new Push_RTL_Statement(is_float, reg));
		reg_tracker->free_register(opd, reg);

		arg_rtl_codes[i] = arg_rtl_code;
	}

	for (int i = num_args - 1; i >= 0; i--)
	{
		rtl_code->append_list(arg_rtl_codes[i]);
	}

	Move_RTL_Statement *mov;
	if (lhs)
	{
		bool is_float = lhs->type == Type::FLOAT;
		RTL_Register *res_reg = reg_tracker->get_register(is_float ? PRIORITY_F0 : PRIORITY_V1);
		RTL_Register *reg = is_float ? reg_tracker->get_float_register() : reg_tracker->get_int_register();
		reg_tracker->mark(lhs, reg);

		rtl_code->append_statement(new Call_RTL_Statement(func_name, res_reg));
		mov = new Move_RTL_Statement(reg, res_reg, false, false, is_float);
	}
	else
	{
		rtl_code->append_statement(new Call_RTL_Statement(func_name));
	}

	for (int i = num_args - 1; i >= 0; i--)
	{
		rtl_code->append_statement(new Pop_RTL_Statement(args[i]->type == Type::FLOAT));
	}

	if (lhs)
	{
		rtl_code->append_statement(mov);
	}
	return rtl_code;
}

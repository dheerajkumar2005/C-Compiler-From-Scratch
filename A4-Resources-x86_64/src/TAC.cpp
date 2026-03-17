#include "TAC.hpp"

Variable_TAC_Operand::Variable_TAC_Operand(std::string *name, Scope *declaring_scope)
	: name(name), declaring_scope(declaring_scope)
{
}

std::string Variable_TAC_Operand::to_string() const {
	return *name+"_"; 
}

Int_Const_TAC_Operand::Int_Const_TAC_Operand(int num) : ival(num){}
std::string Int_Const_TAC_Operand::to_string() const{
	return std::to_string(ival);
}

Float_Const_TAC_Operand::Float_Const_TAC_Operand(float num) : fval(num){}
std::string Float_Const_TAC_Operand::to_string() const{
	std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
	return out.str();
}

String_Const_TAC_operand::String_Const_TAC_operand(char* _sval): sval(_sval){}
std::string String_Const_TAC_operand::to_string() const{
	return sval;
}


int Temporary_TAC_Operand::tac_temp_count = 0;
Temporary_TAC_Operand::Temporary_TAC_Operand(): temp_number(tac_temp_count++){}
std::string Temporary_TAC_Operand::to_string() const{
	return "temp" + std::to_string(temp_number);
}

int Shared_Temporary_TAC_Operand::tac_stemp_count = 0;
Shared_Temporary_TAC_Operand::Shared_Temporary_TAC_Operand():stemp_number(tac_stemp_count++){}
std::string Shared_Temporary_TAC_Operand::to_string() const{
	return "stemp" + std::to_string(stemp_number);
}

int TAC_Label::tac_label_count = 0;
TAC_Label::TAC_Label() : label_number(tac_label_count++) {}
std::string TAC_Label::to_string() const
{
	return "Label" + std::to_string(label_number);
}

Assignment_TAC_Statement::Assignment_TAC_Statement(TAC_Operand *lhs, Binary_Operator op, TAC_Operand *opd1, TAC_Operand *opd2)
	: lhs(lhs), op(binary_to_tac(op)), opd1(opd1), opd2(opd2)
{
}

Assignment_TAC_Statement::Assignment_TAC_Statement(TAC_Operand *lhs, Unary_Operator op, TAC_Operand *opd1)
	: lhs(lhs), op(unary_to_tac(op)), opd1(opd1), opd2(nullptr)
{
}

Assignment_TAC_Statement::Assignment_TAC_Statement(TAC_Operand *lhs, TAC_Operand *opd1)
	: lhs(lhs), op(TAC_Operator::NOP), opd1(opd1), opd2(nullptr)
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

Goto_TAC_Statement::Goto_TAC_Statement(TAC_Label *_label) : label(_label) {}
std::string Goto_TAC_Statement::to_string() const{
	return "goto " + label->to_string();
}

If_Goto_TAC_Statement::If_Goto_TAC_Statement(TAC_Operand *_cond, TAC_Label *_label)
	: condition(_cond), label(_label) {}

std::string If_Goto_TAC_Statement::to_string() const{
	return "if(" + condition->to_string() + ") goto " + label->to_string();
}

IO_TAC_Statement::IO_TAC_Statement(IO_Kind _kind, TAC_Operand* _opd) : kind(_kind), opd(_opd){};
std::string IO_TAC_Statement::to_string() const{
	if(kind == IO_Kind::READ){
		return "read " + opd->to_string();
	}
	else{
		return "write " + opd->to_string();
	}
}

Label_TAC_Statement::Label_TAC_Statement(TAC_Label *_label) : label(_label) {}
std::string Label_TAC_Statement::to_string() const{
	return label->to_string() + ": ";
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

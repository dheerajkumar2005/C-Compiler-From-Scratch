#include "TAC.hpp"

// TODO: Check correctness
std::ostream &operator<<(std::ostream &os, TAC_Operator op)
{
	if (op == TAC_Operator::NEGATE)
	{
		os << "-";
	}
	else if (op == TAC_Operator::LOGICAL_NOT)
	{
		os << "!";
	}
	else if (op == TAC_Operator::ADD)
	{
		os << "+";
	}
	else if (op == TAC_Operator::SUBTRACT)
	{
		os << "-";
	}
	else if (op == TAC_Operator::MULTIPLY)
	{
		os << "*";
	}
	else if (op == TAC_Operator::DIVIDE)
	{
		os << "/";
	}
	else if (op == TAC_Operator::LT)
	{
		os << "<";
	}
	else if (op == TAC_Operator::LE)
	{
		os << "<=";
	}
	else if (op == TAC_Operator::GT)
	{
		os << ">";
	}
	else if (op == TAC_Operator::GE)
	{
		os << ">=";
	}
	else if (op == TAC_Operator::NE)
	{
		os << "!=";
	}
	else if (op == TAC_Operator::EQ)
	{
		os << "==";
	}
	else if (op == TAC_Operator::LOGICAL_AND)
	{
		os << "&&";
	}
	else if (op == TAC_Operator::LOGICAL_OR)
	{
		os << "||";
	}
	else
	{
		throw_SemanticError("Unexpected type");
	}
	return os;
}

std::string op_to_string(TAC_Operator op)
{
	std::ostringstream oss;
	oss << op;
	return oss.str();
}

// Variable_TAC_Opd::Variable_TAC_Opd(string *s) { name = s; }

// std::string Variable_TAC_Operand::to_string() const { return *name; }

int Temporary_TAC_Operand::tac_temp_count = 0;

Temporary_TAC_Operand::Temporary_TAC_Operand()
	: temp_number(tac_temp_count++)
{
}

std::string Temporary_TAC_Operand::to_string() const
{
	return "t" + std::to_string(temp_number);
}

Int_Const_TAC_Operand::Int_Const_TAC_Operand(int num)
	: num(num)
{
}

std::string Int_Const_TAC_Operand::to_string() const
{
	return std::to_string(num);
}

TAC_Statement::TAC_Statement(TAC_Operand *lhs, TAC_Operator op, TAC_Operand *opd1, TAC_Operand *opd2)
	: lhs(lhs), op(op), opd1(opd1), opd2(opd2)
{
}

TAC_Statement::TAC_Statement(TAC_Operand *lhs, TAC_Operator op, TAC_Operand *opd1)
	: lhs(lhs), op(op), opd1(opd1), opd2(nullptr)
{
}

TAC_Statement::TAC_Statement(TAC_Operand *lhs, TAC_Operand *opd1)
	: lhs(lhs), op(TAC_Operator::NOP), opd1(opd1), opd2(nullptr)
{
}

// TODO: Add support for conditional/unconditional jumps
std::string TAC_Statement::to_string() const
{
	if (op == TAC_Operator::NOP)
	{
		if (lhs)
		{
			return lhs->to_string() + " = " + opd1->to_string();
		}
		else
		{
			return opd1->to_string();
		}
	}
	else
	{
		if (opd2 != NULL)
		{
			return lhs->to_string() + " = " + opd1->to_string() + op_to_string(op) + opd2->to_string();
		}
		else
		{
			return lhs->to_string() + " = " + op_to_string(op) + opd1->to_string();
		}
	}
}

void Code::append_list(Code *c)
{
	for (auto it = (c->get_list())->begin(); it != (c->get_list())->end(); ++it)
	{
		if (*it != NULL)
			stmt_list->push_back(*it);
	}
}

void Code::print_code()
{
	for (auto it = stmt_list->begin(); it != stmt_list->end(); ++it)
	{
		if (*it != NULL)
			(*it)->print_stmt();
	}
}

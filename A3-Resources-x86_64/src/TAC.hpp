
#ifndef TAC_HPP
#define TAC_HPP

#include <string>
#include <iostream>
#include <sstream>

#include "Errors.hpp"

enum class TAC_Operator
{
	NOP,
	// Unary
	NEGATE,
	LOGICAL_NOT,
	// Binary
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	LOGICAL_AND,
	LOGICAL_OR,
	LT,
	LE,
	GT,
	GE,
	NE,
	EQ,
};

std::ostream &operator<<(std::ostream &os, TAC_Operator op);

class TAC_Operand
{
public:
	virtual std::string to_string() const = 0;
};

class Variable_TAC_Operand : public TAC_Operand
{
	// TODO: Change this to be the symtab pointer
	// std::string *name;

public:
	// TODO: Add constructor

	virtual std::string to_string() const override final;
};

class Temporary_TAC_Operand : public TAC_Operand
{
	static int tac_temp_count;

	int temp_number;

public:
	Temporary_TAC_Operand();

	void print_opd();
	virtual std::string to_string() const override final;
};

class Int_Const_TAC_Operand : public TAC_Operand
{
	int num;

public:
	Int_Const_TAC_Operand(int num);

	virtual std::string to_string() const override final;
};

// TODO: Add TAC_Operand types for the other constants too

class TAC_Statement
{
	TAC_Operand *lhs;
	TAC_Operator op;
	TAC_Operand *opd1;
	TAC_Operand *opd2;

public:
	TAC_Statement(TAC_Operand *lhs, TAC_Operator op, TAC_Operand *opd1, TAC_Operand *opd2);
	TAC_Statement(TAC_Operand *lhs, TAC_Operator op, TAC_Operand *opd1);
	TAC_Statement(TAC_Operand *lhs, TAC_Operand *opd1);

	std::string to_string() const;
};

class Code
{
	list<TAC_Statement *> *stmt_list;

public:
	Code() { stmt_list = new list<TAC_Statement *>; }
	~Code();

	void append_statement(TAC_Statement *s) { stmt_list->push_back(s); }
	void append_list(Code *c);
	list<TAC_Statement *> *get_list() { return stmt_list; }

	void print_code();
};

#endif

#ifndef TAC_HPP
#define TAC_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <iomanip>

#include "Errors.hpp"
#include "Program.hpp"
#include "utils.hpp"

enum class IO_Kind{
	READ,
	WRITE
};

// std::ostream &operator<<(std::ostream &os, TAC_Operator op);

class TAC_Operand
{
public:
	virtual std::string to_string() const = 0;
};

class Variable_TAC_Operand : public TAC_Operand
{
	std::string *name;
	Scope *declaring_scope;

public:
	Variable_TAC_Operand(std::string *name, Scope *declaring_scope);

	virtual std::string to_string() const override final;
};

class Int_Const_TAC_Operand : public TAC_Operand
{
	int ival;

public:
	Int_Const_TAC_Operand(int ival);

	virtual std::string to_string() const override final;
};

class Float_Const_TAC_Operand : public TAC_Operand
{
	float fval;

public:
	Float_Const_TAC_Operand(float fval);

	virtual std::string to_string() const override final;
};

class String_Const_TAC_operand : public TAC_Operand{
	std::string sval;

public:
	String_Const_TAC_operand(char *_sval);
	virtual std::string to_string() const override final;
};

class Temporary_TAC_Operand : public TAC_Operand
{
	static int tac_temp_count;
	int temp_number;

public:
	Temporary_TAC_Operand();

	virtual std::string to_string() const override final;
};

class Shared_Temporary_TAC_Operand : public TAC_Operand{
	static int tac_stemp_count;
	int stemp_number;

public:
	Shared_Temporary_TAC_Operand();
	virtual std::string to_string() const override final;
};

class TAC_Label
{
	static int tac_label_count;
	int label_number;

public:
	TAC_Label();
	std::string to_string() const;
};

class TAC_Statement{
public:
	virtual std::string to_string() const = 0;
};

// Transformed to Move_RTL_Statement
class Assignment_TAC_Statement : public TAC_Statement
{
public:
	// TEMP
	TAC_Operand *lhs;
	TAC_Operator op;
	TAC_Operand *opd1;
	TAC_Operand *opd2;

	Assignment_TAC_Statement(TAC_Operand *lhs, Binary_Operator op, TAC_Operand *opd1, TAC_Operand *opd2);
	Assignment_TAC_Statement(TAC_Operand *lhs, Unary_Operator op, TAC_Operand *opd1);
	Assignment_TAC_Statement(TAC_Operand *lhs, TAC_Operand *opd1);

	virtual std::string to_string() const override final;
};

// Transformed to Goto_RTL_Statement
class Goto_TAC_Statement : public TAC_Statement
{
	TAC_Label *label;

public:
	Goto_TAC_Statement(TAC_Label *_label);
	virtual std::string to_string() const override final;
};

// Transformed to If_Goto_RTL_Statement
class If_Goto_TAC_Statement : public TAC_Statement
{
	TAC_Operand* condition;
	TAC_Label *label;

public:
	If_Goto_TAC_Statement(TAC_Operand *_cond, TAC_Label *_label);
	virtual std::string to_string() const override final;
};

// Transformed to Read_RTL_Statement and Write_RTL_Statement
class IO_TAC_Statement : public TAC_Statement
{
	IO_Kind kind;
	TAC_Operand* opd;

	public:
		IO_TAC_Statement(IO_Kind _kind, TAC_Operand* _opd);
		virtual std::string to_string() const override final;
};

// Transformed to Label_RTL_Statement
class Label_TAC_Statement : public TAC_Statement
{
	TAC_Label *label;

public:
	Label_TAC_Statement(TAC_Label *_label);
	virtual std::string to_string() const override final;
};

class Code
{
	std::list<TAC_Statement *> *stmt_list;

public:
	Code();

	void append_statement(TAC_Statement *s);
	void append_list(Code *c);

	std::string to_string() const;
};

#endif

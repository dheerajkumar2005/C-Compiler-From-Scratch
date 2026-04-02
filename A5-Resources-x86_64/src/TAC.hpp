#ifndef TAC_HPP
#define TAC_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <iomanip>
#include <map>

#include "Errors.hpp"
#include "Program.hpp"
#include "utils.hpp"
#include "RTL_Code.hpp"
#include "RegisterTracker.hpp"

enum class IO_Kind
{
	READ,
	WRITE
};

class Variable_TAC_Operand : public TAC_Operand
{
	std::string *name;
	Scope *declaring_scope;

public:
	Variable_TAC_Operand(Type type, std::string *name, Scope *declaring_scope);

	virtual std::string to_string() const override final;
};

class Int_Const_TAC_Operand : public TAC_Operand
{
public:
	int ival;

	Int_Const_TAC_Operand(int ival);

	virtual std::string to_string() const override final;
};

class Float_Const_TAC_Operand : public TAC_Operand
{
public:
	float fval;

	Float_Const_TAC_Operand(float fval);

	virtual std::string to_string() const override final;
};

class String_Const_TAC_Operand : public TAC_Operand
{	
	
public:
	std::string sval;
	String_Const_TAC_Operand(char *_sval);
	virtual std::string to_string() const override final;
};

class Temporary_TAC_Operand : public TAC_Operand
{
	static int tac_temp_count;
	int temp_number;

public:
	Temporary_TAC_Operand(Type type);

	virtual std::string to_string() const override final;
};

class Shared_Temporary_TAC_Operand : public TAC_Operand
{
	static int tac_stemp_count;
	int stemp_number;

public:
	Shared_Temporary_TAC_Operand(Type type);
	virtual std::string to_string() const override final;
};

class TAC_Label
{
	static int tac_label_count;

public:
	int label_number;
	TAC_Label();
	std::string to_string() const;
};

class TAC_Statement
{
public:
	virtual std::string to_string() const = 0;
	virtual RTL_Code *to_rtl(RegisterTracker *reg_tracker) const = 0;
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
	virtual RTL_Code *to_rtl(RegisterTracker *reg_tracker) const override final;
};

// Transformed to Goto_RTL_Statement
class Goto_TAC_Statement : public TAC_Statement
{
	TAC_Label *label;

public:
	Goto_TAC_Statement(TAC_Label *_label);
	virtual std::string to_string() const override final;
	virtual RTL_Code *to_rtl(RegisterTracker *reg_tracker) const override final;
};

// Transformed to If_Goto_RTL_Statement
class If_Goto_TAC_Statement : public TAC_Statement
{
	TAC_Operand *condition;
	TAC_Label *label;

public:
	If_Goto_TAC_Statement(TAC_Operand *_cond, TAC_Label *_label);
	virtual std::string to_string() const override final;
	virtual RTL_Code *to_rtl(RegisterTracker *reg_tracker) const override final;
};

// Transformed to Read_RTL_Statement and Write_RTL_Statement
class IO_TAC_Statement : public TAC_Statement
{
	IO_Kind kind;
	TAC_Operand *opd;

public:
	IO_TAC_Statement(IO_Kind _kind, TAC_Operand *_opd);
	virtual std::string to_string() const override final;
	virtual RTL_Code *to_rtl(RegisterTracker *reg_tracker) const override final;
};

// Transformed to Label_RTL_Statement
class Label_TAC_Statement : public TAC_Statement
{
	TAC_Label *label;

public:
	Label_TAC_Statement(TAC_Label *_label);
	virtual std::string to_string() const override final;
	virtual RTL_Code *to_rtl(RegisterTracker *reg_tracker) const override final;
};

#endif

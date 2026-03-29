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

enum class IO_Kind
{
	READ,
	WRITE
};

class RTL_Register
{
public:
	int priority;

	RTL_Register(int priority);
};

// struct RTL_Register_Comparator
// {
// 	bool operator()(const RTL_Register *lhs, const RTL_Register *rhs)
// 	{
// 		return lhs->priority < rhs->priority;
// 	}
// };

class TAC_Operand
{
public:
	Type type;

	TAC_Operand(Type type);
	virtual std::string to_string() const = 0;
};

class RTL_Statement
{
public:
	bool is_float;

	RTL_Statement(bool is_float);
	virtual std::string to_string() const = 0;
};

class RTL_Code
{
public:
	std::list<RTL_Statement *> *stmt_list;

	RTL_Code();

	void append_statement(RTL_Statement *rtl_statement);
	void append_list(RTL_Code *rtl_code);
	std::string to_string() const;
};

class Load_Int_RTL_Statement : public RTL_Statement
{
public:
	RTL_Register *reg;
	int ival;

	Load_Int_RTL_Statement(RTL_Register *reg, int ival);
	virtual std::string to_string() const override final;
};

class Load_Float_RTL_Statement : public RTL_Statement
{
public:
	RTL_Register *reg;
	float fval;

	Load_Float_RTL_Statement(RTL_Register *reg, float fval);
	virtual std::string to_string() const override final;
};


class Load_RTL_Statement : public RTL_Statement
{
public:
	RTL_Register *reg;
	TAC_Operand *var;

	Load_RTL_Statement(RTL_Register *reg, TAC_Operand *var, bool is_float = false);
	virtual std::string to_string() const override final;
};

class Store_RTL_Statement : public RTL_Statement
{
public:
	RTL_Register *reg;
	TAC_Operand *var;

	Store_RTL_Statement(RTL_Register *reg, TAC_Operand *var, bool is_float = false);
	~Store_RTL_Statement() = default;

	virtual std::string to_string() const override final;
};

class Move_RTL_Statement : public RTL_Statement{
	public:
		RTL_Register* dest;
		RTL_Register* src;
		bool is_movtf;
		bool is_movt;

		Move_RTL_Statement(RTL_Register* dest, RTL_Register* src, bool is_movtf, bool is_movt, bool is_float);
		std::string to_string() const final override;
};

class Compute_RTL_Statement : public RTL_Statement
{
public:
	RTL_Register *lhs;
	RTL_Operator op;
	RTL_Register *opd1;
	RTL_Register *opd2;

	Compute_RTL_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2 = nullptr, bool is_float = false);
	virtual std::string to_string() const override final;
};

class Goto_RTL_Statement : public RTL_Statement
{
public:
	int label_number;

	Goto_RTL_Statement(int label_number);
	virtual std::string to_string() const override final;
};

class If_Goto_RTL_Statement : public RTL_Statement
{
public:
	RTL_Register *predicate;
	int label_number;

	If_Goto_RTL_Statement(RTL_Register *predicate, int label_number);
	virtual std::string to_string() const override final;
};

class Read_RTL_Statement : public RTL_Statement
{
public:
	Read_RTL_Statement(bool is_float);
	virtual std::string to_string() const override final;
};

// TODO: Note that while printing expressions, need to move and not load
class Write_RTL_Statement : public RTL_Statement
{
public:
	Write_RTL_Statement(bool is_float);
	virtual std::string to_string() const override final;
};

class Label_RTL_Statement : public RTL_Statement
{
public:
	int label_number;
	Label_RTL_Statement(int label_number);
	virtual std::string to_string() const override final;
};

class RegisterTracker
{

public:
	std::map<int, RTL_Register *> all_regs;

	std::unordered_map<TAC_Operand *, int> reg_map;
	std::map<int, bool> available_int_regs;
	std::map<int, bool> available_float_regs;

	RegisterTracker();

	RTL_Register *get_register(TAC_Operand *opd);
	RTL_Register *get_register(int priority);

	RTL_Register *get_int_register();
	RTL_Register *get_float_register();

	void mark(TAC_Operand *opd, RTL_Register *reg);

	void free_register(TAC_Operand *opd, RTL_Register *reg);
};

// std::ostream &operator<<(std::ostream &os, TAC_Operator op);

struct Scope
{
	Scope_Kind kind;
	Scope *parent_scope;

	std::unordered_map<std::string, Symbol_Table_Entry *> sym_tab;
	Func_Signature *func_sig; // nullptr for non-functions

	RegisterTracker *reg_tracker;

	Scope(Scope_Kind kind, Scope *parent_scope = nullptr, Func_Signature *func_sig = nullptr);
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

class String_Const_TAC_operand : public TAC_Operand
{
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

class Code
{
public:
	std::list<TAC_Statement *> *stmt_list;
	Code();

	void append_statement(TAC_Statement *s);
	TAC_Statement *pop_statement();
	void append_list(Code *c);

	std::string to_string() const;
};

#endif

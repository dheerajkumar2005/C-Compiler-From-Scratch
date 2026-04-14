#ifndef __RTL__
#define __RTL__

#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <list>

#include "utils.hpp"
#include "Program.hpp"

class RTL_Register
{
public:
    int priority;

    RTL_Register(int priority);
};

// Not really registers but what the heck
extern RTL_Register *sp;
extern RTL_Register *fp;

std::string register_to_string(RTL_Register *reg);

class ASM_Code;

class RTL_Statement
{
public:
    bool is_float;
    Scope *eval_scope;

    RTL_Statement(bool is_float, Scope *_eval_scope = nullptr);
    virtual std::string to_string() const = 0;
    virtual ASM_Code *to_asm() const = 0;
};

class Load_Int_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    int ival;

    Load_Int_RTL_Statement(RTL_Register *reg, int ival);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Load_Float_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    float fval;

    Load_Float_RTL_Statement(RTL_Register *reg, float fval);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Load_String_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    int string_label;
    std::string sval;
    Load_String_RTL_Statement(RTL_Register *reg, std::string sval, int string_label);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Load_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;

    Load_RTL_Statement(Scope *_eval_scope, RTL_Register *reg, std::string var_name, bool is_float = false);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Store_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;

    Store_RTL_Statement(Scope *_eval_scope, RTL_Register *reg, std::string var_name, bool is_float = false);
    ~Store_RTL_Statement() = default;

    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Move_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *dest;
    RTL_Register *src;
    bool is_movtf;
    bool is_movt;

    Move_RTL_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float);
    std::string to_string() const final override;
    virtual ASM_Code *to_asm() const override final;
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
    virtual ASM_Code *to_asm() const override final;
};

class Goto_RTL_Statement : public RTL_Statement
{
public:
    int label_number;

    Goto_RTL_Statement(int label_number);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class If_Goto_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *predicate;
    int label_number;

    If_Goto_RTL_Statement(RTL_Register *predicate, int label_number);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Read_RTL_Statement : public RTL_Statement
{
public:
    Read_RTL_Statement(bool is_float);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

// TODO: Note that while printing expressions, need to move and not load
class Write_RTL_Statement : public RTL_Statement
{
public:
    Write_RTL_Statement(bool is_float);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Label_RTL_Statement : public RTL_Statement
{
public:
    int label_number;
    Label_RTL_Statement(int label_number);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Return_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string func_name;

    Return_RTL_Statement(RTL_Register *_reg, const std::string &func_name, bool _is_float);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Call_RTL_Statement : public RTL_Statement
{
public:
    std::string func_name;
    RTL_Register *lhs;

    Call_RTL_Statement(const std::string &func_name, RTL_Register *lhs = nullptr);

    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Push_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;

    Push_RTL_Statement(bool is_float, RTL_Register *_reg);

    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Pop_RTL_Statement : public RTL_Statement
{
public:
    Pop_RTL_Statement(bool is_float);

    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class ASM_Statement
{
public:
    bool is_float;

    ASM_Statement(bool is_float);
    virtual std::string to_string() const = 0;
};

class Load_Int_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *reg;
    int ival;
    Load_Int_ASM_Statement(RTL_Register *reg, int ival);
    virtual std::string to_string() const override final;
};

class Load_Float_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *reg;
    float fval;
    Load_Float_ASM_Statement(RTL_Register *reg, float fval);
    virtual std::string to_string() const override final;
};

class Load_String_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *reg;
    std::string sval;
    int string_label;
    Load_String_ASM_Statement(RTL_Register *reg, std::string sval, int string_label);
    virtual std::string to_string() const override final;
};

class Load_Local_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *data_reg;
    int offset;
    RTL_Register *base_reg;

    Load_Local_ASM_Statement(RTL_Register *_data_reg, int _offset, RTL_Register *_base_reg, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Load_Global_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *data_reg;
    std::string var_name;

    Load_Global_ASM_Statement(RTL_Register *_data_reg, const std::string &_var_name, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Store_Local_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *data_reg;
    int offset;
    RTL_Register *base_reg;

    Store_Local_ASM_Statement(RTL_Register *_data_reg, int _offset, RTL_Register *_base_reg, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Store_Global_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *data_reg;
    std::string var_name;

    Store_Global_ASM_Statement(RTL_Register *_data_reg, const std::string &var_name, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Move_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *dest;
    RTL_Register *src;
    bool is_movtf;
    bool is_movt;
    Move_ASM_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float);
    std::string to_string() const final override;
};

class Compute_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *lhs;
    RTL_Operator op;
    RTL_Register *opd1;
    RTL_Register *opd2;
    Compute_ASM_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2 = nullptr, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Compute_Immediate_Integer_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *lhs;
    RTL_Operator op;
    RTL_Register *opd1;
    int opd2;

    Compute_Immediate_Integer_ASM_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, int opd2);
    virtual std::string to_string() const override final;
};

class Compute_Immediate_Float_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *lhs;
    RTL_Operator op;
    RTL_Register *opd1;
    float opd2;

    Compute_Immediate_Float_ASM_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, float opd2);
    virtual std::string to_string() const override final;
};

class Goto_ASM_Statement : public ASM_Statement
{
public:
    int label_number;
    Goto_ASM_Statement(int label_number);
    virtual std::string to_string() const override final;
};

class If_Goto_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *predicate;
    int label_number;
    If_Goto_ASM_Statement(RTL_Register *predicate, int label_number);
    virtual std::string to_string() const override final;
};

class Syscall_ASM_Statement : public ASM_Statement
{
public:
    Syscall_ASM_Statement();
    virtual std::string to_string() const override final;
};

class Label_ASM_Statement : public ASM_Statement
{
public:
    int label_number;
    Label_ASM_Statement(int label_number);
    virtual std::string to_string() const override final;
};

class Jump_ASM_Statement : public ASM_Statement
{
public:
    std::string label;

    Jump_ASM_Statement(const std::string &label);
    virtual std::string to_string() const override final;
};

class JAL_ASM_Statement : public ASM_Statement
{
public:
    std::string func_name;

    JAL_ASM_Statement(const std::string &_func_name);

    virtual std::string to_string() const override final;
};

class ASM_Code
{
public:
    std::list<ASM_Statement *> *stmt_list;
    ASM_Code();
    void append_statement(ASM_Statement *asm_statement);
    void append_list(ASM_Code *asm_code);
    std::string to_string() const;
    bool is_empty();
};

#endif

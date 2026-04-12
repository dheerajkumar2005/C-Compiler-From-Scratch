#ifndef __RTL__
#define __RTL__

#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <list>

#include "utils.hpp"

class RTL_Register
{
public:
    int priority;

    RTL_Register(int priority);
};

class ASM_Code;

class RTL_Statement
{
public:
    bool is_float;

    RTL_Statement(bool is_float);
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
    static int string_count;
    static std::unordered_map<std::string, int> s_map;
    int string_label;
    std::string sval;
    Load_String_RTL_Statement(RTL_Register *reg, std::string sval);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Load_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;

    Load_RTL_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
    virtual std::string to_string() const override final;
    virtual ASM_Code *to_asm() const override final;
};

class Store_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;

    Store_RTL_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
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

    Return_RTL_Statement(RTL_Register *_reg, bool _is_float);
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
    Load_String_ASM_Statement(RTL_Register *reg, std::string sval);
    virtual std::string to_string() const override final;
};

class Load_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;
    Load_ASM_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Store_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;
    Store_ASM_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
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

class Return_ASM_Statement : public ASM_Statement
{
public:
    RTL_Register *reg;
    Return_ASM_Statement(RTL_Register *_reg, bool _is_float);
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
};

#endif

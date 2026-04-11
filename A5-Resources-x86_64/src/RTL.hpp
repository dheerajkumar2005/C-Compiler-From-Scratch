#ifndef __RTL__
#define __RTL__

#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>

#include "utils.hpp"

class RTL_Register
{
public:
    int priority;

    RTL_Register(int priority);
};

class RTL_Statement
{
public:
    bool is_float;

    RTL_Statement(bool is_float);
    virtual std::string to_string() const = 0;
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
};

class Load_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;

    Load_RTL_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
    virtual std::string to_string() const override final;
};

class Store_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;
    std::string var_name;

    Store_RTL_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
    ~Store_RTL_Statement() = default;

    virtual std::string to_string() const override final;
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

class Return_RTL_Statement : public RTL_Statement
{
public:
    RTL_Register *reg;

    Return_RTL_Statement(RTL_Register *_reg, bool _is_float);
    virtual std::string to_string() const override final;
};

class Call_RTL_Statement : public RTL_Statement
{
public:
    std::string func_name;
    std::vector<std::string> args;

    Call_RTL_Statement(const std::string &name, const std::vector<std::string> &arg_strs);

    virtual std::string to_string() const override final;
};

#endif

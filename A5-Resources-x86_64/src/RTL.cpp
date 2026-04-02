#include "RTL.hpp"

RTL_Register::RTL_Register(int priority) : priority(priority)
{
}

RTL_Statement::RTL_Statement(bool is_float) : is_float(is_float)
{
}

Load_Int_RTL_Statement::Load_Int_RTL_Statement(RTL_Register *reg, int ival)
    : RTL_Statement(false), reg(reg), ival(ival)
{
}

std::string Load_Int_RTL_Statement::to_string() const
{
    std::string result;
    result = "iLoad:\t" + rtl_priority_to_register(reg->priority) + " <- " + std::to_string(ival);
    return result;
}

Load_Float_RTL_Statement::Load_Float_RTL_Statement(RTL_Register *reg, float fval)
    : RTL_Statement(true), reg(reg), fval(fval)
{
}

std::string Load_Float_RTL_Statement::to_string() const
{
    std::string result;
    result = "iLoad.d:\t" + rtl_priority_to_register(reg->priority) + " <- ";
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
    result += out.str();
    return result;
}

int Load_String_RTL_Statement::string_count = 0;
std::unordered_map<std::string, int> Load_String_RTL_Statement::s_map;

Load_String_RTL_Statement::Load_String_RTL_Statement(RTL_Register *reg, std::string sval)
    : RTL_Statement(false), reg(reg), sval(sval)
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

std::string Load_String_RTL_Statement::to_string() const
{
    std::string result;
    result = "load_addr:\t" + rtl_priority_to_register(reg->priority) + " <- " + "_str_" + std::to_string(string_label);
    return result;
}

// TODO: Removed the "Can load only variables and shared temporaries" check
Load_RTL_Statement::Load_RTL_Statement(RTL_Register *reg, std::string var_name, bool is_float)
    : RTL_Statement(is_float), reg(reg), var_name(var_name)
{
}

std::string Load_RTL_Statement::to_string() const
{
    if (!is_float)
    {
        return "load:\t" + rtl_priority_to_register(reg->priority) + " <- " + var_name;
    }
    else
    {
        return "load.d:\t" + rtl_priority_to_register(reg->priority) + " <- " + var_name;
    }
}

// TODO: Removed the "Can load only variables and shared temporaries" check
Store_RTL_Statement::Store_RTL_Statement(RTL_Register *reg, std::string var_name, bool is_float)
    : RTL_Statement(is_float), reg(reg), var_name(var_name)
{
}

std::string Store_RTL_Statement::to_string() const
{
    if (!is_float)
    {
        return "store:\t" + var_name + " <- " + rtl_priority_to_register(reg->priority);
    }
    else
    {
        return "store.d:\t" + var_name + " <- " + rtl_priority_to_register(reg->priority);
    }
}

Move_RTL_Statement::Move_RTL_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float)
    : RTL_Statement(is_float), dest(dest), src(src), is_movtf(is_movtf), is_movt(is_movt)
{
}

std::string Move_RTL_Statement::to_string() const
{
    std::string result;
    std::string dest_name = rtl_priority_to_register(dest->priority);
    std::string src_name = (src) ? rtl_priority_to_register(src->priority) : "zero";
    if (is_movtf)
    {
        if (is_movt)
        {
            result = "movt:\t" + dest_name + " <- " + src_name + " , 0";
        }
        else
        {
            result = "movf:\t" + dest_name + " <- " + src_name + " , 0";
        }
    }
    else
    {
        if (!is_float)
        {
            result = "move:\t" + dest_name + " <- " + src_name;
        }
        else
        {
            result = "move.d:\t" + dest_name + " <- " + src_name;
        }
    }
    return result;
}

Compute_RTL_Statement::Compute_RTL_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2, bool is_float)
    : RTL_Statement(is_float), lhs(lhs), op(op), opd1(opd1), opd2(opd2)
{
}

std::string Compute_RTL_Statement::to_string() const
{
    std::string result;

    std::string result_name = rtl_priority_to_register(lhs->priority);
    std::string opd1_name = rtl_priority_to_register(opd1->priority);
    std::string opd2_name = (opd2) ? rtl_priority_to_register(opd2->priority) : "";
    std::string op_name = (is_float) ? op_float_to_string(op) : op_to_string(op);

    if (op == RTL_Operator::NEGATE || op == RTL_Operator::LOGICAL_NOT)
    {
        result = op_name + ":\t" + result_name + " <- " + opd1_name;
    }
    else if (is_relational_op(op) && is_float)
    {
        result = op_name + ":\t" + opd1_name + " , " + opd2_name;
    }
    else
    {
        result = op_name + ":\t" + result_name + " <- " + opd1_name + " , " + opd2_name;
    }

    return result;
}

Goto_RTL_Statement::Goto_RTL_Statement(int label_number)
    : RTL_Statement(false), label_number(label_number)
{
}

std::string Goto_RTL_Statement::to_string() const
{
    std::string result = "goto:\tLabel" + std::to_string(label_number);
    return result;
}

If_Goto_RTL_Statement::If_Goto_RTL_Statement(RTL_Register *predicate, int label_number)
    : RTL_Statement(false), predicate(predicate), label_number(label_number)
{
}

std::string If_Goto_RTL_Statement::to_string() const
{
    std::string result = "bgtz:\t" + rtl_priority_to_register(predicate->priority) + " , " + "Label" + std::to_string(label_number);
    return result;
}

Read_RTL_Statement::Read_RTL_Statement(bool is_float)
    : RTL_Statement(is_float)
{
}

std::string Read_RTL_Statement::to_string() const
{
    return "read";
}

Write_RTL_Statement::Write_RTL_Statement(bool is_float)
    : RTL_Statement(is_float)
{
}

std::string Write_RTL_Statement::to_string() const
{
    return "write";
}

Label_RTL_Statement::Label_RTL_Statement(int label_number)
    : RTL_Statement(false), label_number(label_number)
{
}

std::string Label_RTL_Statement::to_string() const
{
    std::string result = "Label" + std::to_string(label_number) + ":";
    return result;
}

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

ASM_Code *Load_Int_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *iload_stmt = new Load_Int_ASM_Statement(reg, ival);
    asm_code->append_statement(iload_stmt);
    return asm_code;
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

ASM_Code *Load_Float_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *fload_stmt = new Load_Float_ASM_Statement(reg, fval);
    asm_code->append_statement(fload_stmt);
    return asm_code;
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

ASM_Code *Load_String_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *sload_stmt = new Load_String_ASM_Statement(reg, sval);
    asm_code->append_statement(sload_stmt);
    return asm_code;
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

ASM_Code *Load_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *load_stmt = new Load_ASM_Statement(reg, this->var_name, is_float);
    asm_code->append_statement(load_stmt);
    return asm_code;
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

ASM_Code *Store_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *store_stmt = new Store_ASM_Statement(reg, this->var_name, is_float);
    asm_code->append_statement(store_stmt);
    return asm_code;
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

ASM_Code *Move_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *move_stmt = new Move_ASM_Statement(dest, src, is_movtf, is_movt, is_float);
    asm_code->append_statement(move_stmt);
    return asm_code;
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

ASM_Code *Compute_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *compute_stmt = new Compute_ASM_Statement(lhs, op, opd1, opd2, is_float);
    asm_code->append_statement(compute_stmt);
    return asm_code;
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

ASM_Code *Goto_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *goto_stmt = new Goto_ASM_Statement(label_number);
    asm_code->append_statement(goto_stmt);
    return asm_code;
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

ASM_Code *If_Goto_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *if_goto_stmt = new If_Goto_ASM_Statement(predicate, label_number);
    asm_code->append_statement(if_goto_stmt);
    return asm_code;
}

Read_RTL_Statement::Read_RTL_Statement(bool is_float)
    : RTL_Statement(is_float)
{
}

std::string Read_RTL_Statement::to_string() const
{
    return "read";
}

ASM_Code *Read_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *syscall_stmt = new Syscall_ASM_Statement();
    asm_code->append_statement(syscall_stmt);
    return asm_code;
}

Write_RTL_Statement::Write_RTL_Statement(bool is_float)
    : RTL_Statement(is_float)
{
}

std::string Write_RTL_Statement::to_string() const
{
    return "write";
}

ASM_Code *Write_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *syscall_stmt = new Syscall_ASM_Statement();
    asm_code->append_statement(syscall_stmt);
    return asm_code;
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

ASM_Code *Label_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *label_stmt = new Label_ASM_Statement(label_number);
    asm_code->append_statement(label_stmt);
    return asm_code;
}

Return_RTL_Statement::Return_RTL_Statement(RTL_Register *_reg, bool _is_float)
    : RTL_Statement(_is_float), reg(_reg)
{
}

std::string Return_RTL_Statement::to_string() const
{
    return "return\t" + rtl_priority_to_register(reg->priority);
}

ASM_Code *Return_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *return_stmt = new Return_ASM_Statement(reg, is_float);
    asm_code->append_statement(return_stmt);
    return asm_code;
}

Call_RTL_Statement::Call_RTL_Statement(const std::string &func_name, RTL_Register *lhs)
    : RTL_Statement(false), func_name(func_name), lhs(lhs)
{
}

std::string Call_RTL_Statement::to_string() const
{
    std::string result;
    if (lhs)
    {
        result += rtl_priority_to_register(lhs->priority) + " = ";
    }
    result += "call " + func_name;
    return result;
}

ASM_Code *Call_RTL_Statement::to_asm() const
{

    // ASM_Code *asm_code = new ASM_Code();
    // ASM_Statement *return_stmt = new Return_ASM_Statement(reg, is_float);
    // asm_code->append_statement(return_stmt);
    // return asm_code;
}

Push_RTL_Statement::Push_RTL_Statement(bool is_float, RTL_Register *_reg)
    : RTL_Statement(is_float), reg(_reg)
{
}

std::string Push_RTL_Statement::to_string() const
{
    return "push:\t" + rtl_priority_to_register(reg->priority);
}

ASM_Code *Push_RTL_Statement::to_asm() const
{
    // TODO
    ASM_Code *asm_code = new ASM_Code();
    if (is_float)
    {
    }
}

Pop_RTL_Statement::Pop_RTL_Statement(bool is_float)
    : RTL_Statement(is_float)
{
}

std::string Pop_RTL_Statement::to_string() const
{
    return "pop";
}

ASM_Code *Pop_RTL_Statement::to_asm() const
{
    // TODO
    ASM_Code *asm_code = new ASM_Code();
}

ASM_Statement::ASM_Statement(bool is_float)
    : is_float(is_float)
{
}

Load_Int_ASM_Statement::Load_Int_ASM_Statement(RTL_Register *reg, int ival)
    : ASM_Statement(false), reg(reg), ival(ival)
{
}

ASM_Code::ASM_Code() : stmt_list(new std::list<ASM_Statement *>)
{
}

void ASM_Code::append_statement(ASM_Statement *asm_statement)
{
    if (asm_statement)
    {
        stmt_list->push_back(asm_statement);
    }
}

void ASM_Code::append_list(ASM_Code *asm_code)
{
    if (asm_code && asm_code->stmt_list)
    {
        for (auto it = asm_code->stmt_list->begin(); it != asm_code->stmt_list->end(); ++it)
        {
            append_statement(*it);
        }
    }
}

std::string ASM_Code::to_string() const
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
#include "RTL.hpp"

RTL_Register::RTL_Register(int priority) : priority(priority)
{
}

RTL_Register *sp = new RTL_Register(PRIORITY_SP);
RTL_Register *fp = new RTL_Register(PRIORITY_FP);

std::string register_to_string(RTL_Register *reg)
{
    return rtl_priority_to_register(reg->priority);
}

RTL_Statement::RTL_Statement(bool is_float, Scope *_eval_scope)
    : is_float(is_float), eval_scope(_eval_scope)
{
}

Load_Int_RTL_Statement::Load_Int_RTL_Statement(RTL_Register *reg, int ival)
    : RTL_Statement(false), reg(reg), ival(ival)
{
}

std::string Load_Int_RTL_Statement::to_string() const
{
    std::string result;
    result = "iLoad:\t" + register_to_string(reg) + " <- " + std::to_string(ival);
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
    result = "iLoad.d:\t" + register_to_string(reg) + " <- ";
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
    result += out.str();
    return result;
}

ASM_Code *Load_Float_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    Load_Float_ASM_Statement *fload_stmt = new Load_Float_ASM_Statement(reg, fval);
    asm_code->append_statement(fload_stmt);
    return asm_code;
}


Load_String_RTL_Statement::Load_String_RTL_Statement(RTL_Register *reg, std::string sval, int string_label)
    : RTL_Statement(false), reg(reg), sval(sval), string_label(string_label)
{
}

std::string Load_String_RTL_Statement::to_string() const
{
    std::string result;
    result = "load_addr:\t" + register_to_string(reg) + " <- " + "_str_" + std::to_string(string_label);
    return result;
}

ASM_Code *Load_String_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    ASM_Statement *sload_stmt = new Load_String_ASM_Statement(reg, sval,string_label);
    asm_code->append_statement(sload_stmt);
    return asm_code;
}

Load_RTL_Statement::Load_RTL_Statement(Scope *_eval_scope, RTL_Register *reg, std::string var_name, bool _is_stemp, bool is_float)
    : RTL_Statement(is_float, _eval_scope), reg(reg), var_name(var_name), is_stemp(_is_stemp)
{
    if (!eval_scope || !eval_scope->parent_scope)
    {
        throw_SemanticError("Expected atleast 2 levels of scopage my man");
    }
}

std::string Load_RTL_Statement::to_string() const
{
    if (!is_float)
    {
        return "load:\t" + register_to_string(reg) + " <- " + var_name;
    }
    else
    {
        return "load.d:\t" + register_to_string(reg) + " <- " + var_name;
    }
}

ASM_Code *Load_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    auto &local_sym_tab = eval_scope->sym_tab;

    if (is_stemp)
    {
        std::string stemp_name = "$" + var_name;
        if (local_sym_tab.find(stemp_name) == local_sym_tab.end())
        {
            throw_SemanticError("Bro stemps have gots to be in the local symtab only...");
        }

        auto de = dynamic_cast<Data_Entry *>(local_sym_tab[stemp_name]);
        if (!de)
        {
            throw_SemanticError("Has to be a data entry homie!");
        }
        asm_code->append_statement(new Load_Local_ASM_Statement(reg, de->offset, fp, is_float));
    }
    else
    {
        std::string temp_var_name = var_name.substr(0, var_name.size() - 1);
        if (local_sym_tab.find(temp_var_name) != local_sym_tab.end())
        {
            auto de = dynamic_cast<Data_Entry *>(local_sym_tab[temp_var_name]);
            if (!de)
            {
                throw_SemanticError("Expected it to be a data entry");
            }

            asm_code->append_statement(new Load_Local_ASM_Statement(reg, de->offset, fp, is_float));
        }
        else
        {
            // I am assuming if the var is not in local scope then its in global scope and also there is a local and global scope, nothing else
            asm_code->append_statement(new Load_Global_ASM_Statement(reg, var_name, is_float));
        }
    }

    return asm_code;
}

Store_RTL_Statement::Store_RTL_Statement(Scope *_eval_scope, RTL_Register *reg, std::string var_name, bool _is_stemp, bool is_float)
    : RTL_Statement(is_float, _eval_scope), reg(reg), var_name(var_name), is_stemp(_is_stemp)
{
}

std::string Store_RTL_Statement::to_string() const
{
    if (!is_float)
    {
        return "store:\t" + var_name + " <- " + register_to_string(reg);
    }
    else
    {
        return "store.d:\t" + var_name + " <- " + register_to_string(reg);
    }
}

ASM_Code *Store_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    auto &local_sym_tab = eval_scope->sym_tab;

    if (is_stemp)
    {
        std::string stemp_name = "$" + var_name;
        if (local_sym_tab.find(stemp_name) == local_sym_tab.end())
        {
            throw_SemanticError("For store also, stemps have to be in the local symtab only...");
        }

        auto de = dynamic_cast<Data_Entry *>(local_sym_tab[stemp_name]);
        if (!de)
        {
            throw_SemanticError("Expected a data entry homie buddy");
        }

        asm_code->append_statement(new Store_Local_ASM_Statement(reg, de->offset, fp, is_float));
    }
    else
    {
        std::string temp_var_name = var_name.substr(0, var_name.size() - 1);
        if (local_sym_tab.find(temp_var_name) != local_sym_tab.end())
        {
            auto de = dynamic_cast<Data_Entry *>(local_sym_tab[temp_var_name]);
            if (!de)
            {
                throw_SemanticError("Expected it to be a data entry");
            }

            asm_code->append_statement(new Store_Local_ASM_Statement(reg, de->offset, fp, is_float));
        }
        else
        {
            // I am assuming if the var is not in local scope then its in global scope and also there is a local and global scope, nothing else
            asm_code->append_statement(new Store_Global_ASM_Statement(reg, var_name, is_float));
        }
    }

    return asm_code;
}

Move_RTL_Statement::Move_RTL_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float)
    : RTL_Statement(is_float), dest(dest), src(src), is_movtf(is_movtf), is_movt(is_movt)
{
}

std::string Move_RTL_Statement::to_string() const
{
    std::string result;
    std::string dest_name = register_to_string(dest);
    std::string src_name = (src) ? register_to_string(src) : "zero";
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

    std::string result_name = register_to_string(lhs);
    std::string opd1_name = register_to_string(opd1);
    std::string opd2_name = (opd2) ? register_to_string(opd2) : "";
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
    std::string result = "bgtz:\t" + register_to_string(predicate) + " , " + "Label" + std::to_string(label_number);
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

Return_RTL_Statement::Return_RTL_Statement(RTL_Register *_reg, const std::string &func_name, bool _is_float)
    : RTL_Statement(_is_float), reg(_reg), func_name(func_name)
{
}

std::string Return_RTL_Statement::to_string() const
{
    return "return\t" + register_to_string(reg);
}

ASM_Code *Return_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    asm_code->append_statement(new Jump_ASM_Statement("epilogue_" + func_name));
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
        result += register_to_string(lhs) + " = ";
    }
    result += "call " + func_name;
    return result;
}

ASM_Code *Call_RTL_Statement::to_asm() const
{
    ASM_Code *asm_code = new ASM_Code();
    asm_code->append_statement(new JAL_ASM_Statement(func_name));
    return asm_code;
}

Push_RTL_Statement::Push_RTL_Statement(bool is_float, RTL_Register *_reg)
    : RTL_Statement(is_float), reg(_reg)
{
}

std::string Push_RTL_Statement::to_string() const
{
    return "push:\t" + register_to_string(reg);
}

ASM_Code *Push_RTL_Statement::to_asm() const
{
    int offset = is_float ? -4 : 0;
    int step = is_float ? 8 : 4;

    ASM_Code *asm_code = new ASM_Code();
    asm_code->append_statement(new Store_Local_ASM_Statement(reg, offset, sp, is_float));
    asm_code->append_statement(new Compute_Immediate_Integer_ASM_Statement(sp, RTL_Operator::SUBTRACT, sp, step));
    return asm_code;
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
    int step = is_float ? 8 : 4;

    ASM_Code *asm_code = new ASM_Code();
    asm_code->append_statement(new Compute_Immediate_Integer_ASM_Statement(sp, RTL_Operator::ADD, sp, step));
    return asm_code;
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

bool ASM_Code::is_empty(){
    return stmt_list->size() == 0;
}
ASM_Statement::ASM_Statement(bool is_float)
    : is_float(is_float)
{
}


Load_Int_ASM_Statement::Load_Int_ASM_Statement(RTL_Register *reg, int ival)
    : ASM_Statement(false), reg(reg), ival(ival) {}

std::string Load_Int_ASM_Statement::to_string() const
{
    std::string result;
    result = "li $" + register_to_string(reg) + ", " + std::to_string(ival);
    return result;
}

Load_Float_ASM_Statement::Load_Float_ASM_Statement(RTL_Register *reg, float fval)
    : ASM_Statement(true), reg(reg), fval(fval) {}

std::string Load_Float_ASM_Statement::to_string() const
{
    std::string result;
    result = "li.d $" + register_to_string(reg) + ", ";
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
    result += out.str();
    return result;
}

Load_String_ASM_Statement::Load_String_ASM_Statement(RTL_Register *reg, std::string sval, int string_label)
    : ASM_Statement(false), reg(reg), sval(sval), string_label(string_label) {}

std::string Load_String_ASM_Statement::to_string() const
{
    int s_label = string_label;
    std::string result = "la $" + register_to_string(reg) + ", " + "_str_" + std::to_string(s_label);
    return result;
}

Load_Local_ASM_Statement::Load_Local_ASM_Statement(RTL_Register *_data_reg, int _offset, RTL_Register *_base_reg, bool is_float)
    : ASM_Statement(is_float), data_reg(_data_reg), offset(_offset), base_reg(_base_reg)
{
}

std::string Load_Local_ASM_Statement::to_string() const
{
    std::string op_str = is_float ? "l.d" : "lw";
    return op_str + " $" + register_to_string(data_reg) + ", " + std::to_string(offset) + "($" + register_to_string(base_reg) + ")";
}

Load_Global_ASM_Statement::Load_Global_ASM_Statement(RTL_Register *_data_reg, const std::string &_var_name, bool is_float)
    : ASM_Statement(is_float), data_reg(_data_reg), var_name(_var_name)
{
}

std::string Load_Global_ASM_Statement::to_string() const
{
    std::string op_str = is_float ? "l.d" : "lw";
    return op_str + " $" + register_to_string(data_reg) + ", " + var_name;
}

Store_Local_ASM_Statement::Store_Local_ASM_Statement(RTL_Register *_data_reg, int _offset, RTL_Register *_base_reg, bool is_float)
    : ASM_Statement(is_float), data_reg(_data_reg), offset(_offset), base_reg(_base_reg)
{
}

std::string Store_Local_ASM_Statement::to_string() const
{
    std::string op_str = is_float ? "s.d" : "sw";
    return op_str + " $" + register_to_string(data_reg) + ", " + std::to_string(offset) + "($" + register_to_string(base_reg) + ")";
}

Store_Global_ASM_Statement::Store_Global_ASM_Statement(RTL_Register *_data_reg, const std::string &var_name, bool is_float)
    : ASM_Statement(is_float), data_reg(_data_reg), var_name(var_name)
{
}

std::string Store_Global_ASM_Statement::to_string() const
{
    std::string op_str = is_float ? "s.d" : "sw";
    return op_str + " $" + register_to_string(data_reg) + ", " + var_name;
}

Move_ASM_Statement::Move_ASM_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float)
    : ASM_Statement(is_float), dest(dest), src(src), is_movtf(is_movtf), is_movt(is_movt) {}

std::string Move_ASM_Statement::to_string() const
{
    std::string result;
    std::string dest_name = register_to_string(dest);
    std::string src_name = (src) ? register_to_string(src) : "zero";
    if (is_movtf)
    {
        if (is_movt)
        {
            result = "movt $" + dest_name + ", $" + src_name + ", 0";
        }
        else
        {
            result = "movf $" + dest_name + ", $" + src_name + ", 0";
        }
    }
    else
    {
        if (!is_float)
        {
            result = "move $" + dest_name + ", $" + src_name;
        }
        else
        {
            result = "mov.d $" + dest_name + ", $" + src_name;
        }
    }
    return result;
}

Compute_ASM_Statement::Compute_ASM_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2, bool is_float)
    : ASM_Statement(is_float), lhs(lhs), op(op), opd1(opd1), opd2(opd2) {}

std::string Compute_ASM_Statement::to_string() const
{
    std::string result;

    std::string result_name = register_to_string(lhs);
    std::string opd1_name = register_to_string(opd1);
    std::string opd2_name = (opd2) ? register_to_string(opd2) : "";
    std::string op_name = (is_float) ? op_float_to_string_asm(op) : op_to_string_asm(op);

    if (op == RTL_Operator::NEGATE)
    {
        result = op_name + " $" + result_name + ", $" + opd1_name;
    }
    else if (op == RTL_Operator::LOGICAL_NOT)
    {
        result = op_name + " $" + result_name + ", $" + opd1_name + ", 1";
    }
    else if (is_relational_op(op) && is_float)
    {
        result = op_name + " $" + opd1_name + ", $" + opd2_name;
    }
    else
    {
        result = op_name + " $" + result_name + ", $" + opd1_name + ", $" + opd2_name;
    }
    return result;
}

Compute_Immediate_Integer_ASM_Statement::Compute_Immediate_Integer_ASM_Statement(RTL_Register *_lhs, RTL_Operator _op, RTL_Register *_opd1, int _opd2)
    : ASM_Statement(false), lhs(_lhs), op(_op), opd1(_opd1), opd2(_opd2)
{
}

std::string Compute_Immediate_Integer_ASM_Statement::to_string() const
{
    std::string lhs_name = register_to_string(lhs);
    std::string op_name = op_to_string_asm(op);
    std::string opd1_name = register_to_string(opd1);

    return op_name + " $" + lhs_name + ", $" + opd1_name + ", " + std::to_string(opd2);
}

Compute_Immediate_Float_ASM_Statement::Compute_Immediate_Float_ASM_Statement(RTL_Register *_lhs, RTL_Operator _op, RTL_Register *_opd1, float _opd2)
    : ASM_Statement(true), lhs(_lhs), op(_op), opd1(_opd1), opd2(_opd2)
{
}

std::string Compute_Immediate_Float_ASM_Statement::to_string() const
{
    std::string lhs_name = register_to_string(lhs);
    std::string op_name = op_float_to_string_asm(op);
    std::string opd1_name = register_to_string(opd1);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << opd2;
    std::string opd2_str = oss.str();

    return op_name + " $" + lhs_name + ", $" + opd1_name + ", " + opd2_str;
}

Goto_ASM_Statement::Goto_ASM_Statement(int label_number)
    : ASM_Statement(false), label_number(label_number) {}

std::string Goto_ASM_Statement::to_string() const
{
    std::string result;
    result = "j Label" + std::to_string(label_number);
    return result;
}

If_Goto_ASM_Statement::If_Goto_ASM_Statement(RTL_Register *pred, int label_no)
    : ASM_Statement(false), predicate(pred), label_number(label_no) {}

std::string If_Goto_ASM_Statement::to_string() const
{
    std::string result;
    result = "bgtz $" + register_to_string(predicate) + ", Label" + std::to_string(label_number);
    return result;
}

Syscall_ASM_Statement::Syscall_ASM_Statement() : ASM_Statement(false) {}

std::string Syscall_ASM_Statement::to_string() const
{
    return "syscall";
}

Label_ASM_Statement::Label_ASM_Statement(int label_no)
    : ASM_Statement(false), label_number(label_no) {}

std::string Label_ASM_Statement::to_string() const
{
    return "Label" + std::to_string(label_number) + ":";
}

Jump_ASM_Statement::Jump_ASM_Statement(const std::string &label)
    : ASM_Statement(false), label(label)
{
}

std::string Jump_ASM_Statement::to_string() const
{
    return "j " + label;
}

JAL_ASM_Statement::JAL_ASM_Statement(const std::string &_func_name)
    : ASM_Statement(false), func_name(_func_name)
{
}

std::string JAL_ASM_Statement::to_string() const
{
    return "jal " + func_name;
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

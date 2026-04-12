#include "ASM.hpp"

ASM_Statement::ASM_Statement(bool is_float) : is_float(is_float){}

Load_Int_ASM_Statement::Load_Int_ASM_Statement(RTL_Register* reg, int ival)
    : ASM_Statement(false), reg(reg),ival(ival) {}

std::string Load_Int_ASM_Statement::to_string() const{
    std::string result;
    result =  "li $" + rtl_priority_to_register(reg->priority) + ", " + std::to_string(ival);
    return result;
}

Load_Float_ASM_Statement::Load_Float_ASM_Statement(RTL_Register* reg, float ival)
    : ASM_Statement(true), reg(reg),fval(fval) {}

std::string Load_Float_ASM_Statement::to_string() const{
    std::string result;
    result =  "li.d $" + rtl_priority_to_register(reg->priority) + ", ";
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
    result += out.str();
    return result;
}

Load_String_ASM_Statement::Load_String_ASM_Statement(RTL_Register* reg, std::string sval)
    : ASM_Statement(false), reg(reg),sval(sval) {}

std::string Load_String_ASM_Statement::to_string() const{
    int s_label = Load_String_RTL_Statement::s_map[sval];
    std::string result = "la $" + rtl_priority_to_register(reg->priority) + ", " + "_str_" + std::to_string(s_label);
    return result;
}

Load_ASM_Statement::Load_ASM_Statement(RTL_Register* reg, std::string var_name, bool is_float)
    :ASM_Statement(is_float), reg(reg), var_name(var_name){}


std::string Load_ASM_Statement::to_string() const{
    std::string result;
    auto local_sym_tab = curr_scope->sym_tab;
    if(local_sym_tab.find(var_name) != local_sym_tab.end()){
        int offset = dynamic_cast<Data_Entry*>(local_sym_tab[var_name])->offset;
        if(!is_float){
            result = "lw $" + rtl_priority_to_register(reg->priority) + ", " + std::to_string(offset) + "($fp)";
        }
        else{
            result = "l.d $" + rtl_priority_to_register(reg->priority) + ", " + std::to_string(offset) + "($fp)";
        }
    }
    else{
        // I am assuming if the var is not in local scope then its in global scope and also there is a local and global scope, nothing else
        if(!is_float){
            result = "lw $" + rtl_priority_to_register(reg->priority) + ", " + var_name + "_";
        }
        else{
            result = "l.d $" + rtl_priority_to_register(reg->priority) + ", " + var_name + "_";
        }
    }
    return result;
}

Store_ASM_Statement::Store_ASM_Statement(RTL_Register* reg, std::string var_name, bool is_float)
    : ASM_Statement(is_float), reg(reg), var_name(var_name) {}

std::string Store_ASM_Statement::to_string() const{
    std::string result;
    auto local_sym_tab = curr_scope->sym_tab;
    if(local_sym_tab.find(var_name) != local_sym_tab.end()){
        int offset = dynamic_cast<Data_Entry*>(local_sym_tab[var_name])->offset;
        if(!is_float){
            result = "sw $" + rtl_priority_to_register(reg->priority) + ", " + std::to_string(offset) + "($fp)";
        }
        else{
            result = "s.d $" + rtl_priority_to_register(reg->priority) + ", " + std::to_string(offset) + "($fp)";
        }
    }
    else{
        // I am assuming if the var is not in local scope then its in global scope and also there is a local and global scope, nothing else
        if(!is_float){
            result = "sw $" + rtl_priority_to_register(reg->priority) + ", " + var_name + "_";
        }
        else{
            result = "s.d $" + rtl_priority_to_register(reg->priority) + ", " + var_name + "_";
        }
    }
    return result;
}

Move_ASM_Statement::Move_ASM_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float)
    : ASM_Statement(is_float), dest(dest), src(src) , is_movtf(is_movtf), is_movt(is_movt) {}


std::string Move_ASM_Statement::to_string() const{
    std::string result;
    std::string dest_name = rtl_priority_to_register(dest->priority);
    std::string src_name = (src) ? rtl_priority_to_register(src->priority) : "zero";
    if (is_movtf){
        if (is_movt){
            result = "movt $" + dest_name + ", $" + src_name + ", 0";
        }
        else{
            result = "movf $" + dest_name + ", $" + src_name + ", 0";
        }
    }
    else{
        if (!is_float){
            result = "move $" + dest_name + ", $" + src_name;
        }
        else{
            result = "mov.d $" + dest_name + ", $" + src_name;
        }
    }
    return result;
}

Compute_ASM_Statement::Compute_ASM_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2, bool is_float)
    : ASM_Statement(is_float), lhs(lhs), op(op), opd1(opd1), opd2(opd2) {}

std::string Compute_ASM_Statement::to_string() const{
    std::string result;

    std::string result_name = rtl_priority_to_register(lhs->priority);
    std::string opd1_name = rtl_priority_to_register(opd1->priority);
    std::string opd2_name = (opd2) ? rtl_priority_to_register(opd2->priority) : "";
    std::string op_name = (is_float) ? op_float_to_string_asm(op) : op_to_string_asm(op);

    if (op == RTL_Operator::NEGATE){
        result = op_name + " $" + result_name + ", $" + opd1_name;
    }
    else if (op == RTL_Operator::LOGICAL_NOT){
        result = op_name + " $" + result_name + ", $" + opd1_name + ", 1";  
    }
    else if (is_relational_op(op) && is_float){
        result = op_name + " $" + opd1_name + ", $" + opd2_name;
    }
    else{
        result = op_name + " $" + result_name + ", $" + opd1_name + ", $" + opd2_name;
    }
    return result;
}

Goto_ASM_Statement::Goto_ASM_Statement(int label_number) 
    : ASM_Statement(false), label_number(label_number) {}

std::string Goto_ASM_Statement::to_string() const{
    std::string result;
    result = "j Label" + std::to_string(label_number);
    return result;
}

If_Goto_ASM_Statement::If_Goto_ASM_Statement(RTL_Register *pred, int label_no)
    : ASM_Statement(false), predicate(pred), label_number(label_no){}

std::string If_Goto_ASM_Statement::to_string() const{
    std::string result;
    result = "bgtz $" + rtl_priority_to_register(predicate->priority) + ", Label" + std::to_string(label_number);
    return result;
}

Syscall_ASM_Statement::Syscall_ASM_Statement() : ASM_Statement(false) {}

std::string Syscall_ASM_Statement::to_string() const{
    return "syscall";
}

Label_ASM_Statement::Label_ASM_Statement(int label_no)
    : ASM_Statement(false), label_number(label_no) {}

std::string Label_ASM_Statement::to_string() const{
    return "Label" + std::to_string(label_number) + ":";
}

Return_ASM_Statement::Return_ASM_Statement(RTL_Register* reg, bool is_float)
    : ASM_Statement(is_float), reg(reg) {}

std::string Return_ASM_Statement::to_string() const{
    std::string func_name = curr_scope->func_sig->name;
    return "j epilogue_" + func_name;
}



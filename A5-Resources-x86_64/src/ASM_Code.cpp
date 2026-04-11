#include "ASM_Code.hpp"

ASM_Code::ASM_Code() : stmt_list(new std::list<ASM_Statement *>){}

void ASM_Code::append_statement(ASM_Statement *asm_statement){
    if (asm_statement){
        stmt_list->push_back(asm_statement);
    }
}

void ASM_Code::append_list(ASM_Code *asm_code){
    if (asm_code && asm_code->stmt_list){
        for (auto it = asm_code->stmt_list->begin(); it != asm_code->stmt_list->end(); ++it){
            append_statement(*it);
        }
    }
}

std::string ASM_Code::to_string() const{
    std::string result;
    for (auto it = stmt_list->begin(); it != stmt_list->end(); ++it){
        if (*it){
            result += (*it)->to_string() + "\n";
        }
    }
    return result;
}
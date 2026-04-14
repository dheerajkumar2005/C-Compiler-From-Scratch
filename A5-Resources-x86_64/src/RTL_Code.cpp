#include "RTL_Code.hpp"

RTL_Code::RTL_Code() : stmt_list(new std::list<RTL_Statement *>)
{
}

void RTL_Code::append_statement(RTL_Statement *rtl_statement)
{
    if (rtl_statement)
    {
        stmt_list->push_back(rtl_statement);
    }
}

void RTL_Code::append_list(RTL_Code *rtl_code)
{
    if (rtl_code && rtl_code->stmt_list)
    {
        for (auto it = rtl_code->stmt_list->begin(); it != rtl_code->stmt_list->end(); ++it)
        {
            append_statement(*it);
        }
    }
}

bool RTL_Code::is_empty() const
{
    return !stmt_list || stmt_list->empty();
}

std::string RTL_Code::to_string() const
{
    std::string result;
    for (auto it = stmt_list->begin(); it != stmt_list->end(); ++it)
    {
        if (*it)
        {
            std::string temp = (*it)->to_string();
            result += temp + "\n";
            // result += (*it)->to_string() + "\n";
        }
    }
    return result;
}

ASM_Code* RTL_Code::get_asm() {
    ASM_Code* asm_code = new ASM_Code();
    for (RTL_Statement* stmt : *stmt_list){
        asm_code->append_list(stmt->to_asm());
    }
    return asm_code;
}
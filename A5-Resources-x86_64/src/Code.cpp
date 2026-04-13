#include "Code.hpp"

Code::Code()
    : stmt_list(new std::list<TAC_Statement *>)
{
}

void Code::append_statement(TAC_Statement *s)
{
    if (s)
    {
        stmt_list->push_back(s);
    }
}

void Code::append_list(Code *c)
{
    if (c && c->stmt_list)
    {
        for (auto it = c->stmt_list->begin(); it != c->stmt_list->end(); ++it)
        {
            append_statement(*it);
        }
    }
}

bool Code::is_empty() const
{
    return !stmt_list || stmt_list->empty();
}

std::string Code::to_string() const
{
    std::string result;
    for (auto stmt : *stmt_list)
    {
        if (stmt)
        {
            result += stmt->to_string() + "\n";
        }
    }
    return result;
}
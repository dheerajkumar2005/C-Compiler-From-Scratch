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

TAC_Statement *Code::pop_statement()
{
    if (!stmt_list || stmt_list->empty())
    {
        return nullptr;
    }

    TAC_Statement *stmt = stmt_list->back();
    stmt_list->pop_back();
    return stmt;
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

std::string Code::to_string() const
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
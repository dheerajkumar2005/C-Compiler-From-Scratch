#include <string>
#include "Errors.hpp"

extern int sa_parse;

void throw_SemanticError(const std::string &msg)
{
    if (!sa_parse)
    {
        throw new SemanticError(msg);
    }
}
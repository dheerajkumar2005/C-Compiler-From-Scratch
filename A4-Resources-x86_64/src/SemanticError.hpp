#ifndef SEMANTIC_ERROR_H
#define SEMANTIC_ERROR_H

#include <stdexcept>
#include <string>

class SemanticError : public std::runtime_error
{
public:
    SemanticError(const std::string &msg);
};

#endif
#include "SemanticError.hpp"

SemanticError::SemanticError(const std::string &msg) : std::runtime_error(msg) {}
#include "SymTabEntry.hpp"

std::map<std::string, SymTabEntry *> SymTabEntry::global_scope;
std::map<std::string, SymTabEntry *> SymTabEntry::local_scope;

#include <getopt.h>
#include <fstream>
#include <iostream>
#include <cstdio>

// #include "y.tab.h"

extern FILE *yyin;
extern "C" {
    int yyparse();
    int yylex();
}

enum Option {
    SA_SCAN = 1,
    SA_PARSE,
    SA_AST,
    SA_TAC,
    SA_RTL,

    SHOW_TOKENS,
    SHOW_AST,
    SHOW_TAC,
    SHOW_RTL,
    SHOW_SYMTAB,
    SHOW_ASM,

    GEN_TEMP_SYMB_TABLE,
    SINGLE_STMT_BB,
    SUPPRESS_COMMENTS,

    DEMO,
    USAGE,
    VERSION,
};

int main(int argc, char * argv[]) {
    bool sa_scan = false;
    bool sa_parse = false;
    bool sa_ast = false;
    bool sa_tac = false;
    bool sa_rtl = false;

    bool show_tokens = false;
    bool show_ast = false;
    bool show_tac = false;
    bool show_rtl = false;
    bool show_symtab = false;
    bool show_asm = false;
    
    bool gen_temp_symb_table = false;
    bool single_stmt_bb = false;
    bool suppress_comments = false;
    
    bool demo = false;
    bool usage = false;
    bool version = false;

    static struct option long_opts[] = {
        {"sa-scan", no_argument, nullptr, Option::SA_SCAN},
        {"sa-parse", no_argument, nullptr, Option::SA_PARSE},
        {"sa-ast", no_argument, nullptr, Option::SA_AST},
        {"sa-tac", no_argument, nullptr, Option::SA_TAC},
        {"sa-rtl", no_argument, nullptr, Option::SA_RTL},

        {"show-tokens", no_argument, nullptr, Option::SHOW_TOKENS},
        {"show-ast", no_argument, nullptr, Option::SHOW_AST},
        {"show-tac", no_argument, nullptr, Option::SHOW_TAC},
        {"show-rtl", no_argument, nullptr, Option::SHOW_RTL},
        {"show-symtab", no_argument, nullptr, Option::SHOW_SYMTAB},
        {"show-asm", no_argument, nullptr, Option::SHOW_ASM},

        {"gen-temp-symb-table", no_argument, nullptr, Option::GEN_TEMP_SYMB_TABLE},
        {"single-stmt-bb", no_argument, nullptr, Option::SINGLE_STMT_BB},
        {"suppress-comments", no_argument, nullptr, Option::SUPPRESS_COMMENTS},

        {"demo", no_argument, nullptr, Option::DEMO},
        {"usage", no_argument, nullptr, Option::USAGE},
        {"version", no_argument, nullptr, Option::VERSION},

        {nullptr, 0, nullptr, 0}
    };

    // processing all the flags
    int opt;
    while((opt = getopt_long(argc, argv, "desV", long_opts, nullptr)) != -1) {
        switch(opt) {
            case Option::SA_SCAN: sa_scan = true; break;
            case Option::SA_PARSE: sa_parse = true; break;
            case Option::SA_AST: sa_ast = true; break;
            case Option::SA_TAC: sa_tac = true; break;
            case Option::SA_RTL: sa_rtl = true; break;

            case Option::SHOW_TOKENS: show_tokens = true; break;
            case Option::SHOW_AST: show_ast = true; break;
            case Option::SHOW_TAC: show_tac = true; break;
            case Option::SHOW_RTL: show_rtl = true; break;
            case Option::SHOW_SYMTAB: show_symtab = true; break;
            case Option::SHOW_ASM: show_asm = true; break;
            
            case Option::GEN_TEMP_SYMB_TABLE: gen_temp_symb_table = true; break;
            case Option::SINGLE_STMT_BB: 
            case 'e': single_stmt_bb = true; break;
            case Option::SUPPRESS_COMMENTS: 
            case 's': suppress_comments = true; break;

            case Option::DEMO: 
            case 'd': demo = true; break;
            case Option::USAGE: usage = true; break;
            case Option::VERSION: 
            case 'V': version = true; break;

            default: return 1;
        }
    }

    // processing the filename
    if(optind != argc - 1) {
        // zero or more than one filename
        return 1;
    }
    const char *filename = argv[optind];
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }

    yyin = std::fopen(filename, "r");
    return yyparse();
}

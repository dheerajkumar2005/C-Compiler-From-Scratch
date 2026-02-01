#include <getopt.h>
#include <fstream>
#include <iostream>

// #include "y.tab.h"

extern FILE *yyin;
extern FILE *yyout;
extern "C" {
    int yyparse();
    int yylex();
    void yyerror(const char *msg);

    // Used by the lexer
    // C++ declarations get name-mangled in the .o file
    int show_tokens;
    int scanner_error;
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
    int sa_scan = 0;
    int sa_parse = 0;
    int sa_ast = 0;
    int sa_tac = 0;
    int sa_rtl = 0;

    show_tokens = 0;
    int show_ast = 0;
    int show_tac = 0;
    int show_rtl = 0;
    int show_symtab = 0;
    int show_asm = 0;
    
    int gen_temp_symb_table = 0;
    int single_stmt_bb = 0;
    int suppress_comments = 0;
    
    int demo = 0;

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
            case Option::SA_SCAN: sa_scan = 1; break;
            case Option::SA_PARSE: sa_parse = 1; break;
            case Option::SA_AST: sa_ast = 1; break;
            case Option::SA_TAC: sa_tac = 1; break;
            case Option::SA_RTL: sa_rtl = 1; break;

            case Option::SHOW_TOKENS: show_tokens = 1; break;
            case Option::SHOW_AST: show_ast = 1; break;
            case Option::SHOW_TAC: show_tac = 1; break;
            case Option::SHOW_RTL: show_rtl = 1; break;
            case Option::SHOW_SYMTAB: show_symtab = 1; break;
            case Option::SHOW_ASM: show_asm = 1; break;
            
            case Option::GEN_TEMP_SYMB_TABLE: gen_temp_symb_table = 1; break;
            case Option::SINGLE_STMT_BB: 
            case 'e': single_stmt_bb = 1; break;
            case Option::SUPPRESS_COMMENTS: 
            case 's': suppress_comments = 1; break;

            case Option::DEMO: 
            case 'd': demo = 1; break;
            case Option::USAGE: {
                std::cout << R"(Usage: A1-sclp [-des?V] [--sa-scan] [--sa-parse] [--sa-ast] [--sa-tac]
            [--sa-rtl] [--show-tokens] [--show-ast] [--show-tac] [--show-rtl]
            [--show-symtab] [--show-asm] [--demo] [--gen-temp-symb-table]
            [--single-stmt-bb] [--suppress-comments] [--help] [--usage]
            [--version] [FILE])" << std::endl;
                return 0;
            }
            case Option::VERSION: 
            case 'V': {
                std::cout << "Sclp version: A6" << std::endl;
                return 0;
            }

            default: return 1;
        }
    }

    // processing the filename
    if(optind != argc - 1) {
        // zero or more than one filename
        return 1;
    }
    std::string filename = argv[optind];
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }
    
    yyin = std::fopen(filename.c_str(), "r");
    yyout = nullptr;

    if(show_tokens) {
        if(demo) {
            yyout = stdout;
        } else {
            std::string outfilename = filename + ".toks";
            yyout = std::fopen(outfilename.c_str(), "w");
        }
    }

    if(sa_scan) {
        while(true) {
            scanner_error = 0;
            int next_token = yylex();
            if(scanner_error) {
                // yyerror("I don't know why");
                yyerror("syntax error");
                return 1;
            }
            if(!next_token) {
                return 0;
            }
        }
        return 0;
    }

    return yyparse();
}

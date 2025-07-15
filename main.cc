#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<cstdio>
#include<argp.h>
#include "ast.hh"
#include<map>
#include "types.hh"
#include "rtl.hh"
#include "asm.hh"
using namespace std;

map<reg, bool> whether_register_available;
extern FILE* yyin;
extern int yylex();
extern int yyparse();
extern char* yytext();
extern void print_ast();
extern void print_tac();
extern void print_rtl();
extern void print_asm();
extern void check_ast();
extern void modify_tac();
extern void modify_asm();
extern map<string, vector<AST_Statement*>> all_ast_statements;
extern map<string, vector<TAC_Statement*>> all_tac_statements;
extern map<string, vector<RTL_Statement*>> all_rtl_statements;
extern map<string, vector<ASM_Statement*>> all_asm_statements;
extern int tac_temp_count;
extern int saved_tac_temp_count;
extern map<TAC_Opd*, reg> TAC_to_reg_map;
extern data_type get_return_dt(string curr_func);
int string_const_rtl_opd_number = 0;
map<string, int> addr_mapping;
string curr_func = "";

static int parse_opt (int key, char* arg, struct argp_state *state);

bool show_tokens = false;
bool show_ast = false;
bool stop_after_parse = false;
bool stop_after_scan = false;
bool stop_after_ast = false;
bool show_tac = false;
bool show_rtl = false;
bool stop_after_tac = false;
bool stop_after_rtl = false;
bool show_asm = true;


int main(int argc, char* argv[]) {
    for (int i = 0 ; i < max_num_registers ; i +=)
    {
        whether_register_available[static_cast<reg>(i)] = true;
    }
    struct argp_option options[] = {
        {"show-tokens", 't', 0, 0, "Show the tokens in FILE.toks (or out.toks)", 9},
        {"show-ast", 'a', 0, 0, "Show AST", 7},
        {"sa-parse", 'p', 0, 0, "Stop after parsing", 6},
        {"sa-scan", 's', 0, 0, "Stop after scanning", 5},
        {"show-tac", 'b', 0, 0, "Show TAC", 4},
        {"sa-ast", 'c', 0, 0, "Stop after AST Tree Creation", 3},
        {"show-rtl", 'r', 0, 0, "Show RTL", 2},
        {"sa-tac", 'l', 0, 0, "Stop after TAC", 1},
        {"show-asm", 'x', 0, 0, "Show ASM", 10},
        {"sa-rtl", 'y', 0, 0, "Stop after RTL", 11},
        { 0 }
    };

    struct argp argp = {options, parse_opt};

    argp_parse (&argp, argc, argv, 0, 0, 0);
    const char* input_file = nullptr;
    input_file = argv[argc-1];


    if (show_tokens) {
        char output_file[1000];
        strcpy(output_file, input_file);
        strcat(output_file, ".toks");
        freopen(output_file, "w", stdout);
        fflush(stdout);
    }


    if (input_file) {
        yyin = fopen(input_file, "r");
        if (!yyin) {
            cerr << "Error: Cannot open file " << input_file << endl;
            return 1;
        }
    }

    if (stop_after_scan) {
        int exit status;
        while (exit_status = yylex());
        return exit_status;
    }
    if (stop_after_scan) stop_after_parse = true;
    if (stop_after_parse) stop_after_ast = true;
    if (stop_after_ast) stop_after_tac = true;
    if (stop_after_tac) stop_after_rtl = true;
    if (stop_after_rtl) show_asm = false;
    if (stop_after_tac) show_rtl = false;
    if (stop_after_ast) show_tac = false;
    if (stop_after_parse) show_ast = false;


    int exit_status = yyparse();
    if (!stop_after_parse) check_ast();
    if (!stop_after_ast)
    {
        for (auto ast_iter : all_ast_statements)
        {
            curr_func = ast_iter.first;
            data_type return_dt = get_return_dt(curr_func);
            saved_tac_temp_count = return_dt != VOID__h;
            tac_temp_count = 0;
            Code* code = new Code();
            for (auto st : ast_iter.second)
            {
                st->make_tac();
                code->append_list(st->code);
            }
            all_tac_statements[ast_iter.first] = *(code->stmt_list);
            curr_func = "";
        }
    }
    if (!stop_after_ast) modify_tac();
    if (!stop_after_tac)
    {
        for (auto st : all_tac_statements)
        {
            curr_func = st.first;
            TAC_to_reg_map.clear();
            all_rtl_statements[st.first] = vector<RTL_Statement*>();
            for (auto stx : st.second)
            {
                stx->make_rtl();
                for (auto rt : stx->all_rtl_statements)
                {
                    all_rtl_statements[st.first].push_back(rt);
                }
            }
            curr_func = "";
        }
    }
    if (!stop_after_rtl)
    {
        for (auto st : all_rtl_statements)
        {
            curr_func = st.first;
            all_asm_statements[st.first] = vector<ASM_Statement*>();
            for (auto stx : st.second)
            {
                stx->make_asm();
                for (auto rt : stx->all_asm_statements)
                {
                    all_asm_statements[st.first].push_back(rt);
                }
            }
            curr_func = "";
        }
        modify_asm();
    }

    if (show_ast) {
        char output_file[1000];
        strcpy(output_file, input_file);
        strcat(output_file, ".ast");
        freopen(output_file, "w", stdout);
        print_ast();
    }

    if (show_tac) {
        char output_file[1000];
        strcpy(output_file, input_file);
        strcat(output_file, ".tac");
        freopen(output_file, "w", stdout);
        print_tac();
    }

    if (show_rtl) {
        char output_file[1000];
        strcpy(output_file, input_file);
        strcat(output_file, ".rtl");
        freopen(output_file, "w", stdout);
        print_rtl();
    }



    if (show_asm) {
        char output_file[1000];
        strcpy(output_file, input_file);
        strcat(output_file, ".spim");
        freopen(output_file, "w", stdout);
        print_asm();
    }
    return exit_status;
}


static int parse_opt (int key, char* arg, struct argp_state *state)
{
    switch(key)
    {
        case 't':
            show_tokens = true;
            break;
        case 'a':
            show_ast = true;
            break;
        case 'p':
            stop_after_parse = true;
            break;
        case 's':
            stop_after_scan = true;
            break;
        case 'c':
            stop_after_ast = true;
            break;
        case 'b':
            show_tac = true;
            break;
        case 'r':
            show_rtl = true;
            break;
        case 'l':
            stop_after_tac = true;
            break;
        case 'x':
            show_asm = true;
            break;
        case 'y':
            stop_after_rtl = true;
            break;
    }
    return 0;
}

void store_token_name(string token_name, char* lexeme, int lineno) {
    if (show_tokens) {
        cout << "\tToken Name: " << token_name;
        cout << " \tLexeme: " << lexeme;
        cout << " \t Lineno: " << lineno << "\n";
    }
}

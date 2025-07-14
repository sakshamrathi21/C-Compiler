#ifndef COMMON_HH
#define COMMON_HH

#include <stdio.h>
#include <assert.h>
#include <argp.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <utility>

using namespace std;

#include "types.hh"
#include "ast.hh"
#include "other_classes.hh"
#include "tac.hh"
#include "rtl.hh"
#include "asm.hh"

void add_variables_to_global_symtab(vec_var_type*);
void add_functions_to_loca_symtab(vec_func_type*);
void throw_error_and_exit(string error_msg);
void print_ast();
void print_tac();
void print_rtl();

#endif
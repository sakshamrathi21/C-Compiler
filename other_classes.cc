#include "other_classes.hh"

vec_var_type::vec_var_type(string * name) {
    add_name(name);
}

vec_var_type::vec_var_type(string * name, int type) {
    add_name_and_type(name, type);
}

void vec_var_type::add_name(string * name) {
    entries.push_back(make_pair(0, name));
}

void vec_var_type::add_name_and_type(string * name, int type) {
    entries.push_back(make_pair(type, name));
}

void vec_var_type::set_type_to_all(int type) {
    for (auto& i : entries) {
        i.first = type;
    }
}

void vec_var_type::append_another(vec_var_type * other) {
    for (const auto& i : other->entries) {
        entries.push_back(i);
    }
}

void push_func_to_vector(func_signature* curr_func, bool isDecl)
{
    for (auto i : global_symtab)
    {
        if (i.first == curr_func->func_name) throw_error_and_exit("Procedure name coincides with a global variable.");
    }
    for (auto iter_func_pair : all_func_signatures)
    {
        func_signature* iter_func = iter_func_pair.first;
        bool iter_fuc_decl = iter_func_pair.second.first;
        if (iter_func->func_name != curr_func->func_name) continue;
        if (iter_fuc_decl == isDecl) throw_error_and_exit("Function defined/declared earlier.");
        if (!iter_fuc_decl && isDecl) throw_error_and_exit("Function defined before declaration.");
        if (!(*iter_func == *curr_func)) throw_error_and_exit("Functions can't be overloaded");
    }
    all_func_signatures.push_back(make_pair(curr_func, make_pair(isDecl, false)));
    name_to_signature[curr_func->func_name + (curr_func->func_name == "main" ? "" : "_")] = curr_func;
}

vector<pair<int, string*>> get_local_var_list(string curr_func) {
    return name_to_signature[curr_func]->local_var_list->entries;
}

vector<pair<int, string*>> get_formal_var_list(string curr_func) {
    return name_to_signature[curr_func]->param_list->entries;
}

bool whether_void(string curr_func) {
    return name_to_signature[curr_func]->dt == VOID__h;
}

data_type get_return_dt(string curr_func) {
    return name_to_signature[curr_func]->dt;
}
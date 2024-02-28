#include "symbol_table.h"

VarTableLine symbol_table[100] = {0};
int symbol_table_len = 0;

bool var_exist(const char *name)
{
    for (int i = 0; i < symbol_table_len; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            return true;
        }
    }
    return false;
}

int get_var_index(const char *name)
{
    for (int i = 0; i < symbol_table_len; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

bool declare_new_var(const char *name)
{
    if (var_exist(name))
        return false;
    VarTableLine *var = &symbol_table[symbol_table_len];
    var->data.p = NULL;
    var->name = malloc(strlen(name) + 1);
    strcpy(var->name, name);
    var->type = Undefined;
    symbol_table_len++;
    return true;
}

bool assign_real_var(const char *name, const float value)
{
    int var_index = get_var_index(name);
    if (var_index == -1)
    {
        return false;
    }
    VarTableLine *var = &symbol_table[var_index];
    var->data.f = value;
    var->type = Real;
    return true;
}

// 返回拥有所有权的Token*
// return NULL when var not found
Token *get_var_value(const char *name)
{
    int var_index = get_var_index(name);
    if (var_index == -1)
        return NULL;
    VarTableLine *var = &symbol_table[var_index];
    if (var->type != Real)
        return NULL;
    Token *res = malloc((sizeof(Token)));
    res->type = Float;
    res->v.f = var->data.f;
    return res;
}

// OptionFloat get_real_var_value(const char *name)
// {
//     for (int i = 0; i < symbol_table_len; i++)
//     {
//         if (strcmp(symbol_table[i].name, name) == 0)
//             return some_float(symbol_table[i].data.f);
//     }
//     return none_float();
// }
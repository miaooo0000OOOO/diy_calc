#include "symbol_table.h"

VarTableLine symbol_table[100] = {0};
int symbol_table_len = 0;

bool assign_real_var(const char *name, const float value)
{
    for (int i = 0; i < symbol_table_len; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            symbol_table[i].data.f = value;
            return true;
        }
    }
    // int symbol_table_max_len = sizeof(symbol_table) / sizeof(VarTableLine);
    // if (symbol_table_len + 1 >= symbol_table_max_len)
    // {
    //     return false;
    // }
    // symbol_table[symbol_table_len].name = malloc(strlen(name) + 1); // 不回收
    // strcpy(symbol_table[symbol_table_len].name, name);
    // symbol_table[symbol_table_len].data.f = value;
    // symbol_table[symbol_table_len].type = Real;
    return false;
}

// 返回拥有所有权的Token*
// return NULL when var not found
Token *get_var_value(const char *name)
{
    for (int i = 0; i < symbol_table_len; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            Token *res = malloc((sizeof(Token)));
            res->type = Float;
            res->v.f = symbol_table[i].data.f;
            return res;
        }
    }
    return NULL;
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
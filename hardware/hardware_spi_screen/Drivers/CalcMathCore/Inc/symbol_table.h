#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "type.h"
#include "tokens.h"
// #include "calculation.h"

typedef enum VarType
{
    Undefined, // 未定义
    Real,      // 实数
    Complex,   // 复数
    Tensor,    // 张量
} VarType;

typedef union VarData
{
    int i;
    float f;
    void *p;
} VarData;

typedef struct VarTableLine
{
    VarType type;
    char *name;
    VarData data;
} VarTableLine;

// extern VarTableLine symbol_table[100];
// extern int symbol_table_len;
bool var_exist(const char *name);
int get_var_index(const char *name);
bool declare_new_var(const char *name);
bool assign_real_var(const char *name, const float value);
Token *get_var_value(const char *name);
// OptionFloat get_real_var_value(const char *name);

#endif
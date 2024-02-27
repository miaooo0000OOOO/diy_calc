#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "type.h"
#include "tokens.h"
// #include "calculation.h"

typedef enum VarType
{
    Real = 0, // 实数
    Complex,  // 复数
    Tensor,   // 张量
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

extern VarTableLine symbol_table[100];
extern int symbol_table_len;

bool assign_real_var(const char *name, const float value);
Token *get_var_value(const char *name);
// OptionFloat get_real_var_value(const char *name);

#endif
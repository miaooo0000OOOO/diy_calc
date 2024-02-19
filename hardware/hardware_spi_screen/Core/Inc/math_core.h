
#ifndef __MATH_CORE_H__
#define __MATH_CORE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define EPSILON 0.001
    typedef size_t usize;

    typedef enum VarType
    {
        Real = 0, // 实数
        Complex,  // 复数
        Tensor,   // 张量
    } VarType;

    typedef struct VarTableLine
    {
        char *name;
        VarType type;
        usize data;
    } VarTableLine;

    extern VarTableLine symbol_table[100];
    extern int symbol_table_len;

    typedef enum TokenType
    {
        Int = 0,          // 整数
        Float,            // 浮点数
        Add,              // 加
        Sub,              // 减
        Neg,              // 负
        Mul,              // 乘
        Div,              // 除
        Pow,              // 幂
        Mod,              // 模
        LeftParenthesis,  // 左括号
        RightParenthesis, // 有括号
        Eq,               // 等号
        Func,             // 函数
        Var,              // 变量
        Fraction          // 分数
    } TokenType;

    typedef struct Token
    {
        TokenType type;
        usize value;
    } Token;

    // a/b +-a
    typedef struct FractionData
    {
        int a; // symbol
        int b; // b > 0
    } FractionData;

    extern Token token_list[1024];
    extern int tokens_len;
    extern int gtoken_ind;

    int parse_to_token_list(char *str);

    typedef struct AST_Node
    {
        Token *token;
        struct AST_Node *left;
        struct AST_Node *right;
    } AST_Node;

    typedef struct BindingPower
    {
        float left;
        float right;
    } BindingPower;

    AST_Node *parse_to_ast();
    Token *get_var_value(char *name);
    Token *calc(AST_Node *node);
    AST_Node *copy_node(AST_Node *node);
    AST_Node *deep_copy_node(AST_Node *node);
    bool assign_real_var(char *name, float value);
    float calc_x_expr(AST_Node *node, float x);
    void recu_free_ast(AST_Node *node);
    AST_Node *solve(AST_Node *node);

#ifdef __cplusplus
}
#endif
#endif /*__MATH_CORE_H__ */

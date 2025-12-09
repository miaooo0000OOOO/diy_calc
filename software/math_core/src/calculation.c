#include "calculation.h"

// 返回拥有所有权的Token*
// 输入拥有所有权的Token*
// 消耗t
// return NULL when t == NULL
static Token *transform_token_and_free(Token *f(const Token *const t), Token *t)
{
    if (t == NULL)
        return NULL;
    Token *temp = f(t);
    free(t);
    return temp;
}

// 返回拥有所有权的AST_Node*
// 输入拥有所有权的AST_Node*
// 消耗node
// return NULL when node == NULL
static AST_Node *transform_ast_and_free(AST_Node *f(const AST_Node *const node), AST_Node *node)
{
    if (node == NULL)
        return NULL;
    AST_Node *temp = f(node);
    recu_free_ast(node);
    return temp;
}

// 返回拥有所有权的Token*
// never return NULL
Token *cast_int2float(const Token *const t)
{
    if (t->type == Float)
        return dump_token(t);
    assert(t->type == Int);
    Token *res = malloc(sizeof(Token));
    res->v.f = (float)t->v.i;
    res->type = Float;
    return res;
}

// 返回拥有所有权的Token*
// return NULL when error
Token *powii(const Token *const l_int, const Token *const r_int)
{
    assert(l_int->type == Int && r_int->type == Int);
    // while (!(l_int->type == Int && r_int->type == Int))
    //     ;
    int li, ri, i, acc;
    bool neg = false;
    li = l_int->v.i;
    ri = r_int->v.i;
    if (li == 0 && ri <= 0)
        // 0^0
        // 0^(-x) (x>0)
        return NULL;
    if (li == 1)
    {
        return dump_token(l_int);
    }
    if (ri < 0)
    {
        ri = -ri;
        neg = true;
    }
    // assert(ri > 0);
    acc = 1;
    for (i = 0; i < ri; i++)
    {
        acc *= li;
    }
    Token *const res = malloc(sizeof(Token));
    if (neg)
    {
        res->type = Float;
        res->v.f = 1. / (float)acc;
    }
    else
    {
        res->type = Int;
        res->v.i = acc;
    }
    return res;
}

// 返回拥有所有权的Token*
// return NULL when r_int->v.i == 0
Token *divii(const Token *const l_int, const Token *const r_int)
{
    if (r_int->v.i == 0)
        return NULL;
    const int l = l_int->v.i;
    const int r = r_int->v.i;
    Token *res = malloc(sizeof(Token));
    if (l % r == 0)
    {
        // l整除r
        res->type = Int;
        res->v.i = l / r;
    }
    else
    {
        res->type = Float;
        res->v.f = (float)l / (float)r;
    }
    return res;
}

// 计算二元运算并返回结果节点
// 优化版本:简化重复逻辑,统一错误处理
static AST_Node *calculate_binary_op(Token *l, Token *r, TokenType op, AST_Node *res)
{
    // 类型已经统一,直接计算
    if (l->type == Float && r->type == Float)
    {
        switch (op)
        {
        case Add:
            l->v.f += r->v.f;
            break;
        case Sub:
            l->v.f -= r->v.f;
            break;
        case Mul:
            l->v.f *= r->v.f;
            break;
        case Div:
            if (r->v.f == 0.f) goto error; // x/0 error
            l->v.f /= r->v.f;
            break;
        case Mod:
            if (r->v.f == 0.f) goto error; // x%0 error
            l->v.f = fmodf(l->v.f, r->v.f);
            break;
        case Pow:
            l->v.f = powf(l->v.f, r->v.f);
            break;
        default: goto error;
        }
        free(r);
        res->token = l;
        return res;
    }
    else if (l->type == Int && r->type == Int)
    {
        switch (op)
        {
        case Add:
            l->v.i += r->v.i;
            free(r);
            res->token = l;
            return res;
        case Sub:
            l->v.i -= r->v.i;
            free(r);
            res->token = l;
            return res;
        case Mul:
            l->v.i *= r->v.i;
            free(r);
            res->token = l;
            return res;
        case Div: {
            // 整数除法可能需要转换为Float
            AST_Node *ret = alloc_node(divii(l, r), NULL, NULL);
            free(l); free(r); recu_free_ast(res);
            return ret;
        }
        case Mod:
            if (r->v.i == 0) goto error; // x%0 error
            l->v.i %= r->v.i;
            free(r);
            res->token = l;
            return res;
        case Pow: {
            // 幂运算可能需要转换为Float
            AST_Node *ret = alloc_node(powii(l, r), NULL, NULL);
            free(l); free(r); recu_free_ast(res);
            return ret;
        }
        default: goto error;
        }
    }

error:
    free(l);
    free(r);
    recu_free_ast(res);
    return NULL;
}

// 返回持有所有权的AST_Node*
// return NULL when error
AST_Node *op_polymorphism(const AST_Node *const base_l, const AST_Node *const base_r, const TokenType op)
{
    assert(base_l != NULL && base_r != NULL);

    // 创建结果节点
    AST_Node *res = malloc(sizeof(AST_Node));
    res->left = res->right = NULL;
    res->token = NULL;

    // 复制token(获得所有权)
    Token *l = dump_token(base_l->token);
    Token *r = dump_token(base_r->token);

    // 类型统一
    if (l->type == Float && r->type == Int)
    {
        r = transform_token_and_free(cast_int2float, r);
    }
    else if (l->type == Int && r->type == Float)
    {
        l = transform_token_and_free(cast_int2float, l);
    }

    return calculate_binary_op(l, r, op, res);
}

// 前向声明
int simple_pow(const int x, const int y);

// 二分查找根的数据结构
typedef struct {
    const AST_Node *node;
    float xl, xr;
    float l, r;
    float tol;
    int max_iter;
    int expand_iter;
    bool solution_found;
} DichotomyState;

// 初始化二分查找状态
static void init_dichotomy_state(DichotomyState *state, const AST_Node *node,
                                 float left_x, float right_x, float tol, int max_iter)
{
    state->node = node;
    state->xl = left_x;
    state->xr = right_x;
    state->tol = tol;
    state->max_iter = max_iter;
    state->expand_iter = (max_iter == 1000) ? 4 : 8; // 根据迭代次数选择扩展范围
    state->solution_found = false;

    // 计算端点值
    state->l = get_delta(node, state->xl);
    state->r = get_delta(node, state->xr);
}

// 扩展搜索区间直到找到根或确定无解
static bool expand_search_interval(DichotomyState *state)
{
    if (state->l * state->r <= 0)
        return true; // 已经找到不同号的区间

    for (int i = 1; i < state->expand_iter; i++)
    {
        float step = (state->xr - state->xl) / simple_pow(2, i);
        for (int j = 0; j < simple_pow(2, i); j += 2)
        {
            state->xr = state->xl + (step + step * j);
            state->r = get_delta(state->node, state->xr);
            if (state->l * state->r <= 0)
                return true;
        }
    }
    return false;
}

// 执行二分查找,返回收敛的x值或NAN
static float perform_dichotomy(DichotomyState *state)
{
    // 检查端点是否已经是根
    if (state->l == 0.f)
        return state->xl;
    if (state->r == 0.f)
        return state->xr;

    // 如果无法找到不同号的区间,返回NAN
    if (!expand_search_interval(state))
        return NAN;

    float xm = NAN;
    float prev_xm;

    for (int i = 0; i < state->max_iter; i++)
    {
        state->l = get_delta(state->node, state->xl);
        prev_xm = xm;
        xm = (state->xl + state->xr) / 2.f;

        // 检查是否收敛或达到浮点精度极限
        if (xm == prev_xm || state->xr - state->xl <= state->tol)
        {
            state->solution_found = true;
            return xm;
        }

        float m = get_delta(state->node, xm);

        // 找到一个根
        if (m == 0.f)
        {
            state->solution_found = true;
            return xm;
        }

        // 缩小区间
        if (state->l * m < 0)
            state->xr = xm;
        else
            state->xl = xm;
    }

    // 检查最后一次迭代是否达到误差容忍度
    if (state->xr - state->xl <= state->tol)
    {
        state->solution_found = true;
        return xm;
    }

    return NAN;
}

// 返回拥有所有权的AST_Node*
// never return NULL
AST_Node *neg(const AST_Node *const node)
{
    assert(node->token->type == Int || node->token->type == Float);
    AST_Node *const res = alloc_node(malloc(sizeof(Token)), NULL, NULL);
    if (node->token->type == Int)
    {
        res->token->type = Int;
        res->token->v.i = -node->token->v.i;
    }
    else if (node->token->type == Float)
    {
        res->token->type = Float;
        res->token->v.f = -node->token->v.f;
    }
    return res;
}

// -1 no
int try_sqrti(int x)
{
    int i;
    if (x == 0)
        return 0;
    for (i = 1;; i += 2)
    {
        x -= i;
        if (x < 0)
            return -1;
        else if (x == 0)
        {
            return (i + 1) / 2;
        }
    }
}

// 返回拥有所有权的Token*
// return NULL when error
Token *sqrt_token(const Token *const x)
{
    assert(x->type == Int || x->type == Float);
    Token *t = dump_token(x);
    if (t->type == Int)
    {
        if (t->v.i < 0)
        {
            free(t);
            return NULL;
        }
        int temp = try_sqrti(t->v.i);
        if (temp != -1)
        {
            t->v.i = temp;
            return t;
        }
        else
        {
            t->v.f = sqrtf(t->v.i);
            t->type = Float;
            if (t->v.f == NAN)
            {
                free(t);
                return NULL;
            }
            return t;
        }
    }
    t->v.f = sqrtf(t->v.f);
    if (t->v.f == NAN)
    {
        free(t);
        return NULL;
    }
    return t;
}

// 返回拥有所有权的Token*
// return NULL when error
Token *fx_float_token(const Token *const x, float f(float))
{
    assert(x->type == Int || x->type == Float);
    Token *res;
    if (x->type == Int)
    {
        res = cast_int2float(x);
    }
    else if (x->type == Float)
    {
        res = dump_token(x);
    }
    else
    {
        return NULL;
    }
    res->v.f = f(res->v.f);
    if (!isfinite(res->v.f) && !isnan(res->v.f))
    {
        free(res);
        return NULL;
    }
    return res;
}

float cotf(float x)
{
    return 1. / tanf(x);
}

// 返回拥有所有权的AST_Node*
// return NULL when error
AST_Node *func_call(const AST_Node *const fn_node)
{
    if (fn_node == NULL)
        return NULL;
    assert(fn_node->token->type == Func);
    assert(fn_node->left->token->type == Int || fn_node->left->token->type == Float);
    const float *const fp = &fn_node->left->token->v.f;
    const int *const ip = &fn_node->left->token->v.i;
    const char *fn_name = fn_node->token->v.name;
    const Token *const fn_inner = fn_node->left->token;
    assert(fn_inner->type == Int || fn_inner->type == Float);
    if (strcmp(fn_name, "sqrt") == 0)
    {
        return alloc_node(sqrt_token(fn_inner), NULL, NULL);
    }
    else if (strcmp(fn_name, "sin") == 0)
    {
        return alloc_node(fx_float_token(fn_inner, sinf), NULL, NULL);
    }
    else if (strcmp(fn_name, "cos") == 0)
    {
        return alloc_node(fx_float_token(fn_inner, cosf), NULL, NULL);
    }
    else if (strcmp(fn_name, "tan") == 0)
    {
        return alloc_node(fx_float_token(fn_inner, tanf), NULL, NULL);
    }
    else if (strcmp(fn_name, "cot") == 0)
    {
        return alloc_node(fx_float_token(fn_inner, cotf), NULL, NULL);
    }
    else if (strcmp(fn_node->token->v.name, "ln") == 0)
    {
        // e为底
        return alloc_node(fx_float_token(fn_inner, logf), NULL, NULL);
    }
    else if (strcmp(fn_node->token->v.name, "log") == 0)
    {
        // 10为底
        return alloc_node(fx_float_token(fn_inner, log10f), NULL, NULL);
    }
    else if (strcmp(fn_node->token->v.name, "abs") == 0)
    {
        AST_Node *res = alloc_node(malloc(sizeof(Token)), NULL, NULL);
        if (fn_inner->type == Int)
        {
            res->token->type = Int;
            res->token->v.i = abs(*ip);
        }
        else if (fn_inner->type == Float)
        {
            res->token->type = Float;
            res->token->v.i = fabsf(*fp);
        }
        return res;
    }
    else
    {
        return NULL;
    }
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
            res->v.f = *((float*)symbol_table[i].data);
            return res;
        }
    }
    return NULL;
}

// 返回拥有所有权的AST_Node*
// return NULL when error
AST_Node *recu_calc(const AST_Node *const node)
{
    if (node == NULL)
        return NULL;
    if (node->token->type == Neg)
    {
        AST_Node *neg_inner = recu_calc(node->left);
        AST_Node *res = neg(neg_inner);
        recu_free_ast(neg_inner);
        return res;
    }
    if (node->token->type == Func)
    {
        AST_Node *cp = deep_copy_node(node);
        cp->left = transform_ast_and_free(recu_calc, cp->left);
        return transform_ast_and_free(func_call, cp);
    }
    if (node->token->type == Var)
    {
        return alloc_node(get_var_value(node->token->v.name), NULL, NULL);
    }
    if (node->token->type == Int || node->token->type == Float)
        // 基本情况
        return copy_node(node);
    // 递归情况
    AST_Node *l_res = recu_calc(node->left);
    AST_Node *r_res = recu_calc(node->right);
    AST_Node *res = op_polymorphism(l_res, r_res, node->token->type);
    recu_free_ast(l_res);
    recu_free_ast(r_res);
    return res;
}

// 返回拥有所有权的Token*
// return NULL when error
Token *calc(const AST_Node *const node)
{
    if (node == NULL)
        return NULL;
    if (node->token->type == Eq)
    {
        // 方程
        return NULL;
    }
    AST_Node *res = recu_calc(node);
    if (res == NULL)
        return NULL;
    Token *ret = dump_token(res->token);
    recu_free_ast(res);
    return ret;
}

bool assign_real_var(const char *name, const float value)
{
    for (int i = 0; i < symbol_table_len; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            float *fp = (float *)&symbol_table[i].data;
            *fp = value;
            return true;
        }
    }
    return false;
}

float get_delta(const AST_Node *const node, float x)
{
    assert(assign_real_var("x", x));
    Token *tl, *tr;
    tl = calc(node->left);
    tr = calc(node->right);
    tl = transform_token_and_free(cast_int2float, tl);
    tr = transform_token_and_free(cast_int2float, tr);
    float l, r;
    l = tl->v.f;
    r = tr->v.f;
    free(tl);
    free(tr);
    return l - r;
}

float calc_x_expr(const AST_Node *const node, float x)
{
    assign_real_var("x", x);
    Token *t;
    t = calc(node);
    t = transform_token_and_free(cast_int2float, t);
    return t->v.f;
}

AST_Node *solve(const AST_Node *const node, const Token *const initial_x)
{
    assert(node != NULL);
    if (node->token->type != Eq)
    {
        return NULL;
    }
    // assign x
    float x, y, y_, dydx;
    Token *temp;
    if (initial_x == NULL)
    {
        temp = get_var_value("x");
        assert(temp != NULL);
        assert(temp->type == Float);
        x = temp->v.f;
        free(temp);
    }
    else
    {
        assert(initial_x->type == Float);
        x = initial_x->v.f;
    }
    for (int i = 0; i < 1000; i++)
    {
        y = get_delta(node, x - EPSILON);
        y_ = get_delta(node, x + EPSILON);
        dydx = (y_ - y) / EPSILON / 2;
        if (dydx == 0)
        {
            continue;
        }
        x = x - y / dydx;
    }
    AST_Node *eq = malloc(sizeof(AST_Node));
    AST_Node *x_node = malloc(sizeof(AST_Node));
    AST_Node *value_node = malloc(sizeof(AST_Node));

    eq->token = malloc(sizeof(Token));
    eq->token->type = Eq;
    eq->token->v.i = 0;
    eq->left = x_node;
    eq->right = value_node;
    x_node->token = malloc(sizeof(Token));
    x_node->token->type = Var;
    strncpy(x_node->token->v.name, "x", 15);
    x_node->token->v.name[15] = '\0';
    x_node->left = x_node->right = NULL;
    value_node->token = malloc(sizeof(Token));
    value_node->token->type = Float;
    value_node->token->v.f = x;
    value_node->left = value_node->right = NULL;
    return eq;
}

// float recu_solve_dichotomy_float(const AST_Node *const node, const Token *const left_x, const Token *const right_x)
// {
// }

// x^y
int simple_pow(const int x, const int y)
{
    int acc = 1;
    int i = 0;
    for (i = 0; i < y; i++)
    {
        acc *= x;
    }
    return acc;
}

AST_Node *ast_x_eq_float(float x)
{
    AST_Node *eq = malloc(sizeof(AST_Node));
    AST_Node *x_node = malloc(sizeof(AST_Node));
    AST_Node *value_node = malloc(sizeof(AST_Node));

    eq->token = malloc(sizeof(Token));
    eq->token->type = Eq;
    eq->token->v.i = 0;
    eq->left = x_node;
    eq->right = value_node;
    x_node->token = malloc(sizeof(Token));
    x_node->token->type = Var;
    strncpy(x_node->token->v.name, "x", 15);
    x_node->token->v.name[15] = '\0';
    x_node->left = x_node->right = NULL;
    value_node->token = malloc(sizeof(Token));
    value_node->token->type = Float;
    value_node->token->v.f = x;
    value_node->left = value_node->right = NULL;
    return eq;
}

AST_Node *solve_dichotomy(const AST_Node *const node, const Token *const left_x, const Token *const right_x)
{
    assert(node != NULL && node->token->type == Eq);
    assert(left_x != NULL && left_x->type == Float);
    assert(right_x != NULL && right_x->type == Float);

    // 使用辅助函数进行二分查找
    DichotomyState state;
    init_dichotomy_state(&state, node, left_x->v.f, right_x->v.f, EPSILON, 100000);

    float solution = perform_dichotomy(&state);

    if (!state.solution_found)
        return NULL;

    return ast_x_eq_float(solution);
}

OptionFloat some_float(float value)
{
    OptionFloat of;
    of.valid = true;
    of.value = value;
    return of;
}

OptionFloat none_float()
{
    OptionFloat of;
    of.valid = false;
    of.value = 0;
    return of;
}

OptionFloat solve_dichotomy_float(const AST_Node *const node, float left_x, float right_x)
{
    assert(node != NULL && node->token->type == Eq);

    // 使用辅助函数进行二分查找
    DichotomyState state;
    init_dichotomy_state(&state, node, left_x, right_x, EPSILON, 1000);

    float solution = perform_dichotomy(&state);

    if (!state.solution_found)
        return none_float();

    return some_float(solution);
}
#include "ast.h"

// ==================== ASTParser State Structure ====================

// Forward declaration for ASTParser
typedef struct ASTParser ASTParser;

// AST parser state structure to encapsulate all parsing state
struct ASTParser {
    Token *tokens;
    int token_index;
    int token_count;
};

// ==================== Forward Declarations ====================

static AST_Node *parse_atom(ASTParser *parser);
static AST_Node *parse_parenthesis(ASTParser *parser);
static AST_Node *parse_function_call(ASTParser *parser);
static AST_Node *parse_prefix_operator(ASTParser *parser, Token *op_token);
static AST_Node *expr_bp_ast(ASTParser *parser, int min_bp);

// Initialize parser state
static void init_ast_parser(ASTParser *parser, Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_index = 0;
    parser->token_count = token_count;
}

// Get current token without advancing
static const Token *peek_token_ast(const ASTParser *parser) {
    if (parser->token_index >= parser->token_count)
        return NULL;
    return &parser->tokens[parser->token_index];
}

// Get current token and advance
static Token *next_token_ast(ASTParser *parser) {
    if (parser->token_index >= parser->token_count)
        return NULL;
    return &parser->tokens[parser->token_index++];
}

/// 返回拥有所有权的AST_Node*
// return NULL when t == NULL
AST_Node *alloc_node(Token *const t, AST_Node *const left, AST_Node *const right)
{
    if (t == NULL)
        return NULL;
    AST_Node *const node = malloc(sizeof(AST_Node));
    node->token = t;
    node->left = left;
    node->right = right;
    return node;
}

// ==================== Binding Power Functions ====================

// private
BindingPower prefix_binding_power(const TokenType op)
{
    BindingPower bp;
    if (op == Neg)
    {
        bp.left = -1;
        bp.right = 7;
    }
    else
    {
        bp.left = bp.right = -1;
    }
    return bp;
}

// private
BindingPower infix_binding_power(const TokenType op)
{
    BindingPower bp;
    if (op == Add || op == Sub)
    {
        bp.left = 3;
        bp.right = 4;
    }
    else if (op == Mul || op == Div || op == Mod)
    {
        bp.left = 5;
        bp.right = 6;
    }
    else if (op == Pow)
    {
        bp.left = 9;
        bp.right = 8;
    }
    else if (op == Eq)
    {
        bp.left = 1;
        bp.right = 2;
    }
    else
    {
        bp.right = bp.left = -1;
    }
    return bp;
}

// ==================== Atomic Expression Parsing ====================

// Parse atomic expressions: integers, floats, variables
static AST_Node *parse_atom(ASTParser *parser)
{
    Token *temp_token = dump_token(next_token_ast(parser));
    AST_Node *atom = malloc(sizeof(AST_Node));

    atom->token = temp_token;
    atom->left = atom->right = NULL;

    return atom;
}

// Parse parenthesized expressions
static AST_Node *parse_parenthesis(ASTParser *parser)
{
    // Skip left parenthesis token
    next_token_ast(parser);

    // Parse expression inside parentheses
    AST_Node *expr = expr_bp_ast(parser, 0);

    // Expect right parenthesis
    const Token *right_paren = peek_token_ast(parser);
    assert(right_paren != NULL && right_paren->type == RightParenthesis);
    next_token_ast(parser); // consume right parenthesis

    return expr;
}

// Forward declaration for recursive expression parsing
static AST_Node *expr_bp_ast(ASTParser *parser, int min_bp);

// Forward declaration for recursive expression parsing
static AST_Node *expr_bp_ast(ASTParser *parser, int min_bp);

// Parse prefix operators (currently only negation)
static AST_Node *parse_prefix_operator(ASTParser *parser, Token *op_token)
{
    const BindingPower bp = prefix_binding_power(op_token->type);
    AST_Node *rhs = expr_bp_ast(parser, bp.right);

    AST_Node *op_node = malloc(sizeof(AST_Node));
    op_node->token = op_token;
    op_node->left = rhs;
    op_node->right = NULL;

    return op_node;
}

// Parse function calls
static AST_Node *parse_function_call(ASTParser *parser)
{
    Token *func_token = dump_token(next_token_ast(parser));

    // Expect left parenthesis
    const Token *left_paren = peek_token_ast(parser);
    assert(left_paren != NULL && left_paren->type == LeftParenthesis);
    next_token_ast(parser); // consume left parenthesis

    // Parse function argument
    AST_Node *arg = expr_bp_ast(parser, 0);

    // Expect right parenthesis
    const Token *right_paren = peek_token_ast(parser);
    assert(right_paren != NULL && right_paren->type == RightParenthesis);
    next_token_ast(parser); // consume right parenthesis

    // Create function node
    AST_Node *func_node = malloc(sizeof(AST_Node));
    func_node->token = func_token;
    func_node->left = arg;
    func_node->right = NULL;

    return func_node;
}

// New expression parser using ASTParser state
static AST_Node *expr_bp_ast(ASTParser *parser, int min_bp)
{
    // Parse prefix expression
    const Token *first_token = peek_token_ast(parser);
    if (first_token == NULL)
    {
        // Should not happen with well-formed input
        return NULL;
    }

    AST_Node *lhs;
    TokenType first_type = first_token->type;

    if (first_type == Int || first_type == Float || first_type == Var)
    {
        // Atom: integer, float or variable
        lhs = parse_atom(parser);
    }
    else if (first_type == LeftParenthesis)
    {
        // Parenthesized expression
        lhs = parse_parenthesis(parser);
    }
    else if (first_type == Func)
    {
        // Function call
        lhs = parse_function_call(parser);
    }
    else if (first_type == Neg)
    {
        // Prefix operator (negation)
        Token *op_token = dump_token(next_token_ast(parser));
        lhs = parse_prefix_operator(parser, op_token);
    }
    else
    {
        // Unexpected token
        return NULL;
    }

    // Parse infix expressions
    for (;;)
    {
        const Token *op = peek_token_ast(parser);
        if (op == NULL)
            break;

        const BindingPower bp = infix_binding_power(op->type);
        if (bp.left == -1 || bp.right == -1)
            break;

        if (bp.left < min_bp)
            break;

        // Consume operator token
        next_token_ast(parser);

        // Parse right operand
        AST_Node *rhs = expr_bp_ast(parser, bp.right);

        // Build binary operation node
        lhs = alloc_node(dump_token(op), lhs, rhs);
    }

    return lhs;
}

// 返回拥有所有权的AST_Node*
// never return NULL
AST_Node *parse_to_ast()
{
    // Create parser with global token list
    ASTParser parser;
    init_ast_parser(&parser, token_list, tokens_len);

    // Parse expression
    AST_Node *node = expr_bp_ast(&parser, 0);

    return node;
}

// private
void print_tree_branch(const int layer)
{
    if (layer == 0)
        return;
    int i;
    for (i = 0; i < layer - 1; i++)
        printf(TAB);
    printf(FOLD_TAB);
}

void print_ast(const AST_Node *const node, const int layer)
{
    if (node == NULL)
        return;

    print_tree_branch(layer);
    // Atom
    if (node->token->type == Int)
    {
        printf("Int(%d)\n", node->token->v.i);
        return; // ！
    }
    else if (node->token->type == Float)
    {
        printf("Float(%f)\n", node->token->v.f);
        return;
    }
    else if (node->token->type == Var)
    {
        printf("Var(%s)\n", node->token->v.name);
        return;
    }

    // 递归
    printf("token: ");
    switch (node->token->type)
    {
    case Add:
        printf("+");
        break;
    case Sub:
        printf("Sub");
        break;
    case Mul:
        printf("*");
        break;
    case Div:
        printf("/");
        break;
    case Neg:
        printf("Neg");
        break;
    case Pow:
        printf("^");
        break;
    case Mod:
        printf("%%");
        break;
    case Func:
        printf("Func(%s)", node->token->v.name);
        break;
    case Eq:
        printf("=");
        break;
    default:
        printf("还没实现");
        break;
    }
    printf("\n");
    // for (i = 0; i < layer; i++)
    //     printf(TAB);
    print_ast(node->left, layer + 1);
    // for (i = 0; i < layer; i++)
    //     printf(TAB);
    print_ast(node->right, layer + 1);
}

AST_Node *copy_node(const AST_Node *const node)
{
    AST_Node *new_node = malloc(sizeof(AST_Node));
    new_node->left = node->left;
    new_node->right = node->right;
    new_node->token = dump_token(node->token);
    return new_node;
}

AST_Node *deep_copy_node(const AST_Node *const node)
{
    AST_Node *new_node;
    new_node = copy_node(node);
    if (node->token->type == Int || node->token->type == Float || node->token->type == Var)
    {
    }
    else if (node->token->type == Func || node->token->type == Neg)
    {
        new_node->left = deep_copy_node(node->left);
    }
    else
    {
        new_node->left = deep_copy_node(node->left);
        new_node->right = deep_copy_node(node->right);
    }
    return new_node;
}

void deep_free_ast(const AST_Node *const node)
{
    if (node == NULL)
        return;
    if (node->token != NULL)
        free(node->token);
    deep_free_ast(node->right);
    free(node->right);
    deep_free_ast(node->left);
    free(node->left);
    // free(node);
}

void recu_free_ast(const AST_Node *const node)
{
    deep_free_ast(node);
    free((void*)node);
}


#include "tokens.h"

// Token parser state structure to encapsulate all parsing state
typedef struct TokenParser {
    const char *input;
    usize input_len;
    Token *tokens;
    int tokens_cap;
    int tokens_len;
    int token_index;
    char prev_char;
    int parenthesis_layer;
} TokenParser;

// Initialize parser state
static void init_parser(TokenParser *parser, const char *input, Token *tokens, int tokens_cap) {
    parser->input = input;
    parser->input_len = strlen(input);
    parser->tokens = tokens;
    parser->tokens_cap = tokens_cap;
    parser->tokens_len = 0;
    parser->token_index = 0;
    parser->prev_char = '\0';
    parser->parenthesis_layer = 0;
}

// Character type classification functions
bool char_in_num(const char c)
{
    return '0' <= c && c <= '9';
}

bool char_is_symbol(const char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '(' || c == ')' || c == '=';
}

bool char_in_alphabet(const char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool char_in_alphabet_and_underline(const char c)
{
    return char_in_alphabet(c) || c == '_';
}

bool char_in_hex(const char c)
{
    return char_in_num(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

bool char_in_a2f(const char c)
{
    return ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

// 返回拥有所有权的Token*
Token *new_token_from_float(float x)
{
    Token *res = malloc(sizeof(Token));
    res->type = Float;
    res->v.f = x;
    return res;
}

// 返回拥有所有权的Token*
Token *new_token_from_int(int x)
{
    Token *res = malloc(sizeof(Token));
    res->type = Int;
    res->v.i = x;
    return res;
}

// ==================== Number Parsing Logic ====================

// 判断字符串是何种数字类型
// 0 Err 1 十六进制整数 2 十进制整数 3 八进制整数 4 二进制整数 5 浮点数
static char classify_number_string(const char str[])
{
    const usize len = strlen(str);
    if (len <= 0)
    {
        return 0; // Err
    }
    if (str[0] == '0') // 0开头的16进制或8进制数或2进制数
    {
        if (len <= 1)
        {
            return 2; // 十进制数字0
        }
        switch (str[1])
        {
        case '.':
            return 5; // 浮点数
        case 'b':
        case 'B':
            return 4; // 二进制整数
        case 'x':
        case 'X':
            return 1; // 十六进制整数
        default:
            return 3; // 八进制整数
        }
    }
    usize i;
    for (i = 1; i < len; i++)
    {
        if (!char_in_num(str[i]))
        {
            if (str[i] == '.')
            {
                return 5; // 浮点数
            }
            else
            {
                return 2; // 十进制整数
            }
        }
    }
    return 2; // 十进制整数
}

// 解析二进制字符串
static unsigned long parse_binary_number(const char *str, const usize len)
{
    assert(len > 2);
    assert(str[0] == '0' && (str[1] == 'b' || str[1] == 'B'));
    unsigned long n = 0;
    for (usize i = 2; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1')
            break;
        n = (n << 1) | (str[i] - '0');
    }
    return n;
}

// 验证数字字符串的有效性并计算结束位置
static usize validate_and_advance_number(const char *str, usize start, usize len, char num_type)
{
    usize i = start;

    // Skip prefix
    switch (num_type)
    {
    case 1: // 十六进制
        i += 2;
        break;
    case 3: // 八进制
        i++;
        break;
    case 4: // 二进制
        i += 2;
        break;
    default:
        break;
    }

    // Validate remaining characters
    for (; i < len; i++)
    {
        char c = str[i];

        if (num_type == 1 && c == '.') // 十六进制
        {
            return 0; // Error: hex with decimal point
        }
        else if (num_type == 2 && char_in_a2f(c)) // 十进制
        {
            return 0; // Error: decimal with hex chars
        }
        else if (num_type == 3 && (c == '8' || c == '9' || char_in_a2f(c))) // 八进制
        {
            return 0; // Error: octal with decimal/hex chars
        }
        else if (num_type == 4 && (char_in_hex(c) && !(c == '0' || c == '1'))) // 二进制
        {
            return 0; // Error: binary with non-binary chars
        }

        // If not a valid number character, stop
        if (!(c == '.' || char_in_hex(c) || c == 'x' || c == 'X'))
        {
            break;
        }
    }

    return i;
}

// 解析数字并创建token
static bool parse_number(TokenParser *parser, usize *pos, Token *token)
{
    char num_type = classify_number_string(&parser->input[*pos]);
    if (num_type == 0)
    {
        return false; // Error
    }

    usize end_pos = validate_and_advance_number(parser->input, *pos, parser->input_len, num_type);
    if (end_pos == 0)
    {
        return false; // Validation failed
    }

    switch (num_type)
    {
    case 4: // 二进制整数
        token->type = Int;
        token->v.i = parse_binary_number(&parser->input[*pos], parser->input_len - *pos);
        break;
    case 5: // 浮点数
        token->type = Float;
        token->v.f = strtof(&parser->input[*pos], NULL);
        break;
    default: // 其他整数
        token->type = Int;
        token->v.i = strtoul(&parser->input[*pos], NULL, 0);
        break;
    }

    *pos = end_pos - 1;
    return true;
}

// ==================== Identifier Parsing Logic ====================

// 解析标识符（变量或函数）并创建token
static bool parse_identifier(TokenParser *parser, usize *pos, Token *token)
{
    const usize MAX_NAME_LEN = 15;
    char *name = malloc(sizeof(char) * (MAX_NAME_LEN + 1));

    usize k;
    for (k = *pos; k < parser->input_len + 1; k++)
    {
        if (!char_in_alphabet_and_underline(parser->input[k]))
        {
            name[k - *pos] = '\0';

            if (parser->input[k] == '(') // 函数
            {
                token->type = Func;
            }
            else // 变量
            {
                token->type = Var;
                symbol_table[symbol_table_len].type = Real;
                symbol_table[symbol_table_len].name = (char *)name;
                symbol_table[symbol_table_len].data = NULL;
                symbol_table_len++;
            }

            token->v.p = name;
            break;
        }
        else
        {
            if (k - *pos >= MAX_NAME_LEN)
            {
                free(name);
                return false; // Name too long
            }
            name[k - *pos] = parser->input[k];
        }
    }

    *pos = k - 1;
    return true;
}

// ==================== Operator Parsing Logic ====================

// 解析减号（可能是减号或负号）
static void parse_minus_operator(TokenParser *parser, usize pos, Token *token)
{
    char prev_c = parser->prev_char;
    if (char_in_num(prev_c) || prev_c == ')' || char_in_alphabet(prev_c)) // 是减号的情况
    {
        token->type = Sub;
    }
    else // 是负号的情况
    {
        token->type = Neg;
    }
    token->v.i = 0;
}

// 解析左括号
static bool parse_left_parenthesis(TokenParser *parser, Token *token)
{
    token->type = LeftParenthesis;
    token->v.i = parser->parenthesis_layer;
    parser->parenthesis_layer++;
    return true;
}

// 解析右括号
static bool parse_right_parenthesis(TokenParser *parser, Token *token)
{
    if (parser->parenthesis_layer == 0)
    {
        return false; // Unmatched parenthesis
    }
    parser->parenthesis_layer--;
    token->type = RightParenthesis;
    token->v.i = parser->parenthesis_layer;
    return true;
}

// 返回Token的不可变引用
// return NULL when out of index
const Token *const peek_token()
{
    if (gtoken_ind >= tokens_len)
        return NULL;
    return &token_list[gtoken_ind];
}

// 返回Token的不可变引用
// return NULL when no next
Token *next_token()
{
    if (gtoken_ind >= tokens_len)
        return NULL;
    Token *temp = &token_list[gtoken_ind];
    gtoken_ind++;
    return temp;
}

// 返回拥有所有权的Token*
// return NULL when t == NULL
Token *dump_token(const Token *const t)
{
    if (t == NULL)
        return NULL;
    Token *new_token = malloc(sizeof(Token));
    new_token->type = t->type;
    new_token->v = t->v;
    return new_token;
}

void copy_token(const Token *const from, Token *const to)
{
    to->type = from->type;
    to->v = from->v;
}

// ==================== Syntax Validation ====================

// adja_char_stynax[i][j] 是否合法
// 字符映射i，上一个字符映射j
// 合法 0 非法 1
static const char adja_char_stynax[8][8] = {
    // 数字 +*/^% - ( ) = 字母 空
    {0, 0, 0, 0, 1, 0, 0, 0}, // 数字
    {0, 1, 1, 0, 0, 1, 0, 1}, //+*/^%
    {0, 1, 1, 0, 0, 0, 0, 0}, //-
    {1, 0, 0, 0, 1, 0, 0, 0}, //(
    {0, 1, 1, 1, 0, 1, 0, 1}, //)
    {0, 1, 1, 1, 0, 1, 0, 1}, //=
    {0, 0, 0, 0, 1, 0, 0, 0}, // 字母
    {0, 1, 1, 1, 0, 1, 0, 0}  // 空
};

static int adja_char_map(const char c)
{
    if (char_in_num(c))
    {
        return 0;
    }
    else if (c == '+' || c == '*' || c == '/' || c == '^' || c == '%')
    {
        return 1;
    }
    else if (c == '-')
    {
        return 2;
    }
    else if (c == '(')
    {
        return 3;
    }
    else if (c == ')')
    {
        return 4;
    }
    else if (c == '=')
    {
        return 5;
    }
    else if (char_in_alphabet(c))
    {
        return 6;
    }
    else
    {
        // '\0'
        return 7;
    }
}

// private
static bool check_adja_char_stynax(const char c, const char prev_c)
{
    return adja_char_stynax[adja_char_map(c)][adja_char_map(prev_c)] == 0;
}

Token token_list[1024] = {0};
int tokens_len = 0;
int gtoken_ind = 0;

// return -1 when error
// return len(tokens)
int parse_to_token_list(const char str[])
{
    const usize len = strlen(str);
    if (len == 0)
    {
        return -1;
    }

    // Initialize parser state
    TokenParser parser;
    init_parser(&parser, str, token_list, 1024);

    // Main parsing loop
    for (usize i = 0, j = 0; i < len; i++, j++)
    {
        char c = str[i];

        // Skip whitespace
        if (c == ' ' || c == '\n' || c == '\r')
        {
            j--;
            continue;
        }

        // Check syntax validity
        if (!check_adja_char_stynax(c, parser.prev_char))
        {
            return -1; // Syntax error
        }

        Token *current_token = &token_list[j];

        // Parse based on character type
        switch (c)
        {
        case '+':
            current_token->type = Add;
            current_token->v.i = 0;
            break;

        case '-':
            parse_minus_operator(&parser, i, current_token);
            break;

        case '*':
            current_token->type = Mul;
            current_token->v.i = 0;
            break;

        case '/':
            current_token->type = Div;
            current_token->v.i = 0;
            break;

        case '^':
            current_token->type = Pow;
            current_token->v.i = 0;
            break;

        case '%':
            current_token->type = Mod;
            current_token->v.i = 0;
            break;

        case '(':
            if (!parse_left_parenthesis(&parser, current_token))
            {
                return -1;
            }
            break;

        case ')':
            if (!parse_right_parenthesis(&parser, current_token))
            {
                return -1;
            }
            break;

        case '=':
            if (parser.parenthesis_layer != 0)
            {
                return -1; // Parenthesis mismatch
            }
            current_token->type = Eq;
            current_token->v.i = 0;
            break;

        default:
            if (char_in_num(c))
            {
                if (!parse_number(&parser, &i, current_token))
                {
                    return -1; // Number parsing error
                }
            }
            else if (char_in_alphabet(c))
            {
                if (!parse_identifier(&parser, &i, current_token))
                {
                    return -1; // Identifier parsing error
                }
            }
            else
            {
                return -1; // Unknown character
            }
            break;
        }

        parser.prev_char = str[i];
        parser.token_index = j;
    }

    // Final validation
    if (!check_adja_char_stynax('\0', parser.prev_char))
    {
        return -1; // Syntax error at end
    }
    if (parser.parenthesis_layer != 0)
    {
        return -1; // Unmatched parentheses
    }

    tokens_len = parser.token_index + 1;
    return tokens_len;
}

void print_terimal_token(const Token *const t, const bool newline)
{
    switch (t->type)
    {
    case Int:
        printf("Int(%d)", t->v.i);
        break;
    case Float:
        printf("Float(%f)", t->v.f);
        break;
    case Var:
        printf("Var(%s)", (char *)t->v.p);
    default:
        break;
    }
    if (newline)
        printf("\n");
}

void print_token_list()
{
    int i;
    // Token this_token;
    for (i = 0; i < tokens_len; i++)
    {
        switch (token_list[i].type)
        {
        case Int:
            printf("Int(%d)", token_list[i].v.i);
            break;
        case Float:
            printf("Float(%f)", token_list[i].v.f);
            break;
        case Add:
            printf("+");
            break;
        case Neg:
            printf("Neg");
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
        case Mod:
            printf("%%");
            break;
        case Pow:
            printf("^");
            break;
        case Func:
            printf("Func{name: %s}", (char *)token_list[i].v.p);
            break;
        case Var:
            printf("Var{name: %s}", (char *)token_list[i].v.p);
            break;
        case LeftParenthesis:
            printf("( Layer: %d", token_list[i].v.i);
            break;
        case RightParenthesis:
            printf(") Layer: %d", token_list[i].v.i);
            break;
        case Eq:
            printf("=");
            break;
        default:
            printf("还没实现");
            break;
        }
        printf("\n");
    }
}
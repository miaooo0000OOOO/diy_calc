#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "src/tokens.h"
#include "src/ast.h"
#include "src/calculation.h"

void test_simple_arithmetic() {
    printf("Testing simple arithmetic...\n");
    parse_to_token_list("2+3*5");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);

    Token *result = calc(node);
    assert(result != NULL);
    assert(result->type == Int);
    assert(result->v.i == 17);

    recu_free_ast(node);
    free(result);
    printf("✓ 2+3*5 = 17\n");
}

void test_parentheses() {
    printf("Testing parentheses...\n");
    parse_to_token_list("(2+3)*5");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);

    Token *result = calc(node);
    assert(result != NULL);
    assert(result->type == Int);
    assert(result->v.i == 25);

    recu_free_ast(node);
    free(result);
    printf("✓ (2+3)*5 = 25\n");
}

void test_negative_number() {
    printf("Testing negative number...\n");
    parse_to_token_list("-5+3");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);

    Token *result = calc(node);
    assert(result != NULL);
    assert(result->type == Int);
    assert(result->v.i == -2);

    recu_free_ast(node);
    free(result);
    printf("✓ -5+3 = -2\n");
}

void test_function_call() {
    printf("Testing function call...\n");
    // We'll just test parsing, not evaluation since it requires special setup
    parse_to_token_list("sin(0)");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);
    assert(node->token->type == Func);

    recu_free_ast(node);
    printf("✓ sin(0) parsed correctly\n");
}

void test_complex_expression() {
    printf("Testing complex expression...\n");
    parse_to_token_list("2+3*(5-1)/2");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);

    Token *result = calc(node);
    assert(result != NULL);
    printf("✓ 2+3*(5-1)/2 = %d\n", result->v.i);
    assert(result->v.i == 8);

    recu_free_ast(node);
    free(result);
}

void test_power_operator() {
    printf("Testing power operator...\n");
    parse_to_token_list("2^3");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);

    Token *result = calc(node);
    assert(result != NULL);
    assert(result->type == Int);
    assert(result->v.i == 8);

    recu_free_ast(node);
    free(result);
    printf("✓ 2^3 = 8\n");
}

void test_nested_parentheses() {
    printf("Testing nested parentheses...\n");
    parse_to_token_list("((2+3)*4)");
    AST_Node *node = parse_to_ast();
    assert(node != NULL);

    Token *result = calc(node);
    assert(result != NULL);
    assert(result->type == Int);
    assert(result->v.i == 20);

    recu_free_ast(node);
    free(result);
    printf("✓ ((2+3)*4) = 20\n");
}

int main() {
    printf("Running AST parsing tests...\n\n");

    test_simple_arithmetic();
    test_parentheses();
    test_negative_number();
    test_function_call();
    test_complex_expression();
    test_power_operator();
    test_nested_parentheses();

    printf("\n✓ All AST parsing tests passed!\n");
    return 0;
}

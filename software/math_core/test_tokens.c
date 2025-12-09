#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "src/tokens.h"

void test_parse_to_token_list() {
    // Test 1: Basic arithmetic
    assert(parse_to_token_list("2+3*5") > 0);
    assert(tokens_len == 5);
    assert(token_list[0].type == Int && token_list[0].v.i == 2);
    assert(token_list[1].type == Add);
    assert(token_list[2].type == Int && token_list[2].v.i == 3);
    assert(token_list[3].type == Mul);
    assert(token_list[4].type == Int && token_list[4].v.i == 5);
    printf("✓ Test 1 passed: Basic arithmetic\n");

    // Test 2: Hex number
    assert(parse_to_token_list("0xFF") > 0);
    assert(token_list[0].type == Int && token_list[0].v.i == 255);
    printf("✓ Test 2 passed: Hex number (0xFF = %d)\n", token_list[0].v.i);

    // Test 3: Binary number
    assert(parse_to_token_list("0b1010") > 0);
    assert(token_list[0].type == Int && token_list[0].v.i == 10);
    printf("✓ Test 3 passed: Binary number (0b1010 = %d)\n", token_list[0].v.i);

    // Test 4: Octal number
    assert(parse_to_token_list("077") > 0);
    assert(token_list[0].type == Int && token_list[0].v.i == 63);
    printf("✓ Test 4 passed: Octal number (077 = %d)\n", token_list[0].v.i);

    // Test 5: Float number
    assert(parse_to_token_list("3.14") > 0);
    assert(token_list[0].type == Float);
    printf("✓ Test 5 passed: Float number (3.14)\n");

    // Test 6: Negative number
    assert(parse_to_token_list("-5") > 0);
    assert(token_list[0].type == Neg);
    printf("✓ Test 6 passed: Negative number prefix\n");

    // Test 7: Subtraction
    assert(parse_to_token_list("5-3") > 0);
    assert(token_list[1].type == Sub);
    printf("✓ Test 7 passed: Subtraction operator\n");

    // Test 8: Parentheses
    assert(parse_to_token_list("(2+3)") > 0);
    assert(token_list[0].type == LeftParenthesis);
    assert(token_list[2].type == Add);
    assert(token_list[4].type == RightParenthesis);
    printf("✓ Test 8 passed: Parentheses\n");

    // Test 9: Variable
    assert(parse_to_token_list("x") > 0);
    assert(token_list[0].type == Var);
    printf("✓ Test 9 passed: Variable\n");

    // Test 10: Function
    assert(parse_to_token_list("sin(x)") > 0);
    assert(token_list[0].type == Func);
    printf("✓ Test 10 passed: Function\n");

    // Test 11: Complex expression
    assert(parse_to_token_list("2+3*(5-1)/2") > 0);
    assert(tokens_len == 11);
    printf("✓ Test 11 passed: Complex expression\n");

    // Test 12: Equation
    assert(parse_to_token_list("x=5") > 0);
    assert(token_list[1].type == Eq);
    printf("✓ Test 12 passed: Equation\n");

    // Test 13: Error cases - invalid hex
    assert(parse_to_token_list("0xFF.GG") == -1);
    printf("✓ Test 13 passed: Invalid hex with decimal point\n");

    // Test 14: Error cases - invalid octal
    assert(parse_to_token_list("089") == -1);
    printf("✓ Test 14 passed: Invalid octal with 8/9\n");

    // Test 15: Error cases - unmatched parenthesis
    assert(parse_to_token_list("(2+3") == -1);
    printf("✓ Test 15 passed: Unmatched parenthesis detected\n");

    // Test 16: Error cases - empty string
    assert(parse_to_token_list("") == -1);
    printf("✓ Test 16 passed: Empty string error handling\n");

    // Test 17: Zero
    assert(parse_to_token_list("0") > 0);
    assert(token_list[0].type == Int && token_list[0].v.i == 0);
    printf("✓ Test 17 passed: Zero value\n");

    // Test 18: Hex with uppercase
    assert(parse_to_token_list("0XFF") > 0);
    assert(token_list[0].type == Int && token_list[0].v.i == 255);
    printf("✓ Test 18 passed: Hex with uppercase (0XFF = %d)\n", token_list[0].v.i);

    // Test 19: Binary with uppercase
    assert(parse_to_token_list("0B1010") > 0);
    assert(token_list[0].type == Int && token_list[0].v.i == 10);
    printf("✓ Test 19 passed: Binary with uppercase (0B1010 = %d)\n", token_list[0].v.i);

    printf("\nAll tests passed! ✓\n");
}

int main() {
    test_parse_to_token_list();
    return 0;
}

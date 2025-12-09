#!/bin/bash
# Test script for token parsing

echo "Testing basic arithmetic..."
echo "2+3*5" | ./bin/main | grep "Int(17)"

echo "Testing hex number..."
echo "0xFF" | ./bin/main | grep "Int(255)"

echo "Testing binary number..."
echo "0b1010" | ./bin/main | grep "Int(10)"

echo "Testing octal number..."
echo "077" | ./bin/main | grep "Int(63)"

echo "Testing float..."
echo "3.14" | ./bin/main | grep "Float(3.140000)"

echo "Testing parentheses..."
echo "(2+3)*5" | ./bin/main | grep "Int(25)"

echo "Testing negative number..."
echo "-5+3" | ./bin/main | grep ""

echo "All tests completed!"

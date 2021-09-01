#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

void infixToPostfix(string infix, string& postfix, int& returnVal);
string removeSpaces(const string& s);

// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.
int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	int returnVal = 0, temp = 0;
	infixToPostfix(infix, postfix, returnVal);

	if (returnVal == 0) {
		stack<int> operandStack;
		int val = 0, operand2 = 0, operand1 = 0;

		for (int i = 0; i < postfix.length(); i++)
		{
			if (isalpha(postfix[i])) {
				if (values.get(postfix[i], val))
					operandStack.push(val);
				else {
					returnVal = 2;
					break;
				}
			}
			else
			{
				operand2 = operandStack.top();
				operandStack.pop();
				operand1 = operandStack.top();
				operandStack.pop();
				switch (postfix[i])
				{
				case '*':
					temp = operand1 * operand2;
					break;
				case '/':
					if (operand2 != 0)
						temp = operand1 / operand2;
					else
						returnVal = 3;
					break;
				case '+':
					temp = operand1 + operand2;
					break;
				case '-':
					temp = operand1 - operand2;
					break;
				default:
					break;
				}
				operandStack.push(temp);
			}
		}
		if(returnVal == 0)
			result = operandStack.top();
	}
	cerr << "return value:" << returnVal << endl;
	cerr << "result:" << result << endl;
	return returnVal;
}

//checks the syntax of an infix operation, returning 0 if it is not syntactically correct,
//1 if it is but there is at least one lower case letter operand not in the map
//and 2 if everything works
void infixToPostfix(string infix, string& postfix,int& returnVal)
{
	int consecutiveOperands = 0, consecutiveOperators = 0;

	infix = removeSpaces(infix);

	cerr << endl <<"infix len:" << infix.length();

	postfix = "";
	stack<char> charStack;

	for (int i = 0; i < infix.length(); i++)
	{
		if (isupper(infix[i]))
			returnVal = 1;
		if(returnVal == 0){
			switch (infix[i])
			{
				case '(':
					if (i != 0 && isalpha(infix[i - 1]))
						returnVal = 1;
					charStack.push(infix[i]);
					consecutiveOperands = 0;
					break;
				case ')':
					if (infix[i - 1] == '(')
						returnVal = 1;
					if (i < infix.length() - 1 && isalpha(infix[i + 1]))
						returnVal = 1;
					while (charStack.top() != '(')
					{
						postfix += charStack.top();
						charStack.pop();
					}
					charStack.pop();
					consecutiveOperands = 0;
					break;
				case '*':
				case '+':
				case '-':
				case '/':
					while (!charStack.empty() && charStack.top() != '(' &&
						((infix[i] == '+' || infix[i] == '-') && (charStack.top() == '*' || charStack.top() == '/')))
					{
						postfix += charStack.top();
						charStack.pop();
					}
					charStack.push(infix[i]);
					consecutiveOperators++;
					consecutiveOperands = 0;
					break;
				default:
					postfix += infix[i];
					consecutiveOperands++;
					consecutiveOperators = 0;
					break;
			}
		}
		if (consecutiveOperands >= 2 || consecutiveOperators >= 2)
			returnVal = 1;
	}

	if (postfix.empty())
		returnVal = 1;

	while (!charStack.empty() && returnVal == 0)
	{
		if (!isalpha(infix[0]) && infix[0] != '(')
			returnVal = 1;
		if (!isalpha(infix[infix.length() - 1]) && infix[infix.length() - 1] != ')')
			returnVal = 1;

		if (charStack.top() != '(') {
			postfix += charStack.top();
			charStack.pop();
		}
		else
			returnVal = 1;
	}
	cerr << endl <<"postfix:" + postfix << endl;
}

string removeSpaces(const string& s)
{
	string sWithNoSpaces;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != ' ')
			sWithNoSpaces += s[i];
	}
	return sWithNoSpaces;
}

int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+" && answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()o", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(*o)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	//// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*" && answer == 999);
	evaluate("a*b", m, pf, answer);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+" && answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/" && answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	cout << "Passed all tests" << endl;
}
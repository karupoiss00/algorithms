#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

constexpr char PLUS = '+';
constexpr char MINUS = '-';
constexpr char MULTIPLY = '*';
constexpr char DIVIDE = '/';
constexpr char POWER = '^';
constexpr char UNARY_MINUS = '~';
constexpr char BRACKET_LEFT = '(';
constexpr char BRACKET_RIGHT = ')';

template <typename T>
struct Stack
{
	Stack(T element)
		: key(element), next(nullptr)
	{}

	T key;
	Stack<T>* next;
};

namespace StackUtils 
{
	template <typename T>
	void Print(Stack<T>* root)
	{
		Stack<T>* next = root;
		cout << "--------- Stack ---------" << endl;
		while (next != nullptr)
		{
			cout << next->key << " ";
			next = next->next;
		}
		cout << endl << "-------------------------" << endl;
	}

	template <typename T>
	size_t Size(Stack<T>* root)
	{
		size_t size = 0;
		Stack<T>* next = root;

		while (next != nullptr)
		{
			next = next->next;
			size++;
		}

		return size;
	}

	template <typename T>
	bool IsEmpty(Stack<T>* stack)
	{
		return stack == nullptr;
	}

	template <typename T>
	void Push(Stack<T>** root, T data)
	{
		Stack<T>* stackNode = new Stack<T>(data);
		stackNode->next = *root;
		*root = stackNode;
#ifdef _DEBUG
		cout << "pushed: " << data << endl;
		Print(*root);
#endif	
	}

	template <typename T>
	T Pop(Stack<T>** root)
	{
		if (IsEmpty(*root))
			throw exception("failed to pop element from empty stack");
		Stack<T>* temp = *root;
		*root = (*root)->next;
		T popped = temp->key;
		
		delete temp;
#ifdef _DEBUG
		cout << "popped: " << popped << endl;
		Print(*root);
#endif	
		return popped;
	}

	template <typename T>
	T Peek(Stack<T>* root)
	{
		if (IsEmpty(root))
			throw exception("failed to peek element from empty stack");
		return root->key;
	}

}

namespace InfixConverter
{
	namespace
	{
		const set<char> OPERATORS =
		{
			PLUS , MINUS , MULTIPLY, DIVIDE, POWER
		};

		bool IsOperator(char ch)
		{
			return OPERATORS.find(ch) != OPERATORS.end();
		}

		unsigned Priority(char op)
		{
			switch (op)
			{
			case BRACKET_LEFT:
				return 0;
			case PLUS:
			case MINUS:
				return 1;
			case MULTIPLY:
			case DIVIDE:
				return 2;
			case POWER:
				return 3;
			case UNARY_MINUS:
				return 4;
			default:
				throw exception("unknown operator");
			}
		}

		bool IsLeftAssociationOperator(char op)
		{
			switch (op)
			{
			case PLUS:
			case MINUS:
			case UNARY_MINUS:
			case MULTIPLY:
			case DIVIDE:
				return true;
			case POWER:
				return false;
			default:
				throw exception("unknown operator");
			}
		}

		void RemoveSpaces(string& s)
		{
			s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
		}

		bool IsUnaryMinus(string const& infixForm, char op, unsigned index)
		{
			return op == MINUS && (index == 0 || (index > 1 && (IsOperator(infixForm[index - 1]) || (infixForm[index - 1] == BRACKET_LEFT))));
		}

		bool IsPrevDigit(string const& infixForm, unsigned index)
		{
			return index > 0 && isdigit(infixForm[index - 1]);
		}


		bool IsOperator(string str)
		{
			return str.length() == 1 && IsOperator(str[0]);
		}

		string ToPostfix(string infixForm)
		{
			Stack<char>* stack = nullptr;
			string result;

			RemoveSpaces(infixForm);

			unsigned i = 0;
			for (auto ch : infixForm)
			{
				if (isdigit(ch))
				{
					if (i > 0 && !isdigit(infixForm[i - 1]))
					{
						result += ' ';
					}
					result += ch;
				}

				if (IsOperator(ch))
				{
					if (IsUnaryMinus(infixForm, ch, i))
					{
						ch = '~';
					}
					
					if (StackUtils::IsEmpty(stack) || (StackUtils::Peek(stack) == BRACKET_LEFT))
					{
						StackUtils::Push(&stack, ch);
					}
					else
					{
						char topOperator = StackUtils::Peek(stack);
						unsigned chPriority = Priority(ch);

						if (chPriority > Priority(topOperator))
						{
							StackUtils::Push(&stack, ch);
						}
						else
						{
							if (IsLeftAssociationOperator(ch))
							{
								while
									(
										!StackUtils::IsEmpty(stack)
										&&
										(
											Priority(StackUtils::Peek(stack))
											>=
											chPriority
										)
									)
								{
									result += ' ';
									result += StackUtils::Pop(&stack);
								}
							}
							else
							{
								while
									(
										!StackUtils::IsEmpty(stack)
										&&
										(
											Priority(StackUtils::Peek(stack))
											>
											chPriority
										)
									)
								{
									result += ' ';
									result += StackUtils::Pop(&stack);
								}
							}
							StackUtils::Push(&stack, ch);
						}
					}
				}

				if (ch == BRACKET_LEFT)
				{
					StackUtils::Push(&stack, ch);
				}

				if (ch == BRACKET_RIGHT)
				{
					while
						(
							!StackUtils::IsEmpty(stack)
							&&
							(
								StackUtils::Peek(stack) != BRACKET_LEFT
							)
						)
					{
						result += ' ';
						result += StackUtils::Pop(&stack);
					}

					if (StackUtils::IsEmpty(stack))
					{
						throw exception("incorrect expression, cannot find pair for )");
					}

					StackUtils::Pop(&stack);
				}

				cout << endl << result << endl;
				StackUtils::Print(stack);
				i++;
			}

			char ch;
			while (!StackUtils::IsEmpty(stack))
			{
				ch = StackUtils::Pop(&stack);
				if (ch != BRACKET_LEFT && ch != BRACKET_RIGHT)
				{
					result += ' ';
					result += ch;
					cout << endl << result << endl;
					StackUtils::Print(stack);
				}
			}

			cout << endl;
			return result;
		}

	}
}

namespace Calculator
{
	namespace
	{
		bool IsUnaryOperator(string op)
		{
			return op.length() == 1 && op[0] == UNARY_MINUS;
		}

		void ExecuteBinaryOperation(char op, Stack<double>** stack)
		{
			double rhs = StackUtils::Pop(&*stack);
			double lhs = StackUtils::Pop(&*stack);
			double result = 0;

			switch (op)
			{
			case PLUS:
				result = lhs + rhs;
				break;
			case MINUS:
				result = lhs - rhs;
				break;
			case MULTIPLY:
				result = lhs * rhs;
				break;
			case DIVIDE:
				result = lhs / rhs;
				break;
			case POWER:
				result = pow(lhs, rhs);
				break;
			default:
				throw exception("unknown binary operator");
			}

			StackUtils::Push(&*stack, result);
		}

		void ExecuteUnaryOperation(char op, Stack<double>** stack)
		{
			double operand = StackUtils::Pop(&*stack);

			switch (op)
			{
			case UNARY_MINUS:
				StackUtils::Push(&*stack, -operand);
				break;
			default:
				throw exception("unknown unary operator");
			}
		}

		void ExecuteOperation(string op, Stack<double>** stack)
		{
			size_t stackSize = StackUtils::Size(*stack);
			if (IsUnaryOperator(op))
			{
				if (stackSize < 1)
				{
					throw exception("invalid expression");
				}
				ExecuteUnaryOperation(op[0], stack);
			}
			else
			{
				if (stackSize < 2)
				{
					throw exception("invalid expression");
				}
				ExecuteBinaryOperation(op[0], stack);
			}
		}
	}

	double CalculateExpression(string const& infixForm)
	{
		Stack<double>* stack = nullptr;
		stringstream postfixForm(InfixConverter::ToPostfix(infixForm));
		string token;

		while (!postfixForm.eof())
		{
			postfixForm >> skipws;
			postfixForm >> token;

			if (InfixConverter::IsOperator(token) || IsUnaryOperator(token))
			{
				try
				{
					ExecuteOperation(token, &stack);
				}
				catch (exception const& e)
				{
					throw e;
				}	
			}
			else
			{
				try
				{
					StackUtils::Push(&stack, stod(token));
				}
				catch (exception const& e)
				{
					throw e;
				}
			}
		}

		return StackUtils::Pop(&stack);
	}
}

int main()
{
	string infixExpression;
	getline(cin, infixExpression);

	if (infixExpression.empty())
	{
		cout << "invalid input expression" << endl;

		return -1;
	}

	cout << "Source expression: " << infixExpression << endl;

	try
	{
		cout << "Result: " << Calculator::CalculateExpression(infixExpression) << endl;
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
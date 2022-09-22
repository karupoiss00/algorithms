#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

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
			cout << next->key << endl;
			next = next->next;
		}
		cout << "-------------------------" << endl;
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
		constexpr char PLUS = '+';
		constexpr char MINUS = '-';
		constexpr char MULTIPLY = '*';
		constexpr char DIVIDE = '/';
		constexpr char POWER = '^';
		constexpr char UNARY_MINUS = '~';
		constexpr char BRACKET_LEFT = '(';
		constexpr char BRACKET_RIGHT = ')';

		const set<char> OPERATORS =
		{
			PLUS , MINUS , MULTIPLY, DIVIDE, POWER
		};

		bool IsOperator(char ch)
		{
			return OPERATORS.find(ch) != OPERATORS.end();
		}

		bool IsOperator(string str)
		{
			return str.length() == 1 && IsOperator(str[0]);
		}

		unsigned Priority(char op)
		{
			switch (op)
			{
			case PLUS:
			case MINUS:
				return 0;
			case MULTIPLY:
			case DIVIDE:
				return 1;
			case POWER:
				return 2;
			case UNARY_MINUS:
				return 3;
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
					
				if (StackUtils::IsEmpty(stack))
				{
					StackUtils::Push(&stack, ch);
				}
				else if (StackUtils::Peek(stack) == BRACKET_LEFT)
				{
					StackUtils::Push(&stack, ch);
				}
				else if (StackUtils::Peek(stack) == BRACKET_RIGHT)
				{
					while (StackUtils::Peek(stack) != BRACKET_LEFT)
					{
						result += StackUtils::Pop(&stack);
					}
					StackUtils::Pop(&stack);
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
									!(
										StackUtils::IsEmpty(stack)
										||
										StackUtils::Peek(stack) == BRACKET_LEFT
									)
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
									!(
										StackUtils::IsEmpty(stack)
										||
										(StackUtils::Peek(stack) == BRACKET_LEFT)
									)
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

							StackUtils::Push(&stack, ch);
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
			}

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
			}
		}

		return result;
	}

	void ExecuteOperation(char op, Stack<double>* stack)
	{
		double rhs = StackUtils::Pop(&stack);
		double lhs = StackUtils::Pop(&stack);
		double result = 0;
		
		switch (op)
		{
		case PLUS:
			result = lhs + rhs;
			break;
		case MINUS:
			result = lhs - rhs;
			break;
		case UNARY_MINUS:
			StackUtils::Push(&stack, lhs);
			StackUtils::Push(&stack, -rhs);
			return;
		case MULTIPLY:
			result = lhs * rhs;
			break;
		case DIVIDE:
			result = lhs / rhs;
			break;
		case POWER:
			result = pow(rhs, lhs);
			break;
		default:
			throw exception("unknown operator");
		}

		StackUtils::Push(&stack, result);
	}

	double CalculateExpression(string const& infixForm)
	{
		Stack<double>* stack = nullptr;
		stringstream postfixForm(ToPostfix(infixForm));
		string token;

		while (!postfixForm.eof())
		{
			postfixForm >> skipws;
			postfixForm >> token;

			if (IsOperator(token) && StackUtils::Size(stack) >= 2)
			{
				ExecuteOperation(token[0], stack);
			}
			else
			{
				try
				{
					double value = stod(token);
					StackUtils::Push(&stack, value);
				}
				catch (exception const& e)
				{
					throw e;
				}
			}
		}

		StackUtils::Print(stack);
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

	try
	{
		cout << InfixConverter::ToPostfix(infixExpression) << endl;
		InfixConverter::CalculateExpression(infixExpression);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
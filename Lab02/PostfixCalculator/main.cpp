#include <iostream>
#include <set>
#include <string>
#include <limits>

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
	void PrintStack(Stack<T>* root)
	{
		Stack<T>* next = root;
		cout << "--------- Stack ---------" << endl;
		while (next != nullptr)
		{
			cout << next->key << endl;
			next = root->next;
		}
		cout << "-------------------------" << endl;
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
		PrintStack(*root);
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
		PrintStack(*root);
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
		constexpr char BRACKET_LEFT = '(';
		constexpr char BRACKET_RIGHT = ')';

		const set<char> OPERATORS =
		{
			PLUS , MINUS , MULTIPLY, DIVIDE,POWER
		};

		bool IsDigit(char ch)
		{
			return ch >= '0' && ch <= '9';
		}

		bool IsOperator(char ch)
		{
			return OPERATORS.find(ch) != OPERATORS.end();
		}

		unsigned Priority(char op)
		{
			switch (op)
			{
			case '+':
			case '-':
				return 0;
			case '*':
			case '/':
				return 1;
			case '^':
				return 2;
			default:
				throw exception("unknown operator");
			}
		}

		bool IsLeftAssociationOperator(char op)
		{
			switch (op)
			{
			case '+':
			case '-':
			case '*':
			case '/':
				return true;
			case '^':
				return false;
			default:
				throw exception("unknown operator");
			}
		}

		void RemoveSpaces(string& s)
		{
			s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
		}
	}

	string ToPostfix(string infixForm)
	{
		Stack<char>* stack = nullptr;
		string result;

		RemoveSpaces(infixForm);

		for (auto const& ch : infixForm)
		{
			if (IsDigit(ch))
			{
				result += ch;
			}

			if (IsOperator(ch))
			{
				result += ' ';
				if (StackUtils::IsEmpty(stack))
				{
					if (ch != MINUS)
					{
						StackUtils::Push(&stack, ch);
					}				
					else
					{
						result += ch;
					}
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
									(
										!StackUtils::IsEmpty(stack)
										||
										(!StackUtils::IsEmpty(stack) && StackUtils::Peek(stack) == BRACKET_LEFT)
									)
									&&
									(
										Priority(StackUtils::Peek(stack))
										>=
										chPriority
									)
								)
							{
								result += StackUtils::Pop(&stack);
								result += ' ';
							}
						}
						else
						{
							while
								(
									(
										!StackUtils::IsEmpty(stack)
										||
										(!StackUtils::IsEmpty(stack) && StackUtils::Peek(stack) == BRACKET_LEFT)
									)
									&&
									(
										Priority(StackUtils::Peek(stack))
										>
										chPriority
									)
								)
							{
								result += StackUtils::Pop(&stack);
								result += ' ';
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
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
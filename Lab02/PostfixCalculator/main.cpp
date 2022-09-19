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

int main()
{
	string infixExpression;
	getline(cin, infixExpression);

	if (infixExpression.empty())
	{
		cout << "invalid input expression" << endl;

		return -1;
	}

	return 0;
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
				continue;
			}

			if (IsOperator(ch))
			{
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
					
					if (Priority(ch) > Priority(topOperator))
					{
						StackUtils::Push(&stack, ch);
					}
					else
					{

					}
				}
				continue;
			}
		}

		return "";
	}
}
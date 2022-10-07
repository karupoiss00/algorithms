#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

constexpr char LEVEL_CHAR = '.';

/*
	24. Имеется  И-ИЛИ   дерево,   соответствующее   некоторому
	множеству  конструкций.  Требуется выдать на экран в наглядном
	виде все элементы дерева (14).

	Глизерин Ростислав Visual Studio 2019 C++17
*/

enum class NodeType
{
	AND = 'a',
	OR = 'o',
	LEAF = 'l',
};

template<typename T>
struct Tree
{
	NodeType type;
	T value;
	int level;
	vector<Tree<T>*> sons;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);

int main(int argc, char* argv[])
{
	SetConsoleCP(1271);
	SetConsoleOutputCP(1271);

	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	try
	{
		Tree<wstring>* root = ReadTreeFromFile(args->inputFileName);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		return nullopt;
	}

	Args args;
	args.inputFileName = argv[1];

	return args;
}

NodeType CharToNodeType(wchar_t ch)
{
	switch (ch)
	{
	case 'o':
		return NodeType::OR;
	case 'a':
		return NodeType::AND;
	case 'l':
		return NodeType::LEAF;
	default:
		throw exception("unknown node type: " + ch);
	}
}

int ReadLevelFromString(wstring const& str)
{
	int level = 0;

	for (auto ch : str)
	{
		if (ch != LEVEL_CHAR)
		{
			break;
		}
		level++;
	}

	return level;
}

wstring ReadValueFromString(wstring const& str)
{
	wstring value;

	for (auto i = str.find_first_not_of(LEVEL_CHAR); i < str.find_last_of(' '); i++)
	{
		value += str[i];
	}

	return value;
}

Tree<wstring>* ReadTreeFromFile(wifstream& file, Tree<wstring>*& root, int currentLevel = 0, Tree<wstring> * *prevNode = nullptr)
{
	wstring line;
	Tree<wstring>* currentNode = nullptr;
	Tree<wstring>* previousNode = prevNode ? *prevNode : nullptr;

	while (getline(file, line))
	{
		currentNode = new Tree<wstring>();
		currentNode->type = CharToNodeType(line.back());
		currentNode->value = ReadValueFromString(line);
		currentNode->level = ReadLevelFromString(line);

		if (currentNode->level == 0)
		{
			root = currentNode;
			previousNode = root;
			continue;
		}

		if ((currentNode->level - currentLevel) == 1)
		{
			root->sons.push_back(currentNode);
		}

		if ((currentNode->level - currentLevel) > 1)
		{
			previousNode->sons.push_back(currentNode);
			Tree<wstring>* notChildNode = ReadTreeFromFile(file, previousNode, currentLevel + 1, &currentNode);

			if (notChildNode)
			{
				if ((notChildNode->level - currentLevel) < 1)
				{
					return notChildNode;
				}
				else
				{
					root->sons.push_back(notChildNode);
					previousNode = notChildNode;
					continue;
				}
			}
		}

		if ((currentNode->level - currentLevel) < 1)
		{
			return currentNode;
		}
		previousNode = currentNode;
	}

	return nullptr;
}

Tree<wstring>* ReadTreeFromFile(string fileName)
{
	Tree<wstring>* root = nullptr;

	wifstream inputFile(fileName);

	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	if (ReadTreeFromFile(inputFile, root))
	{
		throw exception("something went wrong");
	}

	return root;
}
#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>

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
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
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

NodeType CharToNodeType(char ch)
{
	switch(ch)
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

unsigned ReadLevelFromString(wstring& const str)
{
	unsigned level = 0;
	for (auto ch : str)
	{
		if (ch != LEVEL_CHAR)
		{
			break;
		}
		else
		{
			level++;
		}
	}

	return level;
}

wstring ReadValueFromString(wstring& const str)
{
	wstring value;

	for (auto i = str.find_first_not_of(LEVEL_CHAR); str[i] != ' '; i++)
	{
		value += str[i];
	}

	return value;
}

Tree<wstring>* ReadTreeFromFile(wifstream& file, Tree<wstring>* root, unsigned currentLevel = 0)
{
	wstring line;
	while (getline(file, line))
	{
		Tree<wstring>* node = new Tree<wstring>();
		node->type = CharToNodeType(line.back());
		node->value = ReadValueFromString(line);

		unsigned nodeLevel = ReadLevelFromString(line);

		if (nodeLevel > currentLevel)
		{
			root->sons.push_back(node);

			if (node->type == NodeType::LEAF)
			{
				continue;
			}

			ReadTreeFromFile(file, node, nodeLevel);
		}
		else
		{
			return node;
		}
	}

	return nullptr;
}

Tree<wstring>* ReadTreeFromFile(string fileName)
{
	Tree<wstring>* root = nullptr;

	wifstream inputFile(fileName);

	if (inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	 ReadTreeFromFile(inputFile, root);

	return root;
}
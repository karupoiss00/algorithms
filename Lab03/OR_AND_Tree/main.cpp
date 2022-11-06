#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
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
	vector<wstring> variants;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);
void GenerateAllVariants(Tree<wstring>* root);

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
		GenerateAllVariants(root);

		wcout << "Всего вариантов дерева: " << root->variants.size() << endl << endl;

		for (auto const& variant : root->variants)
		{
			wcout << variant << endl << endl;
		}
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
		if (line.empty())
		{
			continue;
		}
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

void GenerateAllAndVariants(Tree<wstring>* andNode, vector<wstring>& result, unsigned depth = 0)
{
	if (depth < andNode->sons.size())
	{
		if (depth == 0)
		{
			auto firstNode = andNode->sons[0];
			for (auto variant : firstNode->variants)
			{
				wstring andVariant;
				for (int i = 0; i < andNode->level; i++)
				{
					andVariant += '.';
				}
				andVariant += andNode->value;
				andVariant += '\n';
				andVariant += variant;
				result.push_back(andVariant);
			}
			GenerateAllAndVariants(andNode, result, ++depth);
			return;
		}

		auto currentNode = andNode->sons[depth];
		vector<wstring> oldResult = result;
		result.clear();
		for (auto variant : oldResult)
		{
			vector<wstring> currentNodeVariants = currentNode->variants;
			for (unsigned i = 0; i < currentNodeVariants.size(); i++)
			{
				result.push_back(variant + currentNodeVariants[i]);
			}
		}

		GenerateAllAndVariants(andNode, result, ++depth);
	}
}

void GenerateAllVariants(Tree<wstring>* root)
{
	wstring variant;
	for (int i = 0; i < root->level; i++)
	{
		variant += '.';
	}
	variant += root->value;
	variant += '\n';

	if (root->type == NodeType::LEAF)
	{
		root->variants.push_back(variant);
		return;
	}

	if (root->type == NodeType::OR)
	{
		for (auto node : root->sons)
		{
			GenerateAllVariants(node);

			for (auto& nodeVariant : node->variants)
			{
				nodeVariant = variant + nodeVariant;
			}
			root->variants.insert(root->variants.end(), node->variants.begin(), node->variants.end());
		}

		return;
	}

	if (root->type == NodeType::AND)
	{
		for (auto node : root->sons)
		{
			GenerateAllVariants(node);
		}

		vector<wstring> andVariants;
		GenerateAllAndVariants(root, andVariants);
		root->variants.insert(root->variants.end(), andVariants.begin(), andVariants.end());
	}
}
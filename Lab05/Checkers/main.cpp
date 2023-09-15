//	20. Ќа  шахматном поле расположены N черных  и  одна  бела€
//	шашка.“ребуетс€  написать  программу, котора€  по  заданному
//	расположению шашек определ€ет, какое  максимальное  количество
//	шашек может вз€ть бела€ шашка за один ход.
//	√лизерин –остислав, Visual Studio 2019

#include <iostream>
#include <optional>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

constexpr int FIELD_SIZE = 8;

constexpr char EMPTY_CELL = '0';
constexpr char WHITE_CHECKER = '1';
constexpr char BLACK_CHECKER = '2';

using Field = array<char, FIELD_SIZE * FIELD_SIZE>;

struct Args
{
	string inputFileName;
};

template<typename T>
struct GraphVertex
{
	T value;
	vector<GraphVertex<T>*> sons;

	~GraphVertex()
	{
		for (auto& son : sons)
		{
			delete son;
		}
	}
};

template <typename T>
T FindMax(GraphVertex<T>* graph)
{
	T result = graph->value;
	T value;

	for (auto& son : graph->sons)
	{
		value = FindMax(son);
		if (result < value)
		{
			result = value;
		}
	}

	return result;
}

optional<Args> ParseArgs(int argc, char* argv[]);
Field ReadFieldFromFile(string fileName);
void GenerateTakes(Field const& field, int checkerIndex, GraphVertex<int>* graph);

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	try
	{
		Field field = ReadFieldFromFile(args->inputFileName);

		int blackIndex = distance(field.begin(), std::find(field.begin(), field.end(), '2'));
		GraphVertex<int> graph{ 0 };

		GenerateTakes(field, blackIndex, &graph);

		cout << FindMax(&graph) << endl;
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

Field ReadField(istream& input)
{
	string line;
	Field field;
	field.fill('0');

	int i = 0;
	while (getline(input, line))
	{
		if (line.empty() || line.length() < FIELD_SIZE)
		{
			continue;
		}

		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field[i * FIELD_SIZE + j] = line[j];
		}

		i++;
	}

	return field;
}

Field ReadFieldFromFile(string fileName)
{
	ifstream inputFile(fileName);

	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	return ReadField(inputFile);
}

bool CanMoveTo(int index)
{
	return index >= 0 && index <= 63;
}

constexpr int RIGHT_TOP_MOVE = -7;
constexpr int RIGHT_BOTTOM_MOVE = 9;
constexpr int LEFT_TOP_MOVE = -9;
constexpr int LEFT_BOTTOM_MOVE = 7;

void GenerateTakes(Field const& field, int checkerPos, GraphVertex<int>* graph)
{
	int rightTopPos = checkerPos + RIGHT_TOP_MOVE;
	if (CanMoveTo(rightTopPos))
	{
		int posAfterTake = rightTopPos + RIGHT_TOP_MOVE;
		bool isBorderPos = ((rightTopPos + 1) % FIELD_SIZE == 0);
		bool canTake = field[rightTopPos] == WHITE_CHECKER 
			&& CanMoveTo(posAfterTake) 
			&& field[posAfterTake] == EMPTY_CELL 
			&& !isBorderPos
			&& posAfterTake != 0;
		if (canTake)
		{
			GraphVertex<int>* newTake = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newTake);
			Field newField = field;

			newField[checkerPos] = EMPTY_CELL;
			newField[rightTopPos] = EMPTY_CELL;
			newField[posAfterTake] = BLACK_CHECKER;

			GenerateTakes(newField, posAfterTake, newTake);
		}
	}

	int rightBottomPos = checkerPos + RIGHT_BOTTOM_MOVE;
	if (CanMoveTo(rightBottomPos))
	{
		int posAfterTake = rightBottomPos + RIGHT_BOTTOM_MOVE;
		bool isBorderPos = ((rightBottomPos + 1) % FIELD_SIZE == 0);
		bool canTake = field[rightBottomPos] == WHITE_CHECKER 
			&& CanMoveTo(posAfterTake) 
			&& !isBorderPos
			&& field[posAfterTake] == EMPTY_CELL;
		if (canTake)
		{
			GraphVertex<int>* newTake = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newTake);
			Field newField = field;

			newField[checkerPos] = EMPTY_CELL;
			newField[rightBottomPos] = EMPTY_CELL;
			newField[posAfterTake] = BLACK_CHECKER;

			GenerateTakes(newField, posAfterTake, newTake);
		}
	}

	int leftTopPos = checkerPos + LEFT_TOP_MOVE;
	if (CanMoveTo(leftTopPos))
	{
		int posAfterTake = leftTopPos + LEFT_TOP_MOVE;
		bool isBorderPos = (leftTopPos % FIELD_SIZE == 0);
		bool canTake = field[leftTopPos] == WHITE_CHECKER 
			&& CanMoveTo(posAfterTake) 
			&& posAfterTake != 63 
			&& field[posAfterTake] == EMPTY_CELL;
		if (canTake)
		{
			GraphVertex<int>* newTake = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newTake);
			Field newField = field;

			newField[checkerPos] = EMPTY_CELL;
			newField[leftTopPos] = EMPTY_CELL;
			newField[posAfterTake] = BLACK_CHECKER;

			GenerateTakes(newField, posAfterTake, newTake);
		}
	}

	int leftBottomPos = checkerPos + LEFT_BOTTOM_MOVE;
	if (CanMoveTo(leftBottomPos))
	{
		int posAfterTake = leftBottomPos + LEFT_BOTTOM_MOVE;
		bool isBorderPos = (leftBottomPos % FIELD_SIZE == 0);
		bool canTake = field[leftBottomPos] == WHITE_CHECKER 
			&& CanMoveTo(posAfterTake)
			&& !isBorderPos
			&& field[posAfterTake] == EMPTY_CELL;
		if (canTake)
		{
			GraphVertex<int>* newTake = new GraphVertex<int>({ graph->value + 1 });
			graph->sons.push_back(newTake);
			Field newField = field;

			newField[checkerPos] = EMPTY_CELL;
			newField[leftBottomPos] = EMPTY_CELL;
			newField[posAfterTake] = BLACK_CHECKER;

			GenerateTakes(newField, posAfterTake, newTake);
		}
	}
}
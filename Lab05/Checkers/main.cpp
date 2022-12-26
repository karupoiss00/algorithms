//20. Ќа  шахматном поле расположены N черных  и  одна  бела€
//шашка.“ребуетс€  написать  программу, котора€  по  заданному
//расположению шашек определ€ет, какое  максимальное  количество
//шашек может вз€ть бела€ шашка за один ход.
//¬вод из файла INPUT.TXT.«адаетс€ 8 строк, кажда€  содержит
//8 символов.—имволом С0Т обозначаетс€ пуста€ клетка, символом
//С1Т положение черной шашки и символом С2Т положение белой шашки.
//¬ывод  в  файл  OUTPUT.TXT.¬ывести  единственное  число Ц
//максимальное количество черных шашек, которых можно  вз€ть  за
//один ход(11).

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

Field ReadFieldFromFile(istream& file)
{
	string line;
	Field field;

	int i = 0;
	while (getline(file, line))
	{
		if (line.empty() && line.length() < FIELD_SIZE)
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

	return ReadFieldFromFile(inputFile);
}

bool CanMove(int index)
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
	if (CanMove(rightTopPos))
	{
		int posAfterTake = rightTopPos + RIGHT_TOP_MOVE;
		bool isBorderPos = ((rightTopPos + 1) % FIELD_SIZE == 0);
		bool canTake = field[rightTopPos] == WHITE_CHECKER 
			&& CanMove(posAfterTake) 
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
	if (CanMove(rightBottomPos))
	{
		int posAfterTake = rightBottomPos + RIGHT_BOTTOM_MOVE;
		bool isBorderPos = ((rightBottomPos + 1) % FIELD_SIZE == 0);
		bool canTake = field[rightBottomPos] == WHITE_CHECKER 
			&& CanMove(posAfterTake) 
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
	if (CanMove(leftTopPos))
	{
		int posAfterTake = leftTopPos + LEFT_TOP_MOVE;
		bool isBorderPos = (leftTopPos % FIELD_SIZE == 0);
		bool canTake = field[leftTopPos] == WHITE_CHECKER 
			&& CanMove(posAfterTake) 
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
	if (CanMove(leftBottomPos))
	{
		int posAfterTake = leftBottomPos + LEFT_BOTTOM_MOVE;
		bool isBorderPos = (leftBottomPos % FIELD_SIZE == 0);
		bool canTake = field[leftBottomPos] == WHITE_CHECKER 
			&& CanMove(posAfterTake)
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
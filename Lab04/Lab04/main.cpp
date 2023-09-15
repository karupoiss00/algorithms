#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <limits>
#include <algorithm>
#include <sstream>

/*
	 19. Имеется  сеть  автомобильных  дорог.  Для каждой дороги
	известна максимальная масса груза, которую можно  провезти  по
	этой   дороге.   С   помощью  алгоритма  Дейкстры   определить
	максимальный   груз,   который  можно  провезти  между   двумя
	указанными городам (10).

	Глизерин Ростислав, Visual Studio 2019, C++17
*/

using namespace std;

constexpr int MAX_INT = numeric_limits<int>::max();
const string NO_WAY = "no way";

class Matrix
{
public:
	Matrix() : m_data({}) {}

	Matrix(const size_t& rows, const size_t& cols)
	{
		Reset(rows, cols);
	}

	void Reset(const size_t& rows, const size_t& cols)
	{
		m_data.resize(rows);
		for (int i = 0; i < rows; ++i)
		{
			m_data.at(i).resize(cols);
		}
	}

	int At(const size_t& row, const size_t& col) const
	{
		return m_data.at(row).at(col);
	}

	int& At(const size_t& row, const size_t& col)
	{
		if (row >= GetNumRows())
		{
			Reset(row, row);
		}

		if (col >= GetNumColumns())
		{
			Reset(col, col);
		}
		return m_data.at(row - 1).at(col - 1);
	}

	size_t GetNumRows() const
	{
		return m_data.size();
	}

	size_t GetNumColumns() const
	{
		if (GetNumRows() > 0)
		{
			return m_data[0].size();
		}

		return 0;
	}

	vector<vector<int>> const& Data() const
	{
		return m_data;
	}
private:
	vector<vector<int>> m_data;
};

struct Args
{
	string inputFileName;
};

struct PathData
{
	size_t weight;
	string path;
};

optional<Args> ParseArgs(int argc, char* argv[]);
optional<Matrix> ReadGraphFromFile(string const& filename);
PathData GetMaxWeight(Matrix& graph, int src, int dest);

int main(int argc, char* argv[]) {
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "filename not found" << endl;
		return -1;
	}

	auto graph = ReadGraphFromFile(args->inputFileName);

	if (!graph)
	{
		cout << args->inputFileName << " invalid filename" << endl;
		return -1;
	}

	int source, destination;

	cout << "Enter source and destinition (<src> <dst>): ";
	cin >> source;
	cin >> destination;

	bool sourceOutOfRange = source - 1 >= graph->GetNumRows();
	bool destinationOutOfRange = destination - 1 >= graph->GetNumColumns();

	if (sourceOutOfRange || destinationOutOfRange)
	{
		cout << "invalid source or destination" << endl;
		return -1;
	}

	PathData path = GetMaxWeight(*graph, source, destination);

	if (path.path == NO_WAY)
	{
		cout << NO_WAY << endl;
		return 0;
	}

	cout << "Max weight from " << source << " to " << destination << ": ";
	cout << path.weight << endl;
	cout << "Path: " << path.path << endl;

	return 0;
}

optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc < 2)
	{
		return nullopt;
	}

	Args args;
	args.inputFileName = argv[1];

	return args;
}

optional<Matrix> ReadGraphFromFile(string const& filename)
{
	ifstream input(filename);

	if (!input.is_open())
	{
		return nullopt;
	}

	Matrix graph;

	while (!input.eof())
	{
		int v1, v2, weight;
		input >> v1;
		input >> v2;
		input >> weight;

		bool incorrectVertexIndex = v1 < 1 || v2 < 1;
		bool negativeWeight = weight < 0;

		if (incorrectVertexIndex || negativeWeight)
		{
			continue;
		}

		graph.At(v1, v2) = weight;
		graph.At(v2, v1) = weight;
	}

	return graph;
}

size_t FindNotVisitedVertexWithMaxWeight(vector<int> weight, vector<bool> visitedVertexes)
{
	int max = -1;
	size_t max_index;

	for (size_t i = 0; i < weight.size(); i++)
	{
		if (visitedVertexes[i] == false && weight[i] >= max)
		{
			max = weight[i];
			max_index = i;
		}
	}

	return max_index;
}

PathData GetMaxWeight(Matrix& graph, int src, int dest)
{
	PathData result{0, NO_WAY};

	size_t size = graph.GetNumRows();

	vector<int> weight(size, -1);
	vector<int> path(size, 0);
	vector<bool> visitedVertexes(size, false);

	weight[src - 1] = MAX_INT;

	int max_index;

	for (unsigned count = 0; count < size - 1; count++)
	{
		max_index = FindNotVisitedVertexWithMaxWeight(weight, visitedVertexes);
		visitedVertexes[max_index] = true;

		for (unsigned j = 0; j < size; j++)
		{
			int path_weight = graph.At(max_index + 1, j + 1);
			int min_path_weight = min(weight[max_index], path_weight);

			if (!visitedVertexes[j] && path_weight && weight[max_index] != -1
				&& min_path_weight > weight[j])
			{
				weight[j] = min_path_weight;
				path[j] = max_index + 1;
			}
		}
	}

	if (weight[dest - 1] != -1)
	{
		stringstream pathStream;
		vector<int> resultPath = {dest};
		
		int v = path[dest - 1];
		while (v != src)
		{
			resultPath.push_back(v);
			v = path[v - 1];
		}

		resultPath.push_back(src);
		copy(resultPath.rbegin(), resultPath.rend(), ostream_iterator<int>(pathStream, " "));
		
		result.weight = weight[dest - 1];
		result.path = pathStream.str();
	}
	else
	{
		result.weight = -1;
	}
	
	return result;
}
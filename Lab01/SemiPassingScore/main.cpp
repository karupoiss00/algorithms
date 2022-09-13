/*
	Вступительные испытания в некоторый вуз состоят из  трех
	экзаменов:  математика  (максимальный  балл  – 9),  информатика
	(максимальный балл – 9), литература  (максимальный балл – 5). В
	текстовом вход имеются  сведения  о  сдаче  абитуриентами  этих
	экзаменов. Строки файла имеют следующий формат:
	<Фамилия> <оценка1> <оценка2> <оценка3>, где <Фамилия> –строка,
	состоящая не более, чем из 20 символов, оценки – числа от 0  до
	максимальной оценки по предмету соответственно. Ноль ставится в
	случае, если экзамен не сдавался, например, после полученной на
	предыдущем экзамене двойки. Все  баллы,  большие  2,  считаются
	удовлетворительными). Пример входных строк:
	Иванов 8 9 3
	Петров 2 0 0
	Задается количество мест K,  на которые претендуют абитуриенты.
	Требуется определить список абитуриентов с полупроходным баллом
	или  сообщить,  что  такой   балл   отсутствует.  Полупроходным
	считается такой балл, что лишь  часть  абитуриентов,  набравших
	этот балл и не получивших ни одной неудовлетворительной оценки,
	попадает в K лучших, которые должны быть зачислены (8).
 
	Глизерин Ростислав Visual Studio 2017
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include <locale> 
#include <windows.h>
#include "Enrolle.h"

using namespace std;

struct Args
{
	string inputFileName;
	string outputFileName;
	unsigned countOfPlaces;
};

optional<Args> ParseArgs(int argc, char* argv[]);
optional<vector<Enrolle>> GetSemiPassingEnrolles(vector<Enrolle> const& enrolles, int placesCount);
void SortEnrolles(vector<Enrolle>& values);
void ReadEnrollesFromFile(string inputFileName, vector<Enrolle>& enrolles);
void WriteEnrolles(optional<vector<Enrolle>> const& enrolles, string outputFilename);

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid arguments" << endl;
		cout << "Usage: " << argv[0] << " <input file> <output file> <count of places>" << endl;

		return -1;
	}

	vector<Enrolle> enrolles;

	try
	{
		ReadEnrollesFromFile(args->inputFileName, enrolles);
		SortEnrolles(enrolles);
		auto semiEnrolles = GetSemiPassingEnrolles(enrolles, args->countOfPlaces);
		WriteEnrolles(semiEnrolles, args->outputFileName);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 4)
	{
		return nullopt;
	}

	Args args;
	args.inputFileName = argv[1];
	args.outputFileName = argv[2];

	try
	{
		args.countOfPlaces = stoi(argv[3]);

		if (args.countOfPlaces <= 0)
		{
			throw exception("invalid count of places");
		}
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;

		return nullopt;
	}

	return args;
}

void ReadEnrollesFromFile(string inputFileName, vector<Enrolle>& enrolles)
{
	ifstream inputFile;
	inputFile.open(inputFileName);

	if (!inputFile.is_open())
	{
		throw exception("failed to open input file");
	}

	string src;

	while (getline(inputFile, src))
	{
		Enrolle enrolle;
		enrolle.fromString(src);

		if (enrolle.isValid())
		{
			enrolles.push_back(enrolle);
		}
	}

	inputFile.close();
}

int Partition(vector<Enrolle>& values, int l, int r) 
{
	int x = values[r].getTotalScore();
	int less = l;

	for (int i = l; i < r; ++i) 
	{
		if (values[i].getTotalScore() <= x)
		{
			swap(values[i], values[less]);
			++less;
		}
	}
	swap(values[less], values[r]);
	return less;
}

void QuickSort(vector<Enrolle>& values, int l, int r)
{
	if (l < r) 
	{
		int q = Partition(values, l, r);
		QuickSort(values, l, q - 1);
		QuickSort(values, q + 1, r);
	}
}

void SortEnrolles(vector<Enrolle>& values)
{
	if (!values.empty()) 
	{
		int lastIndex = static_cast<int>(values.size() - 1);
		QuickSort(values, 0, lastIndex);
	}
}

bool AreAllEntered(int placesCount, int enrollesCount)
{
	return placesCount >= enrollesCount;
}

bool HasSemiPassingScore(vector<Enrolle> const& enrolles, int placesCount)
{
	if (AreAllEntered(placesCount, static_cast<int>(enrolles.size())))
	{
		return false;
	}

	auto lastEntered = enrolles[placesCount - 1];
	auto firstNotAdmitted = enrolles[placesCount];

	if (lastEntered.getTotalScore() != firstNotAdmitted.getTotalScore())
	{
		return false;
	}

	return true;
}

optional<int> GetSemiPassingScore(vector<Enrolle> const& enrolles, int placesCount)
{
	int enrollesCount = static_cast<int>(enrolles.size());

	if (!HasSemiPassingScore(enrolles, placesCount))
	{
		return nullopt;
	}

	return enrolles[placesCount].getTotalScore();
}

vector<Enrolle> GetSemiPassingEnrollesImpl(vector<Enrolle> const& enrolles, int semiPassingScore)
{
	vector<Enrolle> semiPassingEnrolles;

	for (auto const& enrolle : enrolles)
	{
		if (enrolle.getTotalScore() < semiPassingScore)
		{
			break;
		}

		if (enrolle.getTotalScore() == semiPassingScore)
		{
			semiPassingEnrolles.push_back(enrolle);
		}
	}

	return semiPassingEnrolles;
}

optional<vector<Enrolle>> GetSemiPassingEnrolles(vector<Enrolle> const& enrolles, int placesCount)
{
	auto semiPasingScore = GetSemiPassingScore(enrolles, placesCount);

	if (!semiPasingScore)
	{
		return nullopt;
	}

	if (static_cast<int>(enrolles.size()) <= placesCount)
	{
		return enrolles;
	}

	return GetSemiPassingEnrollesImpl(enrolles, semiPasingScore.value());
}

void WriteEnrolles(ostream& output, vector<Enrolle> const& enrolles)
{
	for (auto const& enrolle : enrolles)
	{
		output << enrolle << endl;
	}
}

void WriteEnrolles(optional<vector<Enrolle>> const& enrolles, string outputFilename)
{
	ofstream outFile;
	outFile.open(outputFilename);

	if (!outFile.is_open())
	{
		throw exception("failed to open output file");
	}

	if (enrolles)
	{
		WriteEnrolles(outFile, enrolles.value());
	}
	else
	{
		outFile << "no semipassing score" << endl;
	}
}
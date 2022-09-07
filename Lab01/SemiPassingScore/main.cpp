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
void ReadEnrollesFromFile(string inputFileName, vector<Enrolle>& enrolles);

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
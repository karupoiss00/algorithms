/*
	������������� ��������� � ��������� ��� ������� ��  ����
	���������:  ����������  (������������  ����  � 9),  �����������
	(������������ ���� � 9), ����������  (������������ ���� � 5). �
	��������� ���� �������  ��������  �  �����  �������������  ����
	���������. ������ ����� ����� ��������� ������:
	<�������> <������1> <������2> <������3>, ��� <�������> �������,
	��������� �� �����, ��� �� 20 ��������, ������ � ����� �� 0  ��
	������������ ������ �� �������� ��������������. ���� �������� �
	������, ���� ������� �� ��������, ��������, ����� ���������� ��
	���������� �������� ������. ���  �����,  �������  2,  ���������
	�������������������). ������ ������� �����:
	������ 8 9 3
	������ 2 0 0
	�������� ���������� ���� K,  �� ������� ���������� �����������.
	��������� ���������� ������ ������������ � ������������� ������
	���  ��������,  ���  �����   ����   �����������.  �������������
	��������� ����� ����, ��� ����  �����  ������������,  ���������
	���� ���� � �� ���������� �� ����� �������������������� ������,
	�������� � K ������, ������� ������ ���� ��������� (8).

	�������� ��������� Visual Studio 2017
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
		QuickSort(values, 0, values.size() - 1);
	}
}

bool AreAllEntered(int placesCount, int enrollesCount)
{
	return placesCount >= enrollesCount;
}

bool HasSemiPassingScore(vector<Enrolle> const& enrolles, int placesCount)
{
	if (AreAllEntered(placesCount, enrolles.size()))
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
	int enrollesCount = enrolles.size();

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
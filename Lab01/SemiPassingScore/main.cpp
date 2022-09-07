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
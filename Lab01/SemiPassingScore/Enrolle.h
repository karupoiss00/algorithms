#pragma once
#include <iostream>
#include <string>
#include <sstream>

constexpr int MIN_SCORE = 2;
constexpr int MAX_MATH_SCORE = 9;
constexpr int MAX_INFORMATICS_SCORE = 9;
constexpr int MAX_LITERATURE_SCORE = 5;

struct Enrolle 
{
	std::wstring surname;
	int mathScore;
	int informaticsScore;
	int literatureScore;

	bool isValid() const
	{
		return surname.length() > 0
			&& mathScore > MIN_SCORE && mathScore <= MAX_MATH_SCORE
			&& informaticsScore > MIN_SCORE && informaticsScore <= MAX_INFORMATICS_SCORE
			&& literatureScore > MIN_SCORE && literatureScore <= MAX_LITERATURE_SCORE;
	}

	void fromString(std::wstring str)
	{
		std::wstringstream input(str);
		input >> std::ws;
		input >> surname;
		input >> mathScore;
		input >> informaticsScore;
		input >> literatureScore;
	}

	int getTotalScore() const
	{
		return mathScore + informaticsScore + literatureScore;
	}
};

std::wostream& operator<<(std::wostream& output, Enrolle const& enrolle)
{
	output << enrolle.surname << " " << enrolle.getTotalScore();

	return output;
}

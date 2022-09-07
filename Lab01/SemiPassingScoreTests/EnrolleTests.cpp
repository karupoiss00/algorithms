#include <iostream>
#include <sstream>
#include <string>
#include <catch2/catch.hpp>
#include <Windows.h>
#include "../SemiPassingScore/Enrolle.h"

using namespace std;

TEST_CASE("create from string")
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	GIVEN("valid string")
	{
		stringstream output;
		string enrolleStr = "Петров 7 8 3";
		Enrolle enrolle;

		enrolle.fromString(enrolleStr);

		output << enrolle;

		REQUIRE(output.str() == enrolleStr);
		REQUIRE(enrolle.isValid());
	}

	GIVEN("invalid string")
	{
		stringstream output;
		string enrolleStr = "Петров 7 3";
		Enrolle enrolle;

		enrolle.fromString(enrolleStr);

		REQUIRE(!enrolle.isValid());
	}
}
#pragma once
#include <iostream>
#include <string>
#include "Calculator.h"

class App {
	bool running;
	std::string command;
	double result;

public:
	App();
	int exec();
	void welcome();
	void get_command();
	int check_command();
	void get_answer();
};
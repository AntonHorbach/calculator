#pragma once
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

class Calculator
{
	static bool begin;
	static const std::map<std::string, double(*)(double, double)> funcs;

	static void modify_expr(std::string& expr);
	static bool syntax_check(std::string& expr);
	static bool parse_brackets(const std::string& expr, std::pair<size_t, size_t> &brackets);
	static std::string parse_expr(const std::string& expr);

public:
	static std::string computation(std::string&& expr);
};


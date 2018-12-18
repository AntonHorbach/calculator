#pragma once
#define _USE_MATH_DEFINES

#include <map>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

class Calculator
{
	static bool begin;
	static const std::map<std::string, double(*)(double, double)> basic_funcs;
	static const std::map<std::string, double(*)(double)> funcs;

	static void modify_expr(std::string& expr);
	static bool parentheses_check(std::string& expr);
	static bool parse_parentheses(const std::string& expr, std::pair<size_t, size_t> &parentheses);
	static std::string parse_expr(const std::string& expr);

public:
	static std::string computation(std::string&& expr);
};


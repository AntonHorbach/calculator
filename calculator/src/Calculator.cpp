#include "Calculator.h"

bool Calculator::begin = true;

const std::map<std::string, double(*)(double, double)> Calculator::funcs = {
						{"+", [](double a, double b) {return a + b; }},
						{"*", [](double a, double b) {return a * b; }},
						{"-", [](double a, double b) {return a - b; }},
						{"/", [](double a, double b) {return a / b; }},
						{"^", [](double a, double b) {return pow(a, b); }}
};

void Calculator::modify_expr(std::string & expr)
{
	for (size_t i = 0; i < expr.length(); ++i) {
		if (expr[i] == '*' || expr[i] == '+' || expr[i] == '-'
			|| expr[i] == '/' || expr[i] == '^')
		{
			expr.insert(i, " ");
			expr.insert(i + 2, " ");
			i += 2;
		}
	}
	
	begin = false;
}

bool Calculator::syntax_check(std::string & expr)
{
	expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());

	size_t brackets = 0;
	bool operators = false, operands = false;

	if (expr[0] == '*' || expr[0] == '-' || expr[0] == '+' || expr[0] == '/' ||
		expr[0] == '^' || expr[0] == ')')
		return false;

	for (char c : expr) {
		if (brackets < 0) return false;
		if (c == '*' || c == '-' || c == '+' || c == '/' ||
			c == '^')
		{
			if (operators) return false;
			operators = true;
		}
		else if (c == '(') {
			++brackets;
			operators = false;
		}
		else if (c == ')') {
			--brackets;
			operators = false;
		}
		else operators = false;
	}
	
	if (brackets != 0) return false;

	return true;
}

bool Calculator::parse_brackets(const std::string & expr, std::pair<size_t, size_t> &brackets)
{
	size_t flag = 0;

	for (size_t i = 0; i < expr.length(); ++i) {
		if (expr[i] == '(') {
			if (flag == 0) brackets.first = i;
			++flag;
		}
		else if (expr[i] == ')') {
			--flag;

			if (flag == 0) {
				brackets.second = i;
				return true;
			}
		}
	}

	return false;
}

std::string Calculator::parse_expr(const std::string & expr)
{
	double res;
	std::stringstream ss(expr);

	std::vector<double> operands;
	std::vector<std::string> operators;

	size_t j = 0;
	while (!ss.eof()) {
		std::string temp;
		double d_temp;

		if (j % 2 == 0) {
			ss >> d_temp;
			operands.push_back(d_temp);
		}
		else {
			ss >> temp;
			operators.push_back(temp);
		}

		++j;
	}

	for (size_t i = 0; i < operators.size(); ++i) {
		if (operators[i] == "*" || operators[i] == "^" || operators[i] == "/") {
			operands[i] = funcs.at(operators[i])(operands[i], operands[i + 1]);

			operators.erase(operators.begin() + i);
			operands.erase(operands.begin() + i + 1);
		}
	}

	res = operands[0];
	for (size_t i = 0; i < operators.size(); ++i) {
		res = funcs.at(operators[i])(res, operands[i + 1]);
	}

	return std::to_string(res);
}

std::string Calculator::computation(std::string&& expr)
{
	if (begin) {
		if(!syntax_check(expr)) throw std::invalid_argument(expr);
		modify_expr(expr);
	}
	std::cout << expr << '\n';

	std::pair<size_t, size_t> brackets;

	while (true) {
		if (parse_brackets(expr, brackets)) {
			expr.replace(brackets.first, brackets.second - 
				brackets.first + 1,
				computation(expr.substr(brackets.first + 1,
					brackets.second - brackets.first - 1)));

			parse_brackets(expr, brackets);
		}
		else break;
	}

	begin = true;
	return parse_expr(expr);
}

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

void Calculator::parse_brackets(const std::string & expr, std::vector<std::pair<size_t, size_t>> &brackets)
{
	size_t flag = 0;
	size_t first;

	brackets.clear();

	std::cout << "Parse\n";

	for (size_t i = 0; i < expr.length(); ++i) {
		std::cout << i << ' ' << expr[i] << '\n';

		if (expr[i] == '(') {
			if (flag == 0) first = i;
			++flag;
		}
		else if (expr[i] == ')') {
			--flag;

			if (flag == 0) {
				brackets.push_back({ first, i });
			}
		}
	}
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
			std::cout << d_temp << '\n';
			operands.push_back(d_temp);
		}
		else {
			ss >> temp;
			std::cout << temp << '\n';
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
		std::cout << res << '\n';
	}

	return std::to_string(res);
}

std::string Calculator::computation(std::string&& expr)
{
	if(begin) modify_expr(expr);
	std::cout << expr << '\n';

	std::vector<std::pair<size_t, size_t>> brackets;
	parse_brackets(expr, brackets);

	for (const auto &pair : brackets)
		std::cout << pair.first << ' ' << pair.second << '\n';

	while (true) {
		if (!brackets.empty()) {
			expr.replace(brackets[0].first, brackets[0].second - 
				brackets[0].first + 1,
				computation(expr.substr(brackets[0].first + 1,
					brackets[0].second - brackets[0].first - 1)));

			parse_brackets(expr, brackets);
			for (const auto &pair : brackets)
				std::cout << pair.first << ' ' << pair.second << '\n';
		}
		else break;
	}

	begin = true;
	return parse_expr(expr);
}

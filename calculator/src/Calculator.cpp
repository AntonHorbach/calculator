#include "Calculator.h"

bool Calculator::begin = true;

const std::map<std::string, double(*)(double, double)> Calculator::basic_funcs = {
	{"+", [](double a, double b) {return a + b; }},
	{"*", [](double a, double b) {return a * b; }},
	{"-", [](double a, double b) {return a - b; }},
	{"/", [](double a, double b) {return a / b; }},
	{"^", [](double a, double b) {return pow(a, b); }}
};

const std::map<std::string, double(*)(double)> Calculator::funcs = {
	{"sin", [](double a) { return sin(a); }},
	{"cos", [](double a) {return cos(a); }},
	{"tan", [](double a) {return tan(a); }},
	{"ctan", [](double a) {return 1 / tan(a); }},
	{"asin", [](double a) { return asin(a); }},
	{"acos", [](double a) {return acos(a); }},
	{"atan", [](double a) {return atan(a); }},
	{"actan", [](double a) {return M_PI_2 - atan(a); }},
	{"log", [](double a) {return log(a); }},
	{"sqrt", [](double a) {return sqrt(a); }},
	{"exp", [](double a) {return exp(a); }}
};

void Calculator::modify_expr(std::string & expr)
{
	expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());

	std::string member("");
	const std::string opers("-+*/^");

	size_t count = 0;
	bool oper_status = false;

	for (size_t i = 0; i < expr.length(); ++i) {
		if (!oper_status && i != 0 && std::find(opers.begin(), opers.end(), expr[i]) != opers.end())
		{
			expr.insert(i, " ");
			expr.insert(i + 2, " ");

			i += 2;
			oper_status = true;
			member.clear();
		}
		else {
			member += expr[i];
			member.erase(std::remove(member.begin(), member.end(), ' '), member.end());
			++count;

			if (funcs.find(member) != funcs.end())
			{
				expr.insert(i + 1, " ");

				i += 2;
				member.clear();
				count = 0;
			}

			if (count == 6) {
				member.clear();
				count = 0;
			}

			oper_status = false;
		}
	}

	begin = false;
}

bool Calculator::parentheses_check(std::string & expr)
{
	size_t parentheses = 0;

	for (char c : expr) {
		if (parentheses < 0) return false;
		if (c == '(') ++parentheses;
		else if (c == ')') --parentheses;
	}

	if (parentheses != 0) return false;

	return true;
}

bool Calculator::parse_parentheses(const std::string & expr, std::pair<size_t, size_t> &parentheses)
{
	size_t flag = 0;

	for (size_t i = 0; i < expr.length(); ++i) {
		if (expr[i] == '(') {
			if (flag == 0) parentheses.first = i;
			++flag;
		}
		else if (expr[i] == ')') {
			--flag;

			if (flag == 0) {
				parentheses.second = i;
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

		ss >> temp;

		try {
			if (j % 2 == 0) {
				if (atof(temp.c_str()) == 0.0) {
					double d_temp;

					ss >> d_temp;
					operands.push_back(funcs.at(temp)(d_temp));
				}
				else {
					operands.push_back(atof(temp.c_str()));
				}
			}
			else {
				operators.push_back(temp);
			}
		}
		catch (...) {
			throw std::invalid_argument("Invalid command: " + expr);
		}

		++j;
	}

	for (size_t i = 0; i < operators.size(); ++i) {
		if (operators[i] == "*" || operators[i] == "^" || operators[i] == "/") {
			operands[i] = basic_funcs.at(operators[i])(operands[i], operands[i + 1]);

			operators.erase(operators.begin() + i);
			operands.erase(operands.begin() + i + 1);

			--i;
		}
	}

	res = operands[0];
	for (size_t i = 0; i < operators.size(); ++i) {
		try {
			res = basic_funcs.at(operators[i])(res, operands[i + 1]);
		}
		catch (std::out_of_range &e) {
			throw std::invalid_argument("Invalid operator: " + operators[i]);
		}
	}

	return std::to_string(res);
}

std::string Calculator::computation(std::string&& expr)
{
	if (begin) {
		modify_expr(expr);
		if (!parentheses_check(expr)) throw std::invalid_argument(expr);
	}

	std::cout << expr << '\n';

	std::pair<size_t, size_t> parentheses;

	while (true) {
		if (parse_parentheses(expr, parentheses)) {
			expr.replace(parentheses.first, parentheses.second -
				parentheses.first + 1,
				computation(expr.substr(parentheses.first + 1,
					parentheses.second - parentheses.first - 1)));

			parse_parentheses(expr, parentheses);
		}
		else break;
	}

	begin = true;
	return parse_expr(expr);
}
#include "app.h"

App::App()
{
	running = true;
}

int App::exec()
{
	welcome();

	while (running) {
		get_command();

		if (check_command() == EXIT_SUCCESS) {
			get_answer();
		}
		else std::cout << "Invalid command! Try again!\n";
	}

	return EXIT_SUCCESS;
}

void App::welcome()
{
	std::cout << "------Welcome!------\n"
			  << "Functions: +, -, *, /, ^\nsin, cos, tan, ctan,\n"
	          << "asin, acos, atan, actan\n"
			  <<"exp, log, sqrt\n(enter q to exit)\n\n";
}

void App::get_command()
{
	std::cout << "What u want to do?: ";
	std::getline(std::cin, command);
}

int App::check_command()
{
	if (command == "q") {
		running = false;
		return EXIT_FAILURE;
	}
	else if (command.find_first_not_of("1234567890+-*/^ ()sinoctaqrlgexp.") != std::string::npos)
		return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}

void App::get_answer()
{
	try {
		std::cout << "Result: " << Calculator::computation(std::move(command)) << '\n';
	}
	catch (std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
}

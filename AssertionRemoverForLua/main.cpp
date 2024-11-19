#include "AsserRemover.h"

#include <iostream>

int main(int argc, char* argv[]) {
	// first argument
	if (argc != 2) {
		std::cout << "Missing a path.\n";
		return -1;
	} // just one arg
	else if (argc > 2) {
		std::cout << "Too many args.\n";
		return -1;
	}

	// path to an existing file/dir
	std::filesystem::path pathToFile{ argv[1] };
	pathToFile.make_preferred();
	std::cout << "Preferred Path: " << pathToFile << '\n';
	if (!std::filesystem::exists(pathToFile)) {
		std::cout << "Invalid Path.\n";
		return -1;
	}

	// start
	int numAssers = startAsserRemover(pathToFile);

	std::cout << "\nDone.\n"
		<< numAssers << " assertions were removed.\n";
}
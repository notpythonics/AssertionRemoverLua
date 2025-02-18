#include "AsserRemover.h"

#include <iostream>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Missing a path.\n";
		return -1;
	}

	fs::path pathToFile{ argv[1] };
	pathToFile.make_preferred();
	std::cout << "Preferred path: " << pathToFile << '\n';

	if (!fs::exists(pathToFile)) {
		std::cout << "Invalid path.\n";
		return -1;
	}

	// start
	int numAssers = startAsserRemover(pathToFile);

	std::cout << "\nDone.\n"
		<< numAssers << " assertions were removed.\n";
}
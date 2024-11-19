#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

int numAssers{};

void removeAsserFromDir(std::filesystem::path);

void removeAsserFromFile(std::filesystem::path pathToFile) {
	// store lines that have no assertions
	std::vector<std::string> linesWithoutAsser{};
	linesWithoutAsser.reserve(20);

	const auto* c_pathToFile{ pathToFile.c_str() };

	// dir
	if (std::filesystem::is_directory(pathToFile)) {
		removeAsserFromDir(pathToFile);
		return;
	}

	// file
	std::ifstream ifile{ c_pathToFile };
	// read non-asserted lines to vector
	for (std::string line; std::getline(ifile, line);) {
		if (!(line.find("assert") != line.npos)) {
			linesWithoutAsser.push_back(line);
			++numAssers;
		}
	}

	ifile.close();

	// over-write to file from vector
	std::ofstream ofile{ c_pathToFile, std::ios_base::trunc };
	for (const auto& line : linesWithoutAsser)
		ofile << line << '\n';
}

void removeAsserFromDir(std::filesystem::path pathToDir) {
	// traverse the dir
	std::filesystem::directory_iterator luaRootDir_iter{ pathToDir };
	for (const std::filesystem::directory_entry& entry : luaRootDir_iter) {
		removeAsserFromFile(entry.path());
	}
}

int main(int argc, char* argv[]) {
	// one argument
	if (argc != 2) {
		std::cout << "Missing a path.\n";
		return -1;
	}
	else if(argc > 2){
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
	removeAsserFromFile(pathToFile);

	std::cout << "\nDone.\n"
		<< numAssers << " assertions were removed.\n";
}
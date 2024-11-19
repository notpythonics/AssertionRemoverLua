#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

int numAssers{};

void removeAsserFromDir(std::filesystem::path);

void removeAsserFromFile(std::filesystem::path pathToFile) {
	std::vector<std::string> linesWithoutAsser{};
	linesWithoutAsser.reserve(20);
	const auto* c_pathToFile{ pathToFile.c_str() };

	// sub dir
	if (std::filesystem::is_directory(pathToFile)) {
		removeAsserFromDir(pathToFile); // recursion
		return;
	}

	std::ifstream ifile{ c_pathToFile };

	for (std::string line; std::getline(ifile, line);) {
		if (line.find("assert") != line.npos)
			++numAssers;
		else
			linesWithoutAsser.push_back(line);
	}

	ifile.close();

	std::ofstream ofile{ c_pathToFile, std::ios_base::trunc };
	for (const auto& line : linesWithoutAsser)
		ofile << line << '\n';
}

void removeAsserFromDir(std::filesystem::path pathToDir) {
	std::filesystem::directory_iterator luaRootDir_iter{ pathToDir };
	for (const std::filesystem::directory_entry& entry : luaRootDir_iter) {
		removeAsserFromFile(entry.path());
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Missing a path.\n";
		return -1;
	}
	else if(argc > 2){
		std::cout << "Too many args.\n";
		return -1;
	}

	std::filesystem::path pathToFile{ argv[1] };
	pathToFile.make_preferred();
	std::cout << "Preferred Path: " << pathToFile << '\n';
	if (!std::filesystem::exists(pathToFile)) {
		std::cout << "Invalid Path.\n";
		return -1;
	}

	removeAsserFromFile(pathToFile);

	std::cout << "\nDone.\n"
		<< numAssers << " assertions were removed.\n";
}
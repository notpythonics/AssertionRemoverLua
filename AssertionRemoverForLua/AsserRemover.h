#ifndef ASSER_REMOVER
#define ASSER_REMOVER

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>


inline int startAsserRemover(std::filesystem::path pathToFile) {
	// dir
	if (std::filesystem::is_directory(pathToFile)) {
		return removeAsserFromDir(pathToFile);
	} // file
	else {
		return removeAsserFromFile(pathToFile);
	}
}

inline int removeAsserFromFile(std::filesystem::path pathToFile) {
	const auto* c_pathToFile{ pathToFile.c_str() };
	// lines that have no assertions
	std::vector<std::string> linesWithoutAsser{};
	linesWithoutAsser.reserve(20);
	// how many assertions were removed
	int numAssers{};

	std::ifstream ifile{ c_pathToFile };
	// push non-asserted lines to vector
	for (std::string line; std::getline(ifile, line);) {
		if (line.find("assert") != line.npos)
			++numAssers;
		else
			linesWithoutAsser.push_back(line);
	}

	ifile.close();

	// over-write file from vector
	std::ofstream ofile{ c_pathToFile, std::ios_base::trunc };
	for (const auto& line : linesWithoutAsser)
		ofile << line << '\n';

	return numAssers;
}

inline int removeAsserFromDir(std::filesystem::path pathToDir) {
	// how many assertions were removed
	int numAssers{};

	// traverse the dir
	std::filesystem::directory_iterator luaRootDir_iter{ pathToDir };
	for (const std::filesystem::directory_entry& entry : luaRootDir_iter) {
		numAssers += removeAsserFromFile(entry.path());
	}

	return numAssers;
}

#endif
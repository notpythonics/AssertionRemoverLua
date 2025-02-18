#ifndef ASSER_REMOVER
#define ASSER_REMOVER

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string_view>

namespace fs = std::filesystem;


inline constexpr std::string_view BLACK_LISTED_WORD{ "assert" };

inline int removeAsserFromFile(const fs::path& pathToFile);
inline int removeAsserFromDir(const fs::path& pathToDir);

inline int startAsserRemover(const fs::path& path) {
	if (fs::is_directory(path))
		return removeAsserFromDir(path);
	else
		return removeAsserFromFile(path);
}

inline int removeAsserFromFile(const fs::path& pathToFile) {
	const auto* c_pathToFile{ pathToFile.c_str() };
	// non-asserted lines
	std::vector<std::string> linesWithoutAsser{};
	linesWithoutAsser.reserve(20);
	// assertions removed
	int numAssers{};

	std::ifstream ifile{ c_pathToFile };
	// push non-asserted lines to vector
	for (std::string line; std::getline(ifile, line);) {
		if (line.find(BLACK_LISTED_WORD) != line.npos)
			++numAssers;
		else
			linesWithoutAsser.push_back(line);
	}

	ifile.close();

	// over-write file from vector
	std::ofstream ofile{ c_pathToFile, std::ios_base::trunc };
	for (const auto& line : linesWithoutAsser)
		ofile << line << '\n';

	ofile.close();

	return numAssers;
}

inline int removeAsserFromDir(const fs::path& pathToDir) {
	// how many assertions were removed
	int numAssers{};

	// traverse the dir
	fs::directory_iterator luaRootDir_iter{ pathToDir };
	for (const fs::directory_entry& entry : luaRootDir_iter) {
		const auto& path{ entry.path() };
		if (fs::is_directory(path)) {
			numAssers += removeAsserFromDir(path); // recursion
		}
		else {
			numAssers += removeAsserFromFile(entry.path());
		}
	}

	return numAssers;
}

#endif
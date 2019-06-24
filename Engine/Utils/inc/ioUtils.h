#pragma once

#include <fstream>
#include <istream>

class ioUtils {
public:
	static void ioUtils::getNextLine(std::istringstream& stream, std::ifstream& infile) {
		std::string line;
		while (std::getline(infile, line)) {
			if (line[0] == '#' || line.empty())
				continue;
			(&stream)->str(line);
			(&stream)->clear();
			return;
		}
	}

};
#include <string>
#include <sstream>
#include <iostream>
#include "filemanip.hpp"

int main(int argc, char const *argv[]) {
	std::string data;
	read_text_file(std::string(argv[1]), data);
	std::cout << data << '\n';
	std::istringstream iss(data);
	while (!iss.eof()) {
		std::string tmp;
		iss >> tmp;
		std::cout << tmp << '\n';
	}
	return 0;
}

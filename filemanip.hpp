#ifndef FILEMANIP_HPP_INCLUDED
#define FILEMANIP_HPP_INCLUDED

#include <fstream>
#include <string>

void write_text_file(const std::string filename, const std::string& data) {
	std::ofstream ofs(filename);
	ofs << data;
	ofs.close();
}

void read_text_file(const std::string filename, std::string& data) {
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		throw std::invalid_argument("cannot open "+filename);
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	data = std::string(it, last);
	ifs.close();
}

#endif /* end of include guard: FILEMANIP_HPP_INCLUDED */

#include <iostream>
#include <chrono>

#include "solver.hpp"
#include "filemanip.hpp"

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		throw std::invalid_argument("too much/few arguments.");
	}
	std::string data;
	read_text_file(std::string(argv[1]), data);
	std::stringstream ss{data};
	std::string buf;
	std::vector<std::string> problems;
	while (std::getline(ss, buf)) {
		if (buf == "") {
			continue;
		}
		problems.push_back(buf);
	}
	double total = 0;
	int node_count = 0;
	for (auto& problem: problems) {
		sudoku::board<3> bd;
		bd.string_input(problem);
		sudoku::solver<3> slv;
		std::chrono::system_clock::time_point start, end;
		start = std::chrono::system_clock::now();
		slv.solve(bd, false);
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		std::cout << "time: " << std::dec << elapsed << " microseconds" << '\n';
		node_count += slv.get_node_count();
		total += elapsed;
	}
	std::cout  << std::dec;
	std::cout << "average node: " << (double)node_count/problems.size() << '\n';
	std::cout << "total time: " << std::dec << total << " microseconds" << '\n';
	std::cout << "average time: " << total/problems.size() << " microseconds" << '\n';
	return 0;
}

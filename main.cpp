#include <iostream>
#include <chrono>

#include "board.hpp"
#include "solver.hpp"
#include "filemanip.hpp"

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		throw std::invalid_argument("too much/few arguments.");
	}
	std::string data;
	read_text_file(std::string(argv[2]), data);
	int opt = std::stoi(argv[1]);
	std::chrono::system_clock::time_point start, end;
	if (opt == 3) {
		sudoku::board<3> bd;
		bd.string_input(data);
		bd.show(std::cout);
		sudoku::solver<3> slv;
		start = std::chrono::system_clock::now();
		// slv.solve(bd, false);
		if (slv.is_multiple_solutions(bd)) {
			std::cerr << "multi solution" << '\n';
		}else {
			std::cerr << "single solution" << '\n';
		}
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		for (const auto& a: slv.solution()) {
			a.show(std::cout);
		}
		// std::cout << slv.solution().size() << '\n';
		// std::cout << slv.is_multiple_solutions(bd) << '\n';
		// std::cout << slv.solution().size() << '\n';
		std::cerr << "time: " << std::dec << elapsed << " microseconds" << '\n';
		std::cerr << "node: " << std::dec << slv.get_node_count() << '\n';
	} else if (opt == 4) {
		sudoku::board<4> bd;
		bd.string_input(data);
		bd.show(std::cout);
		while (bd.update());
		sudoku::solver<4> slv;
		start = std::chrono::system_clock::now();
		// slv.solve(bd, false);
		if (slv.is_multiple_solutions(bd)) {
			std::cerr << "multi solution" << '\n';
		}else {
			std::cerr << "single solution" << '\n';
		}
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		for (const auto& a: slv.solution()) {
			a.show(std::cout);
		}
		std::cerr << "time: " << std::dec << elapsed << " microseconds" << '\n';
		std::cerr << "node: " << std::dec << slv.get_node_count() << '\n';
		// std::cout << slv.solution().size() << '\n';
		// std::cout << slv.is_multiple_solutions(bd) << '\n';
		// std::cout << slv.solution().size() << '\n';
		// std::cout << std::dec << slv.get_node_count() << '\n';
	} else {
		throw std::invalid_argument("bitsudoku [3|4] filename");
	}
	return 0;
}

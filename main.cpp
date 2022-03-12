#include <iostream>
#include <chrono>

#include "board.hpp"
#include "solver.hpp"
#include "filemanip.hpp"

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		throw std::invalid_argument("too much/few arguments.");
	}
	std::cout << "bit board sudoku" << std::endl;
	std::string data;
	read_text_file(std::string(argv[2]), data);
	int opt = std::stoi(argv[1]);
	std::chrono::system_clock::time_point start, end;
	if (opt == 3) {
		sudoku::board<3> bd;
		bd.string_input(data);
		bd.show();
		sudoku::solver<3> slv;
		start = std::chrono::system_clock::now();
		slv.solve(bd, false);
		// if (slv.is_multiple_solutions(bd)) {
		// 	std::cout << "multi solution" << '\n';
		// }else {
		// 	std::cout << "single solution" << '\n';
		// }
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		for (const auto& a: slv.result()) {
			a.show();
		}
		// std::cout << slv.result().size() << '\n';
		// std::cout << slv.is_multiple_solutions(bd) << '\n';
		// std::cout << slv.result().size() << '\n';
		std::cout << "time: " << std::dec << elapsed << " microseconds" << '\n';
		std::cout << "node: " << std::dec << slv.get_node_count() << '\n';
	} else if (opt == 4) {
		sudoku::board<4> bd;
		bd.string_input(data);
		bd.show();
		while (bd.update());
		bd.show();
		sudoku::solver<4> slv;
		start = std::chrono::system_clock::now();
		slv.solve(bd, false);
		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
		for (const auto& a: slv.result()) {
			a.show();
		}
		std::cout << "time: " << std::dec << elapsed << " microseconds" << '\n';
		std::cout << "node: " << std::dec << slv.get_node_count() << '\n';
		// std::cout << slv.result().size() << '\n';
		// std::cout << slv.is_multiple_solutions(bd) << '\n';
		// std::cout << slv.result().size() << '\n';
		// std::cout << std::dec << slv.get_node_count() << '\n';
	} else {
		throw std::invalid_argument("bitsudoku [3|4] filename");
	}
	return 0;
}

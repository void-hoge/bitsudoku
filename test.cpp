#include <chrono>
#include "generator.hpp"
#include "board.hpp"
#include "solver.hpp"

int main(int argc, char const *argv[]) {
	sudoku::board<3> bd;
	std::string in;
	std::getline(std::cin, in);
	bd.string_input(in);
	sudoku::solver<3> slv;
	if (slv.is_multiple_solutions(bd)) {
		std::cerr << "multiple solution" << '\n';
		slv.solution().back().show(std::cout);
	} else {
		std::cerr << "single solutions" << '\n';
		slv.solution().back().show(std::cout);
	}
	std::cerr << slv.get_node_count() << '\n';
	return 0;
}

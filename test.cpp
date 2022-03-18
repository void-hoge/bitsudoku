#include <chrono>
#include "generator.hpp"
#include "board.hpp"
#include "solver.hpp"

int main(int argc, char const *argv[]) {
	sudoku::generator<3> gn(440643295);
	while (gn.generate(17));
	gn.reconstruct();
	sudoku::board<3> result = gn.get_board();
	std::cout << result.string_output() << '\n';
	result.show(std::cout);
	sudoku::solver<3> slv;
	if (slv.is_multiple_solutions(result)) {
		std::cerr << "multiple solution" << '\n';
		slv.solution().back().show(std::cout);
	} else {
		std::cerr << "single solutions" << '\n';
		slv.solution().back().show(std::cout);
	}
	return 0;
}

#include <chrono>
#include "generator.hpp"
#include "board.hpp"
#include "solver.hpp"

int main(int argc, char const *argv[]) {
	sudoku::generator<3> gn(0);
	while (gn.generate(17));
	gn.reconstruct();
	sudoku::board<3> result = gn.get_board();
	std::cout << result.string_output() << '\n';
	result.show(std::cout);
	while (result.update());
	result.show(std::cout);
	return 0;
}

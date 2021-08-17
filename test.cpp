#include "board.hpp"
#include "generator.hpp"
#include <random>

int main() {
	std::random_device seed_gen;
	sudoku::generator<4>
	gen(seed_gen());
	// gen(0);
	sudoku::board<4> bd;
	gen.generate(bd, 100);
	bd.show();
	std::cout << bd.string_output() << '\n';
	std::cerr << "hints count: " << (~bd.get_blank()).count() << '\n';
	return 0;
}

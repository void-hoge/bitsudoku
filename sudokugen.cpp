#include "board.hpp"
#include "generator.hpp"
#include <random>

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		throw std::invalid_argument("too much/few arguments.");
	}
	std::cout << "sudoku generator" << '\n';
	const int size = std::stoi(argv[1]);
	const int clues_count = std::stoi(argv[2]);
	std::random_device seed_gen;
	if (size == 3) {
		sudoku::generator<3> gen(seed_gen());
		sudoku::board<3> bd;
		gen.generate(bd, clues_count);
		bd.show();
		std::cout << bd.string_output() << '\n';
		std::cerr << "clues count: " << (~bd.get_blank()).count() << '\n';
	}else if (size == 4) {
		sudoku::generator<4> gen(seed_gen());
		sudoku::board<4> bd;
		gen.generate(bd, clues_count);
		bd.show();
		std::cout << bd.string_output() << '\n';
		std::cerr << "clues count: " << (~bd.get_blank()).count() << '\n';
	}
	return 0;
}

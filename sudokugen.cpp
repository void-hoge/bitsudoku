#include "board.hpp"
#include "generator.hpp"
#include <random>

int main(int argc, char const *argv[]) {
	if (argc != 3 && argc != 4) {
		throw std::invalid_argument("too much/few arguments.");
	}
	const int size = std::stoi(argv[1]);
	const int clue_count = std::stoi(argv[2]);
	unsigned seed = 0;
	if (argc == 4) {
		seed = std::stoul(argv[3]);
	}else {
		std::random_device seed_gen;
		seed = seed_gen();
	}
	std::cerr << "seed: ";
	std::cout << seed << '\n';
	if (size == 3) {
		sudoku::generator<3> gen(seed);
		std::cerr << "attempting";
		while (gen.generate(clue_count)) {
			std::cerr << ".";
		}
		std::cerr << '\n';
		auto solution = gen.get_board();
		gen.reconstruct();
		std::cout << gen.get_board().string_output() << '\n';
		gen.get_board().show(std::cout);
		solution.show(std::cout);
	}else if (size == 4) {
		sudoku::generator<4> gen(seed);
		std::cerr << "attempting";
		while (gen.generate(clue_count)) {
			std::cerr << ".";
		}
		std::cerr << '\n';
		auto solution = gen.get_board();
		gen.reconstruct();
		std::cout << gen.get_board().string_output() << '\n';
		gen.get_board().show(std::cout);
		solution.show(std::cout);
	}else {
		std::cerr << "Invalid size." << '\n';
	}
	return 0;
}

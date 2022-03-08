#include <chrono>
#include "generator.hpp"
#include "board.hpp"

int main(int argc, char const *argv[]) {
	sudoku::generator<3> gn(0);
	while (!gn.generate(20));
	return 0;
}

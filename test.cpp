#include <chrono>
#include "board.cpp"

int main(int argc, char const *argv[]) {
	sudoku::board<3> bd;
	bd.set(0, 0);
	bd.dump();
	return 0;
}

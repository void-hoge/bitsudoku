#include <chrono>
#include "board.cpp"

int main(int argc, char const *argv[]) {
	sudoku::board<3> bd;
	bd.set(0*9+1, 2);
	bd.set(0*9+5, 7);
	bd.set(0*9+6, 8);
	bd.set(1*9+0, 3);
	bd.set(2*9+4, 4);
	bd.set(2*9+5, 5);
	bd.set(4*9+2, 6);
	bd.set(5*9+0, 4);
	bd.set(5*9+2, 7);
	bd.set(6*9+0, 5);
	bd.set(6*9+2, 8);
	bd.set(7*9+0, 6);
	bd.show();
	bd.dump();
	bd.update_naked_subset();
	bd.dump();
	return 0;
}

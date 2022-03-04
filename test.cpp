#include <chrono>
#include "board.cpp"

int main(int argc, char const *argv[]) {
	sudoku::board<3> bd;
	bd.set(0*9+2, 2);
	bd.set(1*9+3, 0);
	bd.set(1*9+4, 1);
	bd.set(2*9+0, 3);
	bd.set(3*9+1, 0);
	bd.set(4*9+1, 1);
	bd.show();
	bd.dump();
	bd.update_hidden_subset();
	bd.dump();
	return 0;
}

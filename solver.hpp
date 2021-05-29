#ifndef SOLVER_HPP_INCLUDED
#define SOLVER_HPP_INCLUDED

#include <vector>
#include <array>
#include <bitset>
#include <iostream>

namespace sudoku {

template<size_t SIZE>
class solver {
private:
	bool recursion(board<SIZE> bd, const size_t pos, const int num);
public:
	size_t node_count;
	solver();
	void solve(board<SIZE> bd);
};


} // namespace sudoku

#include "solver.cpp"

#endif // include guard: SOLVER_HPP_INCLUDED

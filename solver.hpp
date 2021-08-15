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
	size_t node_count;
public:
	board<SIZE> solved;
	solver();
	void solve(board<SIZE> bd);
	size_t get_node_count() const;
};


} // namespace sudoku

#include "solver.cpp"

#endif // include guard: SOLVER_HPP_INCLUDED

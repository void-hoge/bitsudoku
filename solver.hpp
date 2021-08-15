#ifndef SOLVER_HPP_INCLUDED
#define SOLVER_HPP_INCLUDED

#include "board.hpp"
#include <vector>
#include <array>
#include <bitset>
#include <iostream>
#include <functional>

namespace sudoku {

template<size_t SIZE>
class solver {
private:
	bool dfs(board<SIZE> bd, const size_t pos, const int num, const bool full_search);
	bool dfs_check_multi_sol(board<SIZE> bd, const size_t pos, const int num);
	size_t node_count;
	std::vector<board<SIZE>> solved;
	bool is_found_solution;
public:
	solver();
	void solve(board<SIZE> bd, const bool full_search);
	bool is_multiple_solutions(board<SIZE> bd);
	size_t get_node_count() const;
	std::vector<board<SIZE>>& result() {
		return std::ref(solved);
	}
};


} // namespace sudoku

#include "solver.cpp"

#endif // include guard: SOLVER_HPP_INCLUDED

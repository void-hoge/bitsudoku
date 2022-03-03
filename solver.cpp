#ifndef SOLVER_CPP_INCLUDED
#define SOLVER_CPP_INCLUDED

#include "board.hpp"
#include "solver.hpp"
#include <vector>
#include <array>
#include <bitset>
#include <iostream>
#include <functional>

namespace sudoku{

template<size_t SIZE>
solver<SIZE>::solver() {
	node_count = 0;
	is_found_solution = false;
}

template<size_t SIZE>
void solver<SIZE>::solve(board<SIZE> bd, const bool full_search) {
	solved.clear();
	node_count = 1;
	is_found_solution = false;
	try {
		while (bd.update());
	} catch (std::exception e){
		bd.show();
		bd.dump();
		throw std::logic_error("invalid input (no solution)");
	}
	if (!bd.find_error()) {
		bd.show();
		bd.dump();
		throw std::logic_error("invalid input (no solution)");
	}
	const auto blank = bd.get_blank();
	if (blank == 0) {
		is_found_solution = true;
		solved.push_back(bd);
		return;
	}
	// size_t pos = 0;
	// for (;pos < blank.size(); pos++) {
	// 	if (blank[pos]) {
	// 		break;
	// 	}
	// }
	const size_t pos = bd.get_least_unstable();
	const auto settable =  bd.get_settable_num(pos);
	for (const auto& tmp: settable) {
		if (dfs(bd, pos, tmp, full_search)) {
			return;
		}
	}
	if (!full_search) {
		bd.show();
		bd.dump();
		throw std::logic_error("invalid input (no solution)");
	}
}

template<size_t SIZE>
bool solver<SIZE>::dfs(board<SIZE> bd, const size_t pos, const int num, const bool full_search) {
	node_count++;
	if (node_count%10000 == 0) {
		std::cout << std::dec << node_count << std::endl;
	}
	bd.set(pos, num);
	try {
		while (bd.update());
	} catch (std::exception& e) {
		return false;
	}
	if (!bd.find_error()) {
		return false;
	}
	const auto blank = bd.get_blank();
	if (blank == 0) {
		is_found_solution = true;
		solved.push_back(bd);
		return !full_search;
	}
	// size_t npos = 0;
	// for (;npos < blank.size(); npos++) {
	// 	if (blank[npos]) {
	// 		break;
	// 	}
	// }
	const size_t npos = bd.get_least_unstable();
	// std::cerr << blank << '\n';
	const auto settable = bd.get_settable_num(npos);
	for (const auto& tmp: settable) {
		if (dfs(bd, npos, tmp, full_search)) {
			return true;
		}
	}
	return false;
}

template<size_t SIZE>
bool solver<SIZE>::is_multiple_solutions(board<SIZE> bd) {
	solved.clear();
	node_count = 1;
	is_found_solution = false;
	try {
		while (bd.update());
	} catch (std::exception e){
		throw std::logic_error("invalid input (no solution)");
	}
	if (!bd.find_error()) {
		throw std::logic_error("invalid input (no solution)");
	}
	const auto blank = bd.get_blank();
	if (blank == 0) {
		solved.push_back(bd);
		is_found_solution = true;
		return false; // single solution
	}
	// size_t pos = 0;
	// for (;pos < blank.size(); pos++) {
	// 	if (blank[pos]) {
	// 		break;
	// 	}
	// }
	const size_t pos = bd.get_least_unstable();
	const auto settable = bd.get_settable_num(pos);
	for (const auto& tmp: settable) {
		if (dfs_check_multi_sol(bd, pos, tmp)) {
			return true;
		}
	}
	if (is_found_solution) {
		return false;
	} else {
		throw std::logic_error("invalid input (no solution)");
	}
}

template<size_t SIZE>
bool solver<SIZE>::dfs_check_multi_sol(board<SIZE> bd, const size_t pos, const int num) {
	node_count++;
	if (node_count%10000 == 0) {
		std::cout << std::dec << node_count << std::endl;
	}
	bd.set(pos, num);
	try {
		while (bd.update());
	} catch (std::exception& e) {
		return false;
	}
	if (!bd.find_error()) {
		return false;
	}
	const auto blank = bd.get_blank();
	if (blank == 0) {
		solved.push_back(bd);
		if (is_found_solution) {
			return true;
		} else {
			is_found_solution = true;
			return false;
		}
	}
	// size_t npos = 0;
	// for (;npos < blank.size(); npos++) {
	// 	if (blank[npos]) {
	// 		break;
	// 	}
	// }
	size_t npos = bd.get_least_unstable();
	auto settable = bd.get_settable_num(npos);
	for (const auto& tmp: settable) {
		if (dfs_check_multi_sol(bd, npos, tmp)) {
			return true;
		}
	}
	return false;
}

template <size_t SIZE>
size_t solver<SIZE>::get_node_count() const {
	return node_count;
}

} // namespace sudoku

#endif // include guard: SOLVER_CPP_INCLUDED

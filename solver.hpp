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

template<size_t SIZE>
solver<SIZE>::solver() {
	node_count = 0;
}

template<size_t SIZE>
void solver<SIZE>::solve(board<SIZE> bd) {
	while (bd.update());
	const auto blank = bd.get_blank();
	if (blank == 0) {
		bd.show();
		return;
	}
	size_t pos = 0;
	for (;pos < blank.size(); pos++) {
		if (blank[pos]) {
			break;
		}
	}
	auto settable =  bd.get_settable_num(pos);
	for (const auto& tmp: settable) {
		if (recursion(bd, pos, tmp)) {
			return;
		}
	}
}

template<size_t SIZE>
bool solver<SIZE>::recursion(board<SIZE> bd, const size_t pos, const int num) {
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
	const auto blank = bd.get_blank();
	if (blank == 0) {
		bd.show();
		return true;
	}
	size_t npos = 0;
	for (;npos < blank.size(); npos++) {
		if (blank[npos]) {
			break;
		}
	}
	auto settable =  bd.get_settable_num(npos);
	for (const auto& tmp: settable) {
		if (recursion(bd, npos, tmp)) {
			return true;
		}
	}
	return false;
}


} // namespace sudoku

#endif // include guard: SOLVER_HPP_INCLUDED

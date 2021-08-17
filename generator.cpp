#ifndef GENERATOR_CPP_INCLUDED
#define GENERATOR_CPP_INCLUDED

#include "board.hpp"
#include "generator.hpp"
#include "solver.hpp"

namespace sudoku {


// 適当に穴を開けて、解けなくなる直前のものを出力する。
template<size_t SIZE>
void generator<SIZE>::generate(board<SIZE>& bd, const size_t num_of_hints){
	fill(bd);
	// bd.show();
	bd.reset_possibilities();
	bd.build_possibilities();
	const auto notblank = ~bd.get_blank();
	while (true) {
		const size_t tmp = mt()%notblank.count();
		size_t pos = 0, count = 0;
		for (; pos < SIZE*SIZE*SIZE*SIZE; pos++) {
			if (notblank[pos]) {
				if (tmp == count) {
					break;
				}
				count++;
			}
		}
		if (dfs(bd, SIZE*SIZE*SIZE*SIZE-num_of_hints, pos)) {
			break;
		}
	}
	bd = result;
}

template<size_t SIZE>
bool generator<SIZE>::dfs(board<SIZE> bd, const int n, const size_t pos) {
	if (n == 0) {
		result = bd;
		return true;
	}
	bd.erase_stable(pos);
	bd.reset_possibilities();
	bd.build_possibilities();
	const auto notblank = ~bd.get_blank();
	if (slv.is_multiple_solutions(bd)) {
		return false;
	}
	while (true) {
		const size_t tmp = mt()%notblank.count();
		size_t npos = 0, count = 0;
		for (; npos < SIZE*SIZE*SIZE*SIZE; npos++) {
			if (notblank[npos]) {
				if (tmp == count) {
					break;
				}
				count++;
			}
		}
		if (dfs(bd, n-1, npos)) {
			return true;
		}
	}
	return false;
}

// SIZE*SIZE個適当に埋めて、適当に解いたものを解とする。
template<size_t SIZE>
void generator<SIZE>::fill(board<SIZE>& bd) {
	bd.reset_stable();
	bd.reset_possibilities();
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		const auto blank = bd.get_blank();
		size_t tmp = mt()%blank.count();
		size_t pos = 0;
		for (size_t j = 0; pos < tmp; j++) {
			if (blank[j]) {
				pos++;
			}
		}
		const auto settable = bd.get_settable_num(pos);
		int num = settable.at(mt()%settable.size());
		bd.set(pos, num);
	}
	slv.solve(bd, false);
	bd = *(slv.result().begin());
}

} // namespace sudoku

#endif /* end of include guard: GENERATOR_CPP_INCLUDED */

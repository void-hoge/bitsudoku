#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "board.hpp"
#include "solver.cpp"
#include <random>

namespace sudoku {

template<size_t SIZE>
class generator {
private:
	std::mt19937 mt;
	solver<SIZE> slv;
	board<SIZE> result;
	void fill(board<SIZE>& bd);
	bool dfs(board<SIZE> bd, const int n, const size_t pos);
public:
	generator(const unsigned sd): mt(std::mt19937(sd)) {}
	void generate(board<SIZE>& bd, const size_t num_of_hints);
};

} // namespace sudoku

#include "generator.cpp"

#endif /* end of include guard: GENERATOR_HPP_INCLUDED */

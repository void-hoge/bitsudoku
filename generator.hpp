#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "board.hpp"
#include "solver.cpp"
#include <list>
#include <random>

namespace sudoku {

template<size_t SIZE>
class generator {
private:
	std::mt19937 mt;
	solver<SIZE> slv;
	board<SIZE> bd;
	std::list<cell> cells;
public:
	generator(const unsigned sd = 0): mt(std::mt19937(sd)) {}
	bool set_random();
	void reconstruct();
	bool generate(const size_t num_of_hints);
	board<SIZE>& get_board() {
		return bd;
	}
	std::list<cell> get_cells() {
		return cells;
	}
};

} // namespace sudoku

#include "generator.cpp"

#endif /* end of include guard: GENERATOR_HPP_INCLUDED */

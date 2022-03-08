#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "board.hpp"
#include "solver.cpp"
#include <list>
#include <random>

namespace sudoku {

class cell {
private:
	size_t _pos;
	int _num;
public:
	cell(size_t p, int n) {
		pos() = p;
		num() = n;
	}
	size_t& pos() {
		return _pos;
	}
	int& num() {
		return _num;
	}
};

template<size_t SIZE>
class generator {
private:
	std::mt19937 mt;
	solver<SIZE> slv;
	board<SIZE> bd;
	std::list<cell> cells;
public:
	generator(const unsigned sd = 0): mt(std::mt19937(sd)) {}
	void set_random();
	void reconstruct();
	bool generate(const size_t num_of_hints);
};

} // namespace sudoku

#include "generator.cpp"

#endif /* end of include guard: GENERATOR_HPP_INCLUDED */

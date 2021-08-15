#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include "board.hpp"
#include <random>

namespace sudoku {

template<size_t SIZE>
class generator {
private:
	const unsigned seed;
public:
	generator(const unsigned sd) seed(sd) {

	}
	board<SIZE> generate() const;
};

} // namespace sudoku

#include "generator.cpp"

#endif /* end of include guard: GENERATOR_HPP_INCLUDED */

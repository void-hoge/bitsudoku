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
public:
	solver(board<SIZE> bd);
};

template<size_t SIZE>
solver<SIZE>::solver(board <SIZE> bd) {
	while (!bd.update());
}


} // namespace sudoku

#endif // include guard: SOLVER_HPP_INCLUDED

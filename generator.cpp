#ifndef GENERATOR_CPP_INCLUDED
#define GENERATOR_CPP_INCLUDED

#include "board.hpp"
#include "generator.hpp"
#include "solver.hpp"
#include <iterator>

namespace sudoku {

template<size_t SIZE>
void generator<SIZE>::reconstruct() {
	// Reconstucts the board this->bd using std::list<cell> this->cells.
	this->bd.reset_stable();
	this->bd.reset_candidates();
	for (auto& c: this->cells) {
		this->bd.set(c.pos(), c.num());
	}
}

template<size_t SIZE>
void generator<SIZE>::set_random() {
	this->reconstruct();
	while (this->bd.update());
	auto blank = this->bd.get_blank();
	if (blank.count() == 0) {
		return;
	}
	std::vector<size_t> expand_blank;
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		if (blank[i]) {
			expand_blank.push_back(i);
		}
	}
	size_t pos = expand_blank.at(this->mt()%expand_blank.size());
	auto settable = bd.get_settable_num(pos);
	auto num = settable.at(this->mt()%settable.size());
	this->bd.set(pos, num);
	this->cells.push_back(cell(pos, num));
}

template<size_t SIZE>
bool generator<SIZE>::generate(const size_t num_of_clues) {
	this->bd.reset_stable();
	this->bd.reset_candidates();
	this->cells.clear();
	for (size_t i = 0; i < num_of_clues; i++) {
		this->set_random();
		try {
			while (this->bd.update());
		}catch (std::exception e) {
			return false;
		}
	}
	this->reconstruct();
	try {
		while (this->bd.update());
	}catch (std::exception e) {
		return false;
	}
	while (this->bd.is_solved() == false) {
		auto prev = this->bd.get_instability();
		auto score = SIZE*SIZE*SIZE*SIZE*SIZE*SIZE*SIZE*SIZE;
		size_t count = 0;
		auto erase_idx = this->mt()%this->cells.size();
		auto erased = *std::next(this->cells.begin(), erase_idx);
		this->cells.erase(std::next(this->cells.begin(), erase_idx));
		this->set_random();
		while (score >= prev) {
			this->cells.pop_back();
			count++;
			// if ((count % 1000) == 0) {
			// 	std::cout << count << '\n';
			// }
			if (count > 1<<11) {
				return false;
			}
			this->set_random();
			this->reconstruct();
			try {
				while (this->bd.update());
			}catch (std::exception e) {
				this->cells.pop_back();
				continue;
			}
			score = this->bd.get_instability();
		}
		this->reconstruct();
		this->bd.show();
		std::cout << this->bd.string_output() << '\n';
		std::cout << "attempt: " << count << '\n';
		std::cout << prev << " -> " << score << '\n';
		while (this->bd.update());
		std::cout << "candidates: " << this->bd.count_candidates() << '\n';
	}
	this->reconstruct();
	this->bd.show();
	std::cout << this->bd.string_output() << '\n';
	return true;
}

} // namespace sudoku

#endif /* end of include guard: GENERATOR_CPP_INCLUDED */

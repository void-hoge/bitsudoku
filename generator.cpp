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
bool generator<SIZE>::set_random() {
	auto blank = this->bd.get_blank();
	if (blank.count() == 0) {
		return true;
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
	try {
		while (this->bd.update());
	}catch (std::exception e) {
		return false;
	}
	this->cells.push_back(cell(pos, num));
	return true;
}

template<size_t SIZE>
bool generator<SIZE>::generate(const size_t num_of_clues) {
	// std::cerr << "generate front" << '\n';
	this->cells.clear();
	this->reconstruct();
	for (size_t i = 0; i < num_of_clues; i++) {
		if (set_random() == false) {
			i--;
		}
	}
	// std::cout << this->bd.string_output() << std::endl;
	for (size_t i = 0; i < this->cells.size()*2; i++) {
		auto erased = this->cells.front();
		// std::cout << "cell erased " << erased.pos() << " " << erased.num() << '\n';
		auto best_cand = erased;
		auto min_instability = this->bd.get_instability();
		this->cells.pop_front();
		this->reconstruct();
		while (this->bd.update());
		auto candidates = this->bd.expand_candidates();
		auto tmp = this->bd;
		// std::cout << "candidates count " << candidates.size() << '\n';
		auto best_status = tmp;
		for (auto& cand: candidates) {
			this->bd = tmp;
			this->bd.set(cand.pos(), cand.num());
			try {
				while (bd.update());
			}catch (std::exception e) {
				// std::cout << "in the loop " << cand.pos() << " " << cand.num() << '\n';
				continue;
			}
			auto current_instability = this->bd.get_instability();
			if (current_instability < min_instability) {
				min_instability = current_instability;
				best_cand = cand;
				best_status = this->bd;
			}
		}
		// std::cout << "end of loop" << '\n';
		// this->bd = tmp;
		// this->bd.set(best_cand.pos(), best_cand.num());
		cells.push_back(best_cand);
		this->reconstruct();
		// std::cerr << i << " / " << this->cells.size()*2 << '\n';
		// this->bd.show();
		// this->bd.dump();
		// try{
		while (this->bd.update());
		// }catch(std::exception e) {
			// std::cout << "aborting " << best_cand.pos() << " " << best_cand.num() << '\n';
			// std::cout << this->bd.string_output() << '\n';
			// this->bd.dump();
			// std::cout << this->bd.allor() << '\n';
			// best_status.dump();
			// std::cout << best_status.allor() << '\n';
		// 	std::abort();
		// }
	}
	// std::cerr << "checking the solution..." << '\n';
	// std::cerr << "generate back" << '\n';
	return this->slv.is_multiple_solutions(this->bd);
}

} // namespace sudoku

#endif /* end of include guard: GENERATOR_CPP_INCLUDED */

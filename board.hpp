#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <vector>
#include <array>
#include <bitset>
#include <iostream>
#include <string>
#include <functional>

namespace sudoku {

template<size_t len>
void dump_bits(const std::bitset<len>& bits, const int line_len) {
	for (size_t i = 0; i < bits.size(); i++) {
		std::cout << bits[i];
		if (!((i+1)%line_len)) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

template<size_t SIZE>
class board {
private:
	using bits = std::bitset<SIZE*SIZE*SIZE*SIZE>;
	std::array<bits, SIZE*SIZE> candidates;
	std::array<bits, SIZE*SIZE> stable;
	static constexpr auto horizontal_mask_gen = []{
		bits a = 0;
		for (size_t i = 0; i < SIZE*SIZE; i++) {
			a <<= 1;
			a |= 1;
		}
		return a;
	};

	static constexpr auto vertical_mask_gen = [] {
		bits a = 0;
		for (size_t i = 0; i < SIZE*SIZE; i++) {
			a <<= SIZE*SIZE;
			a |= 1;
		}
		return a;
	};

	static constexpr auto block_mask_gen = [] {
		bits a = 0;
		for (size_t i = 0; i < SIZE; i++) {
			a <<= 1;
			a |= 1;
		}
		bits b = 0;
		for (size_t i = 0; i < SIZE; i++) {
			b <<= SIZE*SIZE;
			b |= a;
		}
		return b;
	};
	const bits horizontal_mask = horizontal_mask_gen();
	const bits vertical_mask = vertical_mask_gen();
	const bits block_mask = block_mask_gen();
	int get(const size_t pos) const;
	size_t count_candidates() const;
	void update_xwing();
	void update_xwing_double();
	void update_locked_candidate();
	void update_naked_pair();
	bool recursive_find_hidden_subset(const bits& mask, const bits& subset, const int n, const int p);
	void update_hidden_subset();
	void recursive_find_naked_subset(const bits& current, const int c, const int n, const int p, std::array<int, SIZE>& visited);
	bool recursive_find_block_naked_subset(const std::array<std::bitset<SIZE*SIZE>, SIZE*SIZE*SIZE*SIZE>& grid_candidates, const size_t target, const std::vector<int>& expand, const size_t previous, const size_t n);
	bool recursive_find_horizontal_naked_subset(const std::array<std::bitset<SIZE*SIZE>, SIZE*SIZE*SIZE*SIZE>& grid_candidates, const size_t target, const std::vector<int>& expand, const size_t previous, const size_t n);
	bool recursive_find_vertical_naked_subset(const std::array<std::bitset<SIZE*SIZE>, SIZE*SIZE*SIZE*SIZE>& grid_candidates, const size_t target, const std::vector<int>& expand, const size_t previous, const size_t n);
	void update_naked_subset(const size_t limit = SIZE);
public:
	board();
	void set(const size_t pos, const int num);
	void erase_candidate(const size_t pos, const int num);
	void erase_stable(const size_t pos);
	void build_candidates();
	void vector_input(const std::vector<int>& q);
	void cin_input();
	void string_input(const std::string& q);
	void show() const;
	void dump() const;
	std::string string_output() const;
	bool update();
	size_t stable_count() const;
	bits get_blank() const;
	std::vector<int> get_settable_num(const size_t pos);
	size_t get_least_unstable() const;
	bool find_error() const;
	void reset_candidates() {
		for (auto&a: candidates) {
			a = 0;
			a.flip();
		}
	}
	void reset_stable() {
		for (auto&a: stable) {
			a = 0;
		}
	}
	size_t size() const {
		return SIZE;
	}
	board<SIZE>& operator=(const board<SIZE>& bd) {
		if (bd.size() != SIZE) {
			throw std::logic_error("different sizes");
		}
		this->candidates = bd.candidates;
		this->stable = bd.stable;
		return std::ref(*this);
	}
};

} // namespace sudoku

#include "board.cpp"

#endif // include guard: BOARD_HPP_INCLUDED

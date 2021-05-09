#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <vector>
#include <array>
#include <bitset>
#include <iostream>

namespace sudoku {

template<size_t len>
void dump_bits(const std::bitset<len> bits, const int line_len) {
	for (size_t i = 0; i < bits.size(); i++) {
		std::cout << bits[i];
		if (!((i+1)%line_len)) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

template<size_t len>
size_t bitset_count(const std::bitset<len> bits) {
	size_t count = 0;
	for (size_t i = 0; i < bits.size(); i++) {
		if (bits[i]) {
			count++;
		}
	}
	return count;
}

template<size_t SIZE>
class board {
private:
	using bits = std::bitset<SIZE*SIZE*SIZE*SIZE>;
	std::array<bits, SIZE*SIZE> possibilities;
	std::array<bits, SIZE*SIZE> stable;
	static constexpr auto horizontal_mask_gen = []{
		bits a = 0;
		for (int i = 0; i < SIZE*SIZE; i++) {
			a <<= 1;
			a |= 1;
		}
		return a;
	};

	static constexpr auto vertical_mask_gen = [] {
		bits a = 0;
		for (int i = 0; i < SIZE*SIZE; i++) {
			a <<= SIZE*SIZE;
			a |= 1;
		}
		return a;
	};

	static constexpr auto block_mask_gen = [] {
		bits a = 0;
		for (int i = 0; i < SIZE; i++) {
			a <<= 1;
			a |= 1;
		}
		bits b = 0;
		for (int i = 0; i < SIZE; i++) {
			b <<= SIZE*SIZE;
			b |= a;
		}
		return b;
	};
	const bits horizontal_mask = horizontal_mask_gen();
	const bits vertical_mask = vertical_mask_gen();
	const bits block_mask = block_mask_gen();
	void set(const size_t pos, const int num);
	int get(const size_t pos) const;
public:
	board();
	void vector_input(std::vector<int> q);
	void show() const;
	void dump() const;
	bool update();
	size_t stable_count() const;
};

template<size_t SIZE>
void board<SIZE>::set(const size_t pos, const int num) {
	if (pos >= SIZE*SIZE*SIZE*SIZE) {
		throw std::logic_error("in function void board::set(pos, num): pos out of range.");
	}
	if (num < 0 || num >= SIZE*SIZE) {
		throw std::logic_error("in function void board::set(pos, num): num out of range.");
	}
	stable.at(num)[pos] = true;
	const int x = pos/(SIZE*SIZE);
	const int y = pos%(SIZE*SIZE);
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		if (i == num) {
			possibilities.at(i) &= ~(vertical_mask<<y);
			possibilities.at(i) &= ~(horizontal_mask<<(x*SIZE*SIZE));
			possibilities.at(i) &= ~(block_mask<<((y/SIZE)*SIZE + (x/SIZE)*SIZE*SIZE*SIZE));
			possibilities.at(i) |= (bits)1<<pos;
		}else {
			possibilities.at(i) &= ~((bits)1<<pos);
		}
	}
}

template<size_t SIZE>
int board<SIZE>::get(const size_t pos) const {
	for (size_t i = 0; i < stable.size(); i++) {
		if (stable.at(i)[pos] == 1) {
			return i;
		}
	}
	return -1;
}

template<size_t SIZE>
bool board<SIZE>::update() {
	bits stable_cells = 0;
	for (const auto& a: stable) {
		stable_cells |= a;
	}

	for (size_t i = 0; i < stable.size(); i++) {
		stable.at(i) = possibilities.at(i);
		for (size_t j = 0; j < possibilities.size(); j++) {
			if (i == j) {
				continue;
			}else {
				stable.at(i) &= ~possibilities.at(j);
			}
		}
	}

	bits new_stable_cells = 0;
	for (const auto& a: stable) {
		new_stable_cells |= a;
	}
	new_stable_cells ^= stable_cells;

	for (size_t i = 0; i < new_stable_cells.size(); i++) {
		if (new_stable_cells[i]) {
			set(i, get(i));
		}
	}

	return new_stable_cells == 0;
}

template<size_t SIZE>
board<SIZE>::board() {
	for (auto &a: possibilities) {
		a = 0;
		a.flip();
	}
	for (auto &a: stable) {
		a = 0;
	}
}

template<size_t SIZE>
void board<SIZE>::vector_input(std::vector<int> q) {
	if (q.size() != SIZE*SIZE*SIZE*SIZE) {
		throw std::logic_error("in board::vector_input(std::vector<int> q): size of q is incorrect.");
	}
	for (size_t i = 0; i < q.size(); i++) {
		if (q.at(i) == 0) {
			continue;
		}else {
			set(i, q.at(i)-1);
		}
	}
}

template<size_t SIZE>
void board<SIZE>::show() const {
	std::cout << "+";
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			std::cout << "---";
		}
		std::cout << "+";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		std::cout << "|";
		for (size_t j = 0; j < SIZE*SIZE; j++) {
			int num = get(i * SIZE * SIZE + j);
			if (num != -1) {
				std::cout << " " << std::hex << num << " ";
			} else {
				std::cout << "   ";
			}
			if (!((j + 1) % SIZE)) {
				std::cout << "|";
			}
		}
		std::cout << std::endl;
		if (!((i+1)%SIZE)) {
			std::cout << "+";
			for (size_t j = 0; j < SIZE; j++) {
				for (int k = 0; k < SIZE; k++) {
					std::cout << "---";
				}
				std::cout << "+";
			}
			std::cout << std::endl;
		}
	}
}

template<size_t SIZE>
void board<SIZE>::dump() const {
	std::cout << "possibilities" << std::endl;
	for (int i = 0; i < SIZE*SIZE; i++) {
		std::cout << std::dec << i << std::endl;
		dump_bits(possibilities.at(i), SIZE*SIZE);
	}
	std::cout << std::endl;
//	std::cout << "stable" << std::endl;
//	for (int i = 0; i < SIZE*SIZE; i++) {
//		std::cout << i << std::endl;
//		dump_bits(stable.at(i), SIZE*SIZE);
//	}
}

template<size_t SIZE>
size_t board<SIZE>::stable_count() const {
	bits hoge = 0;
	for (auto& a: stable) {
		hoge |= a;
	}
	return bitset_count(hoge);
}

} // namespace sudoku

#endif // include guard: BOARD_HPP_INCLUDED

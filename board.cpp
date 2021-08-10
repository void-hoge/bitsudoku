#ifndef BOARD_CPP_INCLUDED
#define BOARD_CPP_INCLUDED

#include "board.hpp"
#include <vector>
#include <array>
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>

namespace sudoku {

template<size_t SIZE>
void board<SIZE>::set(const size_t pos, const int num) {
	if (pos >= SIZE*SIZE*SIZE*SIZE) {
		throw std::out_of_range(std::string("in function void board::set(pos, num): pos out of range. pos: ")+std::to_string(pos)+", num: "+std::to_string(num));
	}
	if (num < 0 || num >= SIZE*SIZE) {
		throw std::logic_error(std::string("in function void board::set(pos, num): num out of range. pos: ")+std::to_string(pos)+", num: "+std::to_string(num));
	}
	stable.at(num)[pos] = true;
	const int x = pos/(SIZE*SIZE);
	const int y = pos%(SIZE*SIZE);
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		if (i == num) {
			possibilities.at(i) &= ~(vertical_mask<<y);
			possibilities.at(i) &= ~(horizontal_mask<<(x*SIZE*SIZE));
			possibilities.at(i) &= ~(block_mask<<((y/SIZE)*SIZE + (x/SIZE)*SIZE*SIZE*SIZE));
			possibilities.at(i)[pos] = true;
		}else {
			possibilities.at(i)[pos] = false;
		}
	}
}

template<size_t SIZE>
void board<SIZE>::erase_possibility(const size_t pos, const int num) {
	if (pos >= SIZE*SIZE*SIZE*SIZE) {
		throw std::out_of_range("in function void board::erase_possibility(pos, num): pos out of range.");
	}
	if (num < 0 || num >= SIZE*SIZE) {
		throw std::logic_error("in function void board::erase_possibility(pos, num): num out of range.");
	}
	possibilities.at(num)[pos] = false;
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
size_t board<SIZE>::count_possibilities() const {
	size_t count = 0;
	for (const auto& a: possibilities) {
		count += bitset_count(a);
	}
	return count;
}

template<size_t SIZE>
bool board<SIZE>::update() {
	auto before = count_possibilities();

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

	update_locked_candidate();
	update_xwing_double();
	// update_xwing();

	for (size_t i = 0; i < possibilities.size(); i++) {
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				if (bitset_count(possibilities.at(i) & block_mask<<(y*SIZE + x*SIZE*SIZE*SIZE)) == 1) {
					stable.at(i) |= possibilities.at(i) & block_mask<<(y*SIZE + x*SIZE*SIZE*SIZE);
				}
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

	auto after = count_possibilities();
	return before - after;
}

template<size_t SIZE>
void board<SIZE>::update_xwing_double() {
	// X-wing
	for (auto& a: possibilities) {
		std::vector<size_t> h_xw;
		std::vector<size_t> v_xw;
		for (size_t i = 0; i < SIZE*SIZE; i++) {
			if (bitset_count(a&(horizontal_mask<<(i*SIZE*SIZE))) == 2) {
				h_xw.push_back(i);
			}
			if (bitset_count(a&(vertical_mask<<i)) == 2) {
				v_xw.push_back(i);
			}
		}

		// horizontal
		for (size_t j = 0; j < h_xw.size(); j++) {
			for (size_t k = j+1; k < h_xw.size(); k++) {
				if ((((a>>h_xw.at(j)*SIZE*SIZE)^
				(a>>h_xw.at(k)*SIZE*SIZE))&horizontal_mask) == (bits)0) {
					auto tmp = (a>>h_xw.at(j)*SIZE*SIZE)&horizontal_mask;
					auto mask = (bits)1<<(h_xw.at(j)*SIZE*SIZE) | (bits)1<<(h_xw.at(k)*SIZE*SIZE);
					for (size_t l = 0; l < SIZE*SIZE; l++) {
						if ((tmp&(bits)1<<l) != (bits)0) {
							a &= (mask << l | ~(vertical_mask << l));
						}
					}
				}
			}
		}

		// vertical
		for (size_t j = 0; j < v_xw.size(); j++) {
			for (size_t k = j+1; k < v_xw.size(); k++) {
				if ((((a>>v_xw.at(j))^
				(a>>v_xw.at(k)))&vertical_mask) == (bits)0) {
					auto tmp = (a>>v_xw.at(j))&vertical_mask;
					auto mask = (bits)1<<(v_xw.at(j)) | (bits)1<<v_xw.at(k);
					for (size_t l = 0; l < SIZE*SIZE; l++) {
						if ((tmp&(bits)1<<(l*SIZE*SIZE)) != (bits)0) {
							a &= (mask << (l*SIZE*SIZE) | ~(horizontal_mask << (l*SIZE*SIZE)));
						}
					}
				}
			}
		}
	}
}

template<size_t SIZE>
void board<SIZE>::update_xwing() {
	for (auto&a: possibilities) {
		for (size_t i = 2; i < SIZE; i++) {
			std::vector<size_t> h_xw;
			std::vector<size_t> v_xw;
			for (size_t j = 0; j < SIZE*SIZE; j++) {
				if (bitset_count(a&(horizontal_mask<<(j*SIZE*SIZE))) == i) {
					h_xw.push_back(j);
				}
				if (bitset_count(a&(vertical_mask<<j)) == i) {
					v_xw.push_back(j);
				}
			}
			std::vector<bool> visited_h(h_xw.size(), false);
			std::vector<bool> visited_v(v_xw.size(), false);
			static auto check_visited = [](std::vector<bool>& v) {
				for (auto a:v) {
					if (!a) {
						return true;
					}
				}
				return false;
			};
			for (size_t j = 0; check_visited(visited_h);) {
				while (visited_h.at(j) == true) {
					j++;
				}
				visited_h.at(j) = true;
				std::vector<size_t> stack;
				stack.push_back(h_xw.at(j));
				for (size_t k = j+1; k < h_xw.size(); k++) {
					if ((((a>>h_xw.at(j)*SIZE*SIZE)^(a>>h_xw.at(k)*SIZE*SIZE))&horizontal_mask) == (bits)0) {
						stack.push_back(h_xw.at(k));
						visited_h.at(k) = true;
					}
				}
				if (stack.size() == i) {
					auto tmp = (a>>stack.at(0)*SIZE*SIZE)&horizontal_mask;
					bits mask = 0;
					for (auto b: stack) {
						mask |= (bits)1<<(b*SIZE*SIZE);
					}
					for (size_t l = 0; l < SIZE*SIZE; l++) {
						if ((tmp&(bits)1<<l) != (bits)0) {
							a &= (mask << l | ~(vertical_mask << l));
						}
					}
				}
			}

			for (size_t j = 0; check_visited(visited_v);) {
				while (visited_v.at(j) == true) {
					j++;
				}
				visited_v.at(j) = true;
				std::vector<size_t> stack;
				stack.push_back(v_xw.at(j));
				for (size_t k = j+1; k < v_xw.size(); k++) {
					if ((((a>>v_xw.at(j))^(a>>v_xw.at(k)))&vertical_mask) == (bits)0) {
						stack.push_back(v_xw.at(k));
						visited_v.at(k) = true;
					}
				}
				if (stack.size() == i) {
					auto tmp = (a>>stack.at(0))&vertical_mask;
					bits mask = 0;
					for (auto b: stack) {
						mask |= (bits)1<<b;
					}
					for (size_t l = 0; l < SIZE*SIZE; l++) {
						if ((tmp&(bits)1<<(l*SIZE*SIZE)) != (bits)0) {
							a &= (mask << (l*SIZE*SIZE) | ~(horizontal_mask << (l*SIZE*SIZE)));
						}
					}
				}
			}
		}
	}
}

template<size_t SIZE>
void board<SIZE>::update_locked_candidate() {
	for (auto &a: possibilities) {
		for (size_t x = 0; x < SIZE; x++) {
			for (size_t y = 0; y < SIZE; y++) {
				auto masked = a&block_mask<<(y*SIZE + x*SIZE*SIZE*SIZE);
				for (size_t i = x*SIZE; i < (x+1)*SIZE; i++) {
					if (masked == (masked&(horizontal_mask<<(i*SIZE*SIZE)))) {
						a &= masked|~(horizontal_mask<<(i*SIZE*SIZE));
					}
				}
				for (size_t i = y*SIZE; i < (y+1)*SIZE; i++) {
					if (masked == (masked&vertical_mask<<i)) {
						a &= masked|~(vertical_mask<<i);
					}
				}
			}
		}
		for (size_t i = 0; i < SIZE*SIZE; i++) {
			auto masked_h = a&horizontal_mask<<(i*SIZE*SIZE);
			auto masked_v = a&vertical_mask<<i;
			for (size_t j = 0; j < SIZE; j++) {
				// horizontal
				if (masked_h == (masked_h&(block_mask<<((i/SIZE)*SIZE*SIZE*SIZE + j*SIZE)))) {
					a &= masked_h|~(block_mask<<((i/SIZE)*SIZE*SIZE*SIZE + j*SIZE));
				}
				// vertical
				if (masked_v == (masked_v&(block_mask<<(j*SIZE*SIZE*SIZE + (i/SIZE)*SIZE)))) {
					a &= masked_v|~(block_mask<<(j*SIZE*SIZE*SIZE + (i/SIZE)*SIZE));
				}
			}
		}
	}
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
void board<SIZE>::vector_input(const std::vector<int> q) {
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
void board<SIZE>::cin_input() {
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		int tmp;
		std::cin >> tmp;
		if (tmp == 0) {
			continue;
		}else {
			set(i, tmp-1);
		}
	}
}

template<size_t SIZE>
void board<SIZE>::string_input(const std::string& q) {
	std::istringstream input(q);
	for (int pos = 0; !input.eof() & pos < SIZE*SIZE*SIZE*SIZE; pos++) {
		std::string tmp;
		input >> tmp;
		if (tmp == "-") {
			continue;
		}else {
			try {
				int num = std::stoi(tmp);
				set(pos, num-1);
			} catch (std::invalid_argument e) {
				std::cout << e.what() << ", input: " << tmp << ", pos: " << pos << '\n';
			}
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
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		std::cout << std::dec << i;
		for (size_t j = 0; j < SIZE*SIZE; j++) {
			std::cout << ' ';
		}
	}
	std::cout << '\n';
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		for (size_t j = 0; j < SIZE*SIZE; j++) {
			for (size_t k = 0; k < SIZE*SIZE; k++) {
				std::cout << possibilities.at(j)[i*SIZE*SIZE+k];
			}
			std::cout << ' ';
		}
		std::cout << '\n';
	}
}

template<size_t SIZE>
size_t board<SIZE>::stable_count() const {
	bits tmp = 0;
	for (const auto& a: stable) {
		tmp |= a;
	}
	return bitset_count(tmp);
}

template<size_t SIZE>
std::bitset<SIZE*SIZE*SIZE*SIZE> board<SIZE>::get_blank() const {
	bits tmp = 0;
	for (const auto& a: stable) {
		tmp |= a;
	}
	return ~tmp;
}

template<size_t SIZE>
std::vector<int> board<SIZE>::get_settable_num(const size_t pos) {
	std::vector<int> res;
	for (size_t i = 0; i < possibilities.size(); i++) {
		if (possibilities.at(i)[pos]) {
			res.push_back(i);
		}
	}
	return res;
}

template<size_t SIZE>
bool board<SIZE>::find_error() const {
	// error -> false, no error -> true
	bits tmp = 0;
	for (const auto& a: possibilities) {
		tmp |= a;
	}
	return (~tmp) == 0;
}

} // namespace sudoku

#endif

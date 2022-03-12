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
	// Sets the number num at the position pos.
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
			this->candidates.at(i) &= ~(this->vertical_mask<<y);
			this->candidates.at(i) &= ~(this->horizontal_mask<<(x*SIZE*SIZE));
			this->candidates.at(i) &= ~(this->block_mask<<((y/SIZE)*SIZE + (x/SIZE)*SIZE*SIZE*SIZE));
			this->candidates.at(i)[pos] = true;
		}else {
			this->candidates.at(i)[pos] = false;
		}
	}
}

template<size_t SIZE>
void board<SIZE>::erase_candidate(const size_t pos, const int num) {
	// Erases the bit of candidates at poosition pos.
	if (pos >= SIZE*SIZE*SIZE*SIZE) {
		throw std::out_of_range("in function void board::erase_candidate(pos, num): pos out of range.");
	}
	if (num < 0 || num >= SIZE*SIZE) {
		throw std::logic_error("in function void board::erase_candidate(pos, num): num out of range.");
	}
	this->candidates.at(num)[pos] = false;
}

template<size_t SIZE>
void board<SIZE>::erase_stable(const size_t pos) {
	// Erases the bit at position pos of this->stable when it become not stable.
	if (pos >= SIZE*SIZE*SIZE*SIZE) {
		throw std::out_of_range("in function void board::erase_candidate(pos, num): pos out of range.");
	}
	for (auto&a: this->stable) {
		a[pos] = false;
	}
}

template<size_t SIZE>
void board<SIZE>::build_candidates() {
	// Updates the this->candidates using this->stable.
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		try {
			set(i, get(i));
		}catch (std::logic_error e) {}
	}
}

template<size_t SIZE>
int board<SIZE>::get(const size_t pos) const {
	// Returns the number at the position pos by referencing the this->stable.
	for (size_t i = 0; i < this->stable.size(); i++) {
		if (this->stable.at(i)[pos] == 1) {
			return i;
		}
	}
	return -1;
}

template<size_t SIZE>
size_t board<SIZE>::count_candidates() const {
	// Returns the number of candidates.
	size_t count = 0;
	for (const auto& cand: this->candidates) {
		count += cand.count();
	}
	return count;
}

template<size_t SIZE>
bool board<SIZE>::update() {
	// Erases candidates using solving techniques.
	auto before = this->count_candidates();

	bits stable_cells = 0;
	for (const auto& a: this->stable) {
		stable_cells |= a;
	}
	update_locked_candidate();
	update_xwing();
	update_naked_subset();
	update_hidden_subset();

	for (size_t i = 0; i < this->stable.size(); i++) {
		this->stable.at(i) = this->candidates.at(i);
		for (size_t j = 0; j < this->candidates.size(); j++) {
			if (i == j) {
				continue;
			}else {
				this->stable.at(i) &= ~this->candidates.at(j);
			}
		}
	}

	for (size_t i = 0; i < this->candidates.size(); i++) {
		for (size_t x = 0; x < SIZE; x++) {
			for (size_t y = 0; y < SIZE; y++) {
				if ((this->candidates.at(i) & this->block_mask<<(y*SIZE + x*SIZE*SIZE*SIZE)).count() == 1) {
					this->stable.at(i) |= this->candidates.at(i) & this->block_mask<<(y*SIZE + x*SIZE*SIZE*SIZE);
				}
			}
		}
	}

	bits new_stable_cells = 0;
	for (const auto& a: this->stable) {
		new_stable_cells |= a;
	}
	new_stable_cells ^= stable_cells;

	for (size_t i = 0; i < new_stable_cells.size(); i++) {
		if (new_stable_cells[i]) {
			this->set(i, get(i));
		}
	}

	auto after = this->count_candidates();
	return before - after;
}

template<size_t SIZE>
void board<SIZE>::update_xwing() {
	for (auto& cand: this->candidates) {
		std::vector<size_t> h_xw;
		std::vector<size_t> v_xw;
		for (size_t i = 0; i < SIZE*SIZE; i++) {
			if ((cand&(this->horizontal_mask<<(i*SIZE*SIZE))).count() == 2) {
				h_xw.push_back(i);
			}
			if ((cand&(this->vertical_mask<<i)).count() == 2) {
				v_xw.push_back(i);
			}
		}

		// horizontal
		for (size_t j = 0; j < h_xw.size(); j++) {
			for (size_t k = j+1; k < h_xw.size(); k++) {
				if ((((cand>>h_xw.at(j)*SIZE*SIZE)^(cand>>h_xw.at(k)*SIZE*SIZE))&this->horizontal_mask) == (bits)0) {
					auto tmp = (cand>>h_xw.at(j)*SIZE*SIZE)&this->horizontal_mask;
					auto mask = (bits)1<<(h_xw.at(j)*SIZE*SIZE) | (bits)1<<(h_xw.at(k)*SIZE*SIZE);
					for (size_t l = 0; l < SIZE*SIZE; l++) {
						if ((tmp&(bits)1<<l) != (bits)0) {
							cand &= (mask << l | ~(this->vertical_mask << l));
						}
					}
				}
			}
		}

		// vertical
		for (size_t j = 0; j < v_xw.size(); j++) {
			for (size_t k = j+1; k < v_xw.size(); k++) {
				if ((((cand>>v_xw.at(j))^(cand>>v_xw.at(k)))&this->vertical_mask) == (bits)0) {
					auto tmp = (cand>>v_xw.at(j))&this->vertical_mask;
					auto mask = (bits)1<<(v_xw.at(j)) | (bits)1<<v_xw.at(k);
					for (size_t l = 0; l < SIZE*SIZE; l++) {
						if ((tmp&(bits)1<<(l*SIZE*SIZE)) != (bits)0) {
							cand &= (mask << (l*SIZE*SIZE) | ~(this->horizontal_mask << (l*SIZE*SIZE)));
						}
					}
				}
			}
		}
	}
}

template<size_t SIZE>
void board<SIZE>::update_locked_candidate() {
	for (auto &cand: this->candidates) {
		for (size_t x = 0; x < SIZE; x++) {
			for (size_t y = 0; y < SIZE; y++) {
				auto masked = cand&this->block_mask<<(y*SIZE + x*SIZE*SIZE*SIZE);
				for (size_t i = x*SIZE; i < (x+1)*SIZE; i++) {
					if (masked == (masked&(this->horizontal_mask<<(i*SIZE*SIZE)))) {
						cand &= masked|~(this->horizontal_mask<<(i*SIZE*SIZE));
					}
				}
				for (size_t i = y*SIZE; i < (y+1)*SIZE; i++) {
					if (masked == (masked&this->vertical_mask<<i)) {
						cand &= masked|~(this->vertical_mask<<i);
					}
				}
			}
		}
		for (size_t i = 0; i < SIZE*SIZE; i++) {
			auto masked_h = cand&this->horizontal_mask<<(i*SIZE*SIZE);
			auto masked_v = cand&this->vertical_mask<<i;
			for (size_t j = 0; j < SIZE; j++) {
				// horizontal
				if (masked_h == (masked_h&(this->block_mask<<((i/SIZE)*SIZE*SIZE*SIZE + j*SIZE)))) {
					cand &= masked_h|~(this->block_mask<<((i/SIZE)*SIZE*SIZE*SIZE + j*SIZE));
				}
				// vertical
				if (masked_v == (masked_v&(this->block_mask<<(j*SIZE*SIZE*SIZE + (i/SIZE)*SIZE)))) {
					cand &= masked_v|~(this->block_mask<<(j*SIZE*SIZE*SIZE + (i/SIZE)*SIZE));
				}
			}
		}
	}
}

template<size_t	SIZE>
void board<SIZE>::update_naked_pair() {
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		for (size_t j = i+1; j < SIZE*SIZE; j++) {
			auto pair = this->candidates.at(i) & this->candidates.at(j);
			for (size_t k = 0; k < SIZE*SIZE; k++) {
				if (i != k && j != k) {
					pair &= ~this->candidates.at(k);
				}
			}
			bits mask_h = this->horizontal_mask;
			bits mask_v = this->vertical_mask;
			for (size_t k = 0; k < SIZE*SIZE; k++) {
				bits tmp_h = pair&mask_h;
				if (tmp_h.count() == 2) {
					this->candidates.at(i) &= tmp_h |~mask_h;
					this->candidates.at(j) &= tmp_h |~mask_h;
				}
				mask_h <<= SIZE*SIZE;

				bits tmp_v = pair&mask_v;
				if (tmp_v.count() == 2) {
					this->candidates.at(i) &= tmp_v |~mask_v;
					this->candidates.at(j) &= tmp_v |~mask_v;
				}
				mask_v <<= 1;

				size_t x = k/SIZE;
			 	size_t y = k%SIZE;
				bits mask_b = this->block_mask<<(x*SIZE*SIZE*SIZE + y*SIZE);
				bits tmp_b = pair&mask_b;
				if (tmp_b.count() == 2) {
					this->candidates.at(i) &= tmp_b |~mask_b;
					this->candidates.at(j) &= tmp_b |~mask_b;
				}
			}
		}
	}
}

template<size_t SIZE>
bool board<SIZE>::recursive_find_horizontal_naked_subset(const std::array<cbits, SIZE*SIZE*SIZE*SIZE>& grid_candidates, const size_t target, const std::vector<int>& expand, const size_t previous, const size_t n) {
	// a method for update_naked_subset()
	if (target/(SIZE*SIZE) != previous/(SIZE*SIZE)) {
		return false;
	}
	if (n == 0) {
		const size_t x = previous/(SIZE*SIZE);
		for (const auto n: expand) {
			this->candidates.at(n) &= ~(this->horizontal_mask<<(x*SIZE*SIZE));
			this->candidates.at(n)[previous] = 1;
		}
		return true;
	}
	for (size_t i = previous+1; i%(SIZE*SIZE) != 0; i++) {
		if (grid_candidates.at(target) == grid_candidates.at(i)) {
			if (this->recursive_find_horizontal_naked_subset(grid_candidates, target, expand, i, n-1)) {
				for (const auto n: expand) {
					this->candidates.at(n)[previous] = 1;
				}
				return true;
			}
		}
	}
	return false;
}

template<size_t SIZE>
bool board<SIZE>::recursive_find_vertical_naked_subset(const std::array<cbits, SIZE*SIZE*SIZE*SIZE>& grid_candidates, const size_t target, const std::vector<int>& expand, const size_t previous, const size_t n) {
	// a method for update_naked_subset()
	if (target%(SIZE*SIZE) != previous%(SIZE*SIZE)) {
		return false;
	}
	if (n == 0) {
		const size_t y = previous%(SIZE*SIZE);
		for (const auto n: expand) {
			this->candidates.at(n) &= ~(this->vertical_mask<<y);
			this->candidates.at(n)[previous] = 1;
		}
		return true;
	}
	for (size_t i = previous+SIZE*SIZE; i < SIZE*SIZE*SIZE*SIZE; i+=SIZE*SIZE) {
		if (grid_candidates.at(target) == grid_candidates.at(i)) {
			if (this->recursive_find_vertical_naked_subset(grid_candidates, target, expand, i, n-1)) {
				for (const auto n: expand) {
					this->candidates.at(n)[previous] = 1;
				}
				return true;
			}
		}
	}
	return false;
}

template<size_t SIZE>
bool board<SIZE>::recursive_find_block_naked_subset(const std::array<cbits, SIZE*SIZE*SIZE*SIZE>& grid_candidates, const size_t target, const std::vector<int>& expand, const size_t previous, const size_t n) {
	// a method for update_naked_subset()
	const size_t x_t = target/(SIZE*SIZE);
	const size_t y_t = target%(SIZE*SIZE);
	const size_t bx_t = x_t/SIZE;
	const size_t by_t = y_t/SIZE;

	const size_t x_p = previous/(SIZE*SIZE);
	const size_t y_p = previous%(SIZE*SIZE);
	const size_t bx_p = x_p/SIZE;
	const size_t by_p = y_p/SIZE;

	auto advance = [](size_t pos) {
		auto tmp = pos;
		pos++;
		if ((tmp/SIZE) != (pos/SIZE)) {
			pos += SIZE*SIZE;
			pos -= SIZE;
		}
		return pos;
	};

	auto blockid = [](size_t pos) {
		size_t x = pos/(SIZE*SIZE);
		size_t y = pos%(SIZE*SIZE);
		size_t bx = x/SIZE;
		size_t by = y/SIZE;
		return bx*SIZE+by;
	};

	if (blockid(target) != blockid(previous)) {
		return false;
	}
	if (n == 0) {
		for (const auto n: expand) {
			this->candidates.at(n) &= ~(this->block_mask<<(bx_p*SIZE*SIZE*SIZE + by_p*SIZE));
			this->candidates.at(n)[previous] = 1;
		}
		return true;
	}
	for (size_t i = advance(previous); blockid(previous) == blockid(i); i = advance(i)) {
		if (grid_candidates.at(target) == grid_candidates.at(i)) {
			if (this->recursive_find_block_naked_subset(grid_candidates, target, expand, i, n-1)) {
				for (const auto n: expand) {
					this->candidates.at(n)[previous] = 1;
				}
				return true;
			}
		}
	}
	return false;
}

template<size_t SIZE>
void board<SIZE>::update_naked_subset(const size_t limit) {
	std::array<cbits, SIZE*SIZE*SIZE*SIZE> grid_candidates;
	std::array<uint8_t, SIZE*SIZE*SIZE*SIZE> grid_candidates_num;
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		grid_candidates_num.at(i) = 0;
		grid_candidates.at(i) = 0;
	}
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		for (size_t p = 0; p < SIZE*SIZE*SIZE*SIZE; p++) {
			grid_candidates.at(p)[i] = candidates.at(i)[p];
			grid_candidates_num.at(p) += candidates.at(i)[p];
		}
	}
	for (size_t c = 2; c <= SIZE && c <= limit; c++) {
		for (size_t p = 0; p < SIZE*SIZE*SIZE*SIZE; p++) {
			if (grid_candidates_num.at(p) == c) {
				std::vector<int> expand;
				for (size_t i = 0; i < SIZE*SIZE; i++) {
					if (grid_candidates.at(p)[i]) {
						expand.push_back(i);
					}
				}
				this->recursive_find_horizontal_naked_subset(grid_candidates, p, expand, p, c-1);
				this->recursive_find_vertical_naked_subset(grid_candidates, p, expand, p, c-1);
				this->recursive_find_block_naked_subset(grid_candidates, p, expand, p, c-1);
			}
		}
	}
}

template<size_t SIZE>
bool board<SIZE>::recursive_find_hidden_subset(const bits& mask, const bits& subset, const int n, const int p) {
	if (p >= SIZE*SIZE) {
		return false;
	}
	if (n == 0) {
		for (size_t i = p+1; i < SIZE*SIZE; i++) {
			this->candidates.at(i) &= ~subset;
		}
		return true;
	}
	for (size_t i = p+1; i < SIZE*SIZE; i++) {
		if ((this->candidates.at(i) & mask) == subset) {
			if (this->recursive_find_hidden_subset(mask, subset, n-1, i)) {
				for (size_t j = p+1; j < i; j++) {
					this->candidates.at(j) &= ~subset;
				}
				return true;
			}
			return false;
		}
	}
	return false;
}

// O(SIZE^6~7)
template<size_t SIZE>
void board<SIZE>::update_hidden_subset() {
	for (size_t p = 0; p < SIZE*SIZE; p++) {
		const auto hmask = this->horizontal_mask << (p*SIZE*SIZE);
		const auto vmask = this->vertical_mask << p;
		const size_t x = p/SIZE;
		const size_t y = p%SIZE;
		const auto bmask = this->block_mask << (x*SIZE*SIZE*SIZE + y*SIZE);
		for (size_t c = 2; c <= SIZE; c++) {
			for (size_t i = 0; i < SIZE*SIZE; i++) {
				auto subset = this->candidates.at(i) & hmask;
				if (subset.count() == c) {
					if (this->recursive_find_hidden_subset(hmask, subset, c-1, i)) {
						for (size_t j = 0; j < i; j++) {
							this->candidates.at(j) &= ~subset;
						}
					}
				}
			}
			for (size_t i = 0; i < SIZE*SIZE; i++) {
				auto subset = this->candidates.at(i) & vmask;
				if (subset.count() == c) {
					if (this->recursive_find_hidden_subset(vmask, subset, c-1, i)) {
						for (size_t j = 0; j < i; j++) {
							this->candidates.at(j) &= ~subset;
						}
					}
				}
			}
			for (size_t i = 0; i < SIZE*SIZE; i++) {
				auto subset = this->candidates.at(i) & bmask;
				if (subset.count() == c) {
					if (this->recursive_find_hidden_subset(bmask, subset, c-1, i)) {
						for (size_t j = 0; j < i; j++) {
							this->candidates.at(j) &= ~subset;
						}
					}
				}
			}
		}
	}
}

template<size_t SIZE>
board<SIZE>::board() {
	for (auto &cand: this->candidates) {
		cand = 0;
		cand.flip();
	}
	for (auto &cand: this->stable) {
		cand = 0;
	}
}

template<size_t SIZE>
void board<SIZE>::vector_input(const std::vector<int>& q) {
	if (q.size() != SIZE*SIZE*SIZE*SIZE) {
		throw std::logic_error("in board::vector_input(std::vector<int> q): size of q is incorrect.");
	}
	for (size_t i = 0; i < q.size(); i++) {
		if (q.at(i) == 0) {
			continue;
		}else {
			this->set(i, q.at(i)-1);
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
			this->set(i, tmp-1);
		}
	}
}

template<size_t SIZE>
void board<SIZE>::string_input(const std::string& q) {
	std::istringstream input(q);
	size_t pos = 0;
	for (; !input.eof() && (pos < SIZE*SIZE*SIZE*SIZE); pos++) {
		std::string tmp;
		input >> tmp;
		if (tmp == "-") {
			continue;
		}else {
			try {
				int num = std::stoi(tmp);
				this->set(pos, num-1);
			} catch (std::invalid_argument e) {
				std::cout << e.what() << ", input: " << tmp << ", pos: " << pos << '\n';
			}
		}
	}
	if (pos != SIZE*SIZE*SIZE*SIZE) {
		throw std::logic_error("invalid string input.");
	}
}

template<size_t SIZE>
void board<SIZE>::show() const {
	std::cout << "+";
	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			std::cout << "---";
		}
		std::cout << "+";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		std::cout << "|";
		for (size_t j = 0; j < SIZE*SIZE; j++) {
			int num = this->get(i*SIZE*SIZE+j);
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
				for (size_t k = 0; k < SIZE; k++) {
					std::cout << "---";
				}
				std::cout << "+";
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::dec;
}

template<size_t SIZE>
void board<SIZE>::dump() const {
	std::cout << "candidates" << std::endl;
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
				std::cout << this->candidates.at(j)[i*SIZE*SIZE+k];
			}
			std::cout << ' ';
		}
		std::cout << '\n';
	}
	std::cout << std::dec;
}

template<size_t SIZE>
std::string board<SIZE>::string_output() const {
	std::string result;
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		auto tmp = this->get(i);
		if (tmp == -1) {
			result += "- ";
		}else {
			result += std::to_string(tmp+1);
			result += " ";
		}
	}
	return result;
}

template<size_t SIZE>
size_t board<SIZE>::stable_count() const {
	// Returns the number of stable cells.
	bits tmp = 0;
	for (const auto& a: this->stable) {
		tmp |= a;
	}
	return tmp.count();
}

template<size_t SIZE>
std::bitset<SIZE*SIZE*SIZE*SIZE> board<SIZE>::get_blank() const {
	// Returns the bitset of blank.
	bits tmp = 0;
	for (const auto& a: this->stable) {
		tmp |= a;
	}
	return ~tmp;
}

template<size_t SIZE>
std::vector<int> board<SIZE>::get_settable_num(const size_t pos) {
	// Returns the vector of numbers which can be set on the position pos.
	std::vector<int> res;
	for (size_t i = 0; i < this->candidates.size(); i++) {
		if (this->candidates.at(i)[pos]) {
			res.push_back(i);
		}
	}
	return res;
}

template<size_t SIZE>
size_t board<SIZE>::get_least_unstable() const {
	// Returns the position of the square with the fewest candidates.
	std::array<int, SIZE*SIZE*SIZE*SIZE> tmp;
	for (auto&a: tmp) {
		a = 0;
	}
	for (const auto&cand: this->candidates) {
		for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
			tmp.at(i) += cand[i];
		}
	}
	int min = SIZE*SIZE;
	size_t idx = 0;
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		if (tmp.at(i) > 1 && min > tmp.at(i)) {
			min = tmp.at(i);
			idx = i;
		}
	}
	return idx;
}

template<size_t SIZE>
bool board<SIZE>::find_error() const {
	// Detects that the problem cannot be solved.
	// error -> false, no error -> true
	bits tmp = 0;
	for (const auto& cand: this->candidates) {
		tmp |= cand;
	}
	return tmp.all();
}

template<size_t SIZE>
size_t board<SIZE>::get_instability() const {
	// Returns sum of squares of the number of the candidates for each cells
	std::array<int, SIZE*SIZE*SIZE*SIZE> cells;
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		cells.at(i) = 0;
	}
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		for (size_t p = 0; p < SIZE*SIZE*SIZE*SIZE; p++) {
			cells.at(p) += this->candidates.at(i)[p];
		}
	}
	size_t square_sum = 0;
	for (size_t i = 0; i < SIZE*SIZE*SIZE*SIZE; i++) {
		square_sum += cells.at(i)*cells.at(i);
	}
	return square_sum;
}

template<size_t SIZE>
bool board<SIZE>::is_solved() const {
	return this->get_instability() == SIZE*SIZE*SIZE*SIZE;
}

template<size_t SIZE>
std::vector<cell> board<SIZE>::expand_candidates() const {
	std::vector<cell> cells;
	for (size_t i = 0; i < SIZE*SIZE; i++) {
		auto bit = this->candidates.at(i) & ~this->stable.at(i);
		for (size_t p = 0; p < SIZE*SIZE*SIZE*SIZE; p++) {
			if (bit[p] == true) {
				cells.push_back(cell(p, i));
			}
		}
	}
	return cells;
}

} // namespace sudoku

#endif

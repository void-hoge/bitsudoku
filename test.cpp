#include <vector>
#include <algorithm>
#include <iostream>

bool check(std::vector<bool> v) {
	for (auto a: v) {
		if (a == false) {
			return true;
		}
	}
	return false;
}

int main() {
	std::vector<int> data = {1,1,2,3,1,2,3,2};
	std::vector<bool> visited(data.size(), false);
	int i = 0;
	while (check(visited)) {
		while (visited.at(i) == true) {
			i++;
		}
		visited.at(i) = true;
		std::vector<int> stack;
		stack.push_back(i);
		for (size_t j = i+1; j < data.size(); j++) {
			if (data.at(i) == data.at(j)) {
				stack.push_back(j);
				visited.at(j) = true;
			}
		}
		for (auto a: stack) std::cout << a << ' '; std::cout << '\n';
	}
	return 0;
}

#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cassert>
#include <unordered_map>
#include "../performance/timer.h"

auto parse() {
	std::vector<std::vector<int>> out;
	std::string text;
	while (std::getline(std::cin, text)) {
		std::stringstream stream { text };
		out.push_back(std::vector<int>());
		while (stream >> text) {
			out[out.size() - 1].push_back(std::stoi(text));
		}
	}
	return out;
}

inline long long factorial(long long n) {
	long long out = 1;
	for (int i = 2; i <= n; i++) {
		out *= i;
	}
	return out;
}

long long nchoosek(size_t n, size_t k) {
	return factorial(n) / (factorial(k) * factorial(n - k));
}

long long solution1() {
	auto input = parse();
	Timer start;
	long long out = 0;
	for (const auto& line: input) {
		int item_count = line.size();
		long long diagonal_sum = 0;
		for (int diagonal = 0; diagonal < item_count; diagonal++) {
			diagonal_sum = 0;
			for (int row = diagonal; row < item_count; row++) {
				diagonal_sum += nchoosek(row, diagonal);
			}
			out += diagonal_sum * line[line.size() - 1 - diagonal] * (diagonal % 2 ? -1 : 1);
		}
	}
	return out;
}

long long solution2() {
	auto input = parse();
	Timer start;
	long long out = 0;
	for (const auto& line: input) {
		int item_count = line.size();
		long long diagonal_sum = 0;
		for (int diagonal = 0; diagonal < item_count; diagonal++) {
			diagonal_sum = 0;
			for (int row = diagonal; row < item_count; row++) {
				diagonal_sum -= nchoosek(row, diagonal);
			}
			out += diagonal_sum * line[diagonal] * (diagonal % 2 ? 1 : -1);
		}
	}
	return out;
}

int main(int argc, char** argv) {
	printf("%lld\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

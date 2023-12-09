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

inline __int128 factorial(size_t n) {
	__int128 out = 1;
	for (size_t i = 2; i <= n; i++) {
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
	__int128 out = 0;
	for (const auto& line: input) {
		int item_count = line.size();
		__int128 diagonal_sum = 0;
		for (int diagonal = 0; diagonal < item_count; diagonal++) {
			size_t r = diagonal;
			size_t k = item_count - diagonal;
			diagonal_sum = (k * nchoosek(k + r, k)) / (r + 1);
			out += diagonal_sum * line[line.size() - 1 - diagonal] * (diagonal % 2 ? -1 : 1);
		}
	}
	return out;
}

long long solution2() {
	auto input = parse();
	Timer start;
	__int128 out = 0;
	for (const auto& line: input) {
		int item_count = line.size();
		__int128 diagonal_sum = 0;
		for (int diagonal = 0; diagonal < item_count; diagonal++) {
			size_t r = diagonal;
			size_t k = item_count - diagonal;
			diagonal_sum = (k * nchoosek(k + r, k)) / (r + 1);
			out += diagonal_sum * line[diagonal] * (diagonal % 2 ? 1 : -1);
		}
	}
	return out;
}

int main(int argc, char** argv) {
	printf("%lld\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

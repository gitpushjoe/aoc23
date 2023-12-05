#include <iostream>
#include <vector>
#include <cassert>
#include "../performance/timer.h"

int solution1() {
	std::vector<std::string> lines;
	for (std::string line; std::getline(std::cin, line);) {
		lines.push_back(line);
	}
	Timer start;
	int result = 0;
	int digit = 0;
	std::vector<int> number{0, 0};
	for (auto line : lines) {
		for (auto c : line) {
			if ((c >= '0') && (c <= '9')) {
				int n = c - '0';
				number[digit] = n;
				number[1] = digit == 0 ? number[0] : number[1];
				digit = digit == 0 ? 1 : digit;
			}
		}
		result += 10 * number[0] + number[1];
		digit = 0;
	}
	return result;
}

int solution2() {
	std::string word;
	char c;
	while (std::cin.get(c)) {
		word += c;
	}
	Timer start;
	auto static words = new std::pair<std::string, int>[10] {
		{"one", 0},
		{"two", 0},
		{"three", 0},
		{"four", 0},
		{"five", 0},
		{"six", 0},
		{"seven", 0},
		{"eight", 0},
		{"nine", 0},
	};
	auto reset_words = [&]() {
		for (int i = 0; i < 9; i++) {
			words[i].second = 0;
		}
	};
	int result = 0;
	std::vector<int> digits;
	for (auto c : word) {
		if (c == '\n') {
			result += (digits[0] * 10) + digits[digits.size() - 1];
			digits.clear();
			reset_words();
		}
		if ((c >= '0') && (c <= '9')) {
			digits.push_back(c - '0');
			reset_words();
		} else {
			for (int i = 0; i < 9; i++) {
				if (c == words[i].first[words[i].second]) {
					words[i].second++;
				} else {
					words[i].second = c == words[i].first[0];
				}
				if (words[i].second == words[i].first.size()) {
					digits.push_back(i + 1);
					words[i].second = 0;
				}
			}
		}
	}
	return result;
}

int main(int argc, char** argv) {
	printf("%d\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

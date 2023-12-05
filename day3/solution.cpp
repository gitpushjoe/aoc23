#include <iostream>
#include <vector>
#include <cctype>
#include <array>
#include <string>
#include <unistd.h>
#include <set>
#include "../performance/timer.h"


std::array<int, 2> add_points(const std::array<int, 2>& point1, const std::array<int, 2>& point2) {
	return std::array<int, 2>{point1[0] + point2[0], point1[1] + point2[1]};
};


bool in_bounds(const std::array<int, 2> point, const std::vector<std::string>& lines) {
	return (0 <= point[0]) && (point[0] < lines.size()) && (0 <= point[1]) && (point[1] < lines[0].size());
};

char char_at(const std::vector<std::string>& lines, const std::array<int, 2>& pos) {
	return lines[pos[0]][pos[1]];
}


std::pair<int, std::array<int, 2>> evaluate(std::vector<std::string>& lines, std::array<int, 2> cursor, bool replace) {
	int col = cursor[1];
	int left, right = cursor[1];
	while (cursor[1] >= 0 && isdigit(char_at(lines, cursor))) {
		left = cursor[1];
		cursor[1]--;
	}
	cursor[1] = col;
	while (cursor[1] < lines.size() && isdigit(char_at(lines, cursor))) {
		right = cursor[1];
		cursor[1]++;
	}
	int out = std::stoi(lines[cursor[0]].substr(left, right - left + 1));
	if (replace) {
		lines[cursor[0]].replace(lines[cursor[0]].begin() + left,
			lines[cursor[0]].begin() + right + 1,
			right - left + 1,
			'.');
	}
	return std::pair<int, std::array<int, 2>>{ out, {cursor[0], left }};
};

static const std::vector<std::array<int, 2>> adjacents {
	{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 1}, {1, 0}
};

std::vector<std::string> parse() {
	std::string line;
	std::vector<std::string> lines;
	while (std::cin >> line) {
		lines.push_back(line);
	}
	return lines;
};

int solution1() {
	std::vector<std::string> lines { parse() };
	Timer start;
	int res = 0;
	for (int row = 0; row < lines.size(); row++) {
		std::string l = lines[row];
		for (int col = 0; col < l.size(); col++) {
			char c = l[col];
			std::array<int, 2> pos {row, col};
			if (isdigit(c) || c == '.') {
				continue;
			}
			for (auto pair: adjacents) {
				std::array<int, 2> new_pos = add_points(pos, pair);
				if (in_bounds(new_pos, lines) && isdigit(char_at(lines, new_pos))) {
					res += evaluate(lines, new_pos, true).first;
				}
			}
		}
	}
	return res;
};

int solution2() {
	std::vector<std::string> lines { parse() };
	Timer start;
	int res = 0;
	for (int row = 0; row < lines.size(); row++) {
		std::string l = lines[row];
		for (int col = 0; col < l.size(); col++) {
			char c = l[col];
			std::array<int, 2> pos {row, col};
			if (c != '*') {
				continue;
			}
			std::set<std::pair<int, std::array<int, 2>>> adjacent_nums;
			for (auto pair: adjacents) {
				std::array<int, 2> new_pos = add_points(pos, pair);
				if (in_bounds(new_pos, lines) && isdigit(char_at(lines, new_pos))) {
					adjacent_nums.insert(evaluate(lines, new_pos, false));
				}
			}
			if (adjacent_nums.size() == 2) {
				auto it = adjacent_nums.begin();
				res += it->first * (++it)->first;
			}
		}
	}
	return res;	
}

int main(int argc, char** argv) {
	printf("%d\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

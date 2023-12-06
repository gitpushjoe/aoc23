#include <cctype>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "../performance/timer.h"
#include <sstream>


auto parse() {	
	std::vector<std::vector<int>> data;
	std::string txt;
	while (std::cin >> txt) {
		if (!isdigit(txt.at(0))) {
			data.push_back(std::vector<int>{});
			continue;
		}
		data[data.size() - 1].push_back(std::stoi(txt));
	}
	return data;
}

std::pair<float, float> solve_quadratic(long a, long b, long c) {
	return {
		(-b + pow(pow(b, 2) - 4 * a * c, .5)) / (2 * a),
		(-b - pow(pow(b, 2) - 4 * a * c, .5)) / (2 * a)
	};
}

int solution1() {
	auto input = parse();
	Timer start;
	int res = 1;
	for (int i = 0; i < input[0].size(); i++) {
		int time = input[0][i];
		int dist = input[1][i];
		auto roots = solve_quadratic(-1, time, -dist);
		res *= (ceil(roots.second) - 1) - (floor(roots.first) + 1) + 1;
	}
	return res;
}

int solution2() {
	auto input = parse();
	Timer start;
	std::stringstream time_str, dist_str;
	for (auto& time: input[0]) {
		time_str << time;
	}
	for (auto& dist: input[1]) {
		dist_str << dist;
	}
	auto roots = solve_quadratic(-1, std::stol(time_str.str()), -std::stol(dist_str.str()));
	return (ceil(roots.second) - 1) - (floor(roots.first) + 1) + 1;
}

int main(int argc, char** argv) {
	printf("%d\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

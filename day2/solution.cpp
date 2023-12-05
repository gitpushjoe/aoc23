// Note: Performance metrics include parsing time

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <cassert>
#include "../performance/timer.h"

int solution1() {
	int result = 0;
	std::unordered_map<std::string, int> bag{
		{"red", 12},
		{"green", 13},
		{"blue", 14}
	};
	std::string curr;
	std::string word;
	int game = 1;
	int not_possible_sum = 0;
	Timer start;
	while (std::getline(std::cin, curr, '\n')) {
		std::istringstream stream(curr + ';');
		stream >> word;
		stream >> word;
		std::unordered_map<std::string, int> curr_bag;
		while (stream >> word) {
			int count = std::stoi(word);
			stream >> word;
			std::string color = word.substr(0, word.size() - 1);
			char delim = word.at(word.size() - 1);
			curr_bag[color] = count;
			if (delim == ';') {
				bool possible = std::all_of(
					curr_bag.begin(),
					curr_bag.end(),
					[&](auto it) {
						assert(bag.contains(it.first));
						return bag[it.first] >= it.second; 
					}
				);
				if (!possible) {
					not_possible_sum += game;
					break;
				}
			}
		}
		game++;
	}
	return ((game * (game - 1)) / 2) - not_possible_sum;
}

int solution2() {
	int result = 0;
	Timer start;
	std::unordered_map<std::string, int> bag{
		{"red", 12},
		{"green", 13},
		{"blue", 14}
	};
	std::string curr;
	std::string word;
	int res = 0;
	while (std::getline(std::cin, curr, '\n')) {
		std::istringstream stream(curr + '!');
		stream >> word;
		stream >> word;
		std::unordered_map<std::string, int> curr_bag{
			{"red", 0},
			{"green", 0},
			{"blue", 0}
		};
		while (stream >> word) {
			int count = std::stoi(word);
			stream >> word;
			std::string color = word.substr(0, word.size() - 1);
			char delim = word.at(word.size() - 1);
			curr_bag[color] = std::max(curr_bag[color], count);
			if (delim == '!') {
				res += curr_bag["red"] * curr_bag["green"] * curr_bag["blue"];
			}
		}
	}
	return res;
}

int main(int argc, char** argv) {
	printf("%d\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

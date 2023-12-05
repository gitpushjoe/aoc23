 // Note: Performance metrics include parsing time

#include <cmath>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>
#include <istream>
#include <set>
#include <algorithm>
#include "../performance/timer.h"

typedef std::vector<int> CardVec;
typedef std::set<int> CardsSet;
typedef std::pair<CardsSet, CardVec> CardPair;
typedef std::vector<CardPair> Collection;

Collection parse() {
	Collection collection;
	std::string line;
	while (std::getline(std::cin, line)) {
		std::istringstream l { line };
		std::string word;
		l >> word;
		l >> word;
		collection.push_back(CardPair{});
		while (1) {
			l >> word;
			if (word == "|") {
				break;
			}
			collection[collection.size() - 1].first.insert(std::stoi(word));
		}
		while (l >> word) {
			collection[collection.size() - 1].second.push_back(std::stoi(word));
		}
	}
	return collection;
}

int get_raw_score(CardPair& c) {
	return std::count_if(c.second.begin(), c.second.end(), [&c](int num) {
		return c.first.contains(num);
	});
};

inline int get_score(CardPair& c) {
	int score = get_raw_score(c);
	return score == 0 ? 0 : pow(2, score - 1);
}

int solution1() {
	Timer start;
	Collection collection { parse() };
	return std::accumulate(collection.begin(), 
		collection.end(), 
		0,
		[](int total, CardPair& c) {
			return total + get_score(c);
		}
	);
};

int solution2() {
	Timer start;
	Collection collection { parse() };
	std::vector<int> copies(collection.size(), 0);
	int curr = 1;
	int res = 0;
	for (size_t i = 0; i < copies.size(); i++) {
		size_t raw_score = get_raw_score(collection[i]);
		res += curr;
		if (raw_score > 0) {
			size_t j = std::min(copies.size() - 1, i + raw_score);
			copies[j] += curr;
			curr *= 2;
		}
		curr -= copies[i];
	};
	return res;
};

int main(int argc, char** argv) {
	printf("%d\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

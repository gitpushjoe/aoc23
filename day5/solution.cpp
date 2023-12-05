#include <algorithm>
#include <cctype>
#include <iterator>
#include <limits>
#include <iostream>
#include <math.h>
#include <vector>
#include <sstream>
#include <cassert>
#include "../performance/timer.h"

struct Range {
	long src_base;
	long dest_base;
	long range;
};

struct RangeMap {
	std::vector<Range> ranges;
};

struct Almanac {
	std::vector<long> seeds;
	std::vector<RangeMap> range_maps;
};

Almanac parse() {
	std::string inp;
	std::cin >> inp;
	Almanac out;
	while (std::cin >> inp) {
		if (!inp.size() || !isdigit(inp.at(0))) {
			break;
		}
		out.seeds.push_back(std::stol(inp));
	}
	while (std::cin >> inp) {
		out.range_maps.push_back(RangeMap());
		while (std::cin >> inp) {
			if (!isdigit(inp.at(0))) {
				break;
			}
			RangeMap& range_map = out.range_maps[out.range_maps.size() - 1];
			range_map.ranges.push_back(Range());
			Range& range = range_map.ranges[range_map.ranges.size() - 1];
			range.dest_base = std::stol(inp);
			std::cin >> inp;
			range.src_base = std::stol(inp);
			std::cin >> inp;
			range.range = std::stol(inp);
		}
	}
	return out;
}

void sort_almanac(Almanac& almanac) {
	for (auto& range_map: almanac.range_maps) {
		std::sort(
			range_map.ranges.begin(),
			range_map.ranges.end(),
			[](auto range1, auto range2) {
				return range1.src_base < range2.src_base;
			}
		);
	}
}

auto get_closest_range_itr(long num, const RangeMap& range_map) {
	return std::lower_bound(
		range_map.ranges.begin(),
		range_map.ranges.end(),
		num,
		[](const Range& range, long n) {
			return n >= range.src_base + range.range;
		}
	);
}

bool in_range(long num, const Range& range) {
	return range.src_base <= num && num < range.src_base + range.range;
}

long convert(long num, const RangeMap& range_map) {
	auto range_itr = get_closest_range_itr(num, range_map);
	if (range_itr == range_map.ranges.end()) {
		return num;
	}
	if (in_range(num, *range_itr)) {
		return range_itr->dest_base + num - range_itr->src_base; 
	}
	return num;
}

long convert(long num, std::vector<RangeMap>& range_maps) {
	long out = num;
	for (auto& range_map: range_maps) {
		out = convert(out, range_map);
	}
	return out;
}

long solution1() {
	auto input = parse();
	Timer start;
	sort_almanac(input);
	long res = std::numeric_limits<long>::max();
	std::vector<long> locations(input.seeds.size());
	std::transform(
		input.seeds.begin(),
		input.seeds.end(),
		locations.begin(),
		[&](long& seed) { return convert(seed, input.range_maps); }
	);
	return *std::min_element(locations.begin(), locations.end());
}

long solution2() {
	auto input = parse();
	Timer start;
	sort_almanac(input);
	auto itr = input.seeds.begin();
	long res = std::numeric_limits<long>::max();
	while (itr != input.seeds.end()) {
		long base = *itr;
		long range = *(++itr);
		long seed_start = base;
		long seed_range = range;
		long curr = base;
		while (range > 0) {
			for (auto& range_map: input.range_maps) {
				auto range_itr = get_closest_range_itr(curr, range_map);
				if (range_itr == range_map.ranges.end()) {
					continue;
				}
				if (in_range(curr, *range_itr)) {
					range = std::min(range, range_itr->src_base + range_itr->range - curr);
					curr = range_itr->dest_base + (curr - range_itr->src_base);
					continue;
				}
				range = std::min(range, (++range_itr)->src_base - curr);
			}
			res = std::min(res, curr);
			curr = base + range;
			base = curr;
			range = seed_range - (curr - seed_start);
		}
		itr++;
	}
	return res;
}

int main(int argc, char** argv) {
	printf("%ld\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

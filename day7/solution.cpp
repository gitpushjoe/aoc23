#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
#include "../performance/timer.h"
#include <array>
#include <cassert>

struct Hand {
	std::string cards;
	int bid;
};

constexpr inline int evaluate(const char& c) {
	return ('2' <= c && c <= '9') ? c - '1':
			c == 'T' ? 10 :
			c == 'J' ? 11 :
			c == 'Q' ? 12 :
			c == 'K' ? 13 :
			c == 'A' ? 14 : 
			c == 'j' ? 0 : 100000;
}

inline int card_evaluate(std::string card) {
	return evaluate(card[4]) + 
		evaluate(card[3]) * 16 +
		evaluate(card[2]) * pow(16, 2) +
		evaluate(card[1]) * pow(16, 3) +
		evaluate(card[0]) * pow(16, 4);
}

enum State {
	imp = -1, // impossible
	A = 0,
	AA,
	AB,
	AAA,
	AAB,
	ABC,
	AAAA,
	AAAB,
	AABB,
	AABC,
	ABCD,
	AAAAA, // royal flush
	AAAAB, // four of a kind
	AAABB, // full house
	AAABC, // three of a kind
	AABBC, // two pair
	AABCD, // one pair
	ABCDE  // high card
};

struct Transition {
	std::pair<char, char> swap;
	State new_state;
};

static std::array<Transition, 16 * 5> state_machine {{
//next letter:        A					   B					 C					   D					 E (default)		 // State \/
					{{'E', 'E'}, AA    }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'B'}, AB    }, // A
					{{'E', 'E'}, AAA   }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'B'}, AAB   }, // AA
					{{'E', 'E'}, AAB   }, {{'B', 'A'}, AAB   }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'C'}, ABC   }, // AB
					{{'E', 'E'}, AAAA  }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'B'}, AAAB  }, // AAA
					{{'E', 'E'}, AAAB  }, {{'E', 'E'}, AABB  }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'C'}, AABC  }, // AAB
					{{'E', 'E'}, AABC  }, {{'B', 'A'}, AABC  }, {{'C', 'A'}, AABC  }, {{'E', 'E'}, imp   }, {{'E', 'D'}, ABCD  }, // ABC
					{{'E', 'E'}, AAAAA }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'B'}, AAAAB }, // AAAA
					{{'E', 'E'}, AAAAB }, {{'E', 'E'}, AAABB }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'C'}, AAABC }, // AAAB
					{{'E', 'E'}, AAABB }, {{'B', 'A'}, AAABB }, {{'E', 'E'}, imp   }, {{'E', 'E'}, imp   }, {{'E', 'C'}, AABBC }, // AABB
					{{'E', 'E'}, AAABC }, {{'E', 'E'}, AABBC }, {{'C', 'B'}, AABBC }, {{'E', 'E'}, imp   }, {{'E', 'D'}, AABCD }, // AABC
					{{'E', 'E'}, AABCD }, {{'B', 'A'}, AABCD }, {{'C', 'A'}, AABCD }, {{'D', 'A'}, AABCD }, {{'E', 'E'}, ABCDE }, // ABCDE
}};

struct RankedHand {
	State state = A;
	std::string cards;
	std::string registers = "-----"; // the A, B, C, D, E referenced in State. i.e. "KK335": state == AABBC ; registers == "K35--"
	int bid;
	bool operator>(const RankedHand& rhs) const {
		return state < rhs.state ? true : state != rhs.state ? false :
				card_evaluate(cards) > card_evaluate(rhs.cards) ? true : false;
	}
	bool operator==(const RankedHand& rhs) const {
		return state == rhs.state && cards == rhs.cards;
	}
	RankedHand(std::string cards, int bid): cards(cards), bid(bid), registers("-----") {
		state = A;
		rank();
	}
	inline void rank() {
		registers[0] = cards[0];
		for (int i = 1; i < 5; i++) {
			registers[4] = cards[i];
			int same_card = registers[4] == registers[0] ? 0 : 
				registers[4] == registers[1] ? 1 :
				registers[4] == registers[2] ? 2 :
				registers[4] == registers[3] ? 3 :
				4;
			int idx = state * 5 + same_card;
			std::swap(registers[ state_machine[idx].swap.first - 'A' ], registers[ state_machine[idx].swap.second - 'A' ]);
			state = state_machine[idx].new_state;
		}
		cards.erase(5);
	}
	inline void handle_jokers() {
		if (cards[0] != 'J' && cards[1] != 'J' && cards[2] != 'J' && cards[3] != 'J' && cards[4] != 'J') {
			;
		} else if (state == AAAAB | state == AAABB | state == AAAAA) {
			state = AAAAA;
		} else if (state == AAABC) {
			state = AAAAB;
		} else if (state == AABBC) {
			state = registers['C' - 'A'] == 'J' ? AAABB :
					AAAAB;
		} else if (state == AABCD) {
			state = AAABC;
		} else if (state == ABCDE) {
			state = AABCD;
		}
		for (int i = 0; i < 5; i++) {
			if (cards[i] == 'J') {
				cards[i] = 'j';
			}
		}
	}
};

auto parse() {
	std::vector<Hand> hands;
	std::string txt;
	while (std::cin >> txt) {
		hands.push_back({txt, 0});
		std::cin >> txt;
		hands[hands.size() - 1].bid = std::stoi(txt);
	}
	return hands;
}

int solution1() {
	auto input = parse();
	Timer start;
	std::priority_queue<RankedHand, std::vector<RankedHand>, std::greater<RankedHand>> hands;
	int out = 0;
	for (Hand& hand: input) {
		hands.push(RankedHand(hand.cards, hand.bid));
	}
	for (int i = 0; i < input.size(); i++) {
		RankedHand rh = hands.top();
		out += rh.bid * (i + 1);
		hands.pop();
	}
	return out;
}

int solution2() {
	auto input = parse();
	Timer start;
	std::priority_queue<RankedHand, std::vector<RankedHand>, std::greater<RankedHand>> hands;
	int out = 0;
	for (Hand& hand: input) {
		RankedHand ranked_hand(hand.cards, hand.bid);
		ranked_hand.handle_jokers();
		hands.push(ranked_hand);
	}
	for (int i = 0; i < input.size(); i++) {
		RankedHand rh = hands.top();
		out += rh.bid * (i + 1);
		hands.pop();
	}
	return out;
}

int main(int argc, char** argv) {
	printf("%d\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

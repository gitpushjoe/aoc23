#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <algorithm>
#include <set>
#include <numeric>
#include "../performance/timer.h"

struct Node {
    std::string element;
    std::string left;
    std::string right;
};

auto parse() {
    std::pair<std::string, std::vector<Node>> out;
    std::cin >> out.first;
    std::string txt;
    while (std::cin >> txt) {
        out.second.push_back(Node{txt, "", ""});
        std::cin >> txt;
        std::cin >> txt;
        out.second[out.second.size() - 1].left = txt.substr(1, 3);
        std::cin >> txt;
        out.second[out.second.size() - 1].right = txt.substr(0, 3);
    }
	return out;
}

inline int element_to_int(const std::string& element) {
    return (element[0] - 'A') * (pow(26, 2)) +
        (element[1] - 'A') * pow(26, 1) +
        (element[2] - 'A') * pow(26, 0);
}

size_t solution1() {
	auto input = parse();
	Timer start;
    std::array<std::array<int, 2>, 17576> arr;
    for (const Node& node: input.second) {
        arr[element_to_int(node.element)][0] = element_to_int(node.left);
        arr[element_to_int(node.element)][1] = element_to_int(node.right);
    }
    std::vector<bool> turns(input.first.size());
    std::transform(
        input.first.begin(),
        input.first.end(),
        turns.begin(),
        [](const char c) { return c == 'R'; }
    );
    int pos = 0;
    size_t turn_count = 0;
    int dest = element_to_int("ZZZ");
    while (pos != dest) {
        pos = arr[pos][turns[turn_count++ % turns.size()]];
    }
	return turn_count;
}

size_t solution2() {
	auto input = parse();
	Timer start;
    std::array<std::pair<std::array<int, 3>, std::set<int>>, 17576> arr;
    std::set<int> selected_nodes;
    for (const Node& node: input.second) {
        arr[element_to_int(node.element)].first[0] = element_to_int(node.left);
        arr[element_to_int(node.element)].first[1] = element_to_int(node.right);
        if (element_to_int(node.element) % 26 == 'A' - 'A') {
            selected_nodes.insert(element_to_int(node.element));
        }
    };
    std::vector<bool> turns(input.first.size());
    std::transform(
        input.first.begin(),
        input.first.end(),
        turns.begin(),
        [](const char c) { return c == 'R'; }
    );
    // for (const Node& node: input.second) {
    //     int source = element_to_int(node.element);
    //     int pos = source;
    //     for (int i = 0; i < turns.size(); i++) {
    //         pos = arr[pos].first[turns[i]];
    //         // std::cout << pos % 26 << " " << i << std::endl;
    //         // std::cout << pos % 26 << std::endl;
    //         if (pos % 26 == 'Z' - 'A') {
    //             arr[source].second.insert(i + 1);
    //         }
    //     }
    //     arr[source].first[2] = pos;
    //     // std::cout << "Element=" << node.element << " " << arr[source].first[2] << std::endl;
    //     // for (auto& e: arr[source].second) {
    //     //     std::cout << e << " ";
    //     // }
    //     // std::cout << std::endl;
    // }
    size_t turn_count = 0;
    // return 0;
    std::vector<int> steps;
    // std::set<int> new_nodes;
    for (auto n: selected_nodes) {
        int reps = 0;
        while (n % 26 != 'Z' - 'A') {
            n = arr[n].first[turns[reps++ % turns.size()]];
        }
        steps.push_back(reps);
    }
    size_t out = steps[0];
    for (auto itr = ++steps.begin(); itr != steps.end(); itr++) {
        out = std::lcm(out, *itr);
    }
    return out;
    // while (1) {
    //     // std::cout << "yo" << std::endl;
    //     // if (turn_count % 100000 == 0) {
    //     //     std::cout << turn_count / 263 << std::endl;
    //     // }
    //     auto itr = selected_nodes.begin();
    //     std::set<int> shared_zs(arr[*itr].second.begin(), arr[*itr].second.end());
    //     // std::cout << "yo2" << std::endl;
    //     // std::cout << selected_nodes.size() << std::endl;
    //     new_nodes.insert(arr[*itr].first[2]);
    //     int total = 0;
    //     while (++itr != selected_nodes.end()) {
    //         if (shared_zs.size() > 0) {
    //             std::set<int> new_shared_zs;
    //             std::set_intersection(shared_zs.begin(), shared_zs.end(),
    //                 arr[*itr].second.begin(), arr[*itr].second.end(),
    //                 std::inserter(new_shared_zs, new_shared_zs.begin()));
    //             shared_zs = new_shared_zs;
    //         }
    //         total += arr[*itr].second.size();
    //         new_nodes.insert(arr[*itr].first[2]);
    //     }
    //     if (total == 3) {
    //         return turn_count;
    //     }
    //     // std::cout << total;
    //     // std::cout << "yo3" << std::endl;
    //     if (shared_zs.size() > 0) {
    //         return turn_count;
    //     }
    //     turn_count += 1;
    //     selected_nodes.clear();
    //     // std::cout << "new nodes size" << new_nodes.size() << std::endl;
    //     std::swap(selected_nodes, new_nodes);
    //     // for (auto& sn : selected_nodes) {
    //     //     std::cout << sn << " ";
    //     // }
    //     // std::cout << std::endl;
    // }
    // return 0;
}

int main(int argc, char** argv) {
	printf("%zu\n", ((std::stoi(argv[argc - 1]) - 1) ? solution2() : solution1()));
}

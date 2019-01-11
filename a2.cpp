#include <iostream>
#include <map>

std::map<char, std::pair<int, int>> dict = {
    {'A', {1, 2}},
    {'B', {2, 0}},
    {'C', {0, 2}},
    {'D', {1, 0}},
    {'E', {0, 3}},
    {'F', {0, 3}},
    {'G', {0, 2}},
    {'H', {0, 4}},
    {'I', {0, 2}},
    {'J', {0, 2}},
    {'K', {0, 4}},
    {'L', {0, 2}},
    {'M', {0, 2}},
    {'N', {0, 2}},
    {'O', {1, 0}},
    {'P', {1, 1}},
    {'Q', {1, 2}},
    {'R', {1, 2}},
    {'S', {0, 2}},
    {'T', {0, 3}},
    {'U', {0, 2}},
    {'V', {0, 2}},
    {'W', {0, 2}},
    {'X', {0, 4}},
    {'Y', {0, 3}},
    {'Z', {0, 2}},
};

int main() {
    std::string input;
    std::getline(std::cin, input);
    int a = 0;
    int b = 0;
    for (auto it = input.begin(); it != input.end(); it++) {
        auto pair = dict[*it];
        a += pair.first;
        b += pair.second;
    }
    std::cout << a << std::endl << b << std::endl;
}

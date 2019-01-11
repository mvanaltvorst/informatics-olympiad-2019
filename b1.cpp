#include <iostream>
#include <vector>

std::vector<std::string> words = {
    "TWEE",
    "DRIE",
    "VIER",
    "VIJF",
    "ZES",
    "ZEVEN",
    "ACHT",
    "NEGEN"
};

// takes NUMBER, not INDEX!
bool containsNumber(std::string input, int number) {
    int index = number - 2;
    int j = 0;
    for (auto it = input.begin(); it != input.end(); it++) {
        if (*it == words[index][j]) j++;
        if (j == words[index].length()) {
            return true;
        }
    }
    return false;
}

const std::vector<int> nums = {3, 5, 6, 8};

bool containsAllNumbers(std::string input) {
    for (auto &v : nums) {
        if (!containsNumber(input, v)) {
            return false;
        }
    }
    return true;
}

int main() {
    std::cout << containsAllNumbers("") << std::endl;
    //"ACHTZVDRIEJFS"

}

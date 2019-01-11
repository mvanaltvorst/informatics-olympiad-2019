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

int main() {
    std::string input;
    std::getline(std::cin, input);
    for (int i = 0; i < words.size(); i++) {
        int value = i + 2;
        int j = 0;
        for (auto it = input.begin(); it != input.end(); it++) {
            if (*it == words[i][j]) j++;
            if (j == words[i].length()) {
                std::cout << words[i] << std::endl;
                std::exit(0);
            }
        }
    }
    std::cout << "geen" << std::endl;
}

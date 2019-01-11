#include <iostream>
#include <vector>

const int NUM = 147;

void printVec(std::vector<int> s) {
    for (auto &v:s) {
        std::cout << v << ",";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> s (NUM + 1, NUM);
    s[0] = 0;
    for (int i = 1; i <= NUM; i++) {
        s[i] = std::min(s[i], s[i-1] + 1);
        for (int j = 2; j <= i; j++) {
            if (j*i > NUM) break;
            std::cout << i << "," << j << " current: " << s[j*i] << " new: " << s[i]*s[j] << std::endl;
            s[j*i] = std::min(s[j*i], s[i] + s[j]);
        }
    }
    printVec(s);
    std::cout << s[s.size() - 1] << std::endl;
}
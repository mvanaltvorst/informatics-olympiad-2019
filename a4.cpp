#include <iostream>
#include <unordered_set>

// advance from n to n + sum of digits of n
int advance(int n) {
    int acc = n;
    while (n != 0) {
        acc += (n % 10);
        n /= 10;
    }
    return acc;
}

int startingPoints[3] {1, 3, 9};

int main() {
    int input;
    std::cin >> input;

    // initialize stream of input
    std::unordered_set<int> visited;
    while (input < 1000000) {
        visited.insert(input);
        input = advance(input);
    }

    // now check starting points and see when it collides
    int stream = 0;
    int pointOfCollision = 0;
    for (int j = 0; j < 3; j++) {
        int n = startingPoints[j];
        while (n < 1000000) {
            if (visited.find(n) != visited.end()) {
                stream = startingPoints[j];
                pointOfCollision = n;
                break;
            }
            n = advance(n);
        }
        if (stream != 0) break;
    }
    std::cout << stream << std::endl << pointOfCollision << std::endl;
}

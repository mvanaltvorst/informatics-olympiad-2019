#include <iostream>
#include <vector>

int main() {
    // read input
    int n;
    int buf;
    int amountToPay;
    std::vector<int> coins;

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> buf;
        coins.push_back(buf);
    }

    std::cin >> amountToPay;

    std::vector<unsigned long long int> waysPossible (300);
    waysPossible[0] = 1;

    // first we start with the low coin (1), then we do the higher coins
    // to avoid counting more ways than there actually are
    // (eg. you count 1,2,1 and 1,1,2 as seperate ways, but we only count the low
    // coins first to avoid doing this.)
    for (int j = 0; j < coins.size(); j++) {
        for (int i = 1; i <= amountToPay; i++) {
            if (coins[j] <= i) waysPossible[i] += waysPossible[i - coins[j]];
        }
    }
    std::cout << waysPossible[amountToPay] << std::endl;
}

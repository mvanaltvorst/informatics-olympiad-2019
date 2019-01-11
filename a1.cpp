#include <iostream>

int main() {
    int n;
    int m;
    int v;
    std::cin >> n >> m >> v;
    int offset = 0;
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n + offset; i++) {
            std::cout << '*';
        }
        std::cout << std::endl;
        offset += v;
    }
}

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>


class Graph {
    public:
        std::map<char, std::unordered_set<char>> edges;
        std::unordered_set<char> vertices;
        Graph() {};
        void addEdge(char from, char to) {
            vertices.insert(from);
            vertices.insert(to);
            edges[from].insert(to);
        }
        std::unordered_set<char> * getNeighbours(char from) {
            return &edges[from];
        }
        std::vector<char> getPath() {
            std::vector<char> visited(vertices.size());
            for (auto it = vertices.begin(); it != vertices.end(); it++) {
                if (dfs(&visited, *it, 0)) {
                    return visited;
                }
                visited.clear();
                visited.resize(vertices.size());
            }
            return visited;
        }

        bool dfs(std::vector<char> * path, char location, int depth) {
            (*path)[depth] = location;
            if (depth == vertices.size() - 1) return true;
            auto neighbours = getNeighbours(location);
            for (auto &neighbour : *neighbours) {
                if (std::find(path->begin(), path->end(), neighbour) != path->end()) continue;
                (*path)[depth] = location;
                if (dfs(path, neighbour, depth + 1)) {
                    return true;
                }
            }
            return false;
        }

        void printGraph() {
            for (auto& v : vertices) {
                std::cout << v << ": ";
                for (auto& n : edges[v]) {
                    std::cout << n;
                }
                std::cout << std::endl;
            }
        }
};

int main() {
    int n;
    std::cin >> n;
    std::string input;
    Graph g;
    for (int i = 0; i < n; i++) {
        std::cin >> input;
        for (int j = 0; j < n; j++) {
            if (input[j] == '1') {
                g.addEdge(static_cast<char>(65 + i), static_cast<char>(65 + j));
            }
        }
    }
    auto path = g.getPath();
    for (auto &v : path) {
        std::cout << v;
    }
    std::cout << std::endl;
}

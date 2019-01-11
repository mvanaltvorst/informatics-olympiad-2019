#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>


template <class T> class Node;

template <class T> struct Road {
    T a;
    T b;
    Road(T a, T b) : a(a), b(b) {}
};

template <class T> struct Edge {
public:
    int weight;
    Node<T> * neighbour;
    Edge(Node<T> * neighbour, int weight) : neighbour(neighbour), weight(weight) {}
};

template <class T> class Node {
public:
    T value;
    std::vector<Edge<T>> neighbours;
    bool visited;
    Node(T value) : value(value) {
        visited = false;
    }
    void addEdge(Edge<T> edge) {
        neighbours.push_back(edge);
    }
};

template <class T> struct Path {
    int dist;
    std::vector<T> path;
    Path(std::vector<T> path, int dist) : path(path), dist(dist) {}
    void append(Path<T> d) {
        path.insert(path.end(), d.path.begin() + 1, d.path.end());
        dist += d.dist;
    }
};

// Vals must be unique
template <class T> class Graph {
public:
    std::map<T, Node<T>*> nodes;

    void addNode(T value) {
        Node<T> * n = new Node<T>(value);
        nodes.insert(std::make_pair(value, n));
    }
    void addEdge(T from, T to, int weight) {
        Node<T> * target = nodes.at(to);
        Edge<T> newEdge (target, weight);
        Node<T> * source = nodes.at(from);
        source->addEdge(newEdge);
    }
    void reset() {
        for (auto &keyval : nodes) {
            keyval.first.visited = false;
        }
    }

    Path<T> dijkstra(T from, T to) {
        std::map<Node<T>*, int> minDistances;
        std::map<Node<T>*, Node<T>*> previous;
        std::priority_queue<std::pair<int, Node<T>*>, std::vector<std::pair<int, Node<T>*>>, std::greater<std::pair<int, Node<T>*>>> pq;
        auto fromNode = nodes[from];
        minDistances[fromNode] = 0;
        pq.push(std::make_pair(0, fromNode));
        while (!pq.empty()) {
            Node<T> * currentNode = pq.top().second;
            pq.pop();
            if (currentNode->value == to) break;
            for (auto &edge : currentNode->neighbours) {
                int weight = edge.weight;
                Node<T> * v = edge.neighbour;
                if (minDistances.find(v) == minDistances.end() || minDistances[v] > minDistances[currentNode] + weight) {
                    minDistances[v] = minDistances[currentNode] + weight;
                    pq.push(std::make_pair(minDistances[v], v));
                    previous[v] = currentNode;
                }
            }
        }
        if (minDistances.find(nodes[to]) == minDistances.end()) {
            std::cerr << "[!] Couldn't find path from " << from << " to " << to << std::endl;
            std::cerr << *this;
            std::cerr << "[!] Exiting..." << std::endl;
            std::exit(1);
        }
        std::vector<T> path;
        Node<T> * currentNode = nodes[to];
        while (currentNode != fromNode) {
            path.push_back(currentNode->value);
            currentNode = previous[currentNode];
        }
        path.push_back(from);
        std::reverse(path.begin(), path.end());
        Path<T> p (path, minDistances[nodes[to]]);
        return p;
    }

    Path<T> getDistance(std::vector<T> in) {
        Path<T> acc = dijkstra('A', in[0]);
        for (int i = 0; i < in.size() - 1; i++) {
            acc.append(dijkstra(in[i], in[i + 1]));
        }
        acc.append(dijkstra(in[in.size() - 1], 'A'));
        return acc;
    }

    friend std::ostream& operator<<(std::ostream& os, const Graph<T> g) {
        for (auto &keyval : g.nodes) {
            os << keyval.second->value << ": ";
            int amountOfNeighbours = keyval.second->neighbours.size();
            for (int i = 0; i < amountOfNeighbours; i++) {
                Edge<T> neighbour = keyval.second->neighbours[i];
                os << "[" << neighbour.neighbour->value << ", " << neighbour.weight << "]";
                if (i != amountOfNeighbours - 1) os << ", ";
            }
            os << std::endl;
        }
        return os;
    }
    void free() {
        for (auto& node : nodes) {
            delete node.second;
        }
    }
};

void travelingSalesman(std::vector<char> acc, std::vector<Road<char>> housesLeft, std::vector<std::vector<char>> &results) {
    if (housesLeft.size() == 0) {
        results.push_back(acc);
        return;
    }
    auto oldHousesLeft = housesLeft;
    for (int i = 0; i < oldHousesLeft.size(); i++) {
        auto newAcc = acc;
        auto house = oldHousesLeft[i];
        housesLeft = oldHousesLeft;
        housesLeft.erase(housesLeft.begin() + i);
        newAcc.push_back(house.a);
        newAcc.push_back(house.b);
        travelingSalesman(newAcc, housesLeft, results);

        newAcc = acc;
        newAcc.push_back(house.b);
        newAcc.push_back(house.a);
        travelingSalesman(newAcc, housesLeft, results);
    }
}

int main() {
    Graph<char> g;
    for (int i = 0; i < 25; i++) {
        g.addNode(static_cast<char>(65+i));
    }

    g.addEdge('A', 'B', 4);
    g.addEdge('A', 'C', 4);
    g.addEdge('A', 'D', 4);
    g.addEdge('A', 'E', 4);
    g.addEdge('B', 'N', 4);
    g.addEdge('B', 'M', 3);
    g.addEdge('B', 'J', 3);
    g.addEdge('C', 'P', 4);
    g.addEdge('C', 'J', 3);
    g.addEdge('C', 'K', 3);
    g.addEdge('D', 'R', 4);
    g.addEdge('D', 'L', 3);
    g.addEdge('D', 'K', 3);
    g.addEdge('E', 'T', 4);
    g.addEdge('E', 'L', 3);
    g.addEdge('E', 'M', 3);
    g.addEdge('F', 'Y', 9);
    g.addEdge('F', 'V', 9);
    g.addEdge('G', 'V', 9);
    g.addEdge('G', 'W', 9);
    g.addEdge('H', 'W', 9);
    g.addEdge('H', 'X', 9);
    g.addEdge('I', 'X', 9);
    g.addEdge('I', 'Y', 9);
    g.addEdge('J', 'A', 4);
    g.addEdge('J', 'B', 3);
    g.addEdge('J', 'C', 3);
    g.addEdge('K', 'A', 4);
    g.addEdge('K', 'D', 3);
    g.addEdge('K', 'C', 3);
    g.addEdge('L', 'A', 4);
    g.addEdge('L', 'D', 3);
    g.addEdge('L', 'E', 3);
    g.addEdge('M', 'A', 4);
    g.addEdge('M', 'E', 3);
    g.addEdge('M', 'B', 3);
    g.addEdge('N', 'F', 4);
    g.addEdge('N', 'U', 6);
    g.addEdge('N', 'O', 6);
    g.addEdge('O', 'J', 4);
    g.addEdge('O', 'N', 6);
    g.addEdge('O', 'P', 6);
    g.addEdge('P', 'G', 4);
    g.addEdge('P', 'O', 6);
    g.addEdge('P', 'Q', 6);
    g.addEdge('Q', 'K', 4);
    g.addEdge('Q', 'R', 6);
    g.addEdge('Q', 'P', 6);
    g.addEdge('R', 'H', 4);
    g.addEdge('R', 'Q', 6);
    g.addEdge('R', 'S', 6);
    g.addEdge('S', 'L', 4);
    g.addEdge('S', 'R', 6);
    g.addEdge('S', 'T', 6);
    g.addEdge('T', 'I', 4);
    g.addEdge('T', 'S', 6);
    g.addEdge('T', 'U', 6);
    g.addEdge('U', 'M', 4);
    g.addEdge('U', 'T', 6);
    g.addEdge('U', 'N', 6);
    g.addEdge('V', 'O', 4);
    g.addEdge('V', 'F', 9);
    g.addEdge('V', 'G', 9);
    g.addEdge('W', 'Q', 4);
    g.addEdge('W', 'G', 9);
    g.addEdge('W', 'H', 9);
    g.addEdge('X', 'S', 4);
    g.addEdge('X', 'H', 9);
    g.addEdge('X', 'I', 9);
    g.addEdge('Y', 'U', 4);
    g.addEdge('Y', 'I', 9);
    g.addEdge('Y', 'F', 9);

    std::cout << g;
    std::vector<std::vector<char>> results;
    std::vector<Road<char>> housesLeft {
        Road<char>('J', 'C'),
        Road<char>('E', 'L'),
        Road<char>('U', 'N'),
        Road<char>('O', 'N'),
        Road<char>('Y', 'F'),
        Road<char>('F', 'V')
    };

    travelingSalesman(std::vector<char>(), housesLeft, results);
    std::cout << "Routes: " << results.size() << std::endl;

    Path<char> minPath(std::vector<char>(), -1);
    for (auto &in : results) {
        Path<char> newPath = g.getDistance(in);
        if (minPath.dist == -1 || newPath.dist < minPath.dist) {
            minPath = newPath;
        }
    }

    std::cout << "Min dist: " << minPath.dist << std::endl << "Path: " << std::endl;
    for (auto &v : minPath.path) {
        std::cout << v << ", ";
    }
    std::cout << std::endl;
    
    g.free();
}

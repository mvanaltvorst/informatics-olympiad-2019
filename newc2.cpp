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

const int WIDTH = 8;
const int HEIGHT = 10;

const std::pair<int, int> offsets[8] = {
    {0, -1},
    {1, -1},
    {1, 0},
    {1, 1},
    {0, 1},
    {-1, 1},
    {-1, 0},
    {-1, -1}
};

const std::string inputstring = "\
-*O++-#-\
*-#-#+*O\
-O#O**-*\
*+O-##*+\
*+**O+--\
O++#+*--\
O++#*#*O\
+#O+O#-+\
#@OO#O#-\
OO#+*-*#";

struct Point{
    int x;
    int y;
    Point applyMovement(int movement) {
        std::pair<int, int> offset = offsets[movement];
        return Point{x + offset.first, y + offset.second};
    }
    bool tryMovement(int movement) {
        std::pair<int, int> offset = offsets[movement];
        return ((x + offset.first) < WIDTH && (x + offset.first) >= 0 && (y + offset.second) < HEIGHT && (y + offset.second) >= 0);
    }

    bool operator==(const Point& b) const {
        return (x == b.x && y == b.y);
    }
};

class Board {
    private:
        char matrix[HEIGHT][WIDTH];
        Point startingPos;

    public:
        Board(std::istream& input) {
            // std::string buf;
            for (int i = 0; i < HEIGHT; i++) {
                // std::getline(input, buf);
                for (int j = 0; j < WIDTH; j++) {
                    // matrix[i][j] = buf[j];
                    matrix[i][j] = inputstring[WIDTH*i+j];
                    // if (buf[j] == '@') {
                    if (inputstring[WIDTH*i+j] == '@') {
                        startingPos = Point{j, i};
                    }
                }
            }
        }

        void print() {
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    std::cout << matrix[i][j];
                }
                std::cout << std::endl;
            }
        }

        std::vector<int> getMovements() {
            // start BFS
            Point currentPos = startingPos;
            int gemsLeft = 16;
            std::vector<int> totalMovements;
            std::vector<Point> gemLocs;
            while (true) {
                std::pair<int, std::vector<int>> res = BFS(currentPos, &gemLocs);

                if (res.first == 0) return totalMovements;
                gemsLeft -= res.first;
                totalMovements.insert(totalMovements.end(), res.second.begin(), res.second.end());
                if (gemsLeft <= 0) return totalMovements;
                std::vector<Point> trash;
                for (auto &m : res.second) {
                    currentPos = executeMove(currentPos, m, &trash).first;
                }
            }
        }

        std::pair<int, std::vector<int>> BFS(Point startPosPoint, std::vector<Point> * gemLocs) {
            int gemsFound = 0;
            std::vector<int> movements;

            std::vector<Point> visited;
            std::queue<std::pair<Point, std::vector<int>>> q;
            q.push(std::make_pair(startPosPoint, std::vector<int>()));
            while (!q.empty()) {
                std::pair<Point, std::vector<int>> v = q.front();
                q.pop();
                // std::cout << "current pos:" << v.first.x << "," << v.first.y << std::endl;

                for (int m = 0; m < 8; m++) {
                    std::pair<Point, int> w = executeMove(v.first, m, gemLocs);
                    if (std::find(visited.begin(), visited.end(), w.first) != visited.end()) continue;
                    std::vector<int> newPath = v.second;
                    newPath.push_back(m);
                    if (w.second != 0) {
                        // std::cout << "gems found:" << w.second << std::endl;
                        gemsFound = w.second;
                        movements = newPath;
                        break;
                    }
                    q.push(std::make_pair(w.first, newPath));
                }
                if (gemsFound) break;
            }
            return std::make_pair(gemsFound, movements);
        }

        std::pair<Point, int> executeMove(Point currentPos, int movement, std::vector<Point> * removedGems) {
            Point backupPoint = currentPos;
            Point newPos;
            int gems = 0;
            std::vector<Point> toRemove;
            while (true) {
                bool success = currentPos.tryMovement(movement);
                if (!success) {
                    removedGems->insert(removedGems->end(), toRemove.begin(), toRemove.end());
                    return std::make_pair(currentPos, gems);
                }
                newPos = currentPos.applyMovement(movement);
                char item = getItemAt(newPos);
                if (item == '*') return std::make_pair(backupPoint, 0);
                if (item == '+') {
                    if (std::find(removedGems->begin(), removedGems->end(), newPos) == removedGems->end()) {
                        gems++;
                        toRemove.push_back(newPos);
                    }
                }
                if (item == 'O' || item == '@') {
                    removedGems->insert(removedGems->end(), toRemove.begin(), toRemove.end());
                    return std::make_pair(newPos, gems);
                }
                if (item == '#') {
                    removedGems->insert(removedGems->end(), toRemove.begin(), toRemove.end());
                    return std::make_pair(currentPos, gems);
                }
                // skip empty space (-)
                currentPos = newPos;
            }
        }

        char getItemAt(Point pos) {
            return matrix[pos.y][pos.x];
        }
};

int main() {
    Board b (std::cin);
    b.print();
    Graph<int> g;
    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        g.addNode(i);
    }

    std::vector<Point> trash;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int m = 0; m < 8; m++) {
                std::pair<Point, int> res = b.executeMove(Point{j, i}, m, &trash);
                trash.clear();
                if (res.first == Point{j, i}) continue;
                g.addEdge(i*WIDTH + j, res.first.y * WIDTH + res.first.x, 1);
            }
        }
    }
    std::cout << g;

    // std::vector<int> movements = b.getMovements();
    // for (auto &v : movements) {
    //     std::cout << v;
    // }
    // std::cout << std::endl;
}

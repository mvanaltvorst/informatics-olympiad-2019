#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

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
            std::string buf;
            for (int i = 0; i < HEIGHT; i++) {
                std::getline(input, buf);
                for (int j = 0; j < WIDTH; j++) {
                    matrix[i][j] = buf[j];
                    if (buf[j] == '@') {
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
    std::vector<int> movements = b.getMovements();
    for (auto &v : movements) {
        std::cout << v;
    }
    std::cout << std::endl;
}

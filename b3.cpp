#include <iostream>
#include <vector>
#include <algorithm>

// [S][K]
// [i][j]
// i = student, j = job
const int WIDTH = 8;
const int HEIGHT = 12;
const int DEST_SIZE = std::max(WIDTH, HEIGHT);

struct Point {
    int i;
    int j;
    bool operator==(const Point& other) {
        return other.i == i && other.j == j;
    }
};

class HungarianAlgorithm {
public:
    int matrix[DEST_SIZE][DEST_SIZE] {
        {42, 55, 65, 41, 66, 40, 41, 44},
        {53, 60, 62, 77, 41, 76, 42, 50},
        {40, 51, 62, 60, 78, 62, 73, 72},
        {76, 79, 43, 55, 46, 77, 42, 75},
        {66, 64, 59, 70, 56, 63, 51, 59},
        {58, 52, 50, 44, 42, 52, 49, 46},
        {71, 69, 57, 57, 43, 72, 40, 79},
        {61, 50, 61, 77, 48, 65, 45, 77},
        {66, 53, 60, 44, 53, 61, 60, 68},
        {49, 77, 71, 58, 56, 47, 50, 76},
        {53, 66, 62, 61, 75, 66, 46, 65},
        {57, 66, 52, 53, 45, 71, 54, 55}
    };
    // int matrix[DEST_SIZE][DEST_SIZE] {
    //     {1, 2, 3, 4},
    //     {4, 1, 3, 2},
    //     {1, 3, 5, 4},
    //     {2, 5, 3, 5}
    // };
    void addDummy() {
        int max = 0;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                max = std::max(max, matrix[i][j]);
            }
        }
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = WIDTH; j < HEIGHT; j++) {
                matrix[i][j] = max;
            }
        }
    }
    bool assign(std::vector<Point>& covered, std::vector<Point>& max, int depth, int& maxDepth) {
        if (depth > maxDepth) {
            maxDepth = depth;
            max = covered;
        }
        if (depth == DEST_SIZE) return true;
        for (int i = 0; i < DEST_SIZE; i++) {
            for (int j = 0; j < DEST_SIZE; j++) {
                // if (depth == 0) std::cout << i << "," << j << std::endl;
                // if (std::find(covered.begin(), covered.end(), Point{i, j}) != covered.end()) continue;
                if (matrix[i][j] == 0) {
                    bool shouldBreak = false;
                    for (auto &v : covered) {
                        if (v.i == i || v.j == j) {
                            shouldBreak = true;
                            break;
                        }
                    }
                    if (shouldBreak) continue;
                    covered.push_back(Point{i, j});
                    if (assign(covered, max, depth + 1, maxDepth)) return true;
                    covered.pop_back();
                }
            }
        }
        return false;
    }

    void subtractRowsCols() {
        // rows
        for (int i = 0; i < DEST_SIZE; i++) {
            int min = 100;
            for (int j = 0; j < DEST_SIZE; j++) {
                min = std::min(min, matrix[i][j]);
            }
            for (int j = 0; j < DEST_SIZE; j++) {
                matrix[i][j] -= min;
            }
        }
        // columns
        for (int j = 0; j < DEST_SIZE; j++) {
            int min = 100;
            for (int i = 0; i < DEST_SIZE; i++) {
                min = std::min(min, matrix[i][j]);
            }
            for (int i = 0; i < DEST_SIZE; i++) {
                matrix[i][j] -= min;
            }
        }
    }
    std::vector<Point> solve() {
        std::vector<Point> trash;

        std::vector<Point> points;
        int maxDepth = 0;
        assign(trash, points, 0, maxDepth);
        if (maxDepth == DEST_SIZE) return points;

        std::vector<int> markedRows;

        for (int i = 0; i < DEST_SIZE; i++) {
            bool isUnmarked = true;
            for (auto &v : points) {
                if (v.i == i) {
                    isUnmarked = false;
                    break;
                }
            }
            if (isUnmarked) {
                markedRows.push_back(i);
            }
        }
        std::vector<int> markedColumns;
        for (auto &v : markedRows) {
            for (int j = 0; j < DEST_SIZE; j++) {
                if (matrix[v][j] == 0) {
                    if (std::find(markedColumns.begin(), markedColumns.end(), j) == markedColumns.end()) markedColumns.push_back(j);
                }
            }
        }
        for (auto &v : points) {
            if (std::find(markedColumns.begin(), markedColumns.end(), v.j) != markedColumns.end()) {
                if (std::find(markedRows.begin(), markedRows.end(), v.i) == markedRows.end()) markedRows.push_back(v.i);
            }
        }
        // for (auto &v : markedRows) {
        //     std::cout << v << std::endl;
        // }
        std::vector<int> rowLines;
        for (int i = 0; i < DEST_SIZE; i++) {
            if (std::find(markedRows.begin(), markedRows.end(), i) == markedRows.end()) rowLines.push_back(i);
        }
        std::vector<int> columnLines = markedColumns;
        printLines(rowLines, columnLines);
        return std::vector<Point>();
    }
    void printLines(std::vector<int> rowLines, std::vector<int> columnLines) {
        for (int i = 0; i < DEST_SIZE; i++) {
            for (int j = 0; j < DEST_SIZE; j++) {
                bool rowLine = std::find(rowLines.begin(), rowLines.end(), i) != rowLines.end();
                bool colLine = std::find(columnLines.begin(), columnLines.end(), j) != columnLines.end();
                if (rowLine) {
                    if (colLine) {
                        std::cout << "+\t";
                    } else {
                        std::cout << "-\t";
                    }
                } else if (colLine) {
                    std::cout << "|\t";
                } else {
                    std::cout << matrix[i][j] << "\t";
                }
            }
            std::cout << std::endl;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, HungarianAlgorithm& ha) {
        for (int i = 0; i < DEST_SIZE; i++) {
            for (int j = 0; j < DEST_SIZE; j++) {
                if (ha.matrix[i][j] != 0)
                    os << ha.matrix[i][j] << "\t";
                else
                    os << "\033[31m0\033[0m\t";
            }
            os << std::endl;
        }
        return os;
    }
};

int main() {
    HungarianAlgorithm ha;
    ha.addDummy();
    std::cout << ha << std::endl;
    // ha.subtractRowsCols();
    // std::cout << ha << std::endl;
    // auto ret = ha.solve();
    // for (auto &v : ret) {
    //     std::cout << v.j << "," << v.i << std::endl;
    // }
    // std::vector<Point> pnts = {
    //     Point{1, 6},
    //     Point{2, 5},
    //     Point{3, 1},
    //     Point{4, 3},
    //     Point{6, 8},
    //     Point{7, 7},
    //     Point{8, 2},
    //     Point{9, 4},
    // };
    // int acc = 0;
    // for (auto &v : pnts) {
    //     std::cout << v.i - 1 << "," << v.j - 1 << ": " << ha.matrix[v.i - 1][v.j - 1] << std::endl;
    //     acc += ha.matrix[v.i - 1][v.j - 1];
    // }
    // std::cout << "Answer: " << acc << std::endl;
}
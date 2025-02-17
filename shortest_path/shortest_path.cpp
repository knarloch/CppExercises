#include <vector>
#include <utility>
#include <cassert>
#include <optional>
#include <stdexcept>
#include <queue>
#include <iostream>

using Position = std::pair<size_t, size_t>;

struct Cell {
    Cell(char c) : value{c} {};

    std::optional<std::pair<size_t, size_t>> parent;
    char value;
};

Position FindSource(const std::vector<std::vector<Cell>> &lab) {
    for (auto i = 0; i < lab.size(); i++) {
        for (auto j = 0; j < lab[0].size(); j++) {
            if (lab[i][j].value == 's') {
                return {i, j};
            }
        }
    }
    throw std::runtime_error{"No source"};
}

std::vector<std::pair<size_t, size_t>> ShortestPath(Position source, std::vector<std::vector<Cell>> lab) {
    lab[source.first][source.second].parent = source;
    std::queue<Position> to_proces;
    to_proces.push(source);
    while (not to_proces.empty()) {
        auto pos = to_proces.front();
        to_proces.pop();
        if (lab[pos.first][pos.second].value == 't') {
            std::vector<Position> result_reversed;
            result_reversed.push_back(pos);
            while (lab[pos.first][pos.second].parent != pos) {
                pos = lab[pos.first][pos.second].parent.value();
                result_reversed.push_back(pos);
            }
            return {result_reversed.crbegin(), result_reversed.crend()};
        }
        if (pos.first > 0) {
            auto next = pos;
            next.first -= 1;
            if ((not lab[next.first][next.second].parent) and lab[next.first][next.second].value != 'w') {
                lab[next.first][next.second].parent = pos;
                to_proces.push(next);
            }
        }
        if (pos.second > 0) {
            auto next = pos;
            next.second -= 1;
            if ((not lab[next.first][next.second].parent) and lab[next.first][next.second].value != 'w') {
                lab[next.first][next.second].parent = pos;
                to_proces.push(next);
            }
        }
        if (pos.first < lab.size() - 1) {
            auto next = pos;
            next.first += 1;
            if ((not lab[next.first][next.second].parent) and lab[next.first][next.second].value != 'w') {
                lab[next.first][next.second].parent = pos;
                to_proces.push(next);
            }
        }
        if (pos.second < lab[0].size() - 1) {
            auto next = pos;
            next.second += 1;
            if ((not lab[next.first][next.second].parent) and lab[next.first][next.second].value != 'w') {
                lab[next.first][next.second].parent = pos;
                to_proces.push(next);
            }
        }


    }
    return {};
}

void PrintLabyrinth(const std::vector<std::vector<Cell>> &lab) {
    std::cout << "\n";
    for (const auto &row: lab) {
        for (const auto &cell: row) {
            std::cout << cell.value << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\n";

}

int main() {
    {
        std::vector<std::vector<Cell>> labyrinth{
                {'l', 'l', 'l', 'l'},
                {'s', 'w', 'l', 'l'},
                {'l', 'w', 'l', 'l'},
                {'l', 'l', 't', 'l'},
        };
        auto path = ShortestPath(FindSource(labyrinth), labyrinth);
        assert((path == std::vector<std::pair<size_t, size_t>>{{1, 0},
                                                               {2, 0},
                                                               {3, 0},
                                                               {3, 1},
                                                               {3, 2}}));
        for (const auto pos: path) {
            labyrinth[pos.first][pos.second].value += 'A' - 'a';
        }
        PrintLabyrinth(labyrinth);
    }

    {
        std::vector<std::vector<Cell>> labyrinth{
                {'l', 'w', 'l', 'l'},
                {'s', 'w', 'l', 'l'},
                {'l', 'w', 'l', 'l'},
                {'l', 'w', 't', 'l'},
        };
        auto path = ShortestPath(FindSource(labyrinth), labyrinth);
        assert(path.empty());
        for (const auto pos: path) {
            labyrinth[pos.first][pos.second].value += 'A' - 'a';
        }
        PrintLabyrinth(labyrinth);
    }

    {
        std::vector<std::vector<Cell>> labyrinth{
                {'l', 't', 'l', 'l'},
                {'s', 'w', 'l', 'l'},
                {'l', 'w', 'l', 'l'},
                {'l', 'w', 't', 'l'},
        };
        auto path = ShortestPath(FindSource(labyrinth), labyrinth);
        assert((path == std::vector<std::pair<size_t, size_t>>{{1, 0},
                                                               {0, 0},
                                                               {0, 1}}));
        for (const auto pos: path) {
            labyrinth[pos.first][pos.second].value += 'A' - 'a';
        }
        PrintLabyrinth(labyrinth);
    }

    {
        std::vector<std::vector<Cell>> labyrinth{
                {'l', 't', 'l', 'l'},
                {'s', 'l', 'l', 'l'},
                {'l', 'w', 'l', 'l'},
                {'l', 'w', 't', 'l'},
        };
        auto path = ShortestPath(FindSource(labyrinth), labyrinth);
        assert((path == std::vector<std::pair<size_t, size_t>>{{1, 0},
                                                               {0, 0},
                                                               {0, 1}}));
        for (const auto pos: path) {
            labyrinth[pos.first][pos.second].value += 'A' - 'a';
        }
        PrintLabyrinth(labyrinth);
    }

    {
        std::vector<std::vector<Cell>> labyrinth{
                {'l', 'l', 'l', 'l'},
                {'s', 'l', 'l', 'l'},
                {'l', 't', 'l', 'l'},
                {'l', 'w', 't', 'l'},
        };
        auto path = ShortestPath(FindSource(labyrinth), labyrinth);
        assert((path == std::vector<std::pair<size_t, size_t>>{{1, 0},
                                                               {2, 0},
                                                               {2, 1}}) ||
               (path == std::vector<std::pair<size_t, size_t>>{{1, 0},
                                                               {1, 1},
                                                               {2, 1}}));
        for (const auto pos: path) {
            labyrinth[pos.first][pos.second].value += 'A' - 'a';
        }
        PrintLabyrinth(labyrinth);
    }

    {
        std::vector<std::vector<Cell>> labyrinth{
                {'s', 'w', 'l', 'l', 'l'},
                {'l', 'w', 'l', 'w', 'l'},
                {'l', 'w', 'l', 'w', 'l'},
                {'l', 'l', 'l', 'w', 't'},
        };
        auto path = ShortestPath(FindSource(labyrinth), labyrinth);
        assert(path.size() == 14);
        for (const auto pos: path) {
            labyrinth[pos.first][pos.second].value += 'A' - 'a';
        }
        PrintLabyrinth(labyrinth);
    }

    return 0;
}
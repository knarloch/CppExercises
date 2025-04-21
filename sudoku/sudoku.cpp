#include <array>
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>

using namespace std;

class SudokuLookup {
public:
    SudokuLookup(const vector<vector<int>> &board) {
        N = board.size();
        sqrtN = std::sqrt(N);
        rows = vector<vector<bool>>(N, vector<bool>(N, false));
        cols = vector<vector<bool>>(N, vector<bool>(N, false));
        squares = vector<vector<bool>>(N, vector<bool>(N, false));
        for (auto r = 0; r < N; r++) {
            for (auto c = 0; c < N; c++) {
                if (board[r][c] != 0) {
                    auto assert_me = setIfValid(r, c, board[r][c]);
                    assert(assert_me == true);
                }
            }
        }
    }

    bool setIfValid(int r, int c, int v) {
        v = v - 1;
        if (rows[r][v] == false and cols[c][v] == false and squares[getSquareIndex(r, c)][v] == false) {
            rows[r][v] = true;
            cols[c][v] = true;
            squares[getSquareIndex(r, c)][v] = true;
            return true;
        }
        return false;
    }

    void clear(int r, int c, int v) {
        v = v - 1;
        rows[r][v] = false;
        cols[c][v] = false;
        squares[getSquareIndex(r, c)][v] = false;
    }

    int getN() const {
        return N;
    }

private:
    int N = 0;
    int sqrtN = 0;
    vector<vector<bool>> cols{{}};
    vector<vector<bool>> rows{{}};
    vector<vector<bool>> squares{{}};

    int getSquareBracket(int rc) const {
        for(auto bracket = 0; bracket<sqrtN; bracket++) {
            if (0 <= rc and rc < sqrtN * (bracket+1)) {
                return bracket;
            }
        }
        return -1;
    }

    int getSquareIndex(int r, int c) const {
        auto rb = getSquareBracket(r);
        auto cb = getSquareBracket(c);
        auto result =  3 * cb + rb;
        return result;
    }
};

vector<vector<int>> solveSudoku(SudokuLookup &lookup, vector<vector<int>> &board, int start_r, int start_c) {

    for (auto r = start_r; r < lookup.getN(); r++) {
        for (auto c = start_c; c < lookup.getN(); c++) {
            if (board[r][c] != 0) {
                continue;
            }
            for (auto v = 1; v <= lookup.getN(); v++) {
                if (lookup.setIfValid(r, c, v)) {
                    board[r][c] = v;
                    auto result = solveSudoku(lookup, board, r, c);
                    if (result.empty()) {
                        lookup.clear(r, c, v);
                        board[r][c] = 0;
                    } else {
                        return result;
                    }
                }
            }
            return {};
        }
        start_c=0;
    }
    return board;
}


vector<vector<int>> solveSudoku(vector<vector<int>> board) {
    SudokuLookup lookup{board};
    return solveSudoku(lookup, board, 0, 0);
}

int main() {
    try {
        {
            {
                vector<vector<int>> board = {
                        {7, 8, 0, 4, 0, 0, 1, 2, 0},
                        {6, 0, 0, 0, 7, 5, 0, 0, 9},
                        {0, 0, 0, 6, 0, 1, 0, 7, 8},
                        {0, 0, 7, 0, 4, 0, 2, 6, 0},
                        {0, 0, 1, 0, 5, 0, 9, 3, 0},
                        {9, 0, 4, 0, 6, 0, 0, 0, 5},
                        {0, 7, 0, 3, 0, 0, 0, 1, 2},
                        {1, 2, 0, 0, 0, 7, 4, 0, 0},
                        {0, 4, 9, 2, 0, 6, 0, 0, 7}
                };
                vector<vector<int>> expected = {
                        {7, 8, 5, 4, 3, 9, 1, 2, 6},
                        {6, 1, 2, 8, 7, 5, 3, 4, 9},
                        {4, 9, 3, 6, 2, 1, 5, 7, 8},
                        {8, 5, 7, 9, 4, 3, 2, 6, 1},
                        {2, 6, 1, 7, 5, 8, 9, 3, 4},
                        {9, 3, 4, 1, 6, 2, 7, 8, 5},
                        {5, 7, 8, 3, 9, 4, 6, 1, 2},
                        {1, 2, 6, 5, 8, 7, 4, 9, 3},
                        {3, 4, 9, 2, 1, 6, 8, 5, 7}
                };
                auto solved = solveSudoku(board);

                assert(solved == expected);

            }
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        assert(false);
    }

    return 0;
}

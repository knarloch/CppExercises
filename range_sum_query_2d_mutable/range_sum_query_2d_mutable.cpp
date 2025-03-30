#include <cassert>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

using Matrix = vector<vector<int>>;

struct QuadTree {
    int val{0};
    int x_begin;
    int x_end;
    int y_begin;
    int y_end;

    shared_ptr<QuadTree> ll;
    shared_ptr<QuadTree> lh;
    shared_ptr<QuadTree> hl;
    shared_ptr<QuadTree> hh;

    QuadTree(const Matrix &matrix, int x_begin_, int x_end_, int y_begin_, int y_end_) : x_begin{x_begin_},
                                                                                         x_end{x_end_},
                                                                                         y_begin{y_begin_},
                                                                                         y_end{y_end_} {
        if (x_begin == x_end or y_begin == y_end) {
            return;
        } else if (x_end - x_begin == 1 and y_end - y_begin == 1) {
            val = matrix[x_begin][y_begin];
        } else {
            auto x_mid = x_begin + (x_end - x_begin) / 2;
            auto y_mid = y_begin + (y_end - y_begin) / 2;
            ll = make_shared<QuadTree>(matrix, x_begin, x_mid, y_begin, y_mid);
            lh = make_shared<QuadTree>(matrix, x_begin, x_mid, y_mid, y_end);
            hl = make_shared<QuadTree>(matrix, x_mid, x_end, y_begin, y_mid);
            hh = make_shared<QuadTree>(matrix, x_mid, x_end, y_mid, y_end);
            val = ll->val + lh->val + hl->val + hh->val;
        }
    }

    int query(int qx_begin, int qx_end, int qy_begin, int qy_end) {
        if (qx_begin <= x_begin and x_end <= qx_end and qy_begin <= y_begin and y_end <= qy_end) {
            return val;
        }
        if (x_end <= qx_begin or qx_end <= x_begin or y_end <= qy_begin or qy_end <= y_begin) {
            return 0;
        }
        if( ll and lh and hl and hh) {
            return ll->query(qx_begin, qx_end, qy_begin, qy_end) + lh->query(qx_begin, qx_end, qy_begin, qy_end) +
                   hl->query(qx_begin, qx_end, qy_begin, qy_end) + hh->query(qx_begin, qx_end, qy_begin, qy_end);
        }
        return 0;
    }

    int update(int x, int y, int new_val) {
        if (x_begin <= x and x < x_end and y_begin <= y and y < y_end) {
            if (ll and lh and hl and hh) {
                val = ll->update(x, y, new_val) + lh->update(x, y, new_val) + hl->update(x, y, new_val) +
                      hh->update(x, y, new_val);
                return val;
            }
            val = new_val;
            return val;
        }
        return val;
    }

};

class NumMatrix {
public:
    NumMatrix(vector<vector<int>> &matrix) {
        root = make_shared<QuadTree>(matrix, 0, matrix.size(), 0, matrix[0].size());
    }

    void update(int row, int col, int val) {
        root->update(row, col, val);
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        return root->query(row1, row2 + 1, col1, col2 + 1);
    }

private:
    shared_ptr<QuadTree> root;
};

int main() {
    {
        Matrix m{{3, 0, 1, 4, 2},
                 {5, 6, 3, 2, 1},
                 {1, 2, 0, 1, 5},
                 {4, 1, 0, 1, 7},
                 {1, 0, 3, 0, 5}};
        NumMatrix numMatrix(m);
        auto expected1 = 8;
        auto actual1 = numMatrix.sumRegion(2, 1, 4, 3);
        assert(expected1 == actual1);
        numMatrix.update(3, 2, 2);
        auto expected2 = 10;
        auto actual2 = numMatrix.sumRegion(2, 1, 4, 3);
        assert(expected2 == actual2);
    }
    {
       
        Matrix m{{0,-8},{-5,0},{-6,-3},{2,9},{7,-2},{-6,4},{7,-3},{-3,0},{0,-2},{8,-1}};
        NumMatrix numMatrix(m);

        auto actual1 = numMatrix.sumRegion(6,0,9,1);
        actual1 = numMatrix.sumRegion(7,1,7,1);
        actual1 = numMatrix.sumRegion(9,1,9,1);
        numMatrix.update(4,0,-1);
        numMatrix.update(5,0,3);
        actual1 = numMatrix.sumRegion(7,0,9,0);
        numMatrix.update(0,0,-2);
        numMatrix.update(5,1,-1);
        numMatrix.update(8,1,-7);
         actual1 = numMatrix.sumRegion(8,1,8,1);
    }
    return 0;
}
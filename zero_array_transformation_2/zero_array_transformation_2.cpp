#include <map>
#include <vector>
#include <cassert>

struct Operation {
    size_t begin_closed{};
    size_t end_closed{};
    int delta{};

    Operation(int b, int e, int d) : begin_closed{static_cast<size_t>(b)}, end_closed{static_cast<size_t>(e)},
                                     delta{d} {}
};

size_t
get_min_operations_count_to_zero_array(const std::vector<int> &array, const std::vector<std::vector<int>> &operations) {
    size_t k = 0;
    std::vector<int> deltas(array.size() + 2, 0);
    for (size_t a = 0; a < array.size(); a++) {
        auto d = a + 1;
        deltas[d] += deltas[d - 1];
        while (array[a] - deltas[d] > 0 and k < operations.size()) {
            Operation operation{operations[k][0], operations[k][1], operations[k][2]};
            if (operation.end_closed >= a) {
                deltas[std::max(d, operation.begin_closed + 1)] += operation.delta;
                deltas[operation.end_closed + 1 + 1] -= operation.delta;
            }
            k++;
        }
        if (k == operations.size() and array[a] - deltas[d] > 0) {
            return -1;
        }
    }
    return k;
}

int minZeroArray(const std::vector<int> &nums, const std::vector<std::vector<int>> &queries) {
    std::map<size_t, int> non_zeros;
    for (auto i = 0; i < nums.size(); i++) {
        if (nums[i] != 0) {
            non_zeros[i] = nums[i];
        }
    }
    auto k = 0;
    for (; k < queries.size(); k++) {
        if (non_zeros.empty()) {
            return k;
        }
        const auto &q = queries[k];
        for (auto nz_it = non_zeros.begin(); nz_it != non_zeros.end();) {
            if (nz_it->first < q[0]) {
                nz_it++;
            } else if (nz_it->first <= q[1]) {
                nz_it->second -= q[2];
                if (nz_it->second <= 0) {
                    nz_it = non_zeros.erase(nz_it);
                } else {
                    nz_it++;
                }
            } else {
                break;
            }
        }
    }
    if (non_zeros.empty()) {
        return k;
    }

    return -1;


}

int main() {
    assert(2 == get_min_operations_count_to_zero_array({2, 0, 2}, {{0, 2, 1},
                                                                   {0, 2, 1},
                                                                   {1, 1, 3}}));
    assert(-1 == get_min_operations_count_to_zero_array({4, 3, 2, 1}, {{1, 3, 2},
                                                                       {0, 2, 1}}));

    assert(4 == get_min_operations_count_to_zero_array({7,6,8}, {{0,0,2},{0,1,5},{2,2,5},{0,2,4}}));

    assert(2 == minZeroArray({2, 0, 2}, {{0, 2, 1},
                                         {0, 2, 1},
                                         {1, 1, 3}}));
    assert(-1 == minZeroArray({4, 3, 2, 1}, {{1, 3, 2},
                                             {0, 2, 1}}));

    assert(4 == minZeroArray({7,6,8}, {{0,0,2},{0,1,5},{2,2,5},{0,2,4}}));

    return 0;
}

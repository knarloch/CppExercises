#include <string>
#include <vector>
#include <cassert>

using namespace std;

int jumping_stones(const vector<int> stones) {
    if (stones.size() == 0) {
        return 0;
    }
    vector<int> score(stones.size(), 0);
    for (auto current_stone = 0u; current_stone < stones.size(); current_stone++) {
        for (auto jump_target = current_stone + 1; jump_target < stones.size(); jump_target++) {
            int distance = jump_target - current_stone;
            score[jump_target] = std::max(score[jump_target], score[current_stone] + stones[jump_target] * distance);
        }
    }
    return *(score.rbegin());
}

int main() {
    {
        vector<int> stones{};
        int result = jumping_stones(stones);
        assert(result == 0);
    }
    {
        vector<int> stones{333};
        int result = jumping_stones(stones);
        assert(result == 0);
    }
    {
        vector<int> stones{3, 7, 2, 10, 5, 12, 8, 10, 1};
        int result = jumping_stones(stones);
        assert(result == 81);
    }
    return 0;
}
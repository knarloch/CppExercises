#include <cassert>
#include <cstring>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string_view>
#include <iostream>

struct CacheIndex{
    size_t begin{};
    size_t end{};
    size_t k{};
};
bool operator==(const CacheIndex& l, const CacheIndex&r){
    return l.begin == r.begin and l.end  == r.end and l.k == r.k;
}

namespace std{
    template<>
    struct hash<CacheIndex>{
        size_t operator()(const CacheIndex& ci) const{
            static_assert(sizeof(CacheIndex) == sizeof(CacheIndex::begin)+sizeof(CacheIndex::end)+sizeof(CacheIndex::k));
            std::string_view sv{reinterpret_cast<const char*>(&ci), sizeof(ci)};
            return std::hash<string_view>{}(sv);
        }
    };
}

class Solution {
public:
    int splitArray(const std::vector<int> &nums, int k) {
        if (nums.empty()) {
            return 0;
        }
        return splitArray(nums, 0, nums.size(), k);

    }

private:

    int splitArray(const std::vector<int> &nums, size_t begin, size_t end, size_t k) {
        CacheIndex cache_index{begin, end, k};
        auto it = cache.find(cache_index);
        if (it != cache.end()) {
            return it->second;
        }


        int result = 0;
        if (k == 1) {
            result = std::accumulate(nums.begin() + begin, nums.begin() + end, 0);
        } else {
            int best = std::max(splitArray(nums, begin, begin + 1, 1), splitArray(nums, begin + 1, end, k - 1));
            for (int i = 2; end - (begin + i) >= k - 1; i++) {
                int candidate = std::max(splitArray(nums, begin, begin + i, 1),
                                         splitArray(nums, begin + i, end, k - 1));
                best = std::min(candidate, best);
            }
            result = best;

        }
        cache[cache_index] = result;
        return result;
    }
    std::unordered_map<CacheIndex, int> cache;
};

int main() {
    assert(18 == Solution{}.splitArray({7, 2, 5, 10, 8}, 2));
    assert(5 == Solution{}.splitArray({1, 2, 3, 4, 5}, 4));
    assert(5 == Solution{}.splitArray({1, 2, 3, 4, 5}, 5));
    return 0;

}
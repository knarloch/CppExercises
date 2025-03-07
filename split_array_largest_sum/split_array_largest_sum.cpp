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
    int splitArray(std::vector<int> nums, int k) {
        for(auto i = 1; i< nums.size(); i++){
            nums[i] += nums[i-1];
        }
        if (nums.empty()) {
            return 0;
        }
        return splitArray(nums, 0, nums.size(), k);

    }

private:

    int splitArray(const std::vector<int> &partial_sums, size_t begin, size_t end, size_t k) {
        CacheIndex cache_index{begin, end, k};
        auto it = cache.find(cache_index);
        if (it != cache.end()) {
            return it->second;
        }


        int result = 0;
        if (k == 1) {
            result = partial_sums[end -1] - (begin == 0 ? 0 : partial_sums[begin -1]);
        } else {
            int best = std::max(splitArray(partial_sums, begin, begin + 1, 1), splitArray(partial_sums, begin + 1, end, k - 1));
            for (int i = 2; end - (begin + i) >= k - 1; i++) {
                int candidate = std::max(splitArray(partial_sums, begin, begin + i, 1),
                                         splitArray(partial_sums, begin + i, end, k - 1));
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
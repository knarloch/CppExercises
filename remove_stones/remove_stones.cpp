#include <vector>
#include <unordered_map>
#include <string_view>

#include <cassert>

using Stone = std::pair<int, int>;
struct Hash{
    Hash() = default;
    size_t operator()(const Stone& p )const {
        std::string_view sv(reinterpret_cast<const char*>(&p), sizeof (p));
        return std::hash<std::string_view>{}(sv);
    }
};
template <typename T, typename H>
struct UnionSet{
    const T& find(const T& t){
        if(t== map[t]){
            return t;
        }
        else{
            map[t] = find(map[t]);
            return map[t];
        }
    }
    bool join(const T& l, const T&r){
        auto lp = find(l);
        auto rp = find(r);
        if(lp == rp){
            return false;
        }
        if(rank[lp] < rank[rp]){
            std::swap(lp, rp);
        }
        if(rank[lp] == rank[rp]){
            rank[rp]++;
        }
        map[lp] = map[rp];

        return true;
    }

    void insert(const T& t){
        map[t] = t;
        rank[t] = 0;
    }

    std::unordered_map<T,T, H> map;
    std::unordered_map<T, size_t, H> rank;
};

class Solution {
public:

    int removeStones(const std::vector<std::vector<int>>& stones) {
        int result = 0;

        UnionSet<Stone, Hash> union_set;
        for(const auto & s :stones){
            Stone stone{s[0], s[1]};
            union_set.insert(stone);
        }
        for (auto& [element, parent]: union_set.map){
            for (auto& [other_element, other_parent]: union_set.map) {
                if(element == other_element){
                    continue;
                }
                if(element.first == other_element.first or element.second == other_element.second){
                    bool join_took_place = union_set.join(element, other_element);
                    if(join_took_place){
                        result++;
                    }
                }
            }
        }
        return result;
    }
};

int main() {
    assert(2 == Solution{}.removeStones({{0,1},{1,0},{1,1}}));
    assert(5 == Solution{}.removeStones({{0,0},{0,1},{1,0},{1,2},{2,1},{2,2}}));
    assert(4 == Solution{}.removeStones({{3,2},{3,1},{4,4},{1,1},{0,2},{4,0}}));


    // __x
    // _x
    //
    // _xx
    // x___x
    return 0;
}

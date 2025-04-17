#include <cassert>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

vector<int> topologicalSort(const vector<int> &jobs, const vector<vector<int>> &deps) {
    std::unordered_map<int, std::unordered_set<int>> target_to_source;
    std::unordered_multimap<int, int> source_to_target;
    for (const auto &v: deps) {
        source_to_target.insert({v[0], v[1]});
        target_to_source[v[1]].insert(v[0]);
    }
    std::vector<int> ready;
    for (auto j: jobs) {
        if (target_to_source.count(j) == 0) {
            ready.push_back(j);
        }
    }
    vector<int> result;
    while (not ready.empty()) {
        auto j = ready.back();
        ready.pop_back();
        result.push_back(j);
        auto targets_of_j = source_to_target.equal_range(j);
        for (auto it = targets_of_j.first; it != targets_of_j.second; it++) {
            auto target_of_j = it->second;
            target_to_source[target_of_j].erase(j);
            if (target_to_source[target_of_j].empty()) {
                ready.push_back(target_of_j);
            }
        }
    }
    if(result.size() != jobs.size()){
        return {};
    }
    return result;
}

int main() {
    {
        {
            vector<int> jobs{1, 2, 3, 4};
            vector<vector<int>> deps{
                    {1, 2},
                    {1, 3},
                    {3, 2},
                    {4, 2},
                    {4, 3}
            };
            vector<int> expected_a = {1, 4, 3, 2};
            vector<int> expected_b = {4, 1, 3, 2};
            auto actual = topologicalSort(jobs, deps);
            assert((expected_a == actual) or (expected_b == actual));
        }
    }
}
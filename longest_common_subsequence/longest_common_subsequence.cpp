#include <string>
#include <vector>
#include <cassert>
#include <optional>

using namespace std;

struct Helper {
    size_t patent_i{};
    size_t parent_j{};
    optional<char> suffix;
    size_t lcs_length{};
};

string longestCommonSubsequence(const string &str_i, const string &str_j) {
    vector<vector<Helper>> lcs(str_i.size() + 1, vector<Helper>(str_j.size() + 1));

    for (auto i = 1; i <= str_i.size(); i++) {
        for (auto j = 1; j <= str_j.size(); j++) {
            if (str_i[i - 1] == str_j[j - 1]) {
                lcs[i][j].lcs_length = lcs[i - 1][j - 1].lcs_length + 1;
                lcs[i][j].suffix = str_j[j - 1];
                lcs[i][j].patent_i = i - 1;
                lcs[i][j].parent_j = j - 1;
            } else {
                if (lcs[i - 1][j].lcs_length > lcs[i][j - 1].lcs_length) {
                    lcs[i][j].lcs_length = lcs[i - 1][j].lcs_length;
                    lcs[i][j].patent_i = i - 1;
                    lcs[i][j].parent_j = j;
                } else {
                    lcs[i][j].lcs_length = lcs[i][j - 1].lcs_length;
                    lcs[i][j].patent_i = i;
                    lcs[i][j].parent_j = j - 1;
                }
            }
        }
    }
    std::string reversed;
    std::pair<size_t, size_t> pos{str_i.size(), str_j.size()};
    while (true) {
        const auto &helper = lcs[pos.first][pos.second];
        if (helper.suffix.has_value()) {
            reversed.push_back(helper.suffix.value());
        }
        std::pair<size_t, size_t> next{helper.patent_i, helper.parent_j};
        if (pos == next) {
            break;
        } else {
            pos = next;
        }
    }
    return {reversed.crbegin(), reversed.crend()};
}

int main() {
//    {
//        string expected = "aaa";
//        string actual = longestCommonSubsequence("aaa", "aaaa");
//        assert(expected == actual);
//    }
    {
        string expected = "XYZW";
        string actual = longestCommonSubsequence("ZXVVYZW", "XKYKZPW");
        assert(expected == actual);
    }
}
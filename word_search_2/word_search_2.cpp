#include <vector>
#include <cassert>
#include <string>
#include <set>
#include <stack>
#include <unordered_map>
#include <memory>

using namespace std;


struct Trie {
    string prefix{};
    unordered_map<char, shared_ptr<Trie>> next;
};

shared_ptr<Trie> makeTrie(const set<string> &words) {
    auto root = make_shared<Trie>();
    for (const auto &w: words) {
        auto trie = root;
        for (auto c: w) {
            if (trie->next[c] == nullptr) {
                trie->next[c] = make_shared<Trie>(trie->prefix+c);
            }
            trie = trie->next[c];
        }
        trie->next['\0'] = make_shared<Trie>(trie->prefix);
    }
    return root;
}


void backtrack(int X, int Y, int x, int y, vector<vector<bool>> &visited, const vector<vector<char>> &board,
               const shared_ptr<const Trie> parent, set<string> &result) {
    auto it = parent->next.find(board[x][y]);
    if (it != parent->next.end()) {
        const auto &trie = it->second;

        if (trie->next.count('\0')) {
            result.insert(trie->prefix);
        }
        for (auto [cand_x, cand_y] : {std::pair{x - 1, y}, {x+1, y}, {x, y-1}, {x, y+1}}) {
            if (0 <= cand_x and cand_x < X and 0 <= cand_y and cand_y < Y) {
                if (not visited[cand_x][cand_y]) {
                    visited[x][y] = true;
                    backtrack(X, Y, cand_x, cand_y, visited, board, trie, result);
                }
            }
        }
    }
    visited[x][y] = false;
}

set<string> word_search(const vector<vector<char>> &board, const set<string> &words) {
    auto trie = makeTrie(words);

    int X = board.size();
    int Y = board[0].size();

    vector<vector<bool>> visited;
    for (auto x = 0; x < X; x++) {
        visited.push_back(vector<bool>(Y, false));
    }

    set<string> result;
    for (auto x = 0u; x < X; x++) {
        for (auto y = 0u; y < Y; y++) {
            backtrack(X, Y, x, y, visited, board, trie, result);
        }
    }
    return result;
}

vector<string> findWords(const vector<vector<char>> &board, const vector<string> &words) {
    std::set<string> s{words.begin()    , words.end()};
    auto result = word_search(board, s);
    return {result.begin(), result.end()};
}


int main() {
    {
        vector<vector<char>> board{{'a', 'a'}};
        set<string> words = {"aaa"};
        set<string> expected = {};
        set<string> found = word_search(board, words);
        assert(expected == found);

    }
    {
        vector<vector<char>> board{{'o', 'a', 'a', 'n'},
                                   {'e', 't', 'a', 'e'},
                                   {'i', 'h', 'k', 'r'},
                                   {'i', 'f', 'l', 'v'}};
        set<string> words = {"oath", "pea", "eat", "rain"};
        set<string> expected = {"oath", "eat"};
        set<string> found = word_search(board, words);
        assert(expected == found);

    }
    {
        vector<vector<char>> board{{'a', 'b'},
                                   {'c', 'd'}};
        set<string> words = {"abcd"};
        set<string> expected = {};
        set<string> found = word_search(board, words);
        assert(expected == found);

    }

    return 0;
}

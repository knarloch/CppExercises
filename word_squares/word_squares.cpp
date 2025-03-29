#include <vector>
#include <string>
#include <array>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <set>
#include <memory>
#include <unordered_map>

using namespace std;

//ball
//area
//lead
//lady
namespace v1 {
    //1500ms
    bool WordFitsPartialSquare(const vector<string> &candidate, const string &w) {
        for (auto i = 0; i < candidate.size(); i++) {
            if (w[i] != candidate[i][candidate.size()]) {
                return false;
            }
        }
        return true;
    }

    void backtrack(const set<string> &words, vector<string> &partial_square, set<vector<string>> &result) {
        if (partial_square.size() == words.begin()->size()) {
            result.insert(partial_square);
            return;
        }
        for (const auto &w: words) {
            if (WordFitsPartialSquare(partial_square, w)) {
                partial_square.push_back(w);
                backtrack(words, partial_square, result);
                partial_square.pop_back();
            }
        }
    }

    set<vector<string>> WordSquares(const set<string> &words) {
        set<vector<string>> result;
        vector<string> candidate;
        backtrack(words, candidate, result);
        return result;
    }

    vector<vector<string>> wordSquares(const vector<string> &words) {
        auto result = WordSquares(set<string>{words.begin(), words.end()});
        return {result.begin(), result.end()};
    }
}

namespace v2 {
    // 301ms

    void backtrack(const set<string> &words, vector<string> &partial_square, set<vector<string>> &result) {
        if (partial_square.size() == words.begin()->size()) {
            result.insert(partial_square);
            return;
        }
        string prefix;
        for (const auto &s: partial_square) {
            prefix.push_back(s[partial_square.size()]);
        }
        auto w_begin = words.lower_bound(prefix);
        static constexpr char kLastChar = 127;
        auto w_end = words.lower_bound(prefix + kLastChar);
        for (auto it = w_begin; it != w_end; it++) {
            partial_square.push_back(*it);
            backtrack(words, partial_square, result);
            partial_square.pop_back();
        }
    }

    set<vector<string>> WordSquares(const set<string> &words) {
        set<vector<string>> result;
        vector<string> candidate;
        backtrack(words, candidate, result);
        return result;
    }

    vector<vector<string>> wordSquares(const vector<string> &words) {
        auto result = WordSquares(set<string>{words.begin(), words.end()});
        return {result.begin(), result.end()};
    }
}

namespace v3 {
    //371ms

    struct Trie {
        bool isWord;
        array<shared_ptr<Trie>, 'z' - 'a' + 1> child{nullptr};

        const shared_ptr<Trie> &operator[](char c) const {
            return child[c - 'a'];
        }

        shared_ptr<Trie> &operator[](char c) {
            return child[c - 'a'];
        }

        shared_ptr<Trie> add(char c, bool isLastCharInWord) {
            auto &child = operator[](c);
            if (this->operator[](c) == nullptr) {
                child = make_shared<Trie>(isLastCharInWord);
            }
            return child;
        }


    };

    void CollectWordsInTrie(vector<string> &result, string prefix, shared_ptr<const Trie> trie) {
        if (trie->isWord) {
            result.push_back(prefix);
        }
        for (auto i = 0; i < trie->child.size(); i++) {
            if (trie->child[i]) {
                char c = 'a'+i;
                CollectWordsInTrie(result, prefix + c, trie->child[i]);
            }
        }
    }

    shared_ptr<Trie> CreateTrie(const set<string> &words) {
        auto root = make_shared<Trie>(false);
        for (const auto &w: words) {
            auto trie = root;
            for (auto i = 0; i < w.size(); i++) {
                trie = trie->add(w[i], i + 1 == w.size());
            }
        }
        return root;
    }

    void backtrack(int N, shared_ptr<const Trie> root, vector<string> &partial_square, set<vector<string>> &result) {
        if (partial_square.size() == N) {
            result.insert(partial_square);
            return;
        }

        auto trie = root;
        std::string prefix;

        for (const auto &s: partial_square) {
            prefix.push_back(s[partial_square.size()]);
            trie = (*trie)[s[partial_square.size()]];
            if(trie == nullptr){
                return;
            }
        }
        vector<string> words{};
        CollectWordsInTrie(words,prefix, trie);
        for (const auto &w: words) {
            partial_square.push_back(w);
            backtrack(N, root, partial_square, result);
            partial_square.pop_back();
        }
    }

    set<vector<string>> WordSquares(const set<string> &words) {
        auto trie = CreateTrie(words);
        set<vector<string>> result;
        vector<string> candidate;
        backtrack(words.begin()->size(), trie, candidate, result);
        return result;
    }

    vector<vector<string>> wordSquares(const vector<string> &words) {
        auto result = WordSquares(set<string>{words.begin(), words.end()});
        return {result.begin(), result.end()};
    }
}

namespace v4 {
    // 230

    void backtrack(int N, const unordered_map<string, vector<string>> &prefix_to_word, vector<string> &partial_square, set<vector<string>> &result) {
        if (partial_square.size() == N) {
            result.insert(partial_square);
            return;
        }
        string prefix;
        for (const auto &s: partial_square) {
            prefix.push_back(s[partial_square.size()]);
        }
        auto it = prefix_to_word.find(prefix);
        if(it != prefix_to_word.end()) {
            for (const auto &word: it->second) {
                partial_square.push_back(word);
                backtrack(N, prefix_to_word, partial_square, result);
                partial_square.pop_back();
            }
        }
    }

    unordered_map<string, vector<string>> CreatePrefixToWord(const set<string> &words){
        unordered_map<string, vector<string>> result;
        for(const auto& w: words){
            for(auto i = 0; i<=w.size(); i++){
                result[{w.begin(), w.begin()+i}].push_back(w);
            }
        }
        return result;
    }

    set<vector<string>> WordSquares(const set<string> &words) {
        auto prefix_to_word = CreatePrefixToWord(words);
        set<vector<string>> result;
        vector<string> candidate;
        backtrack(words.begin()->size(), prefix_to_word, candidate, result);
        return result;
    }

    vector<vector<string>> wordSquares(const vector<string> &words) {
        auto result = WordSquares(set<string>{words.begin(), words.end()});
        return {result.begin(), result.end()};
    }
}

int main() {
    {
        set<string> words{"area", "lead", "wall", "lady", "ball"};
        //ball
        //area
        //lead
        //lady
        set<vector<string>> expected{{"ball", "area", "lead", "lady"},
                                     {"wall", "area", "lead", "lady"}};
        {
            using namespace v1;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
        {
            using namespace v2;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
        {
            using namespace v3;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
        {
            using namespace v4;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
    }
    {
        set<string> words{"abat", "baba", "atan", "atal"};
        set<vector<string>> expected{{"baba", "abat", "baba", "atal"},
                                     {"baba", "abat", "baba", "atan"}};
        {
            using namespace v1;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
        {
            using namespace v2;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
        {
            using namespace v3;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
        {
            using namespace v4;
            auto actual = WordSquares(words);
            assert(expected == actual);
        }
    }

    return 0;
}

#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>
#include <unordered_map>
#include <set>
#include <memory>
#include <algorithm>
#include <queue>

struct HotTie;

struct Entry{
    char c{};
    std::shared_ptr<HotTie> child{};
    Entry(char c_, std::shared_ptr<HotTie> child_):c{c_}, child{std::move(child_)}{};
};
static bool operator<(const Entry&l, const Entry&r);

struct PartialResult{
    std::string prefix;
    size_t hotness;
    std::shared_ptr<HotTie> position{};
    PartialResult(std::string prefix_, size_t hotness_, std::shared_ptr<HotTie> position_):prefix{std::move(prefix_)}, hotness{hotness_}, position{std::move(position_)}{};
};

static bool operator<(const PartialResult&l, const PartialResult&r){
    if(l.hotness == r.hotness){
        return l.prefix > r.prefix;
    }
    return l.hotness<r.hotness;
}

struct HotTie{

    std::set<Entry> sorted;
    using ItToSorted = decltype(sorted)::iterator;
    std::unordered_map<char, ItToSorted> children;
    size_t hotness{};

    std::shared_ptr<HotTie> insert(char c, size_t count){
        auto c_and_entry = children.find(c);
        if(c_and_entry != children.end()){
            auto entry = *(c_and_entry->second);
            entry.child->hotness+= count;
            sorted.erase(c_and_entry->second);
            children.erase(c_and_entry);
            auto [inserted_pos, _] = sorted.insert(entry);
            auto [new_child, __] = children.insert({c,inserted_pos});
            return new_child->second->child;
        }else{
            Entry entry{c, std::make_shared<HotTie>()};
            entry.child->hotness+=count;
            auto [inserted_pos, _] =sorted.insert(entry);
            auto [new_child, __] = children.insert({c, inserted_pos});
            return new_child->second->child;
        }
    }

    std::vector<Entry> top_n(size_t n)const{
        std::vector<Entry> result;
        auto last = sorted.rbegin();
        for(auto i=0; i<n and last != sorted.rend(); i++, last++){
            result.push_back(*last);
        }
        return result;
    }

};

bool operator<(const Entry&l, const Entry&r){
    if(l.child->hotness == r.child->hotness) {
        return l.c > r.c;
    }
    return l.child->hotness < r.child->hotness;
}

class AutocompleteSystem {
public:
    AutocompleteSystem(const std::vector<std::string>& sentences, const std::vector<int>& times){
        for(auto i = 0; i<sentences.size(); i++){
            insert(sentences[i]+'\0', times[i]);
        }
    }

    std::vector<std::string> input(char c){
        if(c == '#'){
            c='\0';
            prefix+=c;
            insert(prefix,1);
            current_pos = root;
            prefix = "";
            return {};
        }
        else if(current_pos == nullptr){
            prefix+=c;
            return {};
        }
        else{
            auto hot_tie_it = current_pos->children.find(c);
            if(hot_tie_it == current_pos->children.end()){
                current_pos = nullptr;
                prefix += c;
                return {};
            }
            else{
                prefix+=c;
                current_pos = hot_tie_it->second->child;
                std::vector<std::string> result;
                std::priority_queue<PartialResult, std::vector<PartialResult>> pq;
                push_top_n_to_pq(pq, current_pos, prefix, 3);
                while(not pq.empty() and result.size() < 3){
                    auto pr = pq.top();
                    pq.pop();
                    if(pr.prefix.back() == '\0'){
                        pr.prefix.pop_back();
                        result.push_back(std::move(pr.prefix));
                    }
                    else{
                        push_top_n_to_pq(pq, pr.position, pr.prefix, 3);
                    }
                }
                return result;
            }
        }
    };

private:
    std::shared_ptr<HotTie> root = std::make_shared<HotTie>();
    std::shared_ptr<HotTie> current_pos = root;
    std::string prefix{};

    void push_top_n_to_pq(std::priority_queue<PartialResult, std::vector<PartialResult>>& pq, std::shared_ptr<HotTie> pos, const std::string& prefix, size_t n){
        for(auto top: pos->top_n(n)){
            PartialResult pr{prefix+top.c, top.child->hotness, top.child};
            pq.push(std::move(pr));
        }
    }

    void insert(const std::string& s, size_t count){
        auto pos = root;
        root->hotness+=count;
        for(auto c: s){
            pos = pos->insert(c, count);
        }
    }
};

int main() {
    {
        AutocompleteSystem acs{{"i love you", "island", "iroman", "i love leetcode"}, {5, 3, 2, 2}};
        {
            auto hints = acs.input('i');
            assert(hints[0] == "i love you");
            assert(hints[1] == "island");
            assert(hints[2] == "i love leetcode");
        }
        {
            auto hints = acs.input(' ');
            assert(hints[0] == "i love you");
            assert(hints[1] == "i love leetcode");
        }
        {
            auto hints = acs.input('a');
            assert(hints.empty());
        }
        {
            auto hints = acs.input('#');
            assert(hints.empty());
        }
        {
            auto hints = acs.input('i');
            hints = acs.input(' ');
            hints = acs.input('a');
            assert(hints[0]=="i a");
            assert(hints.size()==1);
        }
    }
    AutocompleteSystem acs{{"i love you", "island", "iroman", "i love leetcode"}, {5, 3, 2, 2}};
    {
        auto hints = acs.input('i');
        assert(hints == (std::vector<std::string>{"i love you", "island", "i love leetcode"}));
        hints = acs.input(' ');
        assert(hints == (std::vector<std::string>{"i love you", "i love leetcode"}));
        hints = acs.input('a');
        assert(hints == (std::vector<std::string>{}));
        hints = acs.input('#');
        assert(hints == (std::vector<std::string>{}));

        hints = acs.input('i');
        assert(hints == (std::vector<std::string>{"i love you", "island", "i love leetcode"}));
        hints = acs.input(' ');
        assert(hints == (std::vector<std::string>{"i love you", "i love leetcode", "i a"}));
        hints = acs.input('a');
        assert(hints == (std::vector<std::string>{"i a"}));
        hints = acs.input('#');
        assert(hints == (std::vector<std::string>{}));

        hints = acs.input('i');
        assert(hints == (std::vector<std::string>{"i love you", "island", "i a"}));

        hints = acs.input(' ');
        assert(hints == (std::vector<std::string>{"i love you", "i a", "i love leetcode"}));

        hints = acs.input('a');
        assert(hints[0] == "i a");

        hints = acs.input('#');
        assert(hints.empty());
    }

    // ,["i love you","island","i love leetcode"]
    // ,["i love you","i love leetcode"]
    // ,[]
    // ,[]
    // ,["i love you","island","i love leetcode"]
    // ,["i love you","i love leetcode","i a"]
    // ,["i a"]
    // ,[]
    // ,["i love you","island","i a"]
    // ,["i love you","i a","i love leetcode"],
    // ["i a"]
    // ,[]]

    return 0;
}

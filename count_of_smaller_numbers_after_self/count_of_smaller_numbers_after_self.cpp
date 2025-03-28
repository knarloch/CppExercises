#include <cassert>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Segment {
    Segment() = default;

    explicit Segment(size_t begin, size_t end, unordered_map<int, size_t> val) : begin{begin}, end{end}, val_to_count{std::move(val)} {};
    size_t begin;
    size_t end;
    unordered_map<int, size_t> val_to_count;
};

Segment merge(const Segment &l, const Segment &r) {
    Segment result{};
    result.begin = l.begin;
    result.end = r.end;
    result.val_to_count = l.val_to_count;
    for(const auto&[ val, count]: r.val_to_count){
        result.val_to_count[val] += count;
    }
    return result;
}

size_t get_upper_bound_as_twos_exponent(size_t x) {
    size_t result = 0;
    while (x > (1 << result)) {
        result++;
    }
    return result;
}

size_t get_left_child_index(size_t parent_index) {
    return 2 * parent_index + 1;
}

vector<Segment> create_segment_tree(const vector<int> &data) {
    size_t k = get_upper_bound_as_twos_exponent(data.size());
    size_t leaf_count = 1 << k;
    size_t segment_tree_size = leaf_count - 1 + leaf_count;
    size_t first_leaf = segment_tree_size - leaf_count;
    int last_nonleaf = first_leaf - 1;


    vector<Segment> tree(segment_tree_size);
    for (auto i = 0u; i < data.size(); i++) {
        tree[i + first_leaf] = Segment{i, i+1, {{data[i],1}}};
    }
    for (auto i = data.size(); i < leaf_count; i++) {
        tree[i + first_leaf] = Segment(i, i+1, {});
    }
    for (int i = last_nonleaf; i >= 0; i--) {
        tree[i] = merge(tree[get_left_child_index(i)], tree[get_left_child_index(i) + 1]);
    }

    return tree;
}

Segment query_segment(const vector<Segment> &tree, size_t index, size_t query_begin, size_t query_end) {
    if (query_begin <= tree[index].begin and tree[index].end <= query_end) {
        return tree[index];
    }
    if(query_end <= tree[index].begin or tree[index].end<=query_begin){
        return Segment{std::max(tree[index].begin, query_begin), std::min(tree[index].end, query_end), {}};
    }
    return merge(query_segment(tree, get_left_child_index(index), query_begin, query_end),
                 query_segment(tree, get_left_child_index(index) + 1, query_begin, query_end));

}

Segment query_segment(const vector<Segment> &tree, size_t query_begin, size_t query_end) {
    return query_segment(tree, 0, query_begin, query_end);
}

vector<int> count_of_smaller_numbers_after_self(const vector<int>& data){
    auto tree = create_segment_tree(data);
    vector<int> result(data.size(), 0);
    for(auto i = 0u; i<result.size();i++){
        for(auto [val, count]: query_segment(tree, i, data.size()).val_to_count){
            if(val< data[i]){
                result[i]+=count;
            }
        }
    }
    return result;
}
int main() {
    {
        auto tree = create_segment_tree({5, 2, 6, 1});

//        {
//            auto answer = query_segment(tree, 0,4);
//            assert(answer.val_to_count == (std::vector<int>{6, 5, 2, 1}));
//        }
//        {
//            auto answer = query_segment(tree, 0,0);
//            assert(answer.val_to_count == (std::vector<int>{}));
//        }
//        {
//            auto answer = query_segment(tree, 0,1);
//            assert(answer.val_to_count == (std::vector<int>{5}));
//        }
//        {
//            auto answer = query_segment(tree, 1,2);
//            assert(answer.val_to_count == (std::vector<int>{2}));
//        }
//        {
//            auto answer = query_segment(tree, 2,3);
//            assert(answer.val_to_count == (std::vector<int>{6}));
//        }
//        {
//            auto answer = query_segment(tree, 3,4);
//            assert(answer.val_to_count == (std::vector<int>{1}));
//        }
//        {
//            auto answer = query_segment(tree, 0,0);
//            assert(answer.val_to_count == (std::vector<int>{}));
//        }
//        {
//            auto answer = query_segment(tree, 1,3);
//            assert(answer.val_to_count == (std::vector<int>{6, 2}));
//        }
//        {
//            auto answer = query_segment(tree, 0,3);
//            assert(answer.val_to_count == (std::vector<int>{6, 5, 2}));
//        }
        {
            auto result = count_of_smaller_numbers_after_self({5, 2, 6, 1});
            assert(result == (std::vector{2, 1, 1, 0}));
        }
    }
    {
        {
            auto result = count_of_smaller_numbers_after_self({0,1,2});
            assert(result == (std::vector{0,0,0}));
        }
    }
    return 0;
}
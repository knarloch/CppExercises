#include <vector>
#include <cassert>
#include <algorithm>
#include <random>

int lower_bound_of_median(const std::vector<int>& v1, size_t b1, size_t e1){
    return v1[(b1+ (e1-b1 - 1)/2)];
}

int find_kth(size_t k, const std::vector<int>& v1, const std::vector<int>& v2){
    size_t b1 = 0;
    size_t e1 = v1.size();
    size_t b2 = 0;
    size_t e2 = v2.size();
    while(e1-b1 > 0 and e2-b2 >0){
        if(k < (e1-b1 + e2-b2)/2){
            if(lower_bound_of_median(v1, b1,e1) > lower_bound_of_median(v2, b2, e2)) {
                e1 -= (e1 - b1 + 1) / 2;
            }
            else {
                e2 -= (e2 - b2 + 1) / 2;
            }
        }
        else{
            if(lower_bound_of_median(v1, b1,e1) > lower_bound_of_median(v2, b2, e2)) {
                auto dropped_count = (e2 - b2 + 1) / 2;
                b2 += dropped_count;
                k -= dropped_count;

            }else {
                auto dropped_count = (e1 - b1 + 1) / 2;
                b1 += dropped_count;
                k -= dropped_count;
            }
        }
    }
    if(e1-b1>0){
        return v1[b1+k];
    }
    return v2[b2+k];
}

double median_of_two_sorted_arrays(const std::vector<int>& v1, const std::vector<int>& v2){
    auto merged_size = v1.size() + v2.size();
    if(merged_size%2 == 1){
        return find_kth(merged_size/2, v1, v2);
    }
    return (find_kth((merged_size-1)/2, v1, v2) + find_kth(merged_size/2, v1, v2))/2.0;
}


double median_of_two_sorted_arrays_basic(const std::vector<int>& v1, const std::vector<int>& v2){
    std::vector<int> merged(v1.size() + v2.size());
    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), merged.begin());
    if(merged.size()%2 == 1){
        return merged[merged.size()/2];
    }
    return (merged[(merged.size()-1)/2] +merged[merged.size()/2])/2.0;
}

int main(){
    assert(1 == median_of_two_sorted_arrays({0, 2}, {1}));
    assert(1 == median_of_two_sorted_arrays({1}, {0, 2}));
    assert(2.5 == median_of_two_sorted_arrays({1,2}, {3, 4}));
    assert(2.5 == median_of_two_sorted_arrays({3, 4}, {1,2}));
    assert(2.5 == median_of_two_sorted_arrays({1, 4}, {2,3}));
    assert(1 == median_of_two_sorted_arrays_basic({0, 2}, {1}));
    assert(1 == median_of_two_sorted_arrays_basic({1}, {0, 2}));
    assert(2.5 == median_of_two_sorted_arrays_basic({1,2}, {3, 4}));
    assert(2.5 == median_of_two_sorted_arrays_basic({3, 4}, {1,2}));
    assert(2.5 == median_of_two_sorted_arrays_basic({1, 4}, {2,3}));

    std::default_random_engine engine(0);
    std::uniform_int_distribution<int> uniform_dist(0, 8192);
    for(auto i = 0; i < 512; i++){
        auto v1_size =uniform_dist(engine);
        std::vector<int> v1(v1_size);
        for(auto& v: v1){
            v = uniform_dist(engine);
        }
        std::sort(v1.begin(), v1.end());
        auto v2_size =uniform_dist(engine);
        std::vector<int> v2(v2_size);
        for(auto& v: v2){
            v = uniform_dist(engine);
        }
        std::sort(v2.begin(), v2.end());
        assert(median_of_two_sorted_arrays(v1, v2) == median_of_two_sorted_arrays_basic(v1, v2));
    }

    return 0;
}
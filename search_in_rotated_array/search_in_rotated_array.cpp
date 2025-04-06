#include <algorithm>
#include <cassert>
#include <vector>

int search_in_rotated_array(int target, const std::vector<int>& array){
    if(array.empty()){
        return -1;
    }

    auto pp = std::partition_point(array.begin(), array.end(), [first_partition_min = array[0]](int value){
        return value>=first_partition_min;
    });

    auto candidate1 = std::lower_bound(array.begin(), pp, target);
    if(candidate1 < pp and *candidate1 == target){
        return candidate1-array.begin();
    }
    auto candidate2 = std::lower_bound(pp, array.end(), target);
    if(candidate2 < array.end() and *candidate2 == target){
        return candidate2-array.begin();
    }
    return -1;
}



int main(){
    {
        assert(-1 == search_in_rotated_array(2345, {}));
    }
    {
        assert(0 == search_in_rotated_array(1, {1}));
        assert(0 == search_in_rotated_array(1, {1, 2}));
        assert(1 == search_in_rotated_array(1, {2, 1}));
    }
    {
        std::vector<int> rotated{4,5,6,7,8,9,1,2,3};
        for(auto i =0; i<rotated.size(); i++){
            auto result = search_in_rotated_array(rotated[i], rotated);
            assert(i == result);
        }
        assert(-1 == search_in_rotated_array(2345, rotated));

    }
    return 0;
}

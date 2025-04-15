#include <vector>
#include <cassert>

using namespace std;

void mergeSort(vector<int>& out, vector<int>& in, int begin, int end) {
    // Write your code here.
    if(end - begin <= 1){
        return ;
    }
    auto pivot = (end + begin) /2;

    mergeSort(in, out, begin, pivot);
    mergeSort(in, out, pivot, end);

    auto l =in.begin()+begin;
    auto r = in.begin()+pivot;
    auto cursor = out.begin()+begin;

    while(l != in.begin()+pivot and r != in.begin()+end){
        if (*l< *r){
            *cursor = *l;
            cursor++;
            l++;
        }
        else{
            *cursor = *r;
            cursor++;
            r++;
        }
    }
    while(l != in.begin()+pivot){
        *cursor = *l;
        cursor++;
        l++;
    }
    while(r != in.end()){
        *cursor = *r;
        cursor++;
        r++;
    }
}

vector<int> mergeSort(vector<int> array) {
    vector<int> result = array;
    mergeSort(result, array, 0, array.size());
    return result;
}

int main(){
    {
        vector<int> unsorted{8, 5, 2, 9, 5, 6, 3};
        vector<int> sorted{2, 3, 5, 5, 6, 8, 9};
        auto result = mergeSort(unsorted);
        assert(sorted == result);
    }
}
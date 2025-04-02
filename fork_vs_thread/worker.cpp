#include <chrono>
#include <cmath>
#include "worker.hpp"

void do_some_work(){

    int64_t x = (std::chrono::steady_clock::now() - std::chrono::steady_clock::time_point{}).count();
    x= x*x;
    x = x/2;
    auto y = std::pow(x,0.45);
    if(y<0.0){
        std::terminate();
    }
}
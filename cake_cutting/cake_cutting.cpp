#include <vector>
#include <set>
#include <tuple>
#include <cassert>

using namespace std;

struct Cake {
    Cake(double xx, double yy, double sideside) : x{xx}, y{yy}, side{sideside} {}

    double x{};
    double y{};
    double side{};
};

struct Interval {
    explicit Interval(const Cake &cake) {
        x_min = cake.x;
        x_max = cake.x + cake.side;
        a = cake.side;
    }

    Interval(double x_min_, double x_max_, double a_) : x_min{x_min_}, x_max{x_max_}, a{a_} {}

    double x_min{};
    double x_max{};
    double a{};

};

bool operator<(const Interval &l, const Interval &r) {
    if (l.x_min == r.x_min) {
        return l.x_max < r.x_max;
    }
    return l.x_min < r.x_min;
}

std::tuple<Interval, Interval, Interval> OverlapIntervals(const Interval &l, const Interval &r) {
    Interval leftmost(l.x_min, r.x_min, l.a);
    Interval middle(r.x_min, std::min(l.x_max, r.x_max), l.a + r.a);
    Interval rightmost(std::min(l.x_max, r.x_max), std::max(l.x_max, r.x_max), l.x_max < r.x_max ? r.a : l.a);
    return {leftmost, middle, rightmost};
}

double TotalCake(const std::vector<Cake> &data) {
    double result = 0;
    for (const auto &cake: data) {
        result += cake.side * cake.side;
    }
    return result;
}


double FindCuttingPoint(const std::vector<Cake> &data) {
    double target = TotalCake(data) / 2;
    std::set<Interval> intervals;

    for (const auto &cake: data) {
        if (cake.side == 0) {
            continue;
        }
        Interval dummy{cake};
        auto it = intervals.lower_bound(dummy);
        while (it != intervals.end() and dummy.x_min < it->x_max) {
            auto extracted = *it;
            auto [l, m, r] = OverlapIntervals(extracted, dummy);
            intervals.erase(it);
            if(l.x_max-l.x_min>0){
                intervals.insert(l);
            }
            if(m.x_max-m.x_min>0){
                intervals.insert(m);
            }

            auto [inserted_it, _] = intervals.insert(m);
            dummy = r;
            it = inserted_it++;
        }
        if(dummy.x_max-dummy.x_min>0){
            intervals.insert(dummy);
        }
    }

    double cake_so_far = 0;
    for (const auto &interval: intervals) {
        if (target <= cake_so_far + interval.a * (interval.x_max - interval.x_min)) {
            return (target - cake_so_far) / interval.a + interval.x_min;
        }
        cake_so_far += +interval.a * (interval.x_max - interval.x_min);
    }
    return -1.0;
}

int main() {
    {
        std::vector<Cake> data = {Cake(1.0, 1.0, 2.0), Cake(4.0, 1.0, 2.0)};
        assert(3.0 == FindCuttingPoint(data));
    }
    {
        std::vector<Cake> data = {Cake(0.0, 1.0, 10.0)};
        assert(5.0 == FindCuttingPoint(data));
    }
    {
        std::vector<Cake> data = {Cake(0.0, 1.0, 10.0), Cake(0.0, 1.0, 1.0), Cake(12, 2.0, 1.0)};
        assert(5.0 == FindCuttingPoint(data));
    }
    {
        std::vector<Cake> data = {Cake(0.0, 1.0, 4), Cake(3.0, 1.0, 1.0)};
        assert(2.125 == FindCuttingPoint(data));
    }



    return 0;
}

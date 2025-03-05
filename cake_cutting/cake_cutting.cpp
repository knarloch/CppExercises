#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

struct Cake {
    Cake(double xx, double yy, double sideside) : x{xx}, y{yy}, side{sideside} {}

    double x{};
    double y{};
    double side{};
};

struct IntervalBorder {
    double y0{};
    double a{};
    double offset{};

    double cake_ammount(double y) const {
        return a * (y - y0) + offset;
    }
};

bool operator<(const IntervalBorder &l, const IntervalBorder &r) {
    return l.y0 < r.y0;
}

double FindCuttingY(const std::vector<Cake> cakes) {
    double target = 0.0;
    std::vector<IntervalBorder> ibs;
    ibs.reserve(cakes.size() * 2);
    for (const auto &cake: cakes) {
        ibs.push_back(IntervalBorder{.y0 = cake.y, .a = cake.side});
        ibs.push_back(IntervalBorder{.y0 = cake.y + cake.side, .a = -1.0 * cake.side});
        target += cake.side * cake.side * 0.5;
    }
    std::sort(ibs.begin(), ibs.end());

    // do a rolling sum of interval borders
    // in other words treat ibs as derivatives
    // and integrate them back to cake_amount(y) function.
    for (auto i = 1u; i < ibs.size(); i++) {
        ibs[i].a += ibs[i - 1].a;
        ibs[i].offset = ibs[i - 1].cake_ammount(ibs[i].y0);
        if (ibs[i].cake_ammount(ibs[i].y0) >= target) {
            const auto &target_interval = ibs[i - 1];
            // target == f(y) == a(y-y0) + offset;
            return (target - target_interval.offset) / target_interval.a + target_interval.y0;
        }
    }
    return -1.0;
}

int main() {
    {
        std::vector<Cake> data = {Cake(1.0, 0.0, 2.0), Cake(4.0, 1.0, 2.0)};
        assert(1.5 == FindCuttingY(data));
    }
    {
        std::vector<Cake> data = {Cake(1.0, 0.0, 10.0)};
        assert(5.0 == FindCuttingY(data));
    }
    {
        std::vector<Cake> data = {Cake(1.0, 1.0, 10.0)};
        assert(6.0 == FindCuttingY(data));
    }
    {
        std::vector<Cake> data = {Cake(1.0, 0.0, 10.0), Cake(21.0, 0.0, 1.0), Cake(22.0, 9.0, 1.0)};
        assert(5.0 == FindCuttingY(data));
    }
    {
        std::vector<Cake> data = {Cake(1.0, 0.0, 10.0), Cake(21.0, 0.0, 1.0), Cake(22.0, 19.0, 1.0)};
        assert(5.0 == FindCuttingY(data));
    }
    return 0;
}

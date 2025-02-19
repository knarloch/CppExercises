#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>


using GateName = std::string;
using LicensePlate = std::string;

struct LogEntry {
    time_t timestamp{};
    LicensePlate license_plate;
    GateName gate_name;
};

using Cents = int;

using PriceList = std::unordered_map<GateName, std::unordered_map<GateName, Cents>>;

std::unordered_map<LicensePlate, Cents>
GetTollPerLicensePlate(const std::vector<LogEntry> &log, const PriceList &price_list) {
    struct TimestampAndGateName {
        decltype(LogEntry::timestamp) timestamp;
        decltype(LogEntry::gate_name) gate_name;
    };
    std::unordered_map<LicensePlate, std::vector<TimestampAndGateName>> sorted_events;
    for (const auto &event: log) {
        sorted_events[event.license_plate].push_back({event.timestamp, event.gate_name});
    }
    std::unordered_map<LicensePlate, Cents> result;
    for (auto &[license_plate, events]: sorted_events) {
        std::sort(events.begin(), events.end(), [](const TimestampAndGateName &l, const TimestampAndGateName &r) {
            return l.timestamp < r.timestamp;
        });
        Cents ammount = 0;
        for (auto it = events.begin(); it + 1 < events.end(); it += 2) {
            const auto &entry = *it;
            const auto &exit = *(it + 1);
            ammount += price_list.at(entry.gate_name).at(exit.gate_name);
        }
        result[license_plate] = ammount;
    }
    return result;
}

int main() {

    PriceList price_list{{"A", {{"A", 0}, {"B", 1}, {"C", 3}}},
                         {"B", {{"A", 1}, {"B", 0}, {"C", 2}}},
                         {"C", {{"A", 3}, {"B", 2}, {"C", 0}}}};

    std::vector<LogEntry> events{{1,  "xxx", "A"},
                                 {41, "aaa", "B"},
                                 {1,  "aaa", "C"},
                                 {32, "xxx", "C"},
                                 {34, "xxx", "B"}};

    auto result = GetTollPerLicensePlate(events, price_list);
    assert(result["xxx"] == 3);
    assert(result["aaa"] == 2);

    return 0;
}
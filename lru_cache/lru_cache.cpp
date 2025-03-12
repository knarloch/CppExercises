#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <optional>
#include <list>

class BadLruCache {
public:
    using Timestamp = int64_t;
    using Value = int;
    using Key = int;
    struct Entry {
        Value value;
        Timestamp timestamp;
    };

    BadLruCache(int capacity) : capacity_{capacity} {

    }

    std::optional<int> get(Key key) {
        auto it = entries_.find(key);
        if (it == entries_.end()) {
            return {};
        }
        clock_++;
        access_timestamps_.erase(it->second.timestamp);
        it->second.timestamp = clock_;
        access_timestamps_[clock_] = key;
        return it->second.value;
    }

    void put(Key key, Value value) {
        clock_++;
        auto &entry = entries_[key];
        access_timestamps_.erase(entry.timestamp);
        entry = Entry{.value = value, .timestamp = clock_};
        access_timestamps_[clock_] = key;

        while (entries_.size() > capacity_) {
            auto lru_it = access_timestamps_.find(lru_timestamp++);
            if (lru_it != access_timestamps_.end()) {
                entries_.erase(lru_it->second);
                access_timestamps_.erase(lru_it);
            }
        }
    }


    std::unordered_map<Key, Entry> entries_;
    std::unordered_map<Timestamp, Key> access_timestamps_;
    Timestamp clock_ = 0;
    Timestamp lru_timestamp = 0;
    int64_t capacity_;
};

class LruCache {
public:
    using Value = int;
    using Key = int;

    LruCache(int capacity) : capacity_{capacity} {

    }

    std::optional<int> get(Key key) {
        auto it = index_.find(key);
        if (it == index_.end()) {
            return {};
        }
        entries_.push_back(*(it->second));
        entries_.erase(it->second);
        it->second = --entries_.end();
        return it->second->second;
    }

    void put(Key key, Value value) {
        auto it = index_.find(key);
        if (it == index_.end()) {
            entries_.push_back({key, value});
            index_.insert({key, (--entries_.end())});
            assert(index_.size()>0);
            while(entries_.size() > capacity_){
                index_.erase(entries_.begin()->first);
                entries_.erase(entries_.begin());
            }
        }
        else {
            entries_.push_back({key, value});
            entries_.erase(it->second);
            it->second = entries_.end()--;
        }
    }


    using Entries = std::list<std::pair<Key, Value>>;
    Entries entries_;
    std::unordered_map<Key, Entries::iterator> index_;
    int64_t capacity_;
};


int main() {
    
    BadLruCache badLruCache{2};
    badLruCache.put(1, 1); // cache is {1=1}
    badLruCache.put(2, 2); // cache is {1=1, 2=2}
    assert(badLruCache.get(1) == 1);    // return 1
    badLruCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
    assert(badLruCache.get(2) == std::nullopt);    // returns -1 (not found)
    badLruCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
    assert(badLruCache.get(1) == std::nullopt);    // return -1 (not found)
    assert(badLruCache.get(3) == 3);    // return 3
    assert(badLruCache.get(4)==4);    // return 4

    LruCache lruCache{2};
    lruCache.put(1, 1); // cache is {1=1}
    lruCache.put(2, 2); // cache is {1=1, 2=2}
    assert(lruCache.get(1) == 1);    // return 1
    lruCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
    assert(lruCache.get(2) == std::nullopt);    // returns -1 (not found)
    lruCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
    assert(lruCache.get(1) == std::nullopt);    // return -1 (not found)
    assert(lruCache.get(3) == 3);    // return 3
    assert(lruCache.get(4)==4);    // return 4

    return 0;
}
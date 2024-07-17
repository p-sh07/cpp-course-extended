//01.06.24
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>

#include "log_duration.h"

using namespace std;

class RandomContainer {
public:
    RandomContainer() {
        values_pool_.reserve(1000000);
    }
    
    void Insert(int val) {
        values_pool_.push_back(val);
        values_index_[val] = values_pool_.size() - 1;
    }
    void Remove(int val) {
        size_t val_index = values_index_.at(val);
        std::swap(values_pool_[val_index], values_pool_.back());
        
        values_pool_.pop_back();
        values_index_.erase(val);
    }
    bool Has(int val) const {
        return values_index_.count(val) > 0;
    }
    int GetRand() const {
        uniform_int_distribution<int> distr(0, values_pool_.size() - 1);
        int rand_index = distr(engine_);
        return values_pool_[rand_index];
    }

private:
    vector<int> values_pool_;
    unordered_map<int, size_t> values_index_;
    mutable mt19937 engine_;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    RandomContainer container;
    int query_num = 0;
    cin >> query_num;
    {
        LOG_DURATION("Requests handling"s);
        for (int query_id = 0; query_id < query_num; query_id++) {
            string query_type;
            cin >> query_type;
            if (query_type == "INSERT"s) {
                int value = 0;
                cin >> value;
                container.Insert(value);
            } else if (query_type == "REMOVE"s) {
                int value = 0;
                cin >> value;
                container.Remove(value);
            } else if (query_type == "HAS"s) {
                int value = 0;
                cin >> value;
                if (container.Has(value)) {
                    cout << "true"s << endl;
                } else {
                    cout << "false"s << endl;
                }
            } else if (query_type == "RAND"s) {
                cout << container.GetRand() << endl;
            }
        }
    }
    return 0;
}

/*
5
INSERT 3
INSERT 5
INSERT 1
INSERT 2
INSERT 4

 */

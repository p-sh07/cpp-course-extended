#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class RouteManager {
public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].push_back(finish);
        reachable_lists_[finish].push_back(start);
    }
    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        if (reachable_lists_.count(start) < 1) {
            return result;
        }
        const vector<int>& reachable_stations = reachable_lists_.at(start);
        if (!reachable_stations.empty()) {
            result = min(result, abs(finish -
              *min_element( reachable_stations.begin(),
                            reachable_stations.end(),
                            [finish](int lhs, int rhs) {
                               return abs(lhs - finish) < abs(rhs - finish);
            })));
        }
        return result;
    }
    
private:
    unordered_map<int, vector<int>> reachable_lists_;
};

int main() {
    RouteManager routes;
    
    int query_count;
    cin >> query_count;
    
    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD"s) {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO"s) {
            cout << routes.FindNearestFinish(start, finish) << "\n"s;
        }
    }
}

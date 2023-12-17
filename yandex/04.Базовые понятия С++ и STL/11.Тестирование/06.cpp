//
//  04.Базовые понятия С++ и STL
//  Тема 11.Тестирование
//  Задача 06.Декомпозиция и отладка кода
//
//  Created by Pavel Sh on 03.12.2023.
//
#include <algorithm>
#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string operation_code;
    is >> operation_code;

    if (operation_code == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        q.stops.resize(stop_count);
        for (string& stop : q.stops) {
            is >> stop;
        }
    } else if (operation_code == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (operation_code == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (operation_code == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    string stop;
    vector<string> buses_for_stop;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.buses_for_stop.empty()) {
        os << "No stop"s;
    } else {
        for (const string& bus : r.buses_for_stop) {
            os << bus << " ";
        }
    }
    return os;
}

struct StopsForBusResponse {
    string bus;
    bool any_found;
    vector<pair<string, vector<string>>> stops_for_bus;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (!r.any_found) {
        os << "No bus"s;
        return os;
    }
    for (const auto& stop_and_buses : r.stops_for_bus) {
        os << "Stop "s << stop_and_buses.first << ": ";
        if (stop_and_buses.second.empty()) {
            os << "no interchange";
        } else {
            for (const auto& bus : stop_and_buses.second) {
                if (bus != r.bus) {
                    os << bus << " ";
                }
            }
            if (stop_and_buses.second.size() == 1 && stop_and_buses.second.front() == r.bus) {
                os << "no interchange";
            }
        }
        os << endl;
    }
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> buses_to_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.buses_to_stops.empty()) {
        os << "No buses"s;
        return os;
    }
    for (const auto& bus_and_stops : r.buses_to_stops) {
        os << "Bus "s << bus_and_stops.first << ": ";
        for (const string& stop : bus_and_stops.second) {
            os << stop << " ";
        }
        os << endl;
    }
    return os;
}

class BusManager {
private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;

public:
    // Добавление нового автобуса и его маршрутов
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops[bus] = stops;
        for (const string& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
    }

    // Получение списка автобусов для остановки
    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse response;
        if (stops_to_buses.count(stop) > 0) {
            response.buses_for_stop = stops_to_buses.at(stop);
        }
        return response;
    }

    // Получение списка остановок для автобуса
    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse response;
        if (buses_to_stops.count(bus) > 0) {
            response.any_found = true;
            for (const auto& stop : buses_to_stops.at(bus)) {
                vector<string> buses_here;
                for (const auto& other_bus : stops_to_buses.at(stop)) {
                    if (bus != other_bus) {
                        buses_here.push_back(other_bus);
                    }
                }
                response.stops_for_bus.push_back(make_pair(stop, buses_here));
            }
        } else {
            response.any_found = false;
        }
        return response;
    }

    // Получение полного списка автобусов и их маршрутов
    AllBusesResponse GetAllBuses() const {
        return AllBusesResponse{buses_to_stops};
    }
    
    size_t GetBusesSize() {
        return buses_to_stops.size();
    }
    size_t GetStopsSize() {
        return stops_to_buses.size();
    }
    //returns True when Query is processed successfully
    bool ProcessQuery(const string& query_string) {
        stringstream in(query_string), out;
        Query q;
        in >> q;
        switch (q.type) {
            case QueryType::NewBus:
                AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                out << GetBusesForStop(q.stop);
                break;
            case QueryType::StopsForBus:
                out << GetStopsForBus(q.bus);
                break;
            case QueryType::AllBuses:
                out << GetAllBuses();
                break;
        }

        return q.type == QueryType::NewBus || !out.str().empty();
    }
    string GetQueryOutput(const string& query_string) {
        stringstream in(query_string), out;
        Query q;
        in >> q;
        switch (q.type) {
            case QueryType::NewBus:
                AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                out << GetBusesForStop(q.stop);
                break;
            case QueryType::StopsForBus:
                out << GetStopsForBus(q.bus);
                break;
            case QueryType::AllBuses:
                out << GetAllBuses();
                break;
        }
//        cerr << "TESTING: [" << QueryTypeToStr(q.type) << "]\n";
//        cerr << out.str() << endl;
        return out.str();
    }
};



//---------- UNIT TESTS for class: --------------------//
void TestBusInput() {
    BusManager bm;
    assert(bm.ProcessQuery("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s));
    assert(bm.ProcessQuery("NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo"s));
    cerr << bm.GetBusesSize() << endl;
    assert(bm.GetBusesSize() == 2);
    assert(bm.GetStopsSize() == 6);
}
void TestEmptyResponses() {
    BusManager bm;
    assert(bm.ProcessQuery("ALL_BUSES"s));
    assert(bm.ProcessQuery("BUSES_FOR_STOP Marushkino"s));
    assert(bm.ProcessQuery("BUSES_FOR_STOP Marushkino"s));
    assert(bm.GetQueryOutput("ALL_BUSES"s) == "No buses"s);
    assert(bm.GetQueryOutput("BUSES_FOR_STOP Marushkino"s) == "No stop"s);
    assert(bm.GetQueryOutput("STOPS_FOR_BUS 272"s) == "No bus"s);
}
void TestBusesForStop() {
    BusManager bm;
    assert(bm.ProcessQuery("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s));
    assert(bm.ProcessQuery("NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo"s));
    cerr << "PRINTING: \"" << bm.GetQueryOutput("BUSES_FOR_STOP Solntsevo"s) << '\"' << endl;
    cerr << "EXPECTED: " << "\"32K\"" << endl;
    assert(bm.GetQueryOutput("BUSES_FOR_STOP Solntsevo"s) == "32K");
    assert(bm.GetQueryOutput("BUSES_FOR_STOP Marushkino"s) == "32 32K");
}
void TestStopsForBus() {
    BusManager bm;
    assert(bm.ProcessQuery("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s));
    assert(bm.ProcessQuery("NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo"s));
    assert(bm.ProcessQuery("NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo"s));
    assert(bm.ProcessQuery("NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo"s));
    
    const string required_output = "Stop Vnukovo: 32 32K 950\n"s
                                   "Stop Moskovsky: no interchange\n"s
                                   "Stop Rumyantsevo: no interchange\n"s
                                   "Stop Troparyovo: 950"s;
    cerr << "Printing:\n" << bm.GetQueryOutput("STOPS_FOR_BUS 272"s) << endl;
    cerr << "Expected:\n" << required_output << endl;
    assert(bm.GetQueryOutput("STOPS_FOR_BUS 272"s) == required_output);
}
void TestAllBuses() {
    BusManager bm;
    assert(bm.ProcessQuery("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s));
    assert(bm.ProcessQuery("NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo"s));
    assert(bm.ProcessQuery("NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo"s));
    assert(bm.ProcessQuery("NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo"s));
    
    const string required_output = "Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo\n"s
                                   "Bus 32: Tolstopaltsevo Marushkino Vnukovo\n"s
                                   "Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo\n"s
                                   "Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo"s;
    cerr << "Printing:\n" << bm.GetQueryOutput("ALL_BUSES"s) << endl;
    cerr << "Expected:\n" << required_output << endl;
    assert(bm.GetQueryOutput("ALL_BUSES"s) == required_output);
}

void UnitTestsBusManager() {
    TestBusInput();
    TestEmptyResponses();
    TestBusesForStop();
    TestStopsForBus();
    TestAllBuses();
//    TestMain();
}

//----------------------------------------------------//

// Реализуйте функции и классы, объявленные выше, чтобы эта функция main
// решала задачу "Автобусные остановки"

int main() {
    UnitTestsBusManager();
    Query q;
    
    int query_count;
    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
 
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}

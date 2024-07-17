#pragma once
#include "geo.h"
#include "domain.h"

#include <algorithm>
#include <list>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//DEBUG
//#include <iostream>
//#include <iomanip>

class TransportDb {
public:
    ~TransportDb() {
        ClearData();
    }
    StopPtr AddStop(std::string stop_name, geo::Coord coords);
    BusPtr AddBus(std::string bus_name, const std::vector<std::string_view>& stops, bool is_roundtrip, std::string_view final_stop = {});
    //NB: Using const function which alters a mutable object, to be able to call in GetRoadDistance const
    void SetRoadDistance(std::string_view from_stop_name, std::string_view to_stop_name, int dist) const;
    
    double GetGeoDistance(StopPtr from, StopPtr to) const;
    int GetRoadDistance(StopPtr from, StopPtr to) const;
    
    BusStat GetBusStat(std::string_view bus_name) const;
    StopStat GetStopStat(std::string_view stop_name) const;
    
    BusSet  GetAllBusesWithStops() const;
    StopSet GetAllStopsWithBuses() const;
//    size_t GetNumBusesWithStops() const;
    
private:
    using StopPair = std::pair<StopPtr, StopPtr>;
    
    void AddBusToStops(BusPtr bus);
    
    std::unordered_set<StopPtr> GetUniqueStops(BusPtr) const;
    BusSet GetBusesForStop(std::string_view stop_name) const;
    std::vector<StopPtr> GetStopPtrs(const std::vector<std::string_view>& bus_stops) const;
    
    void ClearData();
    
    std::unordered_map<std::string_view, Stop*> stop_index_;
    std::unordered_map<std::string_view, Bus*> bus_index_;
    std::unordered_map<std::string_view, std::unordered_set<std::string_view>> stops_to_buses_;
    
    struct SPHasher {
        size_t operator()(const StopPair& ptr_pair) const;
    };
    //can be modified by const member functions (e.g.GetStats)
    mutable std::unordered_map<StopPair, double, SPHasher> geo_distance_table_;
    mutable std::unordered_map<StopPair, int, SPHasher> road_distance_table_;
};

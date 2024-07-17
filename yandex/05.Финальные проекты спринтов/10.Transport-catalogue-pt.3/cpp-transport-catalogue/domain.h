#pragma once
#include "geo.h"

#include <string>
#include <set>
#include <vector>

//For calculations:
inline const double EPSILON = 1e-6;
inline bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}


//======================= Stop & Bus =======================//
struct Stop {
    explicit Stop(std::string stop_name, geo::Coord coords);
    std::string name;
    geo::Coord location;
};

using StopPtr = const Stop*;

struct Bus {
    //move string during construction
    explicit Bus(std::string name, std::vector<StopPtr> stops, bool is_roundtrip, StopPtr final_stop = nullptr);
    std::string name;
    std::vector<StopPtr> stops;
    bool is_roundtrip = false;
    StopPtr final_stop = nullptr;
};

using BusPtr = const Bus*;

struct BusPtrSorter {
    bool operator()(const BusPtr& lhs, const BusPtr& rhs) const;
};

struct StopPtrSorter {
    bool operator()(const StopPtr& lhs, const StopPtr& rhs) const;
};

using BusSet = std::set<BusPtr, BusPtrSorter>;
using StopSet = std::set<StopPtr, StopPtrSorter>;

struct StopStat {
    int request_id = 0;
    bool exists = false;
    BusSet BusesForStop;
};

struct BusStat {
    int request_id = 0;
    bool exists = false;
    int total_stops = 0;
    int unique_stops = 0;
    double road_dist = 0.0;
    double curvature = 0.0;
};

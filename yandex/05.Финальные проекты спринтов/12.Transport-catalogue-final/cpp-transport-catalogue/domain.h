#pragma once
#include "geo.h"

#include <string>
#include <set>
#include <variant>
#include <vector>

//DEBUG output global
#include <iostream>
static constexpr bool CERR_ERRORS_ONLY = false;
static constexpr bool CERR_DISABLE = true;

#define CERR if constexpr (CERR_ERRORS_ONLY || CERR_DISABLE) {} else std::cerr
#define CERR_ERROR if constexpr (CERR_DISABLE) {} else std::cerr

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

//TODO: can inherit stats from parent, if needed
//struct Stat {
//    int request_id = 0;
//    bool exists = false;
//}

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

/*
 {
 "request_id": <id запроса>,
 "total_time": <суммарное время>,
 "items": [
 <элементы маршрута>
 ]
 }
 total_time — суммарное время в минутах, которое требуется для прохождения маршрута, выведенное в виде вещественного числа.
 Обратите внимание, что расстояние от остановки A до остановки B может быть не равно расстоянию от B до A!
 items — список элементов маршрута, каждый из которых описывает непрерывную активность пассажира, требующую временных затрат. А именно элементы маршрута бывают двух типов.
 Wait — подождать нужное количество минут (в нашем случае всегда bus_wait_time) на указанной остановке:
 {
 "type": "Wait",
 "stop_name": "Biryulyovo",
 "time": 6
 }
 Bus — проехать span_count остановок (перегонов между остановками) на автобусе bus, потратив указанное количество минут:
 {
 "type": "Bus",
 "bus": "297",
 "span_count": 2,
 "time": 5.235
 }
 */
//======================= Routing =======================//
enum class RouteItemType {
    wait,
    bus,
};

struct RouteItem {
    RouteItemType type;
    const std::string_view name;
    double time_taken = 0.0;
    int span_count = 0;
    
    std::string GetTypeStr() {
        switch (type) {
            case RouteItemType::wait:
                return "Wait";
                break;
                
            case RouteItemType::bus:
                return "Bus";
                break;
                
            default:
                return "";
                break;
        }
    }
};

struct RouteStat {
    int request_id = 0;
    bool exists = false;
    double total_time = 0;
    std::vector<RouteItem> items;
};

///Another option:

/*
 struct StopWait {
 StopPtr stop;
 int wait_time;
 };
 
 struct TakeBus {
 BusPtr bus;
 int span_count;
 int time_travelled;
 };
 
 std::vector<std::variant<std::monostate, StopWait, TakeBus>> items_;
 */

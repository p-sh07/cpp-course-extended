#pragma once
#include "geo.h"

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

namespace transport {

struct Stop {
    explicit Stop(std::string&& stop_name, geo::Coordinates coords);
    std::string name;
    geo::Coordinates location;
};

struct Route {
    //move string during construction
    explicit Route(std::string&& name, std::vector<const Stop*>&& stops);
    
    std::string name;
    std::vector<const Stop*> stops;
};

struct RouteStats {
    size_t total_stops = 0;
    size_t unique_stops = 0;
    double road_dist = 0.0;
    double curvature = 0.0;
};

struct StopStats {
    StopStats(std::string_view stop_name, std::unordered_set<std::string_view> unique_routes);
    
    // check if route contains buses
    explicit operator bool() const {
        return !sorted_routes.empty();
    }
    
    bool operator!() const {
        return !operator bool();
    }
    std::string_view id;
    std::vector<std::string_view> sorted_routes;
};

}

class TransportCatalogue {
public:
    ~TransportCatalogue() {
        ClearData();
    }
    void AddStop(std::string&& stop_name, geo::Coordinates coords);
    void AddRoute(std::string&& route_name, const std::vector<std::string_view>& stops);
    void SetRoadDistance(std::string_view from_stop_name, std::string_view to_stop_name, int dist);
    
    std::optional<transport::RouteStats> GetRouteStats(std::string_view route_name) const;
    std::optional<transport::StopStats> GetStopStats(std::string_view stop_name) const;
    
private:
    using StopPair = std::pair<const transport::Stop*, const transport::Stop*>;
    
    std::vector<const transport::Stop*> GetStopPtrs(const std::vector<std::string_view>& route_stops) const;
    
    int GetRoadDistance(const transport::Stop* from, const transport::Stop* to) const;
    double GetGeoDistance(const transport::Stop* from, const transport::Stop* to) const;
    
    void AddRouteToStops(const transport::Route* route);
    std::unordered_set<const transport::Stop*> GetUniqueStops(const transport::Route*) const;

    void ClearData();
    
    std::unordered_map<std::string_view, transport::Stop*> stop_index_;
    std::unordered_map<std::string_view, transport::Route*> route_index_;
    std::unordered_map<std::string_view, std::unordered_set<std::string_view>> stops_to_routes_;
    
    struct SPHasher {
        size_t operator()(const StopPair& ptr_pair) const;
    };
    //can be modified by const member functions (e.g.GetStats)
    mutable std::unordered_map<StopPair, double, SPHasher> geo_distance_table_;
    mutable std::unordered_map<StopPair, int, SPHasher> road_distance_table_;
};

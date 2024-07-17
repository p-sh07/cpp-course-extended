#include "transport_catalogue.h"

using std::string;
using std::string_view;
using std::vector;

using transport::Stop;
using transport::Route;
using transport::StopStats;
using transport::RouteStats;

//DEBUG
#include <iostream>
//using std::cerr;
//using std::endl;

Stop::Stop(string&& stop_name, geo::Coordinates coords)
: name(std::move(stop_name))
, location(coords)
{}

Route::Route(string&& name, vector<const Stop*>&& stops_in_order)
: name(std::move(name))
, stops(std::move(stops_in_order)) {}

StopStats::StopStats(std::string_view stop_name, std::unordered_set<std::string_view> unique_routes)
: id(stop_name)
, sorted_routes(std::make_move_iterator(unique_routes.begin()), std::make_move_iterator(unique_routes.end())) {
    std::sort(sorted_routes.begin(), sorted_routes.end());
}

void TransportCatalogue::AddStop(std::string&& stop_name, geo::Coordinates coords) {
    Stop* stop_to_add = new Stop(std::move(stop_name), coords);
    stop_index_[stop_to_add->name] = stop_to_add;
}
void TransportCatalogue::AddRoute(std::string&& route_name, const std::vector<std::string_view>& stops) {
    Route* route_to_add = new Route(std::move(route_name), GetStopPtrs(stops));
    route_index_[route_to_add->name] = route_to_add;
    AddRouteToStops(route_to_add);
}

//Теперь всё понятно =)!
void TransportCatalogue::SetRoadDistance(std::string_view from_stop_name, std::string_view to_stop_name, int dist) {
    road_distance_table_[{stop_index_.at(from_stop_name), stop_index_.at(to_stop_name)}] = dist;
}

std::optional<RouteStats> TransportCatalogue::GetRouteStats(string_view route_name) const {
    if(route_index_.count(route_name) == 0) {
        return std::nullopt;
    }
    auto route = route_index_.at(route_name);
    //prev stop in route for distance calculation
    const Stop* prev_stop = nullptr;
    double route_geo_length = 0.0;
    double route_road_dist = 0.0;
    
    for(const auto& stop : route->stops) {
        route_geo_length += GetGeoDistance(prev_stop, stop);
        route_road_dist += GetRoadDistance(prev_stop, stop);
        prev_stop = stop;
    }
    double curvature = route_road_dist/route_geo_length;
    
    return {{route->stops.size(), GetUniqueStops(route).size(), route_road_dist, curvature}};
}

std::optional<StopStats> TransportCatalogue::GetStopStats(std::string_view stop_name) const {
    if(stop_index_.count(stop_name) == 0) {
        return std::nullopt;                //no stop  - nullopt
    } else if(stops_to_routes_.count(stop_name) == 0) {
        return {StopStats{stop_name, {}}};  //no buses - empty StopStats
    }
    return StopStats(stop_name, stops_to_routes_.at(stop_name));
}

vector<const Stop*> TransportCatalogue::GetStopPtrs(const vector<string_view>& route_stops) const {
    vector<const Stop*> ptr_vector;
    for(const auto& stop : route_stops) {
        ptr_vector.push_back(stop_index_.at(stop));
    }
    return ptr_vector;
}

void TransportCatalogue::AddRouteToStops(const Route* route) {
    for(const auto& stop : route->stops) {
        stops_to_routes_[stop->name].insert(route->name);
    }
}

//All distances are whole positive numbers -> int or size_t
int TransportCatalogue::GetRoadDistance(const Stop* from, const Stop* to) const {
    //invalid stop pointers
    if(!from || !to) {
        return 0;
    }
    auto it = road_distance_table_.find({from, to});
    //if from - to not found, use to - from dist
    if(it == road_distance_table_.end()) {
        it = road_distance_table_.find({to, from});
    }
    //DEBUG
    if(it == road_distance_table_.end()) {
        std::cerr << "*TC-error* No stop distance found" << std::endl;
    }
    //
    return it == road_distance_table_.end() ? -1 : it->second;
}

double TransportCatalogue::GetGeoDistance(const Stop* from, const Stop* to) const {
    //invalid stop pointers
    if(!from || !to) {
        return 0.0;
    }
    //use pair for convenience
    auto stop_pair = std::make_pair(from, to);
    if(geo_distance_table_.count(stop_pair) > 0) {
        return geo_distance_table_.at(stop_pair);
    }
    //compute if not found
    double dist = geo::ComputeDistance(from->location, to->location);
    geo_distance_table_[stop_pair] = dist;
    return dist;
}

std::unordered_set<const transport::Stop*> TransportCatalogue::GetUniqueStops(const Route* route) const {
    return {route->stops.begin(), route->stops.end()};
}

size_t TransportCatalogue::SPHasher::operator()(const StopPair& ptr_pair) const {
    size_t n = 37;
    std::hash<const Stop*> ptr_hash;
    return n*ptr_hash(ptr_pair.first) + ptr_hash(ptr_pair.second);
}

void TransportCatalogue::ClearData() {
    for(auto& [_, stop] : stop_index_) {
        delete stop;
    }
    
    for(auto& [_, route] : route_index_) {
        delete route;
    }
}

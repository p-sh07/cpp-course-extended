#include "transport_catalogue.h"

#include <limits>
#include <ranges>
using std::string;
using std::string_view;
using std::vector;

//DEBUG
#include <iostream>

StopPtr TransportDb::AddStop(std::string stop_name, geo::Coord coords) {
    Stop* added_stop = new Stop(std::move(stop_name), coords);
    stop_index_[added_stop->name] = added_stop;
    return added_stop;
}

BusPtr TransportDb::AddBus(std::string bus_name, const std::vector<std::string_view>& stops, bool is_roundtrip, std::string_view final_stop) {
    
    //TODO: Add final_stop presence in index check?
    StopPtr final_stop_ptr = final_stop.empty() ? nullptr : stop_index_.at(final_stop);

    Bus* added_bus = new Bus(std::move(bus_name), GetStopPtrs(stops), is_roundtrip, final_stop_ptr);
    bus_index_[added_bus->name] = added_bus;
    AddBusToStops(added_bus);
    return added_bus;
}

void TransportDb::SetRoadDistance(std::string_view from_stop_name, std::string_view to_stop_name, int dist) const {
    if(stop_index_.count(from_stop_name) > 0 && stop_index_.count(to_stop_name) > 0) {
        road_distance_table_[{stop_index_.at(from_stop_name), stop_index_.at(to_stop_name)}] = dist;
    } else {
        //DEBUG:
        CERR_ERROR << "Could not add road_dist between stops: " << from_stop_name << " & " << to_stop_name << std::endl;
    }
}

BusStat TransportDb::GetBusStat(string_view bus_name) const {
    if(bus_index_.count(bus_name) == 0) {
        return {}; //empty BusStat with bool exists = 0;
    }
    BusStat stat;
    auto bus = bus_index_.at(bus_name);
    
    stat.exists = true;
    stat.total_stops = static_cast<int>(bus->stops.size());
    stat.unique_stops = static_cast<int>(GetUniqueStops(bus).size());
    
    //prev stop in bus for distance calculation
    StopPtr prev_stop = bus->stops.empty() ? nullptr : bus->stops[0];
    double bus_geo_length = 0.0;
    //start at stop #2
    for(const auto& stop : bus->stops | std::views::drop(1)) {
        bus_geo_length += GetGeoDistance(prev_stop, stop);
        stat.road_dist += GetRoadDistance(prev_stop, stop);

        prev_stop = stop;
    }
    stat.curvature = stat.road_dist/bus_geo_length;

    return stat;
}

StopStat TransportDb::GetStopStat(std::string_view stop_name) const {
    if(stop_index_.count(stop_name) == 0) {
        return {};
    }
    StopStat stat;
    stat.exists = true;
    stat.BusesForStop = GetBusesForStop(stop_name);
    
    return stat;
}

BusSet TransportDb::GetAllBusesWithStops() const {
    BusSet buses;
    if(bus_index_.empty()) {
        return {};
    }
    //NB: BusPtr is a const ptr
    for(const auto& [_, bus_ptr] : bus_index_) {
        if(!bus_ptr->stops.empty()) {
            buses.insert(bus_ptr);
        }
    }
    return buses;
}

StopSet TransportDb::GetAllStopsWithBuses() const {
    StopSet stops;
    if(stop_index_.empty()) {
        return {};
    }
    //NB: BusPtr is a const ptr
    for(const auto& [_, stop_ptr] : stop_index_) {
        if(stops_to_buses_.count(stop_ptr->name) > 0 && !stops_to_buses_.at(stop_ptr->name).empty()) {
            stops.insert(stop_ptr);
        }
    }
    return stops;
}

//size_t TransportDb::GetNumBusesWithStops() const {
//    size_t ans = 0;
//    if(bus_index_.empty()) {
//        return ans;
//    }
//    //NB: BusPtr is a const ptr
//    for(const auto& [_, bus_ptr] : bus_index_) {
//        if(!bus_ptr->stops.empty()) {
//            ++ans;
//        }
//    }
//    return ans;
//}

void TransportDb::AddBusToStops(BusPtr bus) {
    for(const auto& stop : bus->stops) {
        stops_to_buses_[stop->name].insert(bus->name);
    }
}

double TransportDb::GetGeoDistance(StopPtr from, StopPtr to) const {
    //invalid stop pointers
    if(!from || !to) {
        CERR_ERROR << "*Error, GeoDistance: invalid stop pointers passed\n";
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

//All distances are whole positive numbers -> int or size_t
int TransportDb::GetRoadDistance(StopPtr from, StopPtr to) const {
    //invalid stop pointers
    if(!from || !to) {
        CERR_ERROR << "*Error, RoadDistance: invalid stop pointers passed\n";
        return 0;
    }
    auto it = road_distance_table_.find({from, to});
    
    //if from - to not found, use to - from dist
    if(it == road_distance_table_.end()) {
        //CERR << "#Warning, RoadDistance: used Reverse Distance\n";
        it = road_distance_table_.find({to, from});
    }
    //DEBUG
    if(it == road_distance_table_.end()) {
        //CERR_ERROR << " ERROR: No stop distance found for stops [" << from->name << "] & [" << to->name << "]" << std::endl;
    }
    return it == road_distance_table_.end() ? std::numeric_limits<int>::max() : it->second;
}

std::unordered_set<StopPtr> TransportDb::GetUniqueStops(BusPtr bus) const {
    return {bus->stops.begin(), bus->stops.end()};
}

BusSet TransportDb::GetBusesForStop(std::string_view stop_name) const {
    if(stop_index_.count(stop_name) == 0 || stops_to_buses_.count(stop_name) == 0) {
        return {};
    }
    BusSet answer;
    for(const auto& bus : stops_to_buses_.at(stop_name)) {
        answer.insert(bus_index_.at(bus));
    }
    return answer;
}

vector<StopPtr> TransportDb::GetStopPtrs(const vector<string_view>& bus_stops) const {
    vector<StopPtr> ptr_vector;
    for(const auto& stop : bus_stops) {
        ptr_vector.push_back(stop_index_.at(stop));
    }
    return ptr_vector;
}

void TransportDb::ClearData() {
    for(auto& [_, stop] : stop_index_) {
        delete stop;
    }
    
    for(auto& [_, bus] : bus_index_) {
        delete bus;
    }
}

size_t TransportDb::SPHasher::operator()(const StopPair& ptr_pair) const {
    size_t n = 37;
    std::hash<StopPtr> ptr_hash;
    return n*ptr_hash(ptr_pair.first) + ptr_hash(ptr_pair.second);
}

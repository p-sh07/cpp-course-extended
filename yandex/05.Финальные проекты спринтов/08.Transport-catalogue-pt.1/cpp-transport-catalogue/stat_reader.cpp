#include "stat_reader.h"

using std::operator""s;
using std::string_view;

void PrintRouteInfo(string_view route_id, const TransportCatalogue& tansport_catalogue, std::ostream& output) {
    auto stats_optional = tansport_catalogue.GetRouteStats(route_id);
    
    output << "Bus "s << route_id << ": ";
    if(!stats_optional.has_value()) {
        output << "not found\n"s;
        return;
    }
    
    auto& stats = stats_optional.value();
    
    output << std::setprecision(6)
    << stats.total_stops << " stops on route, "s
    << stats.unique_stops << " unique stops, "s
    << stats.length << " route length\n"s;
}

void PrintBusInfo(string_view stop_id, const TransportCatalogue& tansport_catalogue, std::ostream& output) {
    auto stats_optional = tansport_catalogue.GetStopStats(stop_id);
    
    output << "Stop "s << stop_id << ": ";
    
    if(!stats_optional.has_value()) {
        output << "not found\n"s;
        return;
    } else if(!stats_optional.value()) {
        output << "no buses\n"s;
        return;
    }
    
    output << "buses ";
    for(const auto route_name : stats_optional.value().sorted_routes) {
        output << route_name << ' ';
    }
    output << "\n";
}

void stats::ParseAndPrintStat(const TransportCatalogue& tansport_catalogue, string_view request, std::ostream& output) {
    //Parsing
    size_t space_pos = request.find(' ');
    string_view id = request.substr(space_pos+1, request.npos);
    string_view command = request.substr(0, space_pos);
    
    if(command == stats::GET_ROUTE_INFO) {
        PrintRouteInfo(id, tansport_catalogue, output);
    } else if(command == stats::GET_STOP_INFO) {
        PrintBusInfo(id, tansport_catalogue, output);
    }
}

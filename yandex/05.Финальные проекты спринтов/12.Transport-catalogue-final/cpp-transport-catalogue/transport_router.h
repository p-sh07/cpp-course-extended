#pragma once

#include "transport_catalogue.h"
#include "graph.h"
#include "router.h"

#include <memory>

struct BusRouterSettings {    
    int wait_time = 0;
    double velocity_kmh = 0;
};

class BusRouter {
    using Graph = graph::DirectedWeightedGraph<double>;
    using Router = graph::Router<double>;
    using RouteInfo = graph::Router<double>::RouteInfo;
    
public:
    explicit BusRouter(const TransportDb& tdb, BusRouterSettings settings = {});
    
    RouteStat PlotRoute(std::string_view from, std::string_view to) const;
    
    inline void UpdSettings(BusRouterSettings settings) {
        settings_ = settings;
    }
    
private:
    static constexpr double METERS_IN_KM = 1000.0;
    static constexpr double MINUTES_IN_HOUR = 60.0;
    
    BusRouterSettings settings_;
    const TransportDb& tdb_;
    Graph bus_graph_;
    std::unique_ptr<Router> graph_router_ = nullptr;
    
    struct EdgeInfo {
        BusPtr bus = nullptr;
        int span_count = 0;
        double time = 0.0;
        std::string_view from = {};
        std::string_view to = {};
    };
    
    using StopIdMap = std::unordered_map<std::string_view, graph::VertexId>;
    
    StopIdMap stop_ids_;
    std::vector<EdgeInfo> edge_data_;
    
    graph::VertexId FindOrAddStopVertex(std::string_view stop_name);
    void AddStopsToGraph(BusPtr bus, StopPtr from_stop, auto stops_range);
    
    //writes stop ids into map during graph building
    Graph InitGraphFromDb();
    RouteStat BuildRouteStat(const std::optional<RouteInfo>& info) const;
};

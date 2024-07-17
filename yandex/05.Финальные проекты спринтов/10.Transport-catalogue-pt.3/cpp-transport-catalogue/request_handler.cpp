#include "request_handler.h"

RequestHandler::RequestHandler(const TransportDb& tdb, MapRenderer& renderer)
: tdb_(tdb)
, renderer_(renderer) 
{}

BusStat RequestHandler::GetBusStat(int request_id, const std::string_view& bus_name) const {
    //TODO - better way to assign request_id?
    auto stat = tdb_.GetBusStat(bus_name);
    stat.request_id = request_id;
    
    return stat;
}

// Возвращает маршруты, проходящие через
StopStat RequestHandler::GetStopStat(int request_id, const std::string_view& stop_name) const {
    auto stat = tdb_.GetStopStat(stop_name);
    stat.request_id = request_id;
    
    return stat;
}

void RequestHandler::UploadRendererSettings(const std::shared_ptr<RendererSettings> settings) const {
    renderer_.LoadSettings(settings);
}

// Отрисовать карту в поток
void RequestHandler::RenderMap(std::ostream& out) const {
    //returns a set, so buses will be in alphabetical order
    renderer_.AddBusSet(tdb_.GetAllBusesWithStops());
    //stops in alphabetical order
    renderer_.AddStopSet(tdb_.GetAllStopsWithBuses());
                      
    //Init sphere projector after adding all geo points
    renderer_.InitProjector();
    renderer_.RenderOut(out);
}

//void RequestHandler::UploadBusData(MapRenderer& renderer_) const {
//    for(const auto [_, ])
//}

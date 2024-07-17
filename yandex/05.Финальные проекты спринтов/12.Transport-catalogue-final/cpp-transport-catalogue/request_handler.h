#pragma once

#include "map_renderer.h"
#include "transport_router.h"
#include "transport_catalogue.h"

#include <list>
#include <queue>
#include <variant>

class RequestHandler {
public:
    RequestHandler(const TransportDb& tdb, MapRenderer& renderer, BusRouter& router);

    // Возвращает информацию о маршруте (запрос Bus)
    BusStat GetBusStat(int request_id, std::string_view bus_name) const;

    // Возвращает маршруты, проходящие через Stop
    StopStat GetStopStat(int request_id, std::string_view stop_name) const;
    
    // Возвращает маршруты, проходящие через Stop
    RouteStat GetRoute(int request_id, std::string_view from_stop, std::string_view to_stop) const;

    void UploadRendererSettings(const std::shared_ptr<RendererSettings> settings) const;
    void UpdRouterSettings(BusRouterSettings settings) const;
    
    // Отрисовать карту в svg документ
    void RenderMap(std::ostream& out) const;
    
private:
    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
    const TransportDb& tdb_;
    MapRenderer& renderer_;
    BusRouter& router_;
    
    //void UploadBusData(MapRenderer& renderer_) const;
};


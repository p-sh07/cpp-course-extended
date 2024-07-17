#pragma once

#include "map_renderer.h"
#include "transport_catalogue.h"

#include <list>
#include <queue>
#include <variant>

class RequestHandler {
public:
    RequestHandler(const TransportDb& tdb, MapRenderer& renderer);

    // Возвращает информацию о маршруте (запрос Bus)
    BusStat GetBusStat(int request_id, const std::string_view& bus_name) const;

    // Возвращает маршруты, проходящие через Stop
    StopStat GetStopStat(int request_id, const std::string_view& stop_name) const;

    void UploadRendererSettings(const std::shared_ptr<RendererSettings> settings) const;
    
    // Отрисовать карту в svg документ
    void RenderMap(std::ostream& out) const;
    
private:
    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
    const TransportDb& tdb_;
    MapRenderer& renderer_;
    
    //void UploadBusData(MapRenderer& renderer_) const;
};


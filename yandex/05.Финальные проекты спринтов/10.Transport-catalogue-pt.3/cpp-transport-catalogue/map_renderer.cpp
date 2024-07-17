#include "map_renderer.h"

using namespace std::literals;

void MapRenderer::LoadSettings(const std::shared_ptr<RendererSettings> settings) {
    rsets_ = settings;
}

void MapRenderer::InitProjector() {
    if(rsets_) {
        sproj_ = std::make_unique<SphereProjector>(all_geo_points_.begin(), all_geo_points_.end(), rsets_->img_size.x, rsets_->img_size.y, rsets_->padding);
    }
}

void MapRenderer::AddBus(BusPtr bus) {
    buses_to_draw_.insert(bus);
    for(const auto& stop : bus->stops) {
        all_geo_points_.insert(&(stop->location));
    }
}

void MapRenderer::AddStop(StopPtr stop) {
    stops_to_draw_.insert(stop);
}

void MapRenderer::AddBusSet(BusSet buses) {
    buses_to_draw_ = std::move(buses);
    for(const auto& bus : buses_to_draw_) {
        for(const auto& stop : bus->stops) {
            all_geo_points_.insert(&(stop->location));
        }
    }
}

void MapRenderer::AddStopSet(StopSet stops) {
    stops_to_draw_ = std::move(stops);
}

//not used for now:
//void MapRenderer::AddTextLabel(std::string_view text, svg::Point pos, svg::Point offset) {}

void MapRenderer::DrawBusLabel(svg::Document& doc, std::string_view text, svg::Point pos, svg::Color text_clr) {
    const std::string bus_name = std::string(text);
    //Подложка
    doc.Add(svg::Text()
            .SetData(bus_name)
            .SetPosition(pos)
            .SetOffset(rsets_->bus_label_offset)
            .SetFontPoint(rsets_->bus_label_font_size)
            .SetFontFamily(rsets_->font_family)
            .SetFontWeight(rsets_->font_weight)
            
            .SetFillColor(rsets_->underlayer_color)
            .SetStrokeColor(rsets_->underlayer_color)
            .SetStrokeWidth(rsets_->underlayer_width)
            .SetStrokeLineCap(rsets_->line_cap_)
            .SetStrokeLineJoin(rsets_->line_join_)
            );
    //Надпись
    doc.Add(svg::Text()
            .SetData(bus_name)
            .SetPosition(pos)
            .SetOffset(rsets_->bus_label_offset)
            .SetFontPoint(rsets_->bus_label_font_size)
            .SetFontFamily(rsets_->font_family)
            .SetFontWeight(rsets_->font_weight)
            
            .SetFillColor(text_clr)
            );
}

void MapRenderer::DrawStopLabel(svg::Document& doc, std::string_view text, svg::Point pos) {
    const std::string stop_name = std::string(text);
    //Подложка
    doc.Add(svg::Text()
            .SetData(stop_name)
            .SetPosition(pos)
            .SetOffset(rsets_->stop_label_offset)
            .SetFontPoint(rsets_->stop_label_font_size)
            .SetFontFamily(rsets_->font_family)
            
            .SetFillColor(rsets_->underlayer_color)
            .SetStrokeColor(rsets_->underlayer_color)
            .SetStrokeWidth(rsets_->underlayer_width)
            .SetStrokeLineCap(rsets_->line_cap_)
            .SetStrokeLineJoin(rsets_->line_join_)
            );
    //Надпись
    doc.Add(svg::Text()
            .SetData(stop_name)
            .SetPosition(pos)
            .SetOffset(rsets_->stop_label_offset)
            .SetFontPoint(rsets_->stop_label_font_size)
            .SetFontFamily(rsets_->font_family)
            
            .SetFillColor(rsets_->stop_label_text_color)
            );
}

void MapRenderer::DrawBus(BusPtr bus, svg::Color color) {
    svg::Polyline line;
    //TODO: error handling
    if(!rsets_ || !sproj_) {
        throw std::runtime_error("Renderer is not initialised!"s);
    }
    if(!bus) {
        throw std::runtime_error("Renderer tried to accessed a null bus pointer"s);
    }
    if(bus->stops.empty()) {
        //don't draw empty bus routes
        return;
    }
    
    const auto& stops = bus->stops;
    //1.Make bus route line
    for(const auto& stop : stops) {
        line.AddPoint(sproj_->ToImgPt(stop->location));
    }
    //1.1.Draw bus route line
    line.SetFillColor(rsets_->fill_color_)
        .SetStrokeColor(color)
        .SetStrokeWidth(rsets_->line_width)
        .SetStrokeLineCap(rsets_->line_cap_)
        .SetStrokeLineJoin(rsets_->line_join_);
    
    bus_lines_.Add(line);

    //2.Draw bus route labels
    const auto& first_stop = bus->stops[0];
    DrawBusLabel(bus_labels_, bus->name, sproj_->ToImgPt(first_stop->location), color);
    //if not a roundtrip bus, and first stop doesn't match last stop
    if(bus->final_stop && !bus->is_roundtrip && bus->final_stop != first_stop) {
        DrawBusLabel(bus_labels_, bus->name, sproj_->ToImgPt(bus->final_stop->location), color);
    }
}

void MapRenderer::DrawStop(StopPtr stop) {
    const auto stop_pt = sproj_->ToImgPt(stop->location);
    
    stop_circles_.Add(svg::Circle()
            .SetCenter(stop_pt)
            .SetRadius(rsets_->stop_radius)
            .SetFillColor(rsets_->stop_circle_fill_color)
            );
    //4.Draw stop label
    DrawStopLabel(stop_labels_, stop->name, stop_pt);
}

void MapRenderer::DrawAllObjects(svg::Document& doc) {
    size_t counter = 0;

    for(const auto& bus : buses_to_draw_) {
        DrawBus(bus, GetNextColor(counter));
        //advance to next color if bus had stops
        ++counter;
    }
    for(const auto& stop : stops_to_draw_) {
            DrawStop(stop);
            //advance to next color if bus had stops
            ++counter;
    }
    
    //1.Draw bus route lines
    doc = std::move(bus_lines_);
    //2.Draw bus route labels
    doc = std::move(bus_labels_);
    //3.Draw stop sircles
    doc = std::move(stop_circles_);
    //4.Draw stop labels
    doc = std::move(stop_labels_);
}

void MapRenderer::RenderOut(std::ostream& out) {
    svg::Document doc;
    //Entry point -> calls "draw stops" & "draw labels", since all drawing is based on bus routes
    DrawAllObjects(doc);
    
    doc.Render(out);
}

svg::Color MapRenderer::GetNextColor(size_t& counter) const {
    return rsets_->palette[counter % rsets_->palette.size()];
}

void MapRenderer::StoreCoordPtr(const geo::Coord* ptr) {
    all_geo_points_.insert(ptr);
}


//=================== SphereProjector ===================//
// Проецирует широту и долготу в координаты внутри SVG-изображения
svg::Point SphereProjector::ToImgPt(geo::Coord coords) const {
    return {
        (coords.lng - min_lon_) * zoom_coeff_ + padding_,
        (max_lat_ - coords.lat) * zoom_coeff_ + padding_
    };
}

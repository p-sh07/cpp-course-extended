#include "json_reader.h"

#include <sstream>

using namespace std::literals;

//================ JsonReader ================//
JsonReader::JsonReader(TransportDb& tdb, const RequestHandler& handler)
: database_(tdb)
, req_handler_(handler)
{}

void JsonReader::ParseAndAddStops(const json::Array& database_commands, TransportDb& db) const {
    std::unordered_map<std::string_view, std::vector<std::pair<std::string_view, int>>> stops_road_distances;

    for(const auto& entry : database_commands) {
        const auto& map = entry.AsMap();
        
        if(map.at("type"s).AsString() == "Stop"s) {
            
            const std::string_view stop_name = map.at("name"s).AsString();
            
            const geo::Coord stop_coords{map.at("latitude"s).AsDouble(), map.at("longitude"s).AsDouble()};
            
            db.AddStop(std::string(stop_name), stop_coords);
            
            //build road distances map:
            auto& distances = stops_road_distances[stop_name];
            for(const auto& [name, dist] : map.at("road_distances").AsMap()) {
                distances.push_back({name, dist.AsInt()});
            }
        }
    }
    //2.Add Stops Road distances
    for(const auto& [from_stop, distances] : stops_road_distances) {
        for(const auto& [to_stop, dist] : distances) {
            db.SetRoadDistance(from_stop, to_stop, dist);
        }
    }
}
void JsonReader::ParseAndAddBuses(const json::Array& database_commands, TransportDb& db) const {
    //3.Add Bus Routes
    for(const auto& entry : database_commands) {
        const auto& map = entry.AsMap();
        std::string_view final_stop_name = {};
        bool is_roundtrip = false;
        
        if(map.at("type"s).AsString() == "Bus"s) {
            is_roundtrip = map.at("is_roundtrip"s).AsBool();
            const auto& json_stop_arr = map.at("stops"s).AsArray();
            std::vector<std::string_view> stops;
            for(const auto& node : json_stop_arr) {
                stops.push_back(node.AsString());
            }
            //add stops backwards if not roundtrip
            if(!is_roundtrip) {
                for(auto it = std::next(json_stop_arr.rbegin()); it != json_stop_arr.rend(); ++it) {
                    stops.push_back(it->AsString());
                }
                //add final stop name:
                final_stop_name = json_stop_arr.rbegin()->AsString();
            }
            
            db.AddBus(map.at("name"s).AsString(), stops, is_roundtrip, final_stop_name);
        }
    }
}

RendererSettings JsonReader::ParseRendererSettings(const json::Map& rsets) const {
    RendererSettings settings;
    
    settings.img_size.x = rsets.at("width"s).AsDouble();
    settings.img_size.y = rsets.at("height"s).AsDouble();
    
    settings.padding = rsets.at("padding"s).AsDouble();
    
    settings.line_width = rsets.at("line_width"s).AsDouble();
    settings.stop_radius = rsets.at("stop_radius"s).AsDouble();
    
    settings.bus_label_font_size = rsets.at("bus_label_font_size"s).AsInt();
    settings.bus_label_offset = ParsePoint(rsets.at("bus_label_offset"s));
    //TODO: Offset is a size, not a point, possibly change
    
    settings.stop_label_font_size = rsets.at("stop_label_font_size"s).AsDouble();
    settings.stop_label_offset = ParsePoint(rsets.at("stop_label_offset"s));
    
    settings.underlayer_color = ParseColor(rsets.at("underlayer_color"s));
    settings.underlayer_width = rsets.at("underlayer_width"s).AsDouble();
    
    settings.palette = ParsePalette(rsets.at("color_palette"s));
    
    return settings;
}

void JsonReader::ParseStatRequests(const json::Array& stat_reqs, std::queue<StatRequest>& request_queue) {
    //5.Store Database Stat Requests
    if(stat_reqs.empty()) {
        return;
    }
    for(const auto& json_request : stat_reqs) {
        if(!json_request.IsMap() || json_request.AsMap().empty()) {
            throw std::runtime_error("Empty/invalid stat request in json");
        }
        const auto& request_map = json_request.AsMap();
        
        const std::string_view name = request_map.count("name"s) > 0 ? request_map.at("name"s).AsString() : std::string_view{};
        
        request_queue.push({request_map.at("type"s).AsString(), name,
            request_map.at("id"s).AsInt()});
    }
}

void JsonReader::ParseInput(std::istream& in, bool has_settings, bool has_stat_requests) {
    
    parsed_json_ = json::Load(in).GetRoot().AsMap();
    //1,2 & 3. Add stops, stop distances & buses
    if(parsed_json_.count("base_requests"s) > 0) {
        const auto& database_commands = parsed_json_.at("base_requests"s).AsArray();
        //Build Transport Database
        ParseAndAddStops(database_commands, database_);
        ParseAndAddBuses(database_commands, database_);
    }
    //4.Parse and Apply Map Renderer Settings
    if(has_settings && parsed_json_.count("render_settings"s) > 0) {
        const auto& rsets = parsed_json_.at("render_settings").AsMap();
        
        req_handler_.UploadRendererSettings(std::make_shared<RendererSettings>(ParseRendererSettings(rsets)));
    }
    //5.If required, process stat requests
    if(has_stat_requests && parsed_json_.count("stat_requests") > 0) {
        const auto& stat_reqs = parsed_json_.at("stat_requests").AsArray();
        ParseStatRequests(stat_reqs, request_queue_);
    }
}

json::Map JsonReader::MakeStatJson(const BusStat& stat) const {
    return json::Builder{}.StartMap()
           .Key("curvature"s).Value(stat.curvature)
           .Key("request_id"s).Value(stat.request_id)
           .Key("route_length"s).Value(stat.road_dist)
           .Key("stop_count"s).Value(stat.total_stops)
           .Key("unique_stop_count"s).Value(stat.unique_stops)
           .EndMap().Build().AsMap();
}
json::Map JsonReader::MakeStatJson(const StopStat& stat) const {
    
    json::Builder buses;
    buses.StartMap().Key("buses"s).StartArray();
    
    for(auto bus_ptr : stat.BusesForStop) {
        buses.Value(bus_ptr->name);
    }
    buses.EndArray().Key("request_id"s).Value(stat.request_id).EndMap();
    
    return buses.Build().AsMap();
}

void JsonReader::StoreSvgMap(std::string map, int request_id) {
    const json::Map answer = {
        {"map"s, {std::string(std::move(map))}},
        {"request_id"s, {request_id}}
    };
    json::Node node{answer};
    stat_request_answers_.push_back(node);
}

svg::Point JsonReader::ParsePoint(const json::Node& point_node) const {
    return {point_node.AsArray()[0].AsDouble(),
            point_node.AsArray()[1].AsDouble()};
}

svg::Color JsonReader::ParseColor(const json::Node& color_node) const {
    svg::Color result;
    
    if(color_node.IsString()) {
        result.emplace<std::string>(color_node.AsString());
    }
    else if(color_node.IsArray()) {
        auto& color_array = color_node.AsArray();
        if(color_array.size() == 3) {
            svg::Rgb rgb;
            
            rgb.red = static_cast<uint8_t>(color_array[0].AsInt());
            rgb.green = static_cast<uint8_t>(color_array[1].AsInt());
            rgb.blue = static_cast<uint8_t>(color_array[2].AsInt());
            
            result.emplace<svg::Rgb>(rgb);
        }
        else if(color_array.size() == 4) {
            svg::Rgba rgba;
            
            rgba.red = static_cast<uint8_t>(color_array[0].AsInt());
            rgba.green = static_cast<uint8_t>(color_array[1].AsInt());
            rgba.blue = static_cast<uint8_t>(color_array[2].AsInt());
            rgba.opacity = color_array[3].AsDouble();
            
            result.emplace<svg::Rgba>(rgba);
        }
    }
    return result;
}
//comment
std::vector<svg::Color> JsonReader::ParsePalette(const json::Node& pallete_node) const {
    std::vector<svg::Color> result;
    
    for(const auto& color_node : pallete_node.AsArray()) {
        result.emplace_back(ParseColor(color_node));
    }
    return result;
}

void JsonReader::ProcessStatRequests() {
    while(!request_queue_.empty()) {
        StatRequest request = std::move(request_queue_.front());
        request_queue_.pop();
        
        if(request.type == "Bus"s) {
            StoreRequestAnswer(req_handler_.GetBusStat(request.id, request.name));
        }
        else if(request.type == "Stop"s) {
            StoreRequestAnswer(req_handler_.GetStopStat(request.id, request.name));
        }
        else if(request.type == "Map"s) {
            std::stringstream ss;
            req_handler_.RenderMap(ss);
            StoreSvgMap(ss.str(), request.id);
        }
    }
}

void JsonReader::PrintRequestAnswers(std::ostream& out) const {
    if(!stat_request_answers_.empty()) {
        json::Print(json::Document{{stat_request_answers_}}, out);
    }
}

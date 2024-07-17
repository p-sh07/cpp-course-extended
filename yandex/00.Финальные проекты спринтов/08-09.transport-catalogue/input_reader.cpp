#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>

using input::CmdType;
using input::CmdInfo;

using transport::StopDistances;

/**
 * Парсит строку вида "10.123,  -30.1837, ... " и возвращает пару координат (широта, долгота)
 */
geo::Coordinates ParseCoordinates(std::string_view str) {
    static const double nan = std::nan("");
    
    auto not_space = str.find_first_not_of(' ');
    auto comma = str.find(',');
    
    if (comma == str.npos) {
        return {nan, nan};
    }
    
    auto not_space2 = str.find_first_not_of(' ', comma + 1);
    //second comma for cutoff
    auto comma2 = str.find(',', comma+1);
    
    double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));
    double lng = comma2 == str.npos
    ? std::stod(std::string(str.substr(not_space2))) //if no stops dists after coord
    : std::stod(std::string(str.substr(not_space2, comma2 - not_space2)));
    
    return {lat, lng};
}

/**
 * Удаляет пробелы в начале и конце строки
 */
std::string_view Trim(std::string_view string) {
    const auto start = string.find_first_not_of(' ');
    if (start == string.npos) {
        return {};
    }
    return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/**
 * Разбивает строку string на n строк, с помощью указанного символа-разделителя delim
 */
std::vector<std::string_view> Split(std::string_view string, char delim) {
    std::vector<std::string_view> result;
    
    size_t pos = 0;
    while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
        auto delim_pos = string.find(delim, pos);
        if (delim_pos == string.npos) {
            delim_pos = string.size();
        }
        if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty()) {
            result.push_back(substr);
        }
        pos = delim_pos + 1;
    }
    
    return result;
}

/**
 * Парсит маршрут.
 * Для кольцевого маршрута (A>B>C>A) возвращает массив названий остановок [A,B,C,A]
 * Для некольцевого маршрута (A-B-C-D) возвращает массив названий остановок [A,B,C,D,C,B,A]
 */
std::vector<std::string_view> ParseRoute(std::string_view route) {
    if (route.find('>') != route.npos) {
        return Split(route, '>');
    }
    
    auto stops = Split(route, '-');
    std::vector<std::string_view> results(stops.begin(), stops.end());
    results.insert(results.end(), std::next(stops.rbegin()), stops.rend());
    
    return results;
}

std::pair<std::string_view, int> ParseDistAndName(std::string_view str) {
    //Format:"  D1m  to  stop  "
    size_t m_symbol = str.find('m');
    
    if (m_symbol == str.npos) {
        std::cerr << "*** Incorrect stop distance format" << std::endl;
        return {};
    }
    
    Trim(str);
    int dist = std::stoi(std::string(str.substr(0, m_symbol)));
    
    size_t to_symbol = str.find("to");
    
    if (to_symbol == str.npos) {
        std::cerr << "*** Incorrect stop distance format" << std::endl;
        return {};
    }
    
    str.remove_prefix(to_symbol+2);
    
    return std::make_pair(Trim(str), dist);
}

StopDistances ParseStopDistances(std::string_view stops_dist_str) {
    // [Stop X: latitude, longitude,] D1m to stop1, D2m to stop2, ...
    auto stops_distances_strings = Split(stops_dist_str, ',');
    StopDistances distances_to_stops;
    
    for(const auto& dist_and_name_str : stops_distances_strings) {
        distances_to_stops.push_back(ParseDistAndName(dist_and_name_str));
    }
    return distances_to_stops;
}

CmdType StrToCmdType(std::string_view command) {
    if(command == input::NEW_STOP) {
        return CmdType::NewStop;
    } else if(command == input::NEW_ROUTE) {
        return CmdType::NewRoute;
    } else if(command.empty()){
        return CmdType::Empty;
    } else {
        return CmdType::Unknown;
    }
}

CmdInfo ParseCommandDescription(std::string_view line, bool get_distances = false) {
    auto colon_pos = line.find(':');
    if (colon_pos == line.npos) {
        return {};
    }
    
    auto space_pos = line.find(' ');
    if (space_pos >= colon_pos) {
        return {};
    }
    
    auto not_space = line.find_first_not_of(' ', space_pos);
    if (not_space >= colon_pos) {
        return {};
    }
    
    auto cmd_type = StrToCmdType(line.substr(0, space_pos));
    std::string id = std::string(line.substr(not_space, colon_pos - not_space));
    std::string_view description = line.substr(colon_pos + 1);
    
    if(cmd_type == CmdType::NewStop) {
        //Stop X: latitude, longitude, D1m to stop1, D2m to stop2, ...
        auto first_comma = line.find(',', colon_pos);
        auto second_comma = line.find(',', first_comma+1);
        
        if(get_distances) {
            //invalid only if distances are expected:
            if(second_comma == line.npos) {
                return {};
            }
            
            description = line.substr(second_comma + 1); //pass distances
            //change cmd type
            cmd_type = CmdType::AddStopDist;
        } else {
            description = second_comma == line.npos
            ? line.substr(colon_pos + 1) //distances were not included in command
            : line.substr(colon_pos + 1, second_comma - (colon_pos + 1)); //coords excluding dists
        }
    }
    
    return { cmd_type, std::string(id), std::string(description) };
}

CmdInfo::CmdInfo(CmdType type, std::string id, std::string description)
: type(type), id(id), description(description) {
    if(id.empty() || description.empty()) {
        type = CmdType::Empty;
    }
}

void InputReader::ParseLine(std::string_view line) {
    auto command = ParseCommandDescription(line);
    if(command) {
        commands_.push_back(std::move(command));
    }
    //push back & parse a second time to get distances
    if(command.type == CmdType::NewStop) {
        //set bool param to true to get distances string
        auto dist_command = ParseCommandDescription(line, true);
        if(dist_command) {
            commands_.push_back(std::move(dist_command));
        }
    }
}

void InputReader::ApplyCommands([[maybe_unused]] TransportCatalogue& catalogue) {
    std::sort(commands_.begin(), commands_.end(),
              [](const CmdInfo& lhs, const CmdInfo& rhs) {
        return lhs.type < rhs.type;
    });
    
    for(auto& cmd : commands_) {
        switch (cmd.type) {
            case CmdType::NewStop:
                catalogue.AddStop(std::move(cmd.id), ParseCoordinates(cmd.description));
                break;
            case CmdType::NewRoute:
                catalogue.AddRoute(std::move(cmd.id), ParseRoute(cmd.description));
                break;
            case CmdType::AddStopDist:
                //string_views passed to function stay valid until end of cycle
                catalogue.AddRoadDistances(cmd.id, ParseStopDistances(cmd.description));
                //NB. All stops guaranteed to be present in the same input
                break;
                
            case CmdType::Empty:
                std::cerr << "*** Empty command during processing" << std::endl;
                break;

            default:
                break;
                
        }
    }
}

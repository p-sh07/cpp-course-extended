#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <deque>

#include "geo.h"
#include "transport_catalogue.h"

namespace input {

static const std::string NEW_STOP = "Stop";
static const std::string NEW_ROUTE = "Bus";

// Defines command priority
enum class CmdType {
    NewStop      = 1,
    NewRoute     = 2,
    AddStopDist  = 3,
    Unknown      = 100,
    Empty        = 101,
};

// Stores all command data
struct CmdInfo {
    CmdInfo(CmdType type = CmdType::Empty, std::string id = "", std::string description = "");
    // check if command is valid/not empty
    explicit operator bool() const {
        return type != CmdType::Empty
        && !id.empty()
        && !description.empty();
    }
    
    bool operator!() const {
        return !operator bool();
    }
    
    CmdType type;             // Command type
    std::string id;           // route or stop id
    std::string description;  // other parameters
};

}

class InputReader {
public:
    /**
     * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
     */
    void ParseLine(std::string_view line);

    /**
     * Наполняет данными транспортный справочник, используя команды из commands_
     */
    void ApplyCommands(TransportCatalogue& catalogue);

private:
    std::vector<input::CmdInfo> commands_;
};

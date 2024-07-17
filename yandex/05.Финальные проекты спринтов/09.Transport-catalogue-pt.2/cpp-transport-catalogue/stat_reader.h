#pragma once

#include <iosfwd>
#include <iomanip>
#include <string>
#include <string_view>

#include "transport_catalogue.h"

namespace stats {

static const std::string GET_ROUTE_INFO = "Bus";
static const std::string GET_STOP_INFO = "Stop";

void ParseAndPrintStat(const TransportCatalogue& tansport_catalogue, std::string_view request,
                       std::ostream& output);

}


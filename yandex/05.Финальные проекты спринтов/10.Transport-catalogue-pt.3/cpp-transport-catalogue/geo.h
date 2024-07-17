#pragma once

#include <cmath>

namespace geo {
struct Coord {
    double lat;
    double lng;
    bool operator==(const Coord& other) const {
        return lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Coord& other) const {
        return !(*this == other);
    }
};

double ComputeDistance(Coord from, Coord to);
}

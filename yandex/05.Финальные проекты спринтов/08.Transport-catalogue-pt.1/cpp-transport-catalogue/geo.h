#pragma once

#include <cmath>

namespace geo {
struct Coordinates {
    //limit the coordinates to 6 digits from decimal point
//    Coordinates(double x, double y) {
//        double decimals = 1000000.0;
//        lat = std::floor(x * decimals) / decimals;
//        lng = std::floor(y * decimals) / decimals;
//    }
    
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const {
        return lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Coordinates& other) const {
        return !(*this == other);
    }
};

inline double ComputeDistance(Coordinates from, Coordinates to) {
    using namespace std;
    if (from == to) {
        return 0;
    }
    static const double dr = 3.1415926535 / 180.;
    return acos(sin(from.lat * dr) * sin(to.lat * dr)
                + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
    * 6371000;
}
}

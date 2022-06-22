#pragma once

#include <set>
#include <string>
#include <vector>

namespace domain
{

namespace MainReq{
using namespace std::literals;
    const std::string base = "base_requests"s;
    const std::string stat = "stat_requests"s;

    const std::string type = "type"s;

    const std::string bus = "Bus"s;
    const std::string stops = "stops"s;
    const std::string is_roundtrip = "is_roundtrip"s;

    const std::string stop = "Stop"s;
    const std::string name = "name"s;
    const std::string lat = "latitude"s;
    const std::string lon = "longitude"s;
    const std::string road_distances = "road_distances"s;

    const std::string id = "id"s;

}

struct Request{
    std::string type;
    std::string data;
};

struct RequestOut{
    int id;
    std::string type;
    std::string name;
};

struct BusStat{
    std::string name;
    std::size_t count_stops = 0;
    std::size_t count_unic_stops = 0;
    std::size_t length = 0;
    double curvature = 0;
};

double ComputeDistance(double from_lat, double from_lng, double to_lat, double to_lng);

struct StopInfo{
    const std::string& name;
    bool b_stop_is_not_exist = false;
    std::optional<std::set<std::string>> buses;
};

struct Stop{
    std::string name;
    double lat;
    double lng;
};

struct Bus{
    std::string name;
    std::vector<const Stop*> stops;
};

struct StopsLenght{
    std::string from_stop;
    std::string to_stop;
    size_t lenght;
};

}//namespace domain

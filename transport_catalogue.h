#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <math.h>

namespace TransportCatalogue {

namespace detail {

struct Request{
    std::string type;
    std::string data;
};

struct BusInfo{
    std::string name;
    std::size_t count_stops = 0;
    std::size_t count_unic_stops = 0;
    size_t length = 0;
    double curvature = 0;
};

struct StopInfo{
    const std::string& name;
    bool b_stop_is_not_exist = false;
    std::optional<std::set<std::string>> buses;
};

struct Bus;
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

class HasherStopes{
public:
    size_t operator()(std::pair<const Stop*, const Stop*> par) const {
        size_t from_stop = std::hash<const void*>{}(par.first);
        size_t to_stop = std::hash<const void*>{}(par.second);
        return from_stop*37 + to_stop*pow(37,2);
    }
};
}// namespace detail

class TransportCatalogue
{
public:
    TransportCatalogue();

    void AddBus(const detail::Bus& bus);

    void AddStop(const detail::Stop& stop);

    void AddRangeStops(const detail::StopsLenght& stops_lenght);

    void AddBusesFromStop(const std::string &bus_name, const std::vector<std::string>& buses_from_stop );

    detail::BusInfo GetBusInfo(const detail::Bus* bus) const;

    detail::StopInfo GetStopInfo(const detail::Stop* stop) const;

    const std::deque<detail::Stop> GetStops() const { // для тестов
        return stops_;
    };
    const std::deque<detail::Bus> GetBuses() const {// для тестов
        return buses_;
    };

    std::size_t GetRangeStops(const detail::Stop* from_stop, const detail::Stop* to_stop) const ;

    std::optional<const detail::Bus*> FindBus(const std::string& bus_name) const ;

    std::optional<const detail::Stop*> FindStop(const std::string& stop_name) const ;

private:
    std::deque<detail::Stop> stops_;

    std::unordered_map<std::string_view, const detail::Stop*> index_stops_;

    std::deque<detail::Bus> buses_;

    std::map<std::string, std::set<std::string>> buses_from_stop_;

    std::unordered_map<std::string_view, const detail::Bus*> index_buses_;

    std::unordered_map<std::pair<const detail::Stop*, const detail::Stop*>, size_t, detail::HasherStopes> index_rage_;
};

}// namespace TransportCatalogue


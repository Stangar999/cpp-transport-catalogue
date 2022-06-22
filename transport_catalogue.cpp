#include <algorithm>
#include <cassert>
#include <set>

#include "transport_catalogue.h"

using namespace std;

namespace TransportCatalogue {
//----------------------------------------------------------------------------
TransportCatalogue::TransportCatalogue()
{

}
//----------------------------------------------------------------------------
void TransportCatalogue::AddStop(const Stop& stop) {
    stops_.push_back(move(stop));
    index_stops_[stops_.back().name] = &stops_.back();
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddRangeStops(const StopsLenght& stops_lenght) {
    if(index_stops_.find(stops_lenght.from_stop) != index_stops_.end()
       && index_stops_.find(stops_lenght.to_stop) != index_stops_.end())
    {
        index_rage_[pair(index_stops_.at(stops_lenght.from_stop), index_stops_.at(stops_lenght.to_stop))] = stops_lenght.lenght;
    } else {
        cerr << "index_stops_.find(stops_lenght.from/to_stop) == index_stops_.end()";
    }
}
//----------------------------------------------------------------------------
//void TransportCatalogue::AddBusesFromStop(const std::string& bus_name, const std::vector<std::string>& buses_from_stop ) {
//    for(const auto& stop: buses_from_stop){
//        buses_from_stop_[move(stop)].insert(bus_name);
//    }
//}
//----------------------------------------------------------------------------
void TransportCatalogue::AddBusesFromStop(const Bus& bus) {
    for(const auto& stop: bus.stops){
        //buses_from_stop_[stop->name].insert(bus.name);
        buses_from_stop_[stop->name].insert(&bus);
    }
}
//----------------------------------------------------------------------------
size_t TransportCatalogue::GetRangeStops(const Stop* from_stop, const Stop* to_stop) const {
    size_t result = 0;
    if(index_rage_.count(pair(from_stop, to_stop)) != 0 ){
        result = index_rage_.at(pair(from_stop, to_stop));
    } else if(index_rage_.count(pair(to_stop, from_stop)) != 0 ){
        result = index_rage_.at(pair(to_stop, from_stop));
    }
    return result;
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddBus(const Bus& bus) {
    buses_.push_back(move(bus));
    index_buses_[buses_.back().name] = &buses_.back();

    AddBusesFromStop(buses_.back());
}
//----------------------------------------------------------------------------
std::optional<const Bus*> TransportCatalogue::FindBus(std::string_view bus_name) const {
    if(index_buses_.count(bus_name) == 0){
        return nullopt;
    }
    return index_buses_.at(bus_name);
}
//----------------------------------------------------------------------------
std::optional<const Stop*> TransportCatalogue::FindStop(std::string_view stop_name) const {
    if(index_stops_.count(stop_name) == 0){
        return nullopt;
    }
    return index_stops_.at(stop_name);
}
//----------------------------------------------------------------------------
//const std::map<std::string_view, std::set<std::string_view>>& TransportCatalogue::GetBusesFromStop() const
//{
//    return buses_from_stop_;
//}
const std::map<std::string_view, std::unordered_set<const Bus*>>& TransportCatalogue::GetBusesFromStop() const
{
    return buses_from_stop_;
}
//----------------------------------------------------------------------------
//BusStat TransportCatalogue::GetBusInfo(const Bus* bus) const {
//    size_t coutn_stops = bus->stops.size();
//    if(coutn_stops < 2){
//        throw "coutn_stops < 2";
//    }
//    size_t length = 0;
//    double range = 0;
//    set<const Stop*> stops(bus->stops.begin(), bus->stops.end());
//    for(size_t i = 0, j = 1; j < coutn_stops; ++i, ++j){
//        const Stop* from_stop  = bus->stops[i];
//        const Stop* to_stop = bus->stops[j];
//        length += GetRangeStops(from_stop, to_stop);
//        range += domain::ComputeDistance( from_stop->lat, from_stop->lng,
//                                            to_stop->lat, to_stop->lng );
//    }
//    return {bus->name, coutn_stops, stops.size(), length, length / range};
//}
//----------------------------------------------------------------------------
//StopInfo TransportCatalogue::GetStopInfo(const Stop* stop) const {
//    if(buses_from_stop_.count(stop->name) != 0 ){
//        return {stop->name, false, buses_from_stop_.at(stop->name)};
//    } else {
//        return {stop->name, false, nullopt};
//    }
//}
//----------------------------------------------------------------------------
} // namespace TransportCatalogue

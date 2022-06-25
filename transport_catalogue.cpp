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
void TransportCatalogue::AddBusesFromStop(const Bus& bus) {
    for(const auto& stop: bus.stops){
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
const std::map<std::string_view, std::unordered_set<const Bus*>>& TransportCatalogue::GetBusesFromStop() const
{
    return buses_from_stop_;
}
//----------------------------------------------------------------------------
const std::deque<Bus>& TransportCatalogue::GetBuses() const
{
    return buses_;
}
//----------------------------------------------------------------------------
} // namespace TransportCatalogue

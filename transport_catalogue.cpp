#include <algorithm>
#include <cassert>
#include <set>

#include "transport_catalogue.h"
#include "geo.h"

using namespace std;

namespace TransportCatalogue {
//----------------------------------------------------------------------------
TransportCatalogue::TransportCatalogue()
{

}
//----------------------------------------------------------------------------
void TransportCatalogue::AddStop(detail::Stop stop) {
    stops_.push_back(move(stop));
    index_stops_[stops_.back().name] = &stops_.back();
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddRangeStops(detail::StopsLenght stops_lenght)
{
    try {
        index_rage_[pair(index_stops_.at(stops_lenght.from_stop), index_stops_.at(stops_lenght.to_stop))] = stops_lenght.lenght;
    } catch (...) {
        cout << "index_stops.at(name_stop)";
    }
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddBuses_from_stop( std::string bus_name, std::vector<std::string> buses_from_stop )
{
    for(const auto& stop: buses_from_stop){
        buses_from_stop_[move(stop)].insert(bus_name);
    }
}
//----------------------------------------------------------------------------
size_t TransportCatalogue::GetRangeStops(const detail::Stop* from_stop, const detail::Stop* to_stop) const
{
    size_t result = 0;
    if(index_rage_.count(pair(from_stop, to_stop)) != 0 ){
        result = index_rage_.at(pair(from_stop, to_stop));
    } else if(index_rage_.count(pair(to_stop, from_stop)) != 0 ){
        result = index_rage_.at(pair(to_stop, from_stop));
    }
    return result;
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddBus(detail::Bus bus) {
    buses_.push_back(bus);
    index_buses_[buses_.back().name] = &buses_.back();
}
//----------------------------------------------------------------------------
std::optional<const detail::Bus*> TransportCatalogue::FindBus(const std::string &bus_name) const
{
    if(index_buses_.count(bus_name) == 0){
        return nullopt;
    }
    return index_buses_.at(bus_name);
}
//----------------------------------------------------------------------------
std::optional<const detail::Stop*> TransportCatalogue::FindStop(const std::string &stop_name) const
{
    if(index_stops_.count(stop_name) == 0){
        return nullopt;
    }
    return index_stops_.at(stop_name);
}
//----------------------------------------------------------------------------
detail::BusInfo TransportCatalogue::GetBusInfo(const detail::Bus* bus) const
{
    size_t coutn_stops = bus->stops.size();
    if(coutn_stops < 2){
        throw "coutn_stops < 2";
    }
    size_t length = 0;
    double range = 0;
    set<const detail::Stop*> stops(bus->stops.begin(), bus->stops.end());
    for(size_t i = 0, j = 1; j < coutn_stops; ++i, ++j){
        const detail::Stop* from_stop  = bus->stops[i];
        const detail::Stop* to_stop = bus->stops[j];
        length += GetRangeStops(from_stop, to_stop);
        range += detail::ComputeDistance( {from_stop->lat, from_stop->lng},
                                  {to_stop->lat, to_stop->lng} );
    }
    return {bus->name, coutn_stops, stops.size(), length, length / range};
}
//----------------------------------------------------------------------------
detail::StopInfo TransportCatalogue::GetStopInfo(const detail::Stop* stop) const
{
    if(buses_from_stop_.count(stop->name) != 0 ){
        return {stop->name, false, buses_from_stop_.at(stop->name)};
    } else {
        return {stop->name, false, nullopt};
    }

}
//----------------------------------------------------------------------------
} // namespace TransportCatalogue

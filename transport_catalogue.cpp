#include <algorithm>
#include <cassert>
#include <set>

#include "transport_catalogue.h"
#include "geo.h"

using namespace std;

namespace TransportCatalogue {
//----------------------------------------------------------------------------
TransportCatalogue::TransportCatalogue(std::vector<Request>&& requests)
{
    auto compare = [](const Request& lh, const Request& rh){
        return lh.type > rh.type;
    };
    sort(requests.begin(), requests.end(), compare);
    for (auto& request : requests){
        if (request.type == "Stop"){
            AddStop(move(request.data));
        } else{
            AddBus(move(request.data));
        }
    }
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddStop(std::string&& line) {
    // пробелы в начале строки удалены в input_reader
    // название остановки
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_value = line.substr(pos_s, pos_e);
    // широта
    pos_s = line.find_first_not_of(' ', pos_e + 1);
    pos_e = line.find_first_of(',', pos_s);
    double shir = stod(line.substr(pos_s, pos_e - pos_s));
    // долгота
    pos_s = line.find_first_not_of(' ', pos_e + 1);
    pos_e = line.find_first_of(' ', pos_s);
    pos_e = pos_e == -1 ? line.size() : pos_e; // что бы и пробелы удалять и последнюю цифру не отрезать
    double dol = stod(line.substr(pos_s, pos_e - pos_s));

    stops_.push_back({name_value, shir, dol, {}});
    index_stops_[move(name_value)] = &stops_.back();
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddBus(std::string&& line) {
    // пробелы в начале строки удалены в input_reader
    // название маршрута
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_bus = line.substr(pos_s, pos_e);
    std::vector<const Stop*> stops;
    int ich = line.find('>');
    char ch = ich > 0 ? '>' : '-';
    while(pos_e > 0){
        pos_s = line.find_first_not_of(' ', pos_e + 1);
        int pos_ch = line.find_first_of(ch, pos_s);
        pos_e = line.find_last_not_of(' ', pos_ch - 1);
        string name_stop = line.substr(pos_s, (pos_e + 1) - pos_s);
        pos_e = pos_ch;
        try {
           const Stop* ptr_stop = index_stops_.at(name_stop);
           const_cast<Stop*>(ptr_stop)->buses.insert(name_bus);
           stops.push_back(ptr_stop);
        } catch (...) {
            cout << "index_stops.at(name_stop)";
        }
    }
    if(ch == '-'){
        size_t size = stops.size();
        stops.reserve(size*2 - 1);
        std::vector<const Stop*>::iterator it = stops.end() - 2;
        for(size_t i = 0; i < size -1; ++i, --it){
            stops.push_back(*it);
        }
    }
    buses_.push_back({name_bus, move(stops)});
    index_buses_[move(name_bus)] = &buses_.back();
}
//----------------------------------------------------------------------------
std::optional<const Bus*> TransportCatalogue::FindBus(const std::string &bus_name) const
{
    if(!index_buses_.count(bus_name)){
        return nullopt;
    }
    return index_buses_.at(bus_name);
}
//----------------------------------------------------------------------------
std::optional<const Stop*> TransportCatalogue::FindStop(const std::string &stop_name) const
{
    if(!index_stops_.count(stop_name)){
        return nullopt;
    }
    return index_stops_.at(stop_name);
}
//----------------------------------------------------------------------------
BusInfo TransportCatalogue::GetBusInfo(const std::string& name_bus) const
{
    auto opt_bus = FindBus(name_bus);
    if(!opt_bus.has_value()){
        return {name_bus};
    }
    const Bus& bus = *opt_bus.value();
    size_t coutn_stops = bus.stops.size();
    assert(coutn_stops >= 2);
    double range = 0;
    set<const Stop*> stops(bus.stops.begin(), bus.stops.end());
    for(size_t i = 0; (i + 1) < coutn_stops; ++i){
        range += ComputeDistance( {bus.stops[i]->shir, bus.stops[i]->dolg},
                                  {bus.stops[i + 1]->shir, bus.stops[i + 1]->dolg} );
    }
    return {name_bus, coutn_stops, stops.size(), range};
}
//----------------------------------------------------------------------------
StopInfo TransportCatalogue::GetStopInfo(const std::string &name_stop) const
{
    auto opt_stop = FindStop(name_stop);
    if(!opt_stop.has_value()){
        return {name_stop, true, {} };
    }
    const Stop& stop = *opt_stop.value();
    return {name_stop, false, stop.buses};
}
//----------------------------------------------------------------------------
} // namespace TransportCatalogue

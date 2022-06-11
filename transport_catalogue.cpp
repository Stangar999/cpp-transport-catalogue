#include <algorithm>
#include <cassert>
#include <set>

#include "transport_catalogue.h"
#include "geo.h"

using namespace std;

namespace TransportCatalogue {
//----------------------------------------------------------------------------
TransportCatalogue::TransportCatalogue(std::vector<InputReader::detail::Request>&& requests)
{
    // для выставления запросов stop перед bus
    auto compare = [](const InputReader::detail::Request& lh, const InputReader::detail::Request& rh){
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
    for (auto& request : requests){
        if (request.type == "Stop"){
            SetRangeStops(move(request.data));
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
    double dol = 0;
    pos_s = line.find_first_not_of(' ', pos_e + 1);
    if(pos_e = line.find_first_of(',', pos_s); pos_e != -1){
        dol = stod(line.substr(pos_s, pos_e - pos_s));
    }else{
        pos_e = line.find_first_of(' ', pos_s);
        pos_e = pos_e == -1 ? line.size() : pos_e; // что бы и пробелы удалять и последнюю цифру не отрезать
        dol = stod(line.substr(pos_s, pos_e - pos_s));
    }

    stops_.push_back({move(name_value), shir, dol, {}});
    index_stops_[stops_.back().stop] = &stops_.back();
}
//----------------------------------------------------------------------------
void TransportCatalogue::SetRangeStops(std::string&& line)
{
    // пробелы в начале строки удалены в input_reader
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_stop1 = line.substr(pos_s, pos_e);
    pos_e = line.find_first_of(',');
    assert(pos_e != -1);// одна запятая должна быть
    pos_e = line.find_first_of(',', pos_e + 1);
    // пропускаем если дальности не заданы
    if(pos_e == -1){
        return;
    }
    string range;
    while(pos_e > 0){
        pos_s = line.find_first_not_of(' ', pos_e + 1);
        pos_e = line.find_first_of('m', pos_s);
        range = line.substr(pos_s, pos_e - pos_s);
        pos_s = line.find_first_of(" to ", pos_e);
        pos_s = line.find_first_not_of(' ', pos_s + 4);// 4 - " to " колличество символов
        pos_e = line.find_first_of(',', pos_s);
        string name_stop2;
        if(pos_e == -1){
            pos_e = line.find_last_not_of(" ");
            name_stop2 = line.substr(pos_s, (pos_e + 1) - pos_s);
            pos_e = -1;// возвращаем для выхода из цикла
        }else{
            name_stop2 = line.substr(pos_s, pos_e - pos_s);
        }
        try {
            index_rage_[pair(index_stops_.at(name_stop1), index_stops_.at(name_stop2))] = stoll(range);
            //int a = 0;
        } catch (...) {
            cout << "index_stops.at(name_stop)";
        }
    }
    //int a = 0;
}
//----------------------------------------------------------------------------
size_t TransportCatalogue::GetRangeStops(const detail::Stop* stop1, const detail::Stop* stop2) const
{
    size_t result = 0;
    if(index_rage_.count(pair(stop1, stop2))){
        result = index_rage_.at(pair(stop1, stop2));
    } else if(index_rage_.count(pair(stop2, stop1))){
        result = index_rage_.at(pair(stop2, stop1));
    }
    return result;
}
//----------------------------------------------------------------------------
void TransportCatalogue::AddBus(std::string&& line) {
    // пробелы в начале строки удалены в input_reader
    // название маршрута
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_bus = line.substr(pos_s, pos_e);
    std::vector<const detail::Stop*> stops;
    int ich = line.find('>');
    char ch = ich > 0 ? '>' : '-';
    while(pos_e > 0){
        pos_s = line.find_first_not_of(' ', pos_e + 1);
        int pos_ch = line.find_first_of(ch, pos_s);
        pos_e = line.find_last_not_of(' ', pos_ch - 1);
        string name_stop = line.substr(pos_s, (pos_e + 1) - pos_s);
        pos_e = pos_ch;
        try {
           const detail::Stop* ptr_stop = index_stops_.at(name_stop);
           const_cast<detail::Stop*>(ptr_stop)->buses.insert(name_bus);// заполняем перед помещением в дек, можно ли переделать под strig_view?
           stops.push_back(ptr_stop);
        } catch (...) {
            cout << "index_stops.at(name_stop)";
        }
    }
    if(ch == '-'){
        size_t size = stops.size();
        stops.reserve(size*2 - 1);
        std::vector<const detail::Stop*>::iterator it = stops.end() - 2;
        for(size_t i = 0; i < size -1; ++i, --it){
            stops.push_back(*it);
        }
    }
    buses_.push_back({move(name_bus), move(stops)});
    index_buses_[buses_.back().bus] = &buses_.back();
}
//----------------------------------------------------------------------------
std::optional<const detail::Bus*> TransportCatalogue::FindBus(const std::string &bus_name) const
{
    if(!index_buses_.count(bus_name)){
        return nullopt;
    }
    return index_buses_.at(bus_name);
}
//----------------------------------------------------------------------------
std::optional<const detail::Stop*> TransportCatalogue::FindStop(const std::string &stop_name) const
{
    if(!index_stops_.count(stop_name)){
        return nullopt;
    }
    return index_stops_.at(stop_name);
}
//----------------------------------------------------------------------------
InputReader::detail::BusInfo TransportCatalogue::GetBusInfo(const std::string& name_bus) const
{
    auto opt_bus = FindBus(name_bus);
    if(!opt_bus.has_value()){
        return {name_bus};
    }
    const detail::Bus& bus = *opt_bus.value();
    size_t coutn_stops = bus.stops.size();
    assert(coutn_stops >= 2);
    size_t length = 0;
    double range = 0;
    set<const detail::Stop*> stops(bus.stops.begin(), bus.stops.end());
    for(size_t i = 0, j = 1; j < coutn_stops; ++i, ++j){
        const detail::Stop* stop1 = bus.stops[i];
        const detail::Stop* stop2 = bus.stops[j];
        length += GetRangeStops(stop1, stop2);
        range += detail::ComputeDistance( {stop1->shir, stop1->dolg},
                                  {stop2->shir, stop2->dolg} );
    }
    return {name_bus, coutn_stops, stops.size(), length, length / range};
}
//----------------------------------------------------------------------------
InputReader::detail::StopInfo TransportCatalogue::GetStopInfo(const std::string &name_stop) const
{
    auto opt_stop = FindStop(name_stop);
    if(!opt_stop.has_value()){
        return {name_stop, true, {} };
    }
    const detail::Stop& stop = *opt_stop.value();
    return {name_stop, false, stop.buses};
}
//----------------------------------------------------------------------------
} // namespace TransportCatalogue

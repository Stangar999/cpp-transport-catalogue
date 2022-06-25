#include "request_handler.h"

//----------------------------------------------------------------------------
RequestHandler::RequestHandler(const TransportCatalogue::TransportCatalogue& db, renderer::MapRenderer& renderer)
    :db_(db)
    ,renderer_(renderer)
{

}
//----------------------------------------------------------------------------
std::optional<domain::BusStat> RequestHandler::GetBusStat(const std::string_view& bus_name) const
{
    if(auto opt_bus = db_.FindBus(bus_name); opt_bus){
        return CreateBusStat(opt_bus.value());
    } else {
        return std::nullopt;
    }
}
//----------------------------------------------------------------------------
std::optional< const std::unordered_set<const domain::Bus*>* > RequestHandler::GetBusesByStop(const std::string_view& stop_name) const
{
    if(auto opt_stop = db_.FindStop(stop_name); opt_stop){
        if(db_.GetBusesFromStop().count(stop_name) != 0 ){
            return &db_.GetBusesFromStop().at(stop_name) ;
        } else {
            static std::unordered_set<const domain::Bus*> emp{};
            return &emp;
        }
    } else {
        // если остановок с таким именем нет
        return std::nullopt;
    }

}
//----------------------------------------------------------------------------
std::vector<const domain::Bus*> RequestHandler::GetBusesLex() const
{
    // использую сет как фильтр уникальных и сортировщик, кладу в вектор так как потом надо будет только итерироваться
    std::set<const domain::Bus*, domain::CmpBuses> set;
    for(const auto& bus : db_.GetBuses()){
        set.insert(&bus);
    }
    return {set.begin(), set.end()};
}
//----------------------------------------------------------------------------
const std::vector<const domain::Stop*> RequestHandler::GetUnicLexStopsIncludeBuses() const
{
    // использую сет как фильтр уникальных и сортировщик, кладу в вектор так как потом надо будет только итерироваться
    std::set<const domain::Stop*, domain::CmpStops> set;
    for(const auto& bus : db_.GetBuses()){
        for(const auto& stop : bus.stops){
            set.insert(stop);
        }
    }
    return {set.begin(), set.end()};
}
//----------------------------------------------------------------------------
svg::Document RequestHandler::RenderMap() const
{
    return renderer_.GetDocMapBus();
}
//----------------------------------------------------------------------------
domain::BusStat RequestHandler::CreateBusStat(const domain::Bus* bus) const
{
    size_t coutn_stops = bus->stops.size();
    if(coutn_stops < 2){
        throw "coutn_stops < 2";
    }
    size_t length = 0;
    double range = 0;
    std::set<const domain::Stop*> stops(bus->stops.begin(), bus->stops.end());
    for(size_t i = 0, j = 1; j < coutn_stops; ++i, ++j){
        const domain::Stop* from_stop  = bus->stops[i];
        const domain::Stop* to_stop = bus->stops[j];
        length += db_.GetRangeStops(from_stop, to_stop);
        range += domain::ComputeDistance( from_stop->lat, from_stop->lng,
                                            to_stop->lat, to_stop->lng );
    }
    return {bus->name, coutn_stops, stops.size(), length, length / range};
}
//----------------------------------------------------------------------------

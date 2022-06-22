#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */
namespace RequestHandler
{
//----------------------------------------------------------------------------
RequestHandler::RequestHandler(const TransportCatalogue::TransportCatalogue& db)
    :db_(db)
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

//std::optional< const std::set<std::string_view>* > RequestHandler::GetBusesByStop(const std::string_view& stop_name) const
//{
//    if(auto opt_stop = db_.FindStop(stop_name); opt_stop){
//        if(db_.GetBusesFromStop().count(stop_name) != 0 ){
//            return &db_.GetBusesFromStop().at(stop_name) ;
//        } else {
//            static std::set<std::string_view> emp{};
//            return &emp;
//        }
//    } else {
//        // если остановок с таким именем нет
//        return std::nullopt;
//    }

//}
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
}

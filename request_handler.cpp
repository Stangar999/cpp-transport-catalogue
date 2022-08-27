#include "request_handler.h"
#include "serialization.h"

//----------------------------------------------------------------------------
RequestHandler::RequestHandler( TransportCatalogue::TransportCatalogue& db,
                               TransportRouter::TransportRouter& tr,
                               renderer::MapRenderer& renderer)
    :db_(db)
    ,tr_(tr)
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
std::optional<domain::RoutStat> RequestHandler::GetRouteStat(std::string_view stop_from, std::string_view stop_to) const
{
    if(tr_.GetGraphIsNoInit()) {
        tr_.CreateGraph(db_);
    }
    return tr_.GetRouteStat(db_.FindStop(stop_from).value()->id, db_.FindStop(stop_to).value()->id);
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
    return db_.GetBusesLex();
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
void RequestHandler::CallDsrlz(const std::filesystem::path& path)
{
    Serialization d_srlz;
    d_srlz.Deserialize(path, db_);
}
//----------------------------------------------------------------------------
void RequestHandler::CallSrlz(const std::filesystem::path& path) const
{
    Serialization srlz;
    srlz.Serialize(db_, path);
}
//----------------------------------------------------------------------------
domain::BusStat RequestHandler::CreateBusStat(const domain::Bus* bus) const
{
   // изначально подумал о переносе в структуру Bus или BusStat
   // но пернес в TransportCatalogue так как в методе используется из него GetRangeStops
   // а делать зависимость Bus или BusStat от TransportCatalogue я подумал неверно

   return db_.GetBusStat(bus);
}
//----------------------------------------------------------------------------

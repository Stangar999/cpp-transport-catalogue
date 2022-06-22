#include <algorithm>
#include <ostream>
#include "json_reader.h"
#include "request_handler.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */
namespace JsonReader
{
//----------------------------------------------------------------------------
JsonReader::JsonReader(std::istream &in)
{
    using namespace domain;

    auto main_map = std::move(json::Load(in).GetRoot().AsMap());

    if(auto it = main_map.find(MainReq::base); it != main_map.end()){
        auto vec_map = std::move(it->second.AsArray());
        ParseRequestsBase(std::move(vec_map));
    } else {
        std::cout << MainReq::base + " empty" << std::endl;
    }
    if(auto it = main_map.find(MainReq::stat); it != main_map.end()){
        auto vec_map = std::move(it->second.AsArray());
        ParseRequestsStat(vec_map);
    } else {
        std::cout << MainReq::stat + " empty" << std::endl;
    }
    int a  = 0;
}
//----------------------------------------------------------------------------
void JsonReader::ParseRequestsBase(json::Array&& vec_map)
{
    using namespace domain;

    auto compare = [](const auto& lh, const auto& rh){
        return lh.AsMap().at(MainReq::type).AsString() > rh.AsMap().at(MainReq::type).AsString();
    };
    std::sort(vec_map.begin(), vec_map.end(), compare);

    for(const auto& map_type_data : vec_map){
        if(const auto& map_type_stop = map_type_data.AsMap(); map_type_stop.at(MainReq::type).AsString() == MainReq::stop){
            tr_cat_.AddStop(ParseRequestsStops(map_type_stop));
        } else {
            //auto [bus, buses_from_stop] = ParseRequestsBuses(map_type_data.AsMap());
            auto bus =  ParseRequestsBuses(map_type_data.AsMap());
            //tr_cat_.AddBusesFromStop(bus.name, buses_from_stop);
            tr_cat_.AddBus(bus);
        }
    }
    for (const auto& map_type_data : vec_map){
        if (const auto& map_type_stop = map_type_data.AsMap(); map_type_stop.at(MainReq::type).AsString() == MainReq::stop){
            ParseRequestsStopsLenght(move(map_type_stop));
        }
    }
}
//----------------------------------------------------------------------------
void JsonReader::ParseRequestsStat(const json::Array& vec_map)
{
    using namespace domain;

    std::vector<domain::RequestOut> requests;
    RequestOut result;
    requests.reserve(vec_map.size());
    for(const auto& req : vec_map){
        const auto cur_req = req.AsMap();
        requests.push_back( { std::move(cur_req.at(MainReq::id).AsInt()), std::move(cur_req.at(MainReq::type).AsString()), std::move(cur_req.at(MainReq::name).AsString()) } );
    }
    ExecRequestsStat(std::move(requests));
}
//----------------------------------------------------------------------------
void JsonReader::ExecRequestsStat(std::vector<domain::RequestOut>&& requests)
{
    json::Array vec;
    for(const auto& req : requests){
        RequestHandler::RequestHandler req_hand(tr_cat_);
        if(req.type == domain::MainReq::stop){
            vec.emplace_back(PrintResReqStop(req_hand.GetBusesByStop(req.name), req.id));
        } else {
            vec.emplace_back(PrintResReqBus(req_hand.GetBusStat(req.name), req.id));
        }
    }
    json::Print(json::Document{json::Node{vec}}, std::cout);
}
//----------------------------------------------------------------------------
domain::Stop JsonReader::ParseRequestsStops(const json::Dict &req)
{
    using namespace domain;
    try{
        return {req.at(MainReq::name).AsString(), req.at(MainReq::lat).AsDouble(), req.at(MainReq::lon).AsDouble()};
    } catch (...) {
        std::cout << "Fail Stop" << std::endl;
        throw;
    }
}
//----------------------------------------------------------------------------
domain::Bus JsonReader::ParseRequestsBuses(const json::Dict& req)
{
    using namespace domain;
    if(req.at(MainReq::type).AsString() == MainReq::bus){
        try{
            std::vector<const domain::Stop*> stops;
            //std::vector<std::string> buses_from_stop;
            for(const auto& stop_name : req.at(MainReq::stops).AsArray()){
                const auto& str_stop_name = stop_name.AsString();
                try {
                    //buses_from_stop.push_back(str_stop_name);
                    stops.push_back(tr_cat_.FindStop(str_stop_name).value());
                } catch (...) {
                    std::cout << "tc_.FindStop(name_stop).value()";
                }
            }
            // если не кольцевой маршрут дублируем остановки в обратном порядке
            if(!req.at(MainReq::is_roundtrip).AsBool()){
                size_t size = stops.size();
                stops.reserve(size*2 - 1);
                std::vector<const domain::Stop*>::iterator it = stops.end() - 2;
                for(size_t i = 0; i < size -1; ++i, --it){
                    stops.push_back(*it);
                }
            }
            return {req.at(MainReq::name).AsString(), move(stops)};
        } catch (...) {
            std::cout << "Fail Bus" << std::endl;
            throw;
        }
    } else {
        std::cout << "it's not bus" << std::endl;
        throw;
    }
}
//----------------------------------------------------------------------------
void JsonReader::ParseRequestsStopsLenght(const json::Dict& req)
{
    using namespace domain;
    const std::string& name_from_stop = req.at(MainReq::name).AsString();
    std::string range;
    try{
        for(const auto& rd : req.at(MainReq::road_distances).AsMap()){
            tr_cat_.AddRangeStops( {std::move(name_from_stop), rd.first, static_cast<size_t>(rd.second.AsInt())} );
        }
    } catch(...) {
        std::cout << "ParseRequestsStopsLenght FAIL" << std::endl;
        throw;
    }
}
//----------------------------------------------------------------------------
json::Dict JsonReader::PrintResReqBus(std::optional<domain::BusStat>&& bus_stat_opt, int id)
{
    using namespace std::literals;
    if(bus_stat_opt){
        const auto &[name, count_stops, count_unic_stops, lengh, curvature] = *bus_stat_opt;
        return json::Dict{{"curvature"s, curvature},
                       {"request_id"s, id },
                       {"route_length"s, static_cast<double>(lengh) },
                       {"stop_count"s, static_cast<double>(count_stops) },
                       {"unique_stop_count"s, static_cast<double>(count_unic_stops) } };
    } else {
        return json::Dict{{"request_id"s, id },
                       {"error_message"s, "not found" } };
    }
}
//----------------------------------------------------------------------------
//json::Dict JsonReader::PrintResReqStop(std::optional< const std::set<std::string_view>* > buses_opt, int id)
//{
//    using namespace std::literals;
//    if(buses_opt){
//        const auto& buses = *buses_opt;
//        json::Array vec;
//        vec.reserve(buses->size());
//        for(const auto& bus : *buses){
//            vec.push_back(std::string(bus));
//        }
//        return json::Dict{{"buses"s, vec},
//                       {"request_id"s, id } };
//        //json::Print(json::Document{json::Node{map}}, std::cout);
//    } else {
//        return json::Dict{{"request_id"s, id },
//                       {"error_message"s, "not found" } };
//        //json::Print(json::Document{json::Node{map}}, std::cout);
//    }
//}
json::Dict JsonReader::PrintResReqStop(std::optional< const std::unordered_set<const domain::Bus*>* > buses_opt, int id)
{
    using namespace std::literals;
    if(buses_opt){
        const auto& buses = *buses_opt;
        // для сортировки по возрастанию
        std::set<std::string_view> buss;
        for(const auto& bus : *buses){
            buss.insert(bus->name);
        }
        json::Array vec;
        vec.reserve(buses->size());
        for(const auto& bus : buss){
            vec.push_back(std::string(bus));
        }
        return json::Dict{{"buses"s, vec},
                       {"request_id"s, id } };
        //json::Print(json::Document{json::Node{map}}, std::cout);
    } else {
        return json::Dict{{"request_id"s, id },
                       {"error_message"s, "not found" } };
        //json::Print(json::Document{json::Node{map}}, std::cout);
    }
}
//----------------------------------------------------------------------------
}// namespace JsonReader


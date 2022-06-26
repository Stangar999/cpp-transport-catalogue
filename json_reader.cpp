#include <algorithm>
#include <ostream>
#include <sstream>

#include "json_reader.h"


/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */
namespace JsonReader
{
//----------------------------------------------------------------------------
JsonReader::JsonReader(std::istream &in,
                       TransportCatalogue::TransportCatalogue& db,
                       const RequestHandler& req_hand,
                       renderer::MapRenderer& renderer)
    :db_(db)
    ,req_hand_(req_hand)
    ,renderer_(renderer)
{
    using namespace domain;

    auto main_map = std::move(json::Load(in).GetRoot().AsMap());

    if(auto it = main_map.find(MainReq::base); it != main_map.end()){
        auto vec_map = std::move(it->second.AsArray());
        ParseRequestsBase(std::move(vec_map));
    }
    if(auto it = main_map.find(MainReq::render_settings); it != main_map.end()){
        auto map = std::move(it->second.AsMap());
        ParseRequestsRendSett(std::move(map));
    }
    if(auto it = main_map.find(MainReq::stat); it != main_map.end()){
        auto vec_map = std::move(it->second.AsArray());
        ParseRequestsStat(vec_map);
    }
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
            db_.AddStop(ParseRequestsStops(map_type_stop));
        } else {
            auto bus =  ParseRequestsBuses(map_type_data.AsMap());
            db_.AddBus(bus);
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
        requests.push_back( { std::move(cur_req.at(MainReq::id).AsInt()), std::move(cur_req.at(MainReq::type).AsString()),
                              cur_req.count(MainReq::name) != 0 ? std::move(cur_req.at(MainReq::name).AsString()) : "" } );
    }
    ExecRequestsStat(std::move(requests));
}
//----------------------------------------------------------------------------
void JsonReader::ExecRequestsStat(std::vector<domain::RequestOut>&& requests)
{
    json::Array vec;
    for(const auto& req : requests){
        if(req.type == domain::MainReq::stop){
            vec.emplace_back(PrintResReqStop(req_hand_.GetBusesByStop(req.name), req.id));
        } else if(req.type == domain::MainReq::bus) {
            vec.emplace_back(PrintResReqBus(req_hand_.GetBusStat(req.name), req.id));
        } else if(req.type == domain::MainReq::map) {
            vec.emplace_back(PrintResReqMap(req_hand_.RenderMap(), req.id));
        }
    }
    json::Print(json::Document{json::Node{vec}}, std::cout);
}
//----------------------------------------------------------------------------
void JsonReader::ParseRequestsRendSett(const json::Dict&& map)
{
    using namespace renderer::RenderSettingsKey;
    renderer::RenderSettings rnd_sett{};
    if(map.find(width) != map.end()){
        rnd_sett.width = map.at(width).AsDouble();
    }
    if(map.find(height) != map.end()){
        rnd_sett.height = map.at(height).AsDouble();
    }
    if(map.find(padding) != map.end()){
        rnd_sett.padding = map.at(padding).AsDouble();
    }
    if(map.find(line_width) != map.end()){
        rnd_sett.line_width = map.at(line_width).AsDouble();
    }
    if(map.find(stop_radius) != map.end()){
        rnd_sett.stop_radius = map.at(stop_radius).AsDouble();
    }
    if(map.find(bus_label_font_size) != map.end()){
        rnd_sett.bus_label_font_size = map.at(bus_label_font_size).AsInt();
    }
    if(map.find(bus_label_offset) != map.end()){
        const auto& vec = map.at(bus_label_offset).AsArray();
        rnd_sett.bus_label_offset = {vec[0].AsDouble(), vec[1].AsDouble()};
    }
    if(map.find(stop_label_font_size) != map.end()){
        rnd_sett.stop_label_font_size = map.at(stop_label_font_size).AsInt();
    }
    if(map.find(stop_label_offset) != map.end()){
        const auto& vec = map.at(stop_label_offset).AsArray();
        rnd_sett.stop_label_offset = {vec[0].AsDouble(), vec[1].AsDouble()};
    }
    if(map.find(underlayer_color) != map.end()){
        if(map.at(underlayer_color).IsString()){
            rnd_sett.underlayer_color = map.at(underlayer_color).AsString();
        }
        if(map.at(underlayer_color).IsArray()){
            const auto& vec = map.at(underlayer_color).AsArray();
            if(vec.size() == 3){
                rnd_sett.underlayer_color = svg::Rgb{static_cast<uint8_t>(vec[0].AsInt()), static_cast<uint8_t>(vec[1].AsInt()), static_cast<uint8_t>(vec[2].AsInt())};
            } else if (vec.size() == 4) {
                rnd_sett.underlayer_color = svg::Rgba{static_cast<uint8_t>(vec[0].AsInt()), static_cast<uint8_t>(vec[1].AsInt()), static_cast<uint8_t>(vec[2].AsInt()), vec[3].AsDouble()};
            }

        } else {
          //std:: cout << "underlayer_color unknow type" << std::endl;
        }
    }
    if(map.find(underlayer_width) != map.end()){
        rnd_sett.underlayer_width = map.at(underlayer_width).AsDouble();
    }
    if(map.find(color_palette) != map.end()){
      const auto& vec = map.at(color_palette).AsArray();
      rnd_sett.color_palette.clear(); // отчистка заначений по умолчанию
      for(const auto& node : vec){
          svg::Color color;
          if(node.IsString()){
            color = node.AsString();
          } else if(node.IsArray()) {
            const auto& vec = node.AsArray();
            if(vec.size() == 3){
                color = svg::Rgb{static_cast<uint8_t>(vec[0].AsInt()), static_cast<uint8_t>(vec[1].AsInt()), static_cast<uint8_t>(vec[2].AsInt())};
            } else if (vec.size() == 4) {
                color = svg::Rgba{static_cast<uint8_t>(vec[0].AsInt()), static_cast<uint8_t>(vec[1].AsInt()), static_cast<uint8_t>(vec[2].AsInt()), vec[3].AsDouble()};
            }
          } else {
              //std:: cout << "color_palette unknow type" << std::endl;
          }
          rnd_sett.color_palette.emplace_back(color);
      }
    }

    renderer_.SetRenderSettings(std::move(rnd_sett));
    renderer_.SetBuses(req_hand_.GetBusesLex());
    renderer_.SetUnicStops(req_hand_.GetUnicLexStopsIncludeBuses());
}
//----------------------------------------------------------------------------
domain::Stop JsonReader::ParseRequestsStops(const json::Dict &req)
{
    using namespace domain;
    try{
        return {req.at(MainReq::name).AsString(), {req.at(MainReq::lat).AsDouble(), req.at(MainReq::lon).AsDouble()}};
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
            for(const auto& stop_name : req.at(MainReq::stops).AsArray()){
                const auto& str_stop_name = stop_name.AsString();
                try {
                    stops.push_back(db_.FindStop(str_stop_name).value());
                } catch (...) {
                    std::cout << "tc_.FindStop(name_stop).value()";
                }
            }
            // если не кольцевой маршрут дублируем остановки в обратном порядке
            if(!req.at(MainReq::is_roundtrip).AsBool() && stops.size() >= 2){
                size_t size = stops.size();
                stops.reserve(size*2);
                std::vector<const domain::Stop*>::iterator it = stops.end() - 2;
                for(size_t i = 0; i < size -1; ++i, --it){
                    stops.push_back(*it);
                }
            }
            return {req.at(MainReq::name).AsString(), move(stops), req.at(MainReq::is_roundtrip).AsBool()};
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
            db_.AddRangeStops( {std::move(name_from_stop), rd.first, static_cast<size_t>(rd.second.AsInt())} );
        }
    } catch(...) {
        std::cout << "ParseRequestsStopsLenght FAIL" << std::endl;
        throw;
    }
}
//----------------------------------------------------------------------------
json::Dict JsonReader::PrintResReqBus(std::optional<domain::BusStat>&& bus_stat_opt, int id)
{
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
json::Dict JsonReader::PrintResReqStop(std::optional< const std::unordered_set<const domain::Bus*>* > buses_opt, int id)
{
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
    } else {
        return json::Dict{{"request_id"s, id },
                       {"error_message"s, "not found" } };
    }
}
//----------------------------------------------------------------------------
json::Dict JsonReader::PrintResReqMap(std::optional<svg::Document>&& doc_opt, int id)
{
    if(!doc_opt){
        return {};
    }
    const auto& doc = doc_opt.value();
    std::ostringstream str;
    doc.Render(str);
    return json::Dict{{"map"s, str.str()}, {"request_id"s, id } };
}
//----------------------------------------------------------------------------
}// namespace JsonReader


#pragma once
#include <iostream>

//#include "json.h"
#include "json_builder.h"
#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
*/
namespace JsonReader
{
using namespace std::literals;

class JsonReader
{
public:
    JsonReader(std::istream& in, TransportCatalogue::TransportCatalogue& db,
               const RequestHandler& req_hand,
               renderer::MapRenderer& renderer);
private:
    void ParseRequestsBase(json::Array&& vec_map);

    domain::Stop ParseRequestsStops(const json::Dict& req);

    domain::Bus ParseRequestsBuses(const json::Dict& req);

    void ParseRequestsStopsLenght(const json::Dict& req);

    void ParseRequestsStat(const json::Array& vec_map);

    void ExecRequestsStat(std::vector<domain::RequestOut>&& requests);

    void ParseRequestsRendSett(const json::Dict&& map);

    json::Dict PrintResReqBus(std::optional<domain::BusStat>&& bus_stat_opt, int id);

    json::Dict PrintResReqStop(std::optional<const std::unordered_set<const domain::Bus*>*> buses_opt, int id);

    json::Dict PrintResReqMap(std::optional<svg::Document>&& doc_opt, int id);

    TransportCatalogue::TransportCatalogue& db_;

    const RequestHandler& req_hand_;

    renderer::MapRenderer& renderer_;
};

}// namespace JsonReader


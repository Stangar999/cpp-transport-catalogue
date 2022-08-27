﻿#pragma once
#include <filesystem>
#include <iostream>

//#include "json.h"
#include "json_builder.h"
#include "transport_catalogue.h"
#include "transport_router.h"
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
    JsonReader(TransportCatalogue::TransportCatalogue& db,
               TransportRouter::TransportRouter& tr,
               RequestHandler& req_hand,
               renderer::MapRenderer& renderer
               /*std::filesystem::path& path*/);

    /** обработка .json с вводными данными из которых сформируется БД */
    void ParseJsonMakeBase(std::istream& in);

    /** обработка .json с запросами к готовой БД */
    void ParseJsonProcessRequests(std::istream& in);

//    static json::Dict main_map_;

private:
    void ParseRequestsSrlz(const json::Dict&& req, std::string& path);

    void ParseRequestsBase(json::Array&& vec_map);

    domain::Stop ParseRequestsStops(const json::Dict& req);

    domain::Bus ParseRequestsBuses(const json::Dict& req);

    void ParseRequestsStopsLenght(const json::Dict& req);

    void ParseRequestsStat(const json::Array& vec_map);

    void ExecRequestsStat(std::vector<domain::RequestOut>&& requests);

    void ParseRequestsRendSett(const json::Dict&& map);

    void ParseRequestsRoutSett(const json::Dict&& req);

    json::Dict PrintResReqBus(std::optional<domain::BusStat>&& bus_stat_opt, int id);

    json::Dict PrintResReqStop(std::optional<const std::unordered_set<const domain::Bus*>*> buses_opt, int id);

    json::Dict PrintResReqMap(std::optional<svg::Document>&& doc_opt, int id);

    json::Dict PrintResReqRoute(std::optional<domain::RoutStat>&& rout_stat_opt, int id);

    TransportCatalogue::TransportCatalogue& db_;

    TransportRouter::TransportRouter& tr_;

    RequestHandler& req_hand_;

    renderer::MapRenderer& renderer_;

//    std::filesystem::path path_;
};

//static json::Dict /*JsonReader::*/main_map_ = json::Dict();

}// namespace JsonReader


#pragma once
#include <iostream>

#include "json.h"
#include "transport_catalogue.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
*/
namespace JsonReader
{

class JsonReader
{
public:
    JsonReader(std::istream& in);
private:
    void ParseRequestsBase(json::Array&& vec_map);

    domain::Stop ParseRequestsStops(const json::Dict& req);

    domain::Bus ParseRequestsBuses(const json::Dict& req);

    void ParseRequestsStopsLenght(const json::Dict& req);

    void ParseRequestsStat(const json::Array& vec_map);

    void ExecRequestsStat(std::vector<domain::RequestOut>&& requests);

    json::Dict PrintResReqBus(std::optional<domain::BusStat>&& bus_stat_opt, int id);

    //json::Dict PrintResReqStop(std::optional< const std::set<std::string_view>* > buses_opt, int id);

    json::Dict PrintResReqStop(std::optional<const std::unordered_set<const domain::Bus*>*> buses_opt, int id);

    TransportCatalogue::TransportCatalogue tr_cat_;
};

}// namespace JsonReader


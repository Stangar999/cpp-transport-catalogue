#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>

#include  "input_reader.h"
//#include <deque>

namespace TransportCatalogue {
struct Bus;
struct Stop{
    std::string stop;
    double shir;
    double dolg;
    std::set<std::string> buses;
};

struct Bus{
    std::string bus;
    std::vector<const Stop*> stops;
};

//struct request{
//    std::string type;
//    std::string data;
//};

class TransportCatalogue
{
public:
    TransportCatalogue(std::vector<Request>&& requests);

    void AddBus(std::string&& line);

    void AddStop(std::string&& line);

    std::optional<const Bus*> FindBus(const std::string& bus_name) const ;

    std::optional<const Stop*> FindStop(const std::string& stop_name) const ;

    BusInfo GetBusInfo(const std::string &name_bus) const;

    StopInfo GetStopInfo(const std::string &name_stop) const;

    const std::deque<Stop> GetStops() const{
        return stops_;
    };

    const std::deque<Bus> GetBus() const{
        return buses_;
    };

private:
    std::deque<Stop> stops_;
    std::unordered_map<std::string, const Stop*> index_stops_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string, const Bus*> index_buses_;
};
}
// остановки лежат в дек
// дополнительно для каждой остановки в unordered_map положить как ключ имя остановки(можно string_view), а значение адрес остановки в дек
// маршруты лежат в дек
// структура маршрута состоит из имени маршрута, и вектора с адресами остановок
// дополнительно для каждой маршруты в unordered_map положить как ключ имя маршруты(можно string_view), а значение адрес маршрута в дек
// для нахождения расстояния между остановками нужно
// пара указателей на остановки положить в анордеред мап а значение положить расстояние, но придется делать хешер для остановок(на вход принрмающий паару остановок)
// все запросы добавляем в вектор а потом сортируем что бы ыещз был на первом месте, структура тип запроса, строка с данными
// при длинах надо будет еще раз проходиься

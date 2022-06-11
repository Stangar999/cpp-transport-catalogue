#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <math.h>

#include  "input_reader.h"// приходиться подключать из за общих типов, как правильно?

namespace TransportCatalogue {

namespace detail {
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

class HasherStopes{
public:
    size_t operator()(std::pair<const Stop*, const Stop*> par) const {
        size_t stop1 = std::hash<const void*>{}(par.first);
        size_t stop2 = std::hash<const void*>{}(par.second);
        return stop1*37 + stop2*pow(37,2);
    }
};
}// namespace detail

class TransportCatalogue
{
public:
    TransportCatalogue(std::vector<InputReader::detail::Request>&& requests);

    void AddBus(std::string&& line);

    void AddStop(std::string&& line);

    void SetRangeStops(std::string&& line);

    InputReader::detail::BusInfo GetBusInfo(const std::string &name_bus) const;

    InputReader::detail::StopInfo GetStopInfo(const std::string &name_stop) const;

    const std::deque<detail::Stop> GetStops() const {// для тестов
        return stops_;
    };
    const std::deque<detail::Bus> GetBuses() const {// для тестов
        return buses_;
    };

    std::size_t GetRangeStops(const detail::Stop* stop1, const detail::Stop* stop2) const ;

private:
    std::optional<const detail::Bus*> FindBus(const std::string& bus_name) const ;
    std::optional<const detail::Stop*> FindStop(const std::string& stop_name) const ;
    std::deque<detail::Stop> stops_;
    std::unordered_map<std::string_view, const detail::Stop*> index_stops_;
    std::deque<detail::Bus> buses_;
    std::unordered_map<std::string_view, const detail::Bus*> index_buses_;
    std::unordered_map<std::pair<const detail::Stop*, const detail::Stop*>, size_t, detail::HasherStopes> index_rage_;
};

}// namespace TransportCatalogue

// остановки лежат в дек
// дополнительно для каждой остановки в unordered_map положить как ключ имя остановки(можно string_view), а значение адрес остановки в дек
// маршруты лежат в дек
// структура маршрута состоит из имени маршрута, и вектора с адресами остановок
// дополнительно для каждой маршруты в unordered_map положить как ключ имя маршруты(можно string_view), а значение адрес маршрута в дек
// для нахождения расстояния между остановками нужно
// пара указателей на остановки положить в анордеред мап а значение положить расстояние, но придется делать хешер для остановок(на вход принрмающий паару остановок)
// все запросы добавляем в вектор а потом сортируем что бы ыещз был на первом месте, структура тип запроса, строка с данными
// при длинах надо будет еще раз проходиься

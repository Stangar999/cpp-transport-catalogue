#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>

#include  "input_reader.h"
//#include <deque>

namespace TransportCatalogue {
struct Stop{
    std::string stop;
    double shir;
    double dolg;
};

struct Bus{
    std::string bus;
    std::vector<Stop*> stops;
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

    void FindBus();

    void FindStop();

    void GetBusInfo();

    const std::deque<Stop> GetStops() const{
        return stops;
    };
private:
    std::deque<Stop> stops;
    std::unordered_map<std::string, Stop*> index_stops;
    std::deque<Bus> buses;
    std::unordered_map<std::string, Bus*> index_buses;
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

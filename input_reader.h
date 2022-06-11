#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

namespace TransportCatalogue{

namespace InputReader{

namespace detail{

// используется и тут и в stat_reader, думал сосздать отдельный хедер common.h и там обьявить все общие типы, но в тренажер нельзя добавлять новые файлы, как правильно ???
struct Request{
    std::string type;
    std::string data;
};
// используется и тут и в stat_reader, transport_catalogue.h думал сосздать отдельный хедер common.h и там обьявить все общие типы, но в тренажер нельзя добавлять новые файлы, как правильно ???
struct BusInfo{
    std::string bus_name;
    std::size_t count_stops = 0;
    std::size_t count_unic_stops = 0;
    size_t length = 0;
    double curvature = 0;
};
// используется и тут и в stat_reader, transport_catalogue.h думал сосздать отдельный хедер common.h и там обьявить все общие типы, но в тренажер нельзя добавлять новые файлы, как правильно ???
struct StopInfo{
    const std::string& stop_name;
    bool b_stop_is_not_exist = false;
    const std::set<std::string>& buses;
};
}// namespace detail

class InputReader
{
public:
    InputReader(std::istream &input);
    std::vector<detail::Request> GetRequests();
private:
    std::vector<detail::Request> requests;
};

}// namespace InputReader
}// namespace TransportCatalogue

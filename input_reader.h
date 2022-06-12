#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "transport_catalogue.h"

namespace TransportCatalogue{

namespace detail{
std::vector<::TransportCatalogue::detail::Request> ParseRequestOnType(std::istream &input);

std::string GetWord (const std::string& line, char ch_start, char ch_end, size_t& pos_start, size_t& pos_end);
}
namespace InputReader{

class InputReader
{
public:
    InputReader(std::istream &input, TransportCatalogue& tc);

    std::vector<detail::Request> GetRequests();

private:
    std::tuple<detail::Bus, std::vector<std::string>> ParseRequestsBuses(const std::string& line);

    detail::Stop ParseRequestsStops(const std::string& line);

    void ParseRequestsStopsLenght(const std::string& line);

    std::vector<detail::Request> requests;

    TransportCatalogue& tc_;
};

}// namespace InputReader
}// namespace TransportCatalogue

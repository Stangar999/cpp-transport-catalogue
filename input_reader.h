//#pragma once

//#include <cassert>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <set>
//#include <map>

//#include "transport_catalogue.h"
//#include "domain.h"


//namespace TransportCatalogue{

//namespace detail{
//std::vector<domain::Request> ParseRequestOnType(std::istream &input);

////std::string GetWord (const std::string& line, char ch_start, char ch_end, size_t& pos_start, size_t& pos_end);
//}
//namespace InputReader{

//class InputReader
//{
//public:
//    InputReader(std::istream &input, TransportCatalogue& tc);

//    std::vector<domain::Request> GetRequests();

//private:
//    std::tuple<domain::Bus, std::vector<std::string>> ParseRequestsBuses(const std::string& line);

//    domain::Stop ParseRequestsStops(const std::string& line);

//    void ParseRequestsStopsLenght(const std::string& line);

//    std::vector<domain::Request> requests;

//    TransportCatalogue& tc_;
//};

//}// namespace InputReader
//}// namespace TransportCatalogue

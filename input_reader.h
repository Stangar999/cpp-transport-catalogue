#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

struct Request{
    std::string type;
    std::string data;
};

struct BusInfo{
    std::string bus_name;
    std::size_t count_stops = 0;
    std::size_t count_unic_stops = 0;
    double range = 0;
};

struct StopInfo{
    std::string stop_name;
    bool b_stop_is_not_exist = false;
    const std::set<std::string>& buses;
};

class InputReader
{
public:
    InputReader(std::istream &input);
    std::vector<Request> GetRequests();
private:
    std::vector<Request> requests;
};


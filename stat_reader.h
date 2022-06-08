#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "input_reader.h"

//struct Request{
//    std::string type;
//    std::string data;
//};

class StatrReader
{
public:
    StatrReader(std::istream &input);
    std::vector<Request> GetRequests();
    void PrintResult(BusInfo&& bus_inf);
private:
    std::vector<Request> requests;
};

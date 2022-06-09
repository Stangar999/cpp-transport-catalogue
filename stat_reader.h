#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "input_reader.h"

class StatrReader
{
public:
    StatrReader(std::istream &input);
    std::vector<Request> GetRequests();
    void PrintResReqBus(BusInfo&& bus_inf);
    void PrintResReqStop(StopInfo&& stop_inf);
private:
    std::vector<Request> requests;
};

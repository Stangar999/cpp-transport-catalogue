#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "input_reader.h"// приходиться подключать из за общих типов, как правильно?

namespace TransportCatalogue{
namespace StatrReader{
class StatReader
{
public:
    StatReader(std::istream &input);
    std::vector<InputReader::detail::Request> GetRequests();
    void PrintResReqBus(InputReader::detail::BusInfo&& bus_inf);
    void PrintResReqStop(InputReader::detail::StopInfo&& stop_inf);
private:
    std::vector<InputReader::detail::Request> requests;
};

}// namespace StatrReader
}// namespace TransportCatalogue

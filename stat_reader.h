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
    StatReader(std::istream &input, TransportCatalogue& tc, std::ostream &output);
    std::vector<detail::Request> GetRequests();
    void PrintResReqBus(detail::BusInfo&& bus_inf);
    void PrintResReqStop(detail::StopInfo&& stop_inf);
private:
    std::vector<detail::Request> requests;
    std::ostream &output_;
};

}// namespace StatrReader
}// namespace TransportCatalogue

#include <cassert>
#include "stat_reader.h"

using namespace std;

namespace TransportCatalogue{
namespace StatrReader{
//----------------------------------------------------------------------------
StatReader::StatReader(std::istream &input)
{
    InputReader::InputReader ir(input);
    requests = ir.GetRequests();
}
//----------------------------------------------------------------------------
std::vector<InputReader::detail::Request> StatReader::GetRequests() {
    return move(requests);
}
//----------------------------------------------------------------------------
void StatReader::PrintResReqBus(InputReader::detail::BusInfo&& bus_inf)
{
    const auto &[name, count_stops, count_unic_stops, lengh, curvature] = bus_inf;
            //cout << "adress 1" << &name << endl;
    if(!count_stops){
        cout << "Bus " << name << ": not found" << endl;
    } else {
    cout << "Bus " << name << ": " << count_stops << " stops on route, " << count_unic_stops << " unique stops, " << lengh << " route length, " << curvature << " curvature " << endl;
    }
}
//----------------------------------------------------------------------------
void StatReader::PrintResReqStop(InputReader::detail::StopInfo &&stop_inf)
{
    const auto &[name, b_stop_is_not_exist, buses] = stop_inf;
    if(b_stop_is_not_exist){
        cout << "Stop " << name << ": not found" << endl;
    }else if(buses.empty()) {
        cout << "Stop " << name << ": no buses" << endl;
    } else {
        cout << "Stop " << name << ": buses";
        for(const auto& bus_name: buses){
            cout <<" "<< bus_name;
        }
        cout<< endl;
    }
}
//----------------------------------------------------------------------------
}// namespace StatrReader
}// namespace TransportCatalogue

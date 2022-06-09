#include <cassert>
#include "stat_reader.h"

using namespace std;
//----------------------------------------------------------------------------
StatrReader::StatrReader(std::istream &input)
{
    string count;
    getline(input, count);
    int icount = stoi(count);
    requests.reserve(icount);
    for(int i = 0; i < icount; ++i){
        string line;
        getline(input, line);
        int pos_s = line.find_first_not_of(' ');
        int pos_e = line.find_first_of(' ', pos_s);
        string type_req = line.substr(pos_s, pos_e - pos_s);
        assert(type_req == "Bus" || type_req == "Stop");
        pos_s = line.find_first_not_of(' ', pos_e);
//        pos_e = line.find_first_of(':', pos_s);
//        string name_value = line.substr(pos_s, pos_e - pos_s);
        string data = line.substr(pos_s, pos_e - pos_s);
        requests.push_back( {type_req, data} );
    }
}
//----------------------------------------------------------------------------
std::vector<Request> StatrReader::GetRequests() {
    return move(requests);
}
//----------------------------------------------------------------------------
void StatrReader::PrintResReqBus(BusInfo&& bus_inf)
{
    const auto &[name, count_stops, count_unic_stops, range] = bus_inf;
    if(!count_stops){
        cout << "Bus " << name << ": not found" << endl;
    } else {
    cout << "Bus " << name << ": " << count_stops << " stops on route, " << count_unic_stops << " unique stops, " << range << " route length" << endl;
    }
}
//----------------------------------------------------------------------------
void StatrReader::PrintResReqStop(StopInfo &&stop_inf)
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

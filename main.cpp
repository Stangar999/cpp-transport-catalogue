#include <iostream>

#include "input_reader.h"
#include "stat_reader.h"
#include "transport_catalogue.h"
#include "tests.h"

using namespace std;

int main()
{
    tests::BigTest();
    InputReader input_reader(cin);
    TransportCatalogue::TransportCatalogue transport_catalogue(input_reader.GetRequests());
    StatrReader stat_reader(cin);

    for(const auto& [type_req, data] : stat_reader.GetRequests()){
        if(type_req == "Bus"s){
            stat_reader.PrintResReqBus(transport_catalogue.GetBusInfo(data));
        } else {
            stat_reader.PrintResReqStop(transport_catalogue.GetStopInfo(data));
        }
    }
    //cout << "Hello World!" << endl;
    return 0;
}

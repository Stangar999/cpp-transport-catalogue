#include <iostream>

#include "input_reader.h"
#include "stat_reader.h"
#include "transport_catalogue.h"
//#include "tests.h"

using namespace std;
using namespace TransportCatalogue;

int main()
{
    //tests::BigTest();
    InputReader::InputReader input_reader(cin);
    class TransportCatalogue transport_catalogue(input_reader.GetRequests());

    StatrReader::StatReader stat_reader(cin);
    for(const auto& [type_req, data] : stat_reader.GetRequests()){
        //cout << "adress 2" << &data << endl;
        if(type_req == "Bus"s){
            stat_reader.PrintResReqBus(transport_catalogue.GetBusInfo(data));
        } else {
            stat_reader.PrintResReqStop(transport_catalogue.GetStopInfo(data));
        }
    }
    //cout << "Hello World!" << endl;
    return 0;
}

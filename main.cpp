#include <iostream>

#include "input_reader.h"
#include "stat_reader.h"
#include "transport_catalogue.h"
//#include "tests.h"

using namespace std;

int main()
{
    //tests::BigTest();
    InputReader input_reader(cin);
    TransportCatalogue::TransportCatalogue transport_catalogue(input_reader.GetRequests());
    StatrReader stat_reader(cin);

    for(const auto& [_, bus_name] : stat_reader.GetRequests()){
        stat_reader.PrintResult(transport_catalogue.GetBusInfo(bus_name));
    }
    //cout << "Hello World!" << endl;
    return 0;
}

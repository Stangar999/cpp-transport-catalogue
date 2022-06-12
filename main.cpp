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
    class TransportCatalogue transport_catalogue;
    InputReader::InputReader input_reader(cin, transport_catalogue);
    StatrReader::StatReader stat_reader(cin, transport_catalogue, cout);
    return 0;
}

#include <iostream>

#include "input_reader.h"
#include "transport_catalogue.h"
#include "tests.h"

using namespace std;

int main()
{
    tests::BigTest();
    InputReader input_reader(cin);
    TransportCatalogue::TransportCatalogue transport_catalogue(input_reader.Getrequests());
    cout << "Hello World!" << endl;
    return 0;
}

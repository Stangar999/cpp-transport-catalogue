#include <cassert>

#include "tests.h"
#include "transport_catalogue.h"
//#include  "input_reader.h"

using namespace std;
namespace tests{
void TestStops()
{
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Stop Tolstopaltsevo: 55.611087, 37.208290");
        assert(tr.GetStops().back().stop == "Stop Tolstopaltsevo");
        assert(tr.GetStops().back().shir == 55.611087);
        assert(tr.GetStops().back().dolg == 37.208290);
    }
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Stop Tolstopaltsevo:   55.611087,   37.208290   ");
        assert(tr.GetStops().back().stop == "Stop Tolstopaltsevo");
        assert(tr.GetStops().back().shir == 55.611087);
        assert(tr.GetStops().back().dolg == 37.208290);
    }
}

void BigTest()
{
    TestStops();
}
} // namespace tests

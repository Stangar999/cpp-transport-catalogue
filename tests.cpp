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
        tr.AddStop("Tolstopaltsevo: 55.611087, 37.208290");
        assert(tr.GetStops().back().stop == "Tolstopaltsevo");
        assert(tr.GetStops().back().shir == 55.611087);
        assert(tr.GetStops().back().dolg == 37.208290);
    }
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Tolsto paltsevo:   55.611087,   37.208290   ");
        assert(tr.GetStops().back().stop == "Tolsto paltsevo");
        assert(tr.GetStops().back().shir == 55.611087);
        assert(tr.GetStops().back().dolg == 37.208290);
    }
}

void TestBus()
{
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
        tr.AddStop("Biryusinka: 0, 0");
        tr.AddStop("Universam: 0, 0");
        tr.AddStop("Biryulyovo Tovarnaya: 0, 0");
        tr.AddStop("Biryulyovo Passazhirskaya: 0, 0");
        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
        tr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
        assert(tr.GetBus().back().bus == "256");
        std::vector<string> stops = {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};
        //using stps = tr.GetBus().back().stops
        for (size_t i = 0; i < tr.GetBus().back().stops.size(); ++i){
            assert(tr.GetBus().back().stops[i]->stop == stops[i]);
        }
    }
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Tolstopaltsevo: 0, 0");
        tr.AddStop("Marushkino: 0, 0");
        tr.AddStop("Rasskazovka: 0, 0");
        tr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
        assert(tr.GetBus().back().bus == "750");
        std::vector<string> stops = {"Tolstopaltsevo", "Marushkino", "Rasskazovka", "Marushkino", "Tolstopaltsevo"};
        //using stps = tr.GetBus().back().stops
        for (size_t i = 0; i < tr.GetBus().back().stops.size(); ++i){
            assert(tr.GetBus().back().stops[i]->stop == stops[i]);
        }
    }
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
        tr.AddStop("Biryusinka: 0, 0");
        tr.AddStop("Universam: 0, 0");
        tr.AddStop("Biryulyovo Tovarnaya: 0, 0");
        tr.AddStop("Biryulyovo Passazhirskaya: 0, 0");
        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
        tr.AddBus("256:   Biryulyovo Zapadnoye    >   Biryusinka >   Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya >  Biryulyovo Zapadnoye    ");
        assert(tr.GetBus().back().bus == "256");
        std::vector<string> stops = {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};
        //using stps = tr.GetBus().back().stops
        for (size_t i = 0; i < tr.GetBus().back().stops.size(); ++i){
            assert(tr.GetBus().back().stops[i]->stop == stops[i]);
        }
    }
    {
        TransportCatalogue::TransportCatalogue tr({});
        tr.AddStop("Tolstop altsevo: 0, 0");
        tr.AddStop("Marushkino: 0, 0");
        tr.AddStop("Rasskazovka: 0, 0");
        tr.AddBus("750: Tolstop altsevo -   Marushkino  -  Rasskazovka  ");
        assert(tr.GetBus().back().bus == "750");
        std::vector<string> stops = {"Tolstop altsevo", "Marushkino", "Rasskazovka", "Marushkino", "Tolstop altsevo"};
        //using stps = tr.GetBus().back().stops
        for (size_t i = 0; i < tr.GetBus().back().stops.size(); ++i){
            assert(tr.GetBus().back().stops[i]->stop == stops[i]);
        }
    }
}

void BigTest()
{
    TestStops();
    TestBus();
}

} // namespace tests

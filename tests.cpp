#include <cassert>

#include "tests.h"
#include "transport_catalogue.h"
//#include  "input_reader.h"
static const double EPSILON = 0.1;

using namespace std;
//namespace tests{
//void TestStops()
//{
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Tolstopaltsevo: 55.611087, 37.208290");
//        assert(tr.GetStops().back().stop == "Tolstopaltsevo");
//        assert(tr.GetStops().back().lat == 55.611087);
//        assert(tr.GetStops().back().lng == 37.208290);
//    }
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Tolsto paltsevo:   55.611087,   37.208290   ");
//        assert(tr.GetStops().back().stop == "Tolsto paltsevo");
//        assert(tr.GetStops().back().lat == 55.611087);
//        assert(tr.GetStops().back().lng == 37.208290);
//    }
//}

//void TestBus()
//{
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        tr.AddStop("Biryusinka: 0, 0");
//        tr.AddStop("Universam: 0, 0");
//        tr.AddStop("Biryulyovo Tovarnaya: 0, 0");
//        tr.AddStop("Biryulyovo Passazhirskaya: 0, 0");
//        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        tr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
//        assert(tr.GetBuses().back().bus == "256");
//        std::vector<string> stops = {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};
//        //using stps = tr.GetBus().back().stops
//        for (size_t i = 0; i < tr.GetBuses().back().stops.size(); ++i){
//            assert(tr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Tolstopaltsevo: 0, 0");
//        tr.AddStop("Marushkino: 0, 0");
//        tr.AddStop("Rasskazovka: 0, 0");
//        tr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
//        assert(tr.GetBuses().back().bus == "750");
//        std::vector<string> stops = {"Tolstopaltsevo", "Marushkino", "Rasskazovka", "Marushkino", "Tolstopaltsevo"};
//        //using stps = tr.GetBus().back().stops
//        for (size_t i = 0; i < tr.GetBuses().back().stops.size(); ++i){
//            assert(tr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        tr.AddStop("Biryusinka: 0, 0");
//        tr.AddStop("Universam: 0, 0");
//        tr.AddStop("Biryulyovo Tovarnaya: 0, 0");
//        tr.AddStop("Biryulyovo Passazhirskaya: 0, 0");
//        tr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        tr.AddBus("256:   Biryulyovo Zapadnoye    >   Biryusinka >   Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya >  Biryulyovo Zapadnoye    ");
//        assert(tr.GetBuses().back().bus == "256");
//        std::vector<string> stops = {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};
//        //using stps = tr.GetBus().back().stops
//        for (size_t i = 0; i < tr.GetBuses().back().stops.size(); ++i){
//            assert(tr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Tolstop altsevo: 0, 0");
//        tr.AddStop("Marushkino: 0, 0");
//        tr.AddStop("Rasskazovka: 0, 0");
//        tr.AddBus("750: Tolstop altsevo -   Marushkino  -  Rasskazovka  ");
//        assert(tr.GetBuses().back().bus == "750");
//        std::vector<string> stops = {"Tolstop altsevo", "Marushkino", "Rasskazovka", "Marushkino", "Tolstop altsevo"};
//        //using stps = tr.GetBus().back().stops
//        for (size_t i = 0; i < tr.GetBuses().back().stops.size(); ++i){
//            assert(tr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//}

//void TestGetBusInfo()
//{
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Tolstopaltsevo: 55.611087, 37.20829");
//        tr.AddStop("Marushkino: 55.595884, 37.209755");
//        tr.AddStop("Rasskazovka: 55.632761, 37.333324");
//        tr.AddStop("Biryulyovo Zapadnoye: 55.574371, 37.6517");
//        tr.AddStop("Biryusinka: 55.581065, 37.64839");
//        tr.AddStop("Universam: 55.587655, 37.645687");
//        tr.AddStop("Biryulyovo Tovarnaya: 55.592028, 37.653656");
//        tr.AddStop("Biryulyovo Passazhirskaya: 55.580999, 37.659164");
//        tr.AddStop("Rossoshanskaya ulitsa: 55.595579, 37.605757");
//        tr.AddStop("Prazhskaya: 55.611678, 37.603831");
//        tr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
//        tr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
//        tr.AddBus("828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye");
////        const auto& info256 = tr.GetBusInfo("256");
////        assert (info256.bus_name == "256" &&
////                info256.count_stops == 6 &&
////                info256.count_unic_stops == 5 &&
////                std::abs(info256.range - 4371.02) < EPSILON);
////        const auto& info750 = tr.GetBusInfo("750");
////        assert (info750.bus_name == "750" &&
////                info750.count_stops == 5 &&
////                info750.count_unic_stops == 3 &&
////                std::abs(info750.range - 20939.5) < EPSILON);
////        const auto& info751 = tr.GetBusInfo("751");
////        assert (info751.bus_name == "751" &&
////                info751.count_stops == 0 &&
////                info751.count_unic_stops == 0 &&
////                info751.range == 0);
//    }
//}

//void TestGetStopInfo()
//{
//    {
//        TransportCatalogue::TransportCatalogue tr({});
//        tr.AddStop("Tolstopaltsevo: 55.611087, 37.20829");
//        tr.AddStop("Marushkino: 55.595884, 37.209755");
//        tr.AddStop("Rasskazovka: 55.632761, 37.333324");
//        tr.AddStop("Biryulyovo Zapadnoye: 55.574371, 37.6517");
//        tr.AddStop("Biryusinka: 55.581065, 37.64839");
//        tr.AddStop("Universam: 55.587655, 37.645687");
//        tr.AddStop("Biryulyovo Tovarnaya: 55.592028, 37.653656");
//        tr.AddStop("Biryulyovo Passazhirskaya: 55.580999, 37.659164");
//        tr.AddStop("Rossoshanskaya ulitsa: 55.595579, 37.605757");
//        tr.AddStop("Prazhskaya: 55.611678, 37.603831");
//        tr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
//        tr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
//        tr.AddBus("828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye");
//        const auto& infoSamara = tr.GetStopInfo("Samara");
//        assert (infoSamara.stop_name == "Samara" &&
//                infoSamara.b_stop_is_not_exist == 1 &&
//                infoSamara.buses.empty());
//        const auto& infoPrazhskaya = tr.GetStopInfo("Prazhskaya");
//        assert (infoPrazhskaya.stop_name == "Prazhskaya" &&
//                infoPrazhskaya.b_stop_is_not_exist == 0 &&
//                infoPrazhskaya.buses.empty());
//        const auto& infoBiryulyovo = tr.GetStopInfo("Biryulyovo Zapadnoye");
//        assert (infoBiryulyovo.stop_name == "Biryulyovo Zapadnoye" &&
//                infoBiryulyovo.b_stop_is_not_exist == 0 &&
//                *infoBiryulyovo.buses.begin() == "256" &&
//                *(++infoBiryulyovo.buses.begin()) == "828");
//    }
//}

//void BigTest()
//{
//    TestStops();
//    TestBus();
//    TestGetBusInfo();
//    TestGetStopInfo();
//}

//} // namespace tests

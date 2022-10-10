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
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Tolstopaltsevo: 55.611087, 37.208290");
//        assert(trnsprt_routr.GetStops().back().stop == "Tolstopaltsevo");
//        assert(trnsprt_routr.GetStops().back().lat == 55.611087);
//        assert(trnsprt_routr.GetStops().back().lng == 37.208290);
//    }
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Tolsto paltsevo:   55.611087,   37.208290   ");
//        assert(trnsprt_routr.GetStops().back().stop == "Tolsto paltsevo");
//        assert(trnsprt_routr.GetStops().back().lat == 55.611087);
//        assert(trnsprt_routr.GetStops().back().lng == 37.208290);
//    }
//}

//void TestBus()
//{
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        trnsprt_routr.AddStop("Biryusinka: 0, 0");
//        trnsprt_routr.AddStop("Universam: 0, 0");
//        trnsprt_routr.AddStop("Biryulyovo Tovarnaya: 0, 0");
//        trnsprt_routr.AddStop("Biryulyovo Passazhirskaya: 0, 0");
//        trnsprt_routr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        trnsprt_routr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
//        assert(trnsprt_routr.GetBuses().back().bus == "256");
//        std::vector<string> stops = {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};
//        //using stps = trnsprt_routr.GetBus().back().stops
//        for (size_t i = 0; i < trnsprt_routr.GetBuses().back().stops.size(); ++i){
//            assert(trnsprt_routr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Tolstopaltsevo: 0, 0");
//        trnsprt_routr.AddStop("Marushkino: 0, 0");
//        trnsprt_routr.AddStop("Rasskazovka: 0, 0");
//        trnsprt_routr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
//        assert(trnsprt_routr.GetBuses().back().bus == "750");
//        std::vector<string> stops = {"Tolstopaltsevo", "Marushkino", "Rasskazovka", "Marushkino", "Tolstopaltsevo"};
//        //using stps = trnsprt_routr.GetBus().back().stops
//        for (size_t i = 0; i < trnsprt_routr.GetBuses().back().stops.size(); ++i){
//            assert(trnsprt_routr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        trnsprt_routr.AddStop("Biryusinka: 0, 0");
//        trnsprt_routr.AddStop("Universam: 0, 0");
//        trnsprt_routr.AddStop("Biryulyovo Tovarnaya: 0, 0");
//        trnsprt_routr.AddStop("Biryulyovo Passazhirskaya: 0, 0");
//        trnsprt_routr.AddStop("Biryulyovo Zapadnoye: 0, 0");
//        trnsprt_routr.AddBus("256:   Biryulyovo Zapadnoye    >   Biryusinka >   Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya >  Biryulyovo Zapadnoye    ");
//        assert(trnsprt_routr.GetBuses().back().bus == "256");
//        std::vector<string> stops = {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};
//        //using stps = trnsprt_routr.GetBus().back().stops
//        for (size_t i = 0; i < trnsprt_routr.GetBuses().back().stops.size(); ++i){
//            assert(trnsprt_routr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Tolstop altsevo: 0, 0");
//        trnsprt_routr.AddStop("Marushkino: 0, 0");
//        trnsprt_routr.AddStop("Rasskazovka: 0, 0");
//        trnsprt_routr.AddBus("750: Tolstop altsevo -   Marushkino  -  Rasskazovka  ");
//        assert(trnsprt_routr.GetBuses().back().bus == "750");
//        std::vector<string> stops = {"Tolstop altsevo", "Marushkino", "Rasskazovka", "Marushkino", "Tolstop altsevo"};
//        //using stps = trnsprt_routr.GetBus().back().stops
//        for (size_t i = 0; i < trnsprt_routr.GetBuses().back().stops.size(); ++i){
//            assert(trnsprt_routr.GetBuses().back().stops[i]->stop == stops[i]);
//        }
//    }
//}

//void TestGetBusInfo()
//{
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Tolstopaltsevo: 55.611087, 37.20829");
//        trnsprt_routr.AddStop("Marushkino: 55.595884, 37.209755");
//        trnsprt_routr.AddStop("Rasskazovka: 55.632761, 37.333324");
//        trnsprt_routr.AddStop("Biryulyovo Zapadnoye: 55.574371, 37.6517");
//        trnsprt_routr.AddStop("Biryusinka: 55.581065, 37.64839");
//        trnsprt_routr.AddStop("Universam: 55.587655, 37.645687");
//        trnsprt_routr.AddStop("Biryulyovo Tovarnaya: 55.592028, 37.653656");
//        trnsprt_routr.AddStop("Biryulyovo Passazhirskaya: 55.580999, 37.659164");
//        trnsprt_routr.AddStop("Rossoshanskaya ulitsa: 55.595579, 37.605757");
//        trnsprt_routr.AddStop("Prazhskaya: 55.611678, 37.603831");
//        trnsprt_routr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
//        trnsprt_routr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
//        trnsprt_routr.AddBus("828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye");
////        const auto& info256 = trnsprt_routr.GetBusInfo("256");
////        assert (info256.bus_name == "256" &&
////                info256.count_stops == 6 &&
////                info256.count_unic_stops == 5 &&
////                std::abs(info256.range - 4371.02) < EPSILON);
////        const auto& info750 = trnsprt_routr.GetBusInfo("750");
////        assert (info750.bus_name == "750" &&
////                info750.count_stops == 5 &&
////                info750.count_unic_stops == 3 &&
////                std::abs(info750.range - 20939.5) < EPSILON);
////        const auto& info751 = trnsprt_routr.GetBusInfo("751");
////        assert (info751.bus_name == "751" &&
////                info751.count_stops == 0 &&
////                info751.count_unic_stops == 0 &&
////                info751.range == 0);
//    }
//}

//void TestGetStopInfo()
//{
//    {
//        TransportCatalogue::TransportCatalogue trnsprt_routr({});
//        trnsprt_routr.AddStop("Tolstopaltsevo: 55.611087, 37.20829");
//        trnsprt_routr.AddStop("Marushkino: 55.595884, 37.209755");
//        trnsprt_routr.AddStop("Rasskazovka: 55.632761, 37.333324");
//        trnsprt_routr.AddStop("Biryulyovo Zapadnoye: 55.574371, 37.6517");
//        trnsprt_routr.AddStop("Biryusinka: 55.581065, 37.64839");
//        trnsprt_routr.AddStop("Universam: 55.587655, 37.645687");
//        trnsprt_routr.AddStop("Biryulyovo Tovarnaya: 55.592028, 37.653656");
//        trnsprt_routr.AddStop("Biryulyovo Passazhirskaya: 55.580999, 37.659164");
//        trnsprt_routr.AddStop("Rossoshanskaya ulitsa: 55.595579, 37.605757");
//        trnsprt_routr.AddStop("Prazhskaya: 55.611678, 37.603831");
//        trnsprt_routr.AddBus("256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
//        trnsprt_routr.AddBus("750: Tolstopaltsevo - Marushkino - Rasskazovka");
//        trnsprt_routr.AddBus("828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye");
//        const auto& infoSamara = trnsprt_routr.GetStopInfo("Samara");
//        assert (infoSamara.stop_name == "Samara" &&
//                infoSamara.b_stop_is_not_exist == 1 &&
//                infoSamara.buses.empty());
//        const auto& infoPrazhskaya = trnsprt_routr.GetStopInfo("Prazhskaya");
//        assert (infoPrazhskaya.stop_name == "Prazhskaya" &&
//                infoPrazhskaya.b_stop_is_not_exist == 0 &&
//                infoPrazhskaya.buses.empty());
//        const auto& infoBiryulyovo = trnsprt_routr.GetStopInfo("Biryulyovo Zapadnoye");
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

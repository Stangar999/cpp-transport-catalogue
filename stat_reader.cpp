//#include <cassert>
//#include "stat_reader.h"

//using namespace std;

//namespace TransportCatalogue{
//namespace StatrReader{
////----------------------------------------------------------------------------
//StatReader::StatReader(std::istream &input, TransportCatalogue &tc, ostream &output)
//    :output_(output)
//{
//    requests = detail::ParseRequestOnType(input);
//    for(const auto& [type_req, data] : requests){
//        //cout << "adress 2" << &data << endl;
//        if(type_req == "Bus"s){
//            if(auto opt_bus = tc.FindBus(data); opt_bus){
//                PrintResReqBus(tc.GetBusInfo(opt_bus.value()));
//            } else {
//                PrintResReqBus({data});
//            }
//        } else {
//            if(auto opt_stop = tc.FindStop(data); opt_stop){
//                PrintResReqStop(tc.GetStopInfo(opt_stop.value()));
//            } else {
//                PrintResReqStop({data, true, nullopt});
//            }
//        }
//    }
//}
////----------------------------------------------------------------------------
//std::vector<Request> StatReader::GetRequests() {
//    return move(requests);
//}
////----------------------------------------------------------------------------
//void StatReader::PrintResReqBus(BusStat&& bus_inf)
//{
//    const auto &[name, count_stops, count_unic_stops, lengh, curvature] = bus_inf;
//            //cout << "adress 1" << &name << endl;
//    if(!count_stops){
//        output_ << "Bus " << name << ": not found" << endl;
//    } else {
//    output_ << "Bus " << name << ": " << count_stops << " stops on route, " << count_unic_stops << " unique stops, " << lengh << " route length, " << curvature << " curvature " << endl;
//    }
//}
////----------------------------------------------------------------------------
//void StatReader::PrintResReqStop(StopInfo &&stop_inf)
//{
//    const auto &[name, b_stop_is_not_exist, buses] = stop_inf;
//    if(b_stop_is_not_exist) {
//        output_ << "Stop " << name << ": not found" << endl;
//    } else if(!buses.has_value()) {
//        output_ << "Stop " << name << ": no buses" << endl;
//    } else {
//        output_ << "Stop " << name << ": buses";
//        for(const auto& bus_name: buses.value()){
//            output_ << " " << bus_name;
//        }
//        output_<< endl;
//    }
//}
////----------------------------------------------------------------------------
//}// namespace StatrReader
//}// namespace TransportCatalogue

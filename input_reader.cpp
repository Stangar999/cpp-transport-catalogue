#include <algorithm>
#include <cassert>

#include "input_reader.h"

using namespace std;
namespace TransportCatalogue{
namespace InputReader{
//----------------------------------------------------------------------------
InputReader::InputReader(std::istream &input, TransportCatalogue& tc):
    tc_(tc)
{
    requests = detail::ParseRequestOnType(input);
    // для выставления запросов stop перед bus
    auto compare = [](const detail::Request& lh, const detail::Request& rh){
        return lh.type > rh.type;
    };
    sort(requests.begin(), requests.end(), compare);

    for (const auto& request : requests){
        if (request.type == "Stop"){
            tc.AddStop(ParseRequestsStops(request.data));
        } else{
            auto [bus, buses_from_stop] = ParseRequestsBuses(request.data);
            tc.AddBusesFromStop(bus.name, buses_from_stop);
            tc.AddBus(bus);
        }
    }
    for (auto& request : requests){
        if (request.type == "Stop"){
            ParseRequestsStopsLenght(move(request.data));
        }
    }
}
//----------------------------------------------------------------------------
std::vector<detail::Request> InputReader::GetRequests() {
    return move(requests);
}
//----------------------------------------------------------------------------
detail::Stop InputReader::ParseRequestsStops(const std::string& line)
{
    // пробелы в начале строки удалены в конструкторе
    // название остановки
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_value = line.substr(pos_s, pos_e);
    // широта
    pos_s = line.find_first_not_of(' ', pos_e + 1);
    pos_e = line.find_first_of(',', pos_s);
    double lat = stod(line.substr(pos_s, pos_e - pos_s));
    // долгота
    double lng = 0;
    pos_s = line.find_first_not_of(' ', pos_e + 1);
    if(pos_e = line.find_first_of(',', pos_s); pos_e != -1){
        lng = stod(line.substr(pos_s, pos_e - pos_s));
    }else{
        pos_e = line.find_first_of(' ', pos_s);
        pos_e = pos_e == -1 ? line.size() : pos_e; // что бы и пробелы удалять и последнюю цифру не отрезать
        lng = stod(line.substr(pos_s, pos_e - pos_s));
    }
    return { move(name_value), lat, lng };
}
//----------------------------------------------------------------------------
void InputReader::ParseRequestsStopsLenght(const std::string &line)
{
    // пробелы в начале строки удалены в конструкторе
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_from_stop = line.substr(pos_s, pos_e);
    pos_e = line.find_first_of(',');
    if(pos_e == -1){
        throw "pos_e == -1";// одна запятая должна быть
    }
    pos_e = line.find_first_of(',', pos_e + 1);
    // пропускаем если дальности не заданы
    if(pos_e == -1){
        return;
    }
    string range;
    while(pos_e > 0){
        pos_s = line.find_first_not_of(' ', pos_e + 1);
        pos_e = line.find_first_of('m', pos_s);
        range = line.substr(pos_s, pos_e - pos_s);
        pos_s = line.find_first_of(" to ", pos_e);
        pos_s = line.find_first_not_of(' ', pos_s + 4);// 4 - " to " колличество символов
        pos_e = line.find_first_of(',', pos_s);
        string name_to_stop;
        if(pos_e == -1){
            pos_e = line.find_last_not_of(" ");
            name_to_stop = line.substr(pos_s, (pos_e + 1) - pos_s);
            pos_e = -1;// возвращаем для выхода из цикла
        }else{
            name_to_stop = line.substr(pos_s, pos_e - pos_s);
        }
        tc_.AddRangeStops( {name_from_stop, name_to_stop, static_cast<size_t>(stol(range))} );
    }
}
//----------------------------------------------------------------------------
std::tuple<detail::Bus, std::vector<string> > InputReader::ParseRequestsBuses(const std::string& line)
{
    // пробелы в начале строки удалены в input_reader
    // название маршрута
    int pos_s = 0;
    int pos_e = line.find_first_of(':');
    string name_bus = line.substr(pos_s, pos_e);
    std::vector<const detail::Stop*> stops;
    std::vector<std::string> buses_from_stop;
    int ich = line.find('>');
    char ch = ich > 0 ? '>' : '-';
    while(pos_e > 0){
        pos_s = line.find_first_not_of(' ', pos_e + 1);
        int pos_ch = line.find_first_of(ch, pos_s);
        pos_e = line.find_last_not_of(' ', pos_ch - 1);
        string name_stop = line.substr(pos_s, (pos_e + 1) - pos_s);
        pos_e = pos_ch;
        try {
           buses_from_stop.push_back(name_stop);
           stops.push_back(tc_.FindStop(name_stop).value());
        } catch (...) {
            cout << "tc_.FindStop(name_stop).value()";
        }
    }
    if(ch == '-'){
        size_t size = stops.size();
        stops.reserve(size*2 - 1);
        std::vector<const detail::Stop*>::iterator it = stops.end() - 2;
        for(size_t i = 0; i < size -1; ++i, --it){
            stops.push_back(*it);
        }
    }
    return { {move(name_bus), move(stops)}, move(buses_from_stop) };
}
//----------------------------------------------------------------------------
}// namespace InputReader

//----------------------------------------------------------------------------
std::vector<detail::Request> detail::ParseRequestOnType(std::istream &input){
    std::string count;
    getline(input, count);
    int icount = stoi(move(count));
    std::vector<detail::Request> requests;
    requests.reserve(icount);
    for(int i = 0; i < icount; ++i){
        std::string line;
        getline(input, line);
        int pos_s = line.find_first_not_of(' ');
        int pos_e = line.find_first_of(' ', pos_s);
        std::string type_req = line.substr(pos_s, pos_e - pos_s);
        if(type_req != "Stop" && type_req != "Bus"){
            throw "type_req != Stop && type_req != Bus";
        }
        pos_s = line.find_first_not_of(' ', pos_e);
        pos_e = line.find_last_not_of(' ');
        std::string data = line.substr(pos_s, (pos_e + 1) - pos_s);
        requests.push_back( {type_req, move(data)} );
    }
    return requests;
}
//----------------------------------------------------------------------------
}// namespace TransportCatalogue

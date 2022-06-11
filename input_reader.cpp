#include <cassert>
#include "input_reader.h"

using namespace std;
namespace TransportCatalogue{
namespace InputReader{
//----------------------------------------------------------------------------
InputReader::InputReader(std::istream &input)
{
    string count;
    getline(input, count);
    int icount = stoi(move(count));
    requests.reserve(icount);
    for(int i = 0; i < icount; ++i){
        string line;
        getline(input, line);
        int pos_s = line.find_first_not_of(' ');
        int pos_e = line.find_first_of(' ', pos_s);
        string type_req = line.substr(pos_s, pos_e - pos_s);
        assert(type_req == "Stop" || type_req == "Bus");
        pos_s = line.find_first_not_of(' ', pos_e);
        pos_e = line.find_last_not_of(' ');
        string data = line.substr(pos_s, (pos_e + 1) - pos_s);
        requests.push_back( {type_req, move(data)} );
    }
}
//----------------------------------------------------------------------------
std::vector<detail::Request> InputReader::GetRequests() {
    return move(requests);
}
//----------------------------------------------------------------------------
}// namespace InputReader
}// namespace TransportCatalogue

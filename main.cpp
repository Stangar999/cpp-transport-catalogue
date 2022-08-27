#include <iostream>
#include <string_view>
#include <fstream>

#include "json_reader.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "transport_router.h"
#include "serialization.h"
//#include "tests.h"

using namespace std;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    if (mode == "make_base"sv) {
        TransportCatalogue::TransportCatalogue db;
        TransportRouter::TransportRouter tr;
        renderer::MapRenderer rend;
        RequestHandler req_hand(db, tr, rend);
//        std::filesystem::path path;
        JsonReader::JsonReader j_r(/*cin,*/ db, tr, req_hand, rend/*, path*/);
        j_r.ParseJsonMakeBase(cin);
//        Serialization sereliz;
//        sereliz.Serialize(db, path);

    } else if (mode == "process_requests"sv) {
        TransportCatalogue::TransportCatalogue db;
        TransportRouter::TransportRouter tr;
        renderer::MapRenderer rend;
        RequestHandler req_hand(db, tr, rend);
//        std::filesystem::path path;
        JsonReader::JsonReader j_r(/*cin,*/ db, tr, req_hand, rend);
        j_r.ParseJsonProcessRequests(cin);
//        Serialization sereliz;
//        sereliz.Deserialize(path, db);
    } else {
        PrintUsage();
        return 1;
    }
//    cout << endl << "ok" << endl;
}



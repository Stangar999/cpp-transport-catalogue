#include <iostream>

//#include "input_reader.h"
//#include "stat_reader.h"
//#include "transport_catalogue.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "request_handler.h"
//#include "tests.h"

using namespace std;
//using namespace TransportCatalogue;

int main() {
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массиве "stat_requests", построив JSON-массив
     * с ответами.
     * Вывести в stdout ответы в виде JSON
     */
    TransportCatalogue::TransportCatalogue db;
    renderer::MapRenderer rend;
    RequestHandler req_hand(db, rend);
    JsonReader::JsonReader j_r(cin, db, req_hand, rend);
    rend.DrawMapBus();
}

//int main()
//{
//    //tests::BigTest();
//    class TransportCatalogue transport_catalogue;
//    InputReader::InputReader input_reader(cin, transport_catalogue);
//    StatrReader::StatReader stat_reader(cin, transport_catalogue, cout);
//    return 0;
//}

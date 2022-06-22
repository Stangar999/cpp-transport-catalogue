#include <iostream>

//#include "input_reader.h"
//#include "stat_reader.h"
#include "transport_catalogue.h"
#include "json_reader.h"
//#include "tests.h"

using namespace std;
using namespace TransportCatalogue;

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
    JsonReader::JsonReader j_r(cin);
}

//int main()
//{
//    //tests::BigTest();
//    class TransportCatalogue transport_catalogue;
//    InputReader::InputReader input_reader(cin, transport_catalogue);
//    StatrReader::StatReader stat_reader(cin, transport_catalogue, cout);
//    return 0;
//}

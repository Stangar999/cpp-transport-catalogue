#include <iostream>

#include "json_reader.h"
#include "map_renderer.h"
#include "request_handler.h"
//#include "tests.h"

using namespace std;

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
    // не понял я итоговую архитектуру
    // JsonReader считывает из переданого потока документ и загружает в переданный по ссылке TransportCatalogue, он вызывает методы TransportCatalogue
    // MapRenderer как  понимаю должет формировать Document из данных переданных из RequestHandler, что бы не зависеть от TransportCatalogue,
    // и передача осуществляется в JsonReader, а еще JsonReader вызывает печать, и получается на нем все функции координации, а в main только все создается
    // Все это запутанно получается и как то странно
    TransportCatalogue::TransportCatalogue db;
    renderer::MapRenderer rend;
    RequestHandler req_hand(db, rend);
    JsonReader::JsonReader j_r(cin, db, req_hand, rend);
}


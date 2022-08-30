#pragma once
#include <unordered_set>
#include <filesystem>

#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"
#include "transport_router.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)
//using namespace TransportCatalogue;

class RequestHandler {

public:
    // MapRenderer понадобится в следующей части итогового проекта
    RequestHandler(TransportCatalogue::TransportCatalogue& t_c,
                   TransportRouter::TransportRouter& t_r,
                   renderer::MapRenderer& m_r);

    // Возвращает информацию о маршруте (запрос Bus)
    std::optional<domain::BusStat> GetBusStat(const std::string_view& bus_name) const;

    // Возвращает информацию о маршруте (запрос Route)
    std::optional<domain::RoutStat> GetRouteStat(std::string_view stop_from, std::string_view stop_to) const;

    // Возвращает маршруты, проходящие через
    std::optional<const std::unordered_set<const domain::Bus*>*> GetBusesByStop(const std::string_view& stop_name) const;

    std::vector<const domain::Bus*> GetBusesLex() const;

    // Возвращает перечень уникальных остановок в лекс порядке через которые проходят маршруты
    const std::vector<const domain::Stop*> GetUnicLexStopsIncludeBuses() const ;

    svg::Document RenderMap() const;

    void CallDsrlz(const std::filesystem::path& path);

    void CallSrlz(const std::filesystem::path& path) const;

    // если я правильно понял о каком методе идет речь, то он уже есть строка 37
private:
    domain::BusStat CreateBusStat (const domain::Bus* bus) const;

    TransportCatalogue::TransportCatalogue& t_c_;

    TransportRouter::TransportRouter& t_r_;

    renderer::MapRenderer& m_r_;
};

#include <fstream>
#include "serialization.h"

Serialization::Serialization()
{

}

void Serialization::Serialize(const TransportCatalogue::TransportCatalogue& t_c
                              , const std::filesystem::path& path) const
{
    t_c_srlz::TransportCatalogue s_t_c;
    for(const auto& stop : t_c.GetStops()) {
        t_c_srlz::Stop s_stop;
        s_stop.set_name(stop.name);

        t_c_srlz::Coord s_coord;
        s_coord.set_latitude(stop.coord.lat);
        s_coord.set_longitude(stop.coord.lng);
        *s_stop.mutable_coord() = std::move(s_coord);

        s_stop.set_id(stop.id);
        *s_t_c.add_list_stop() = std::move(s_stop);
    }
    for(const auto& bus : t_c.GetBuses()) {
        t_c_srlz::Bus s_bus;
        s_bus.set_name(bus.name);

        for(const auto& stop : bus.stops) {
            *s_bus.add_list_name_stop() = stop->name;
        }

        s_bus.set_is_roundtrip(bus.is_round);
        *s_t_c.add_list_bus() = std::move(s_bus);
    }
    for(const auto& struc : t_c.GetIndexRageStop()) {
        t_c_srlz::StopsLenght s_stop_lenght;
        s_stop_lenght.set_from_stop(struc.first.first->name);
        s_stop_lenght.set_to_stop(struc.first.second->name);
        s_stop_lenght.set_lenght(struc.second);
        *s_t_c.add_list_stop_lenght() = std::move(s_stop_lenght);
    }

    std::ofstream out(path, std::ios::binary);
    s_t_c.SerializeToOstream(&out);
}

void Serialization::Deserialize(const std::filesystem::path& path
                              , TransportCatalogue::TransportCatalogue& t_c) const
{
    std::ifstream in(path, std::ios::binary);
    t_c_srlz::TransportCatalogue s_t_c;
    s_t_c.ParseFromIstream(&in);

    size_t count_stop =  s_t_c.list_stop_size();
    for(size_t i = 0; i < count_stop; ++i) {
        domain::Stop stop;
        stop.name = s_t_c.list_stop(i).name();
        stop.coord.lat = s_t_c.list_stop(i).coord().latitude();
        stop.coord.lng = s_t_c.list_stop(i).coord().longitude();
        stop.id = s_t_c.list_stop(i).id();
        t_c.AddStop(std::move(stop));
    }

    size_t count_bus =  s_t_c.list_bus_size();
    for(size_t i = 0; i < count_bus; ++i) {
        domain::Bus bus;
        bus.name = s_t_c.list_bus(i).name();
        size_t count_name_stop =  s_t_c.list_bus(i).list_name_stop_size();
        for(size_t j = 0; j < count_name_stop; ++j) {
            bus.stops.push_back(t_c.FindStop(s_t_c.list_bus(i).list_name_stop(j)).value());
        }
        bus.is_round = s_t_c.list_bus(i).is_roundtrip();
        t_c.AddBus(std::move(bus));
    }

    size_t count_ranges =  s_t_c.list_stop_lenght_size();
    for(size_t i = 0; i < count_ranges; ++i) {
        domain::StopsLenght stop_lenght;
        stop_lenght.from_stop = s_t_c.list_stop_lenght(i).from_stop();
        stop_lenght.to_stop = s_t_c.list_stop_lenght(i).to_stop();
        stop_lenght.lenght = s_t_c.list_stop_lenght(i).lenght();
        t_c.AddRangeStops(std::move(stop_lenght));
    }

}

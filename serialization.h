#pragma once
#include <filesystem>
#include <transport_catalogue.pb.h>

#include "transport_catalogue.h"

class Serialization {
public:
    Serialization();
    void Serialize(const TransportCatalogue::TransportCatalogue& t_c, const std::filesystem::path& path) const;
    void Deserialize(const std::filesystem::path& path, TransportCatalogue::TransportCatalogue& t_c) const;
private:
};


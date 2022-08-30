#pragma once
#include <filesystem>
#include <transport_catalogue.pb.h>

#include "transport_catalogue.h"
#include "map_renderer.h"
#include "transport_router.h"

class Serialization {
public:
    Serialization();
    void Serialize(const TransportCatalogue::TransportCatalogue& trnsprt_ctlg
                   , const renderer::MapRenderer& map_rendr_
                   , const TransportRouter::TransportRouter& trnsprt_routr_
                   , const std::filesystem::path& path) const;

    void Deserialize(const std::filesystem::path& path
                     , TransportCatalogue::TransportCatalogue& trnsprt_ctlg
                     , renderer::MapRenderer& map_rendr_
                     , TransportRouter::TransportRouter& trnsprt_routr_) const;

private:
    void SerializeTrnsprtCtlg(t_c_srlz::TransportCatalogue& srlz_trnsprt_ctlg, const TransportCatalogue::TransportCatalogue& trnsprt_ctlg) const;
    void SerializeRendrSettng(t_c_srlz::TransportCatalogue& srlz_trnsprt_ctlg, const renderer::MapRenderer& map_rendr_) const;
    void SerializeTrnsprtRoutr(t_c_srlz::TransportCatalogue& srlz_trnsprt_ctlg, const TransportRouter::TransportRouter& trnsprt_routr_) const;

    void DeserializeTrnsprtCtlg(const t_c_srlz::TransportCatalogue& srlz_trnsprt_ctlg, TransportCatalogue::TransportCatalogue& trnsprt_ctlg) const;
    void DeserializeRendrSettng(const t_c_srlz::TransportCatalogue& srlz_trnsprt_ctlg, renderer::MapRenderer& map_rendr_) const;
    void DeserializeTrnsprtRoutr(const t_c_srlz::TransportCatalogue& srlz_trnsprt_ctlg, TransportRouter::TransportRouter& trnsprt_routr_) const;

    r_s_srlz::Color GetCurrentVariantSrlzColor(const svg::Color& color) const;
    svg::Color GetCurrentVariantColor(const r_s_srlz::Color& s_color) const;
};


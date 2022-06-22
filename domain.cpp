#include "domain.h"
#include "geo.h"


double domain::ComputeDistance(double from_lat, double from_lng, double to_lat, double to_lng)
{
    return geo::ComputeDistance( {from_lat, from_lng}, {to_lat, to_lng} );
}

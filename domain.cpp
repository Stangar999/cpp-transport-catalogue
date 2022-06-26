#include "domain.h"

double domain::ComputeDistance(const Stop* from_stop, const Stop* to_stop)
{
    return geo::ComputeDistance( from_stop->coord, to_stop->coord );
}


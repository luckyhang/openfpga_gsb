#ifndef OPENFPGA_PHYSICAL_TILE_UTILS_H
#define OPENFPGA_PHYSICAL_TILE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include <set>
#include "device_grid.h"
#include "physical_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

float find_physical_tile_pin_Fc(t_physical_tile_type_ptr type,
                                const int& pin);

std::set<e_side> find_physical_io_tile_located_sides(const DeviceGrid& grids,
                                                     t_physical_tile_type_ptr physical_tile,
                                                     const bool& enable_gsb_routing = false);

} /* end namespace openfpga */

#endif

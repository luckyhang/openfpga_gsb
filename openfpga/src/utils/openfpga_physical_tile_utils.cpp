/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from 
 * VPR t_physical_tile_type 
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_physical_tile_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find the Fc of a pin in physical tile 
 *******************************************************************/
float find_physical_tile_pin_Fc(t_physical_tile_type_ptr type,
                                const int& pin) {
  for (const t_fc_specification& fc_spec : type->fc_specs) {
    if (fc_spec.pins.end() != std::find(fc_spec.pins.begin(), fc_spec.pins.end(), pin)) {
      return fc_spec.fc_value; 
    }
  }
  /* Every pin should have a Fc, give a wrong value */
  VTR_LOGF_ERROR(__FILE__, __LINE__,
                "Fail to find the Fc for %s.pin[%lu]\n",
                type->name, pin);
  exit(1);
} 

/********************************************************************
 * Find sides/locations of a I/O physical tile in the context of a FPGA fabric
 * The I/O grid may locate at 
 * - one or more border of a FPGA (TOP, RIGHT, BOTTOM, LEFT)
 *   We will collect each side that the I/O locates 
 * - the center of a FPGA
 *   We will add NUM_SIDEs for these I/Os
 *******************************************************************/
std::set<e_side> find_physical_io_tile_located_sides(const DeviceGrid& grids,
                                                     t_physical_tile_type_ptr physical_tile,
                                                     const bool& enable_gsb_routing) {
  std::set<e_side> io_sides;
  bool center_io = false;
 
  /* Search the core part */
  /* shen: the border of grid maybe EMPTY, take care of this situation */
  size_t grid_core_x_end, grid_core_y_end;
  grid_core_x_end = enable_gsb_routing ? grids.width() - 2 : grids.width() - 1;
  grid_core_y_end = enable_gsb_routing ? grids.height() - 2 : grids.height() - 1;
  size_t grid_core_x_beg, grid_core_y_beg;
  grid_core_x_beg = enable_gsb_routing ? 2 : 1;
  grid_core_y_beg = enable_gsb_routing ? 2 : 1;

  size_t grid_border_x_end, grid_border_y_end;
  grid_border_x_end = enable_gsb_routing ? grids.width() - 2 : grids.width() - 1;
  grid_border_y_end = enable_gsb_routing ? grids.height() - 2 : grids.height() - 1;
  size_t grid_border_x_beg, grid_border_y_beg;
  grid_border_x_beg = enable_gsb_routing ? 2 : 1;
  grid_border_y_beg = enable_gsb_routing ? 2 : 1;

  for (size_t ix = grid_core_x_beg; ix < grid_core_x_end; ++ix) {
    for (size_t iy = grid_core_y_beg; iy < grid_core_y_end; ++iy) {
      /* If located in center, we add a NUM_SIDES and finish */
      if (physical_tile == grids[ix][iy].type) {
        io_sides.insert(NUM_SIDES);
        center_io = true;
        break;
      } 
    }
    if (true == center_io) {
      break;
    }
  }  

  /* Search the border side */
  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> fpga_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = grid_border_x_beg; ix < grid_border_x_end; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, grid_border_y_end));
  } 

  /* RIGHT side */
  for (size_t iy = grid_border_y_beg; iy < grid_border_y_end; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(grid_border_x_end, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = grid_border_x_beg; ix < grid_border_x_end; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, grid_border_x_beg-1));
  } 

  /* LEFT side */
  for (size_t iy = grid_border_y_beg; iy < grid_border_y_end; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(grid_border_x_beg-1, iy));
  }
  for (const e_side& fpga_side : fpga_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[fpga_side]) {
      /* If located in center, we add a NUM_SIDES and finish */
      if (physical_tile == grids[io_coordinate.x()][io_coordinate.y()].type) {
        io_sides.insert(fpga_side);
        break;
      } 
    }
  }

  return io_sides;
}

} /* end namespace openfpga */

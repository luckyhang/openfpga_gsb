#ifndef ANNOTATE_RR_GRAPH_H
#define ANNOTATE_RR_GRAPH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"
#include "device_rr_gsb.h"

/* shen: imux|omux|gsb node associate segment name, ask qian */
#define IMUX "imux_medium"
#define OMUX "omux_medium"
#define GSB "gsb_medium"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_device_rr_gsb(const DeviceContext& vpr_device_ctx, 
                            DeviceRRGSB& device_rr_gsb,
                            const bool& verbose_output,
                            const bool& enable_gsb_routing);

void sort_device_rr_gsb_chan_node_in_edges(const RRGraph& rr_graph,
                                           DeviceRRGSB& device_rr_gsb,
                                           const bool& verbose_output);

void annotate_rr_graph_circuit_models(const DeviceContext& vpr_device_ctx, 
                                      const Arch& openfpga_arch,
                                      VprDeviceAnnotation& vpr_device_annotation,
                                      const bool& verbose_output,
                                      const bool& enable_gsb_routing);

} /* end namespace openfpga */

#endif

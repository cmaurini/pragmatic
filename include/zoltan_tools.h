/*  Copyright (C) 2010 Imperial College London and others.
 *
 *  Please see the AUTHORS file in the main source directory for a
 *  full list of copyright holders.
 *
 *  Gerard Gorman
 *  Applied Modelling and Computation Group
 *  Department of Earth Science and Engineering
 *  Imperial College London
 *
 *  g.gorman@imperial.ac.uk
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following
 *  disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */

#ifndef ZOLTAN_COLOUR_H
#define ZOLTAN_COLOUR_H

#include "pragmatic_config.h"

#ifndef HAVE_ZOLTAN
#error No Zoltan support.
#endif

#include <stddef.h>
#include <mpi.h>
#include <zoltan.h>

#ifdef __cplusplus
extern "C" {
#endif

  /*! \brief This is used internally to package the data required by
   * the Zoltan C interface.
   */
  typedef struct {
    /* Rank of local process.
     */
    int rank;

    /* Number of nodes in the graph assigned to the local process.
     */
    size_t npnodes;
    
    /* Total number of nodes on local process.
     */
    size_t nnodes;

    /* Array storing the number of edges connected to each node.
     */
    size_t *nedges;

    /* Array storing the edges in compressed row storage format.
     */
    size_t *csr_edges;

    /* Mapping from local node numbers to global node numbers.
     */
    int *gid;

    /* Process owner of each node.
     */
    int *owner;

    /* Graph colouring.
     */
    int *colour;

    /* Re-order.
     */
    unsigned int *order;
  } zoltan_graph_t;
  
  void zoltan_colour(zoltan_graph_t *graph, int distance, MPI_Comm mpi_comm);
  void zoltan_reorder(zoltan_graph_t *graph);

  /* Only used in zoltan_tools.c
   */
  void pragmatic_zoltan_verify(int ierr, const char *str);
  int num_obj_fn(void* data, int* ierr);
  void obj_list_fn(void* data, int num_gid_entries, int num_lid_entries, ZOLTAN_ID_PTR global_ids,
                   ZOLTAN_ID_PTR local_ids, int wgt_dim, float* obj_wgts, int* ierr);
  void num_edges_multi_fn(void* data, int num_gid_entries, int num_lid_entries, int num_obj, 
                          ZOLTAN_ID_PTR global_ids, ZOLTAN_ID_PTR local_ids, int* num_edges, int* ierr);
  void edge_list_multi_fn(void* data, int num_gid_entries, int num_lid_entries, int num_obj, 
                          ZOLTAN_ID_PTR global_ids, ZOLTAN_ID_PTR local_ids, int* num_edges, ZOLTAN_ID_PTR nbor_global_ids,
                          int* nbor_procs, int wgt_dim, float *ewgts, int* ierr);

#ifdef __cplusplus
}
#endif
#endif

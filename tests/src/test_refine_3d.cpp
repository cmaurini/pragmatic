/* 
 *    Copyright (C) 2010 Imperial College London and others.
 *    
 *    Please see the AUTHORS file in the main source directory for a full list
 *    of copyright holders.
 *
 *    Gerard Gorman
 *    Applied Modelling and Computation Group
 *    Department of Earth Science and Engineering
 *    Imperial College London
 *
 *    amcgsoftware@imperial.ac.uk
 *    
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation,
 *    version 2.1 of the License.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *    USA
 */
#include <iostream>
#include <vector>

#include <omp.h>

#include "Mesh.h"
#include "Surface.h"
#include "VTKTools.h"
#include "MetricField.h"

#include "Refine.h"

using namespace std;

int main(int argc, char **argv){
  Mesh<double, int> *mesh=VTKTools<double, int>::import_vtu("../data/box10x10x10.vtu");

  Surface<double, int> surface(*mesh);

  MetricField<double, int> metric_field(*mesh, surface);

  size_t NNodes = mesh->get_number_nodes();

  vector<double> psi(NNodes);
  for(size_t i=0;i<NNodes;i++)
    psi[i] = 
      pow(mesh->get_coords(i)[0], 4) +
      pow(mesh->get_coords(i)[1], 4) + 
      pow(mesh->get_coords(i)[2], 4);
  
  metric_field.add_field(&(psi[0]), 0.02);
  metric_field.update_mesh();
  
  Refine<double, int> adapt(*mesh, surface);

  double tic = omp_get_wtime();
  adapt.refine(sqrt(2));
  double toc = omp_get_wtime();

  double lrms = mesh->get_lrms();
  double qrms = mesh->get_qrms();

  std::map<int, int> active_vertex_map;
  mesh->defragment(&active_vertex_map);
  surface.defragment(&active_vertex_map);

  int nelements = mesh->get_number_elements();  
  
  VTKTools<double, int>::export_vtu("../data/test_refine_3d", mesh);
  VTKTools<double, int>::export_vtu("../data/test_refine_3d_surface", &surface);

  delete mesh;

  std::cout<<"Coarsen loop time:    "<<toc-tic<<std::endl
           <<"Number elements:      "<<nelements<<std::endl
           <<"Edge length RMS:      "<<lrms<<std::endl
           <<"Quality RMS:          "<<qrms<<std::endl;
  
 if((nelements>110000)&&(nelements<125000)&&(lrms<0.5)&&(qrms<0.7))
    std::cout<<"pass"<<std::endl;
  else
    std::cout<<"fail"<<std::endl;

  return 0;
}
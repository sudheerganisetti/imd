/******************************************************************************
*
* imd_main_risc_3d.c -- main loop, risc specific part, three dimensions
*
******************************************************************************/

/******************************************************************************
* $RCSfile$
* $Revision$
* $Date$
******************************************************************************/

#include "imd.h"


/*****************************************************************************
*
* calc_forces()
*
*****************************************************************************/

void calc_forces(void)
{
  int n, k;

  /* clear global accumulation variables */
  tot_pot_energy = 0.0;
  virial         = 0.0;
  vir_vect.x     = 0.0;
  vir_vect.y     = 0.0;
  vir_vect.z     = 0.0;
  
#ifdef EAM
  memset(eam_rho,  0,(natoms+1)*        sizeof(real));
  memset(eam_ij,   0,(natoms+1)*eam_len*sizeof(real));
  memset(eam_dij_x,0,(natoms+1)*eam_len*sizeof(real));
  memset(eam_dij_y,0,(natoms+1)*eam_len*sizeof(real));
  memset(eam_dij_z,0,(natoms+1)*eam_len*sizeof(real));
#endif /* EAM */

  /* clear per atom accumulation variables */
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (k=0; k<ncells; ++k) {
    int  i,j;
    cell *p;
    p = cell_array + k;
    for (i=0; i<p->n; ++i) {
      p->kraft X(i) = 0.0;
      p->kraft Y(i) = 0.0;
      p->kraft Z(i) = 0.0;
#ifdef UNIAX
      p->dreh_moment X(i) = 0.0;
      p->dreh_moment Y(i) = 0.0;
      p->dreh_moment Z(i) = 0.0;
#endif
#ifdef TRANSPORT
      p->heatcond[i] = 0.0;
#endif     
#ifdef STRESS_TENS
      p->presstens X(i) = 0.0;
      p->presstens Y(i) = 0.0;
      p->presstens Z(i) = 0.0;
      p->presstens_offdia X(i) = 0.0;
      p->presstens_offdia Y(i) = 0.0;
      p->presstens_offdia Z(i) = 0.0;
#endif     
#ifndef MONOLJ
      p->pot_eng[i] = 0.0;
#endif
#ifdef COVALENT
      for (j=0; j<p->n; ++j) {
        p->neigh[j]->n = 0;
      }
#endif
#ifdef EAM2
      p->eam2_rho_h[i] = 0.0; /* zero host electron density at atom site */
#endif
    }
  }

  /* compute forces for all pairs of cells */
  for (n=0; n<6; ++n) {
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
    for (k=0; k<npairs[n]; ++k) {
      vektor pbc;
      pair *P;
      P = pairs[n]+k;
      pbc.x = P->ipbc[0]*box_x.x + P->ipbc[1]*box_y.x + P->ipbc[2]*box_z.x;
      pbc.y = P->ipbc[0]*box_x.y + P->ipbc[1]*box_y.y + P->ipbc[2]*box_z.y;
      pbc.z = P->ipbc[0]*box_x.z + P->ipbc[1]*box_y.z + P->ipbc[2]*box_z.z;
#ifdef EAM
      /* first EAM call */
      do_forces_eam_1(cell_array + P->np, cell_array + P->nq, pbc);
#elif defined(EAM2)
      eam2_do_forces1(cell_array + P->np, cell_array + P->nq, pbc);
#elif defined(UNIAX)
      do_forces_uniax(cell_array + P->np, cell_array + P->nq, pbc);
#else
      do_forces(cell_array + P->np, cell_array + P->nq, pbc);
#endif
    }
  }

#ifdef EAM2
  /* if EAM2, we have to loop a second time over pairs of distinct cells */
  for (n=0; n<6; ++n) {
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
    for (k=0; k<npairs[n]; ++k) {
      vektor pbc;
      pair *P;
      P = pairs[n]+k;
      pbc.x = -(P->ipbc[0]*box_x.x + P->ipbc[1]*box_y.x + P->ipbc[2]*box_z.x);
      pbc.y = -(P->ipbc[0]*box_x.y + P->ipbc[1]*box_y.y + P->ipbc[2]*box_z.y);
      pbc.z = -(P->ipbc[0]*box_x.z + P->ipbc[1]*box_y.z + P->ipbc[2]*box_z.z);
      if (P->np != P->nq)
        eam2_do_forces1(cell_array + P->nq, cell_array + P->np, pbc);
    }
  }

  /* second EAM2 loop over all cells pairs */
  for (n=0; n<6; ++n) {
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
    for (k=0; k<npairs[n]; ++k) {
      vektor pbc;
      pair *P;
      P = pairs[n]+k;
      pbc.x = P->ipbc[0]*box_x.x + P->ipbc[1]*box_y.x + P->ipbc[2]*box_z.x;
      pbc.y = P->ipbc[0]*box_x.y + P->ipbc[1]*box_y.y + P->ipbc[2]*box_z.y;
      pbc.z = P->ipbc[0]*box_x.z + P->ipbc[1]*box_y.z + P->ipbc[2]*box_z.z;
      eam2_do_forces2(cell_array + P->np, cell_array + P->nq, pbc);
    }
  }

  /* if EAM2, we have to loop a second time over pairs of distinct cells */
  for (n=0; n<6; ++n) {
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
    for (k=0; k<npairs[n]; ++k) {
      vektor pbc;
      pair *P;
      P = pairs[n]+k;
      pbc.x = -(P->ipbc[0]*box_x.x + P->ipbc[1]*box_y.x + P->ipbc[2]*box_z.x);
      pbc.y = -(P->ipbc[0]*box_x.y + P->ipbc[1]*box_y.y + P->ipbc[2]*box_z.y);
      pbc.z = -(P->ipbc[0]*box_x.z + P->ipbc[1]*box_y.z + P->ipbc[2]*box_z.z);
      if (P->np != P->nq)
        eam2_do_forces2(cell_array + P->nq, cell_array + P->np, pbc);
    }
  }
#endif /* EAM2 */

#ifdef EAM
  /* EAM cohesive function potential */
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
  for (k=0; k<ncells; ++k) {
    do_forces_eam_2(cell_array + k);
  }
#endif /* EAM */

#ifdef TTBP
  /* TTBP: three body potential */
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
  for (k=0; k<ncells; ++k) {
    do_forces_ttbp(cell_array + k);
  }
#endif /* TTBP */

#ifdef TERSOFF
#ifdef _OPENMP
#pragma omp parallel for reduction(+:tot_pot_energy,virial,vir_vect.x,vir_vect.y,vir_vect.z)
#endif
  for (k=0; k<ncells; ++k) {
    do_forces_tersoff(cell_array + k); 
  }
#endif /* TERSOFF */

}

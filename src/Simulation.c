/*                   _
 *   _ __ ___   ___ | | ___  ___
 *  | '_ ` _ \ / _ \| |/ _ \/ __|
 *  | | | | | | (_) | |  __/ (__
 *  |_| |_| |_|\___/|_|\___|\___| - Molecular Dynamics Framework
 *
 *  Copyright (C) 2016  Carlo Del Don  (deldonc@student.ethz.ch)
 *                      Michel Breyer  (mbreyer@student.ethz.ch)
 *                      Florian Frei   (flofrei@student.ethz.ch)
 *                      Fabian Thuring (thfabian@student.ethz.ch)
 *
 *  This file is distributed under the MIT Open Source License.
 *  See LICENSE.txt for details.
 */

#include <molec/Force.h>
#include <molec/Integrator.h>
#include <molec/InitialCondition.h>
#include <molec/Parameter.h>
#include <molec/Periodic.h>
#include <molec/Simulation.h>
#include <stdlib.h>
#include <stdio.h>

void molec_run_simulation(void (*molec_compute_force)( molec_Simulation_SOA_t*, Real*, int))
{
    // Set parameters
    if(molec_parameter == NULL)
        molec_error("molec_parameter is nullptr\n");

    // Local alias
    const int N = molec_parameter->N;
    const int Nstep = molec_parameter->Nstep;

    // Allocate arrays
    molec_Simulation_SOA_t* sim = malloc(sizeof(molec_Simulation_SOA_t));

    MOLEC_MALLOC(sim->x, sizeof(Real) * N);
    MOLEC_MALLOC(sim->y, sizeof(Real) * N);
    MOLEC_MALLOC(sim->z, sizeof(Real) * N);

    MOLEC_MALLOC(sim->v_x, sizeof(Real) * N);
    MOLEC_MALLOC(sim->v_y, sizeof(Real) * N);
    MOLEC_MALLOC(sim->v_z, sizeof(Real) * N);

#if MOLEC_SOA_SWAP
    MOLEC_MALLOC(sim->x_copy, sizeof(Real) * N);
    MOLEC_MALLOC(sim->y_copy, sizeof(Real) * N);
    MOLEC_MALLOC(sim->z_copy, sizeof(Real) * N);

    MOLEC_MALLOC(sim->v_x_copy, sizeof(Real) * N);
    MOLEC_MALLOC(sim->v_y_copy, sizeof(Real) * N);
    MOLEC_MALLOC(sim->v_z_copy, sizeof(Real) * N);
#endif

    MOLEC_MALLOC(sim->f_x, sizeof(Real) * N);
    MOLEC_MALLOC(sim->f_y, sizeof(Real) * N);
    MOLEC_MALLOC(sim->f_z, sizeof(Real) * N);

    // Set initial conditions
    molec_set_initial_condition(sim);

    // Run sim
    Real Ekin_x = 0.0, Ekin_y = 0.0, Ekin_z = 0.0;
    Real Epot = 0.0;
    
    printf("\n      ================ MOLEC - Simulation steps ================\n\n");
    printf("%10s\t%15s\t%15s\t%15s\n", "Step", "Ekin", "Epot", "Etot");
    for(int n = 1; n <= Nstep; ++n)
    {
        Ekin_x = Ekin_y = Ekin_z = 0.0;
        Epot = 0.0;
    
        // 1. Compute force
        molec_compute_force(sim, &Epot, N);

        // 2. Integrate ODE
        molec_integrator_leapfrog_refrence(sim->x, sim->v_x, sim->f_x, &Ekin_x, N);
        molec_integrator_leapfrog_refrence(sim->y, sim->v_y, sim->f_y, &Ekin_y, N);
        molec_integrator_leapfrog_refrence(sim->z, sim->v_z, sim->f_z, &Ekin_z, N);

        // 3. Apply periodic boundary conditions
        molec_periodic_refrence(sim->x, N);
        molec_periodic_refrence(sim->y, N);
        molec_periodic_refrence(sim->z, N);

        // 4. Report result
        Real Ekin = Ekin_x + Ekin_y + Ekin_z;
        Real Etot = Ekin + Epot;
        printf("%10i\t%15.6f\t%15.6f\t%15.6f\n", n, Ekin, Epot, Etot);
        
        /* molec_print_simulation_SOA(sim); */
    }

    // Free memory
    MOLEC_FREE(sim->x);
    MOLEC_FREE(sim->y);
    MOLEC_FREE(sim->z);

    MOLEC_FREE(sim->v_x);
    MOLEC_FREE(sim->v_y);
    MOLEC_FREE(sim->v_z);

#if MOLEC_SOA_SWAP
    MOLEC_FREE(sim->x_copy);
    MOLEC_FREE(sim->y_copy);
    MOLEC_FREE(sim->z_copy);

    MOLEC_FREE(sim->v_x_copy);
    MOLEC_FREE(sim->v_y_copy);
    MOLEC_FREE(sim->v_z_copy);
#endif

    MOLEC_FREE(sim->f_x);
    MOLEC_FREE(sim->f_y);
    MOLEC_FREE(sim->f_z);

    MOLEC_FREE(sim);
    MOLEC_FREE(molec_parameter);
}

void molec_print_simulation_SOA(const molec_Simulation_SOA_t* sim)
{
    const int N = molec_parameter->N;
    for(int i = 0; i < N; ++i)
        printf(" (%f, %f, %f)\t(%f, %f, %f)\n", sim->x[i], sim->y[i],
               sim->z[i], sim->v_x[i], sim->v_y[i], sim->v_z[i]);
}


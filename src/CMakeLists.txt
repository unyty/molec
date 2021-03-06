# molec - Molecular Dynamics Framework
#
#  Copyright (C) 2016  Carlo Del Don  (deldonc@student.ethz.ch)
#                      Michel Breyer  (mbreyer@student.ethz.ch)
#                      Florian Frei   (flofrei@student.ethz.ch)
#                      Fabian Thuring (thfabian@student.ethz.ch)
#
#  This file is distributed under the MIT Open Source License.
#  See LICENSE.txt for details.

cmake_minimum_required(VERSION 2.8)

set(MOLEC_CORE_SOURCE
    Common.c
    Dump.c
    ForceN2Refrence.c
    ForceCellList.c
    ForceCellListKnuth.c
    ForceQuadrant.c
    Integrator.c
    InitialCondition.c
    Quadrant.c
    LoadConfig.c
    Parameter.c
    Periodic.c
    Simulation.c
    Sort.c
    Timer.c
)

# MolecCore library
add_library(molecCore STATIC ${MOLEC_CORE_SOURCE} ${MOLEC_HEADERS})

# Molec executable
add_executable(molec main.c)
target_link_libraries(molec molecCore argtable2)

# Link against math library (-lm)
if(NOT("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC"))
    target_link_libraries(molec m)
endif(NOT("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC"))

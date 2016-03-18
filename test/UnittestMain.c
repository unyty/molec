/*                   _
 *   _ __ ___   ___ | | ___  ___
 *  | '_ ` _ \ / _ \| |/ _ \/ __|
 *  | | | | | | (_) | |  __/ (__
 *  |_| |_| |_|\___/|_|\___|\___| - Molecular Dynamics Framework
 *
 *  Copyright (C) 2016  Carlo Del Don  (deldonc@student.ethz.ch)
 *                      Michel Breyer  (mbreyer@student.ethz.ch)
 *                      Florian Frei   (frofrei@student.ethz.ch)
 *                      Fabian Thuring (thfabian@student.ethz.ch)
 *
 *  This file is distributed under the MIT Open Source License.
 *  See LICENSE.txt for details.
 */

#define TINYTEST_MAIN
#include "Unittest.h"

/**
 * Setup the Unittest environment
 *
 * For documentaion see https://github.com/thfabian/tinytest
 */
int main(int argc, char* argv[])
{
    tinytest_init(argc, argv);

    // Register new testcases here
    REGISTER_TEST_CASE(molec_UnittestTimer);

    int ret = tinytest_run();
    tinytest_free();
    
    return ret;
}

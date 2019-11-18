/* lcstest.c
 *
 * Copyright (C) 2015 - 2019, Helmut Wollmersdorfer, all rights reserved.
 *
*/


#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <nmmintrin.h>

#include "llcs.h"
#include "llcs.c"


int main (void) {
    clock_t tic;
    clock_t toc;
    double elapsed;
    double rate;
    
    uint64_t count;
    uint64_t megacount;
    uint32_t iters     = 1000000;
    uint32_t megaiters = 1;

    // m=10, n=11, llcs=7, d=4, sim=0.667
    char str1[] = "Choerephon";
    char str2[] = "Chrerrplzon";
    
    uint32_t len1 = strlen(str1);
    uint32_t len2 = strlen(str2);
    
    int length_lcs;

    /* ########## llcs_asci ########## */
   
    tic = clock();

    megaiters = 20;
    
    for (megacount = 0; megacount < megaiters; megacount++) {
  	  for (count = 0; count < iters; count++) {
      	  length_lcs = llcs_asci (str1, str2, len1, len2);
  	  }
  	}

    toc = clock();
    elapsed = (double)(toc - tic) / (double)CLOCKS_PER_SEC;
    rate    = (double)megaiters / (double)elapsed;
    
    // need to use the result to avoid loop unrolling ---------------------vv
    printf("[llcs_asci] iters: %u M Elapsed: %f s Rate: %.1f (M/sec) llcs: %u\n", 
        megaiters, elapsed, rate, length_lcs);
 
    /* #################### */
                      
    return 0;

}

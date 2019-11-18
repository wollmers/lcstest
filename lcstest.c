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

static const uint64_t width = 64;

int count_bits(uint64_t bits) {
  bits = bits - ((bits >> 1) & 0x5555555555555555ull);
  bits = (bits & 0x3333333333333333ull) + ((bits >> 2) & 0x3333333333333333ull);
  // (bytesof(bits) -1) * bitsofbyte = (8-1)*8 = 56 -------------------------------vv
  return ((bits + (bits >> 4) & 0x0f0f0f0f0f0f0f0full) * 0x0101010101010101ull) >> 56;
}

int llcs_asci (char * a, char * b, uint32_t alen, uint32_t blen) {
    // static uint64_t posbits[128] = { 0 }; //    73.4 (M/sec)
    // uint64_t posbits[128] = { 0 };        // 53050.4 (M/sec)
    uint64_t posbits[128];                   // 56338.0 (M/sec)

    uint64_t i;   
    for (i=0; i < 128; i++) { posbits[i] = 0; } // needed

    for (i=0; i < alen; i++) {
      	posbits[(unsigned int)a[i]] |= 0x1ull << (i % width);
    }    

    uint64_t v = ~0ull;
    
    for (i=0; i < blen; i++) {
      uint64_t p = posbits[(unsigned int)b[i]];
      uint64_t u = v & p;
      v = (v + u) | (v - u);
    }

    return count_bits(~v);  // portable
    //return _mm_popcnt_u64(~v); 
}

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

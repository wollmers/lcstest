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



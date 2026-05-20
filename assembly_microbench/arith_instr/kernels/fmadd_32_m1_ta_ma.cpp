

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 32(rv64gc_FMADD.S_32b) )";
unsigned long block_inst = 32;
unsigned long lanes = 1;
unsigned long block_ops = (32*(1*lanes));
unsigned long unroll = 4;

void set_all_reg()
{
    int avl = 1;
    
    

    float *mem = (float *)malloc(avl * sizeof(float));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( float )((float)rand() / RAND_MAX * 10.0);
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
    asm volatile (
        "flw f0, (%0)\n\t"
        "flw f1, (%0)\n\t"
        "flw f2, (%0)\n\t"
        "flw f3, (%0)\n\t"
        "flw f4, (%0)\n\t"
        "flw f5, (%0)\n\t"
        "flw f6, (%0)\n\t"
        "flw f7, (%0)\n\t"
        "flw f8, (%0)\n\t"
        "flw f9, (%0)\n\t"
        "flw f10, (%0)\n\t"
        "flw f11, (%0)\n\t"
        "flw f12, (%0)\n\t"
        "flw f13, (%0)\n\t"
        "flw f14, (%0)\n\t"
        "flw f15, (%0)\n\t"
        "flw f16, (%0)\n\t"
        "flw f17, (%0)\n\t"
        "flw f18, (%0)\n\t"
        "flw f19, (%0)\n\t"
        "flw f20, (%0)\n\t"
        "flw f21, (%0)\n\t"
        "flw f22, (%0)\n\t"
        "flw f23, (%0)\n\t"
        "flw f24, (%0)\n\t"
        "flw f25, (%0)\n\t"
        "flw f26, (%0)\n\t"
        "flw f27, (%0)\n\t"
        "flw f28, (%0)\n\t"
        "flw f29, (%0)\n\t"
        "flw f30, (%0)\n\t"
        "flw f31, (%0)\n\t"
    : /* no input */
    : "r"(mem)
    : "memory"
    );


free(mem);
}



void kernel(unsigned long iters)
{
    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
        "fmadd.s f0, f8, f16, f16 \n\t"
        "fmadd.s f1, f9, f17, f17 \n\t"
        "fmadd.s f2, f10, f18, f18 \n\t"
        "fmadd.s f3, f11, f19, f19 \n\t"
        "fmadd.s f4, f12, f20, f20 \n\t"
        "fmadd.s f5, f13, f21, f21 \n\t"
        "fmadd.s f6, f14, f22, f22 \n\t"
        "fmadd.s f7, f15, f23, f23 \n\t"
        "fmadd.s f8, f16, f24, f24 \n\t"
        "fmadd.s f9, f17, f25, f25 \n\t"
        "fmadd.s f10, f18, f26, f26 \n\t"
        "fmadd.s f11, f19, f27, f27 \n\t"
        "fmadd.s f12, f20, f28, f28 \n\t"
        "fmadd.s f13, f21, f29, f29 \n\t"
        "fmadd.s f14, f22, f30, f30 \n\t"
        "fmadd.s f15, f23, f31, f31 \n\t"
        "fmadd.s f16, f24, f0, f0 \n\t"
        "fmadd.s f17, f25, f1, f1 \n\t"
        "fmadd.s f18, f26, f2, f2 \n\t"
        "fmadd.s f19, f27, f3, f3 \n\t"
        "fmadd.s f20, f28, f4, f4 \n\t"
        "fmadd.s f21, f29, f5, f5 \n\t"
        "fmadd.s f22, f30, f6, f6 \n\t"
        "fmadd.s f23, f31, f7, f7 \n\t"
        "fmadd.s f24, f0, f8, f8 \n\t"
        "fmadd.s f25, f1, f9, f9 \n\t"
        "fmadd.s f26, f2, f10, f10 \n\t"
        "fmadd.s f27, f3, f11, f11 \n\t"
        "fmadd.s f28, f4, f12, f12 \n\t"
        "fmadd.s f29, f5, f13, f13 \n\t"
        "fmadd.s f30, f6, f14, f14 \n\t"
        "fmadd.s f31, f7, f15, f15 \n\t"
        "fmadd.s f0, f8, f16, f16 \n\t"
        "fmadd.s f1, f9, f17, f17 \n\t"
        "fmadd.s f2, f10, f18, f18 \n\t"
        "fmadd.s f3, f11, f19, f19 \n\t"
        "fmadd.s f4, f12, f20, f20 \n\t"
        "fmadd.s f5, f13, f21, f21 \n\t"
        "fmadd.s f6, f14, f22, f22 \n\t"
        "fmadd.s f7, f15, f23, f23 \n\t"
        "fmadd.s f8, f16, f24, f24 \n\t"
        "fmadd.s f9, f17, f25, f25 \n\t"
        "fmadd.s f10, f18, f26, f26 \n\t"
        "fmadd.s f11, f19, f27, f27 \n\t"
        "fmadd.s f12, f20, f28, f28 \n\t"
        "fmadd.s f13, f21, f29, f29 \n\t"
        "fmadd.s f14, f22, f30, f30 \n\t"
        "fmadd.s f15, f23, f31, f31 \n\t"
        "fmadd.s f16, f24, f0, f0 \n\t"
        "fmadd.s f17, f25, f1, f1 \n\t"
        "fmadd.s f18, f26, f2, f2 \n\t"
        "fmadd.s f19, f27, f3, f3 \n\t"
        "fmadd.s f20, f28, f4, f4 \n\t"
        "fmadd.s f21, f29, f5, f5 \n\t"
        "fmadd.s f22, f30, f6, f6 \n\t"
        "fmadd.s f23, f31, f7, f7 \n\t"
        "fmadd.s f24, f0, f8, f8 \n\t"
        "fmadd.s f25, f1, f9, f9 \n\t"
        "fmadd.s f26, f2, f10, f10 \n\t"
        "fmadd.s f27, f3, f11, f11 \n\t"
        "fmadd.s f28, f4, f12, f12 \n\t"
        "fmadd.s f29, f5, f13, f13 \n\t"
        "fmadd.s f30, f6, f14, f14 \n\t"
        "fmadd.s f31, f7, f15, f15 \n\t"
        "fmadd.s f0, f8, f16, f16 \n\t"
        "fmadd.s f1, f9, f17, f17 \n\t"
        "fmadd.s f2, f10, f18, f18 \n\t"
        "fmadd.s f3, f11, f19, f19 \n\t"
        "fmadd.s f4, f12, f20, f20 \n\t"
        "fmadd.s f5, f13, f21, f21 \n\t"
        "fmadd.s f6, f14, f22, f22 \n\t"
        "fmadd.s f7, f15, f23, f23 \n\t"
        "fmadd.s f8, f16, f24, f24 \n\t"
        "fmadd.s f9, f17, f25, f25 \n\t"
        "fmadd.s f10, f18, f26, f26 \n\t"
        "fmadd.s f11, f19, f27, f27 \n\t"
        "fmadd.s f12, f20, f28, f28 \n\t"
        "fmadd.s f13, f21, f29, f29 \n\t"
        "fmadd.s f14, f22, f30, f30 \n\t"
        "fmadd.s f15, f23, f31, f31 \n\t"
        "fmadd.s f16, f24, f0, f0 \n\t"
        "fmadd.s f17, f25, f1, f1 \n\t"
        "fmadd.s f18, f26, f2, f2 \n\t"
        "fmadd.s f19, f27, f3, f3 \n\t"
        "fmadd.s f20, f28, f4, f4 \n\t"
        "fmadd.s f21, f29, f5, f5 \n\t"
        "fmadd.s f22, f30, f6, f6 \n\t"
        "fmadd.s f23, f31, f7, f7 \n\t"
        "fmadd.s f24, f0, f8, f8 \n\t"
        "fmadd.s f25, f1, f9, f9 \n\t"
        "fmadd.s f26, f2, f10, f10 \n\t"
        "fmadd.s f27, f3, f11, f11 \n\t"
        "fmadd.s f28, f4, f12, f12 \n\t"
        "fmadd.s f29, f5, f13, f13 \n\t"
        "fmadd.s f30, f6, f14, f14 \n\t"
        "fmadd.s f31, f7, f15, f15 \n\t"
        "fmadd.s f0, f8, f16, f16 \n\t"
        "fmadd.s f1, f9, f17, f17 \n\t"
        "fmadd.s f2, f10, f18, f18 \n\t"
        "fmadd.s f3, f11, f19, f19 \n\t"
        "fmadd.s f4, f12, f20, f20 \n\t"
        "fmadd.s f5, f13, f21, f21 \n\t"
        "fmadd.s f6, f14, f22, f22 \n\t"
        "fmadd.s f7, f15, f23, f23 \n\t"
        "fmadd.s f8, f16, f24, f24 \n\t"
        "fmadd.s f9, f17, f25, f25 \n\t"
        "fmadd.s f10, f18, f26, f26 \n\t"
        "fmadd.s f11, f19, f27, f27 \n\t"
        "fmadd.s f12, f20, f28, f28 \n\t"
        "fmadd.s f13, f21, f29, f29 \n\t"
        "fmadd.s f14, f22, f30, f30 \n\t"
        "fmadd.s f15, f23, f31, f31 \n\t"
        "fmadd.s f16, f24, f0, f0 \n\t"
        "fmadd.s f17, f25, f1, f1 \n\t"
        "fmadd.s f18, f26, f2, f2 \n\t"
        "fmadd.s f19, f27, f3, f3 \n\t"
        "fmadd.s f20, f28, f4, f4 \n\t"
        "fmadd.s f21, f29, f5, f5 \n\t"
        "fmadd.s f22, f30, f6, f6 \n\t"
        "fmadd.s f23, f31, f7, f7 \n\t"
        "fmadd.s f24, f0, f8, f8 \n\t"
        "fmadd.s f25, f1, f9, f9 \n\t"
        "fmadd.s f26, f2, f10, f10 \n\t"
        "fmadd.s f27, f3, f11, f11 \n\t"
        "fmadd.s f28, f4, f12, f12 \n\t"
        "fmadd.s f29, f5, f13, f13 \n\t"
        "fmadd.s f30, f6, f14, f14 \n\t"
        "fmadd.s f31, f7, f15, f15 \n\t"
        : /* no input */
        : /* no output */
        : 
        );
    }
}

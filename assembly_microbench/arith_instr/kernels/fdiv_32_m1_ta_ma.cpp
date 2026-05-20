

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 32(rv64gc_FDIV.S_32b) )";
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
        "fdiv.s f0, f8, f16 \n\t"
        "fdiv.s f1, f9, f17 \n\t"
        "fdiv.s f2, f10, f18 \n\t"
        "fdiv.s f3, f11, f19 \n\t"
        "fdiv.s f4, f12, f20 \n\t"
        "fdiv.s f5, f13, f21 \n\t"
        "fdiv.s f6, f14, f22 \n\t"
        "fdiv.s f7, f15, f23 \n\t"
        "fdiv.s f8, f16, f24 \n\t"
        "fdiv.s f9, f17, f25 \n\t"
        "fdiv.s f10, f18, f26 \n\t"
        "fdiv.s f11, f19, f27 \n\t"
        "fdiv.s f12, f20, f28 \n\t"
        "fdiv.s f13, f21, f29 \n\t"
        "fdiv.s f14, f22, f30 \n\t"
        "fdiv.s f15, f23, f31 \n\t"
        "fdiv.s f16, f24, f0 \n\t"
        "fdiv.s f17, f25, f1 \n\t"
        "fdiv.s f18, f26, f2 \n\t"
        "fdiv.s f19, f27, f3 \n\t"
        "fdiv.s f20, f28, f4 \n\t"
        "fdiv.s f21, f29, f5 \n\t"
        "fdiv.s f22, f30, f6 \n\t"
        "fdiv.s f23, f31, f7 \n\t"
        "fdiv.s f24, f0, f8 \n\t"
        "fdiv.s f25, f1, f9 \n\t"
        "fdiv.s f26, f2, f10 \n\t"
        "fdiv.s f27, f3, f11 \n\t"
        "fdiv.s f28, f4, f12 \n\t"
        "fdiv.s f29, f5, f13 \n\t"
        "fdiv.s f30, f6, f14 \n\t"
        "fdiv.s f31, f7, f15 \n\t"
        "fdiv.s f0, f8, f16 \n\t"
        "fdiv.s f1, f9, f17 \n\t"
        "fdiv.s f2, f10, f18 \n\t"
        "fdiv.s f3, f11, f19 \n\t"
        "fdiv.s f4, f12, f20 \n\t"
        "fdiv.s f5, f13, f21 \n\t"
        "fdiv.s f6, f14, f22 \n\t"
        "fdiv.s f7, f15, f23 \n\t"
        "fdiv.s f8, f16, f24 \n\t"
        "fdiv.s f9, f17, f25 \n\t"
        "fdiv.s f10, f18, f26 \n\t"
        "fdiv.s f11, f19, f27 \n\t"
        "fdiv.s f12, f20, f28 \n\t"
        "fdiv.s f13, f21, f29 \n\t"
        "fdiv.s f14, f22, f30 \n\t"
        "fdiv.s f15, f23, f31 \n\t"
        "fdiv.s f16, f24, f0 \n\t"
        "fdiv.s f17, f25, f1 \n\t"
        "fdiv.s f18, f26, f2 \n\t"
        "fdiv.s f19, f27, f3 \n\t"
        "fdiv.s f20, f28, f4 \n\t"
        "fdiv.s f21, f29, f5 \n\t"
        "fdiv.s f22, f30, f6 \n\t"
        "fdiv.s f23, f31, f7 \n\t"
        "fdiv.s f24, f0, f8 \n\t"
        "fdiv.s f25, f1, f9 \n\t"
        "fdiv.s f26, f2, f10 \n\t"
        "fdiv.s f27, f3, f11 \n\t"
        "fdiv.s f28, f4, f12 \n\t"
        "fdiv.s f29, f5, f13 \n\t"
        "fdiv.s f30, f6, f14 \n\t"
        "fdiv.s f31, f7, f15 \n\t"
        "fdiv.s f0, f8, f16 \n\t"
        "fdiv.s f1, f9, f17 \n\t"
        "fdiv.s f2, f10, f18 \n\t"
        "fdiv.s f3, f11, f19 \n\t"
        "fdiv.s f4, f12, f20 \n\t"
        "fdiv.s f5, f13, f21 \n\t"
        "fdiv.s f6, f14, f22 \n\t"
        "fdiv.s f7, f15, f23 \n\t"
        "fdiv.s f8, f16, f24 \n\t"
        "fdiv.s f9, f17, f25 \n\t"
        "fdiv.s f10, f18, f26 \n\t"
        "fdiv.s f11, f19, f27 \n\t"
        "fdiv.s f12, f20, f28 \n\t"
        "fdiv.s f13, f21, f29 \n\t"
        "fdiv.s f14, f22, f30 \n\t"
        "fdiv.s f15, f23, f31 \n\t"
        "fdiv.s f16, f24, f0 \n\t"
        "fdiv.s f17, f25, f1 \n\t"
        "fdiv.s f18, f26, f2 \n\t"
        "fdiv.s f19, f27, f3 \n\t"
        "fdiv.s f20, f28, f4 \n\t"
        "fdiv.s f21, f29, f5 \n\t"
        "fdiv.s f22, f30, f6 \n\t"
        "fdiv.s f23, f31, f7 \n\t"
        "fdiv.s f24, f0, f8 \n\t"
        "fdiv.s f25, f1, f9 \n\t"
        "fdiv.s f26, f2, f10 \n\t"
        "fdiv.s f27, f3, f11 \n\t"
        "fdiv.s f28, f4, f12 \n\t"
        "fdiv.s f29, f5, f13 \n\t"
        "fdiv.s f30, f6, f14 \n\t"
        "fdiv.s f31, f7, f15 \n\t"
        "fdiv.s f0, f8, f16 \n\t"
        "fdiv.s f1, f9, f17 \n\t"
        "fdiv.s f2, f10, f18 \n\t"
        "fdiv.s f3, f11, f19 \n\t"
        "fdiv.s f4, f12, f20 \n\t"
        "fdiv.s f5, f13, f21 \n\t"
        "fdiv.s f6, f14, f22 \n\t"
        "fdiv.s f7, f15, f23 \n\t"
        "fdiv.s f8, f16, f24 \n\t"
        "fdiv.s f9, f17, f25 \n\t"
        "fdiv.s f10, f18, f26 \n\t"
        "fdiv.s f11, f19, f27 \n\t"
        "fdiv.s f12, f20, f28 \n\t"
        "fdiv.s f13, f21, f29 \n\t"
        "fdiv.s f14, f22, f30 \n\t"
        "fdiv.s f15, f23, f31 \n\t"
        "fdiv.s f16, f24, f0 \n\t"
        "fdiv.s f17, f25, f1 \n\t"
        "fdiv.s f18, f26, f2 \n\t"
        "fdiv.s f19, f27, f3 \n\t"
        "fdiv.s f20, f28, f4 \n\t"
        "fdiv.s f21, f29, f5 \n\t"
        "fdiv.s f22, f30, f6 \n\t"
        "fdiv.s f23, f31, f7 \n\t"
        "fdiv.s f24, f0, f8 \n\t"
        "fdiv.s f25, f1, f9 \n\t"
        "fdiv.s f26, f2, f10 \n\t"
        "fdiv.s f27, f3, f11 \n\t"
        "fdiv.s f28, f4, f12 \n\t"
        "fdiv.s f29, f5, f13 \n\t"
        "fdiv.s f30, f6, f14 \n\t"
        "fdiv.s f31, f7, f15 \n\t"
        : /* no input */
        : /* no output */
        : 
        );
    }
}

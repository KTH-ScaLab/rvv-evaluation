

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 32(rv64gc_FDIV.H_16b) )";
unsigned long block_inst = 32;
unsigned long lanes = 1;
unsigned long block_ops = (32*(1*lanes));
unsigned long unroll = 4;

void set_all_reg()
{
    int avl = 1;
    
    

    _Float16 *mem = (_Float16 *)malloc(avl * sizeof(_Float16));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( _Float16 )((float)rand() / RAND_MAX * 10.0);
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
    asm volatile (
        "flh f0, (%0)\n\t"
        "flh f1, (%0)\n\t"
        "flh f2, (%0)\n\t"
        "flh f3, (%0)\n\t"
        "flh f4, (%0)\n\t"
        "flh f5, (%0)\n\t"
        "flh f6, (%0)\n\t"
        "flh f7, (%0)\n\t"
        "flh f8, (%0)\n\t"
        "flh f9, (%0)\n\t"
        "flh f10, (%0)\n\t"
        "flh f11, (%0)\n\t"
        "flh f12, (%0)\n\t"
        "flh f13, (%0)\n\t"
        "flh f14, (%0)\n\t"
        "flh f15, (%0)\n\t"
        "flh f16, (%0)\n\t"
        "flh f17, (%0)\n\t"
        "flh f18, (%0)\n\t"
        "flh f19, (%0)\n\t"
        "flh f20, (%0)\n\t"
        "flh f21, (%0)\n\t"
        "flh f22, (%0)\n\t"
        "flh f23, (%0)\n\t"
        "flh f24, (%0)\n\t"
        "flh f25, (%0)\n\t"
        "flh f26, (%0)\n\t"
        "flh f27, (%0)\n\t"
        "flh f28, (%0)\n\t"
        "flh f29, (%0)\n\t"
        "flh f30, (%0)\n\t"
        "flh f31, (%0)\n\t"
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
        "fdiv.h f0, f8, f16 \n\t"
        "fdiv.h f1, f9, f17 \n\t"
        "fdiv.h f2, f10, f18 \n\t"
        "fdiv.h f3, f11, f19 \n\t"
        "fdiv.h f4, f12, f20 \n\t"
        "fdiv.h f5, f13, f21 \n\t"
        "fdiv.h f6, f14, f22 \n\t"
        "fdiv.h f7, f15, f23 \n\t"
        "fdiv.h f8, f16, f24 \n\t"
        "fdiv.h f9, f17, f25 \n\t"
        "fdiv.h f10, f18, f26 \n\t"
        "fdiv.h f11, f19, f27 \n\t"
        "fdiv.h f12, f20, f28 \n\t"
        "fdiv.h f13, f21, f29 \n\t"
        "fdiv.h f14, f22, f30 \n\t"
        "fdiv.h f15, f23, f31 \n\t"
        "fdiv.h f16, f24, f0 \n\t"
        "fdiv.h f17, f25, f1 \n\t"
        "fdiv.h f18, f26, f2 \n\t"
        "fdiv.h f19, f27, f3 \n\t"
        "fdiv.h f20, f28, f4 \n\t"
        "fdiv.h f21, f29, f5 \n\t"
        "fdiv.h f22, f30, f6 \n\t"
        "fdiv.h f23, f31, f7 \n\t"
        "fdiv.h f24, f0, f8 \n\t"
        "fdiv.h f25, f1, f9 \n\t"
        "fdiv.h f26, f2, f10 \n\t"
        "fdiv.h f27, f3, f11 \n\t"
        "fdiv.h f28, f4, f12 \n\t"
        "fdiv.h f29, f5, f13 \n\t"
        "fdiv.h f30, f6, f14 \n\t"
        "fdiv.h f31, f7, f15 \n\t"
        "fdiv.h f0, f8, f16 \n\t"
        "fdiv.h f1, f9, f17 \n\t"
        "fdiv.h f2, f10, f18 \n\t"
        "fdiv.h f3, f11, f19 \n\t"
        "fdiv.h f4, f12, f20 \n\t"
        "fdiv.h f5, f13, f21 \n\t"
        "fdiv.h f6, f14, f22 \n\t"
        "fdiv.h f7, f15, f23 \n\t"
        "fdiv.h f8, f16, f24 \n\t"
        "fdiv.h f9, f17, f25 \n\t"
        "fdiv.h f10, f18, f26 \n\t"
        "fdiv.h f11, f19, f27 \n\t"
        "fdiv.h f12, f20, f28 \n\t"
        "fdiv.h f13, f21, f29 \n\t"
        "fdiv.h f14, f22, f30 \n\t"
        "fdiv.h f15, f23, f31 \n\t"
        "fdiv.h f16, f24, f0 \n\t"
        "fdiv.h f17, f25, f1 \n\t"
        "fdiv.h f18, f26, f2 \n\t"
        "fdiv.h f19, f27, f3 \n\t"
        "fdiv.h f20, f28, f4 \n\t"
        "fdiv.h f21, f29, f5 \n\t"
        "fdiv.h f22, f30, f6 \n\t"
        "fdiv.h f23, f31, f7 \n\t"
        "fdiv.h f24, f0, f8 \n\t"
        "fdiv.h f25, f1, f9 \n\t"
        "fdiv.h f26, f2, f10 \n\t"
        "fdiv.h f27, f3, f11 \n\t"
        "fdiv.h f28, f4, f12 \n\t"
        "fdiv.h f29, f5, f13 \n\t"
        "fdiv.h f30, f6, f14 \n\t"
        "fdiv.h f31, f7, f15 \n\t"
        "fdiv.h f0, f8, f16 \n\t"
        "fdiv.h f1, f9, f17 \n\t"
        "fdiv.h f2, f10, f18 \n\t"
        "fdiv.h f3, f11, f19 \n\t"
        "fdiv.h f4, f12, f20 \n\t"
        "fdiv.h f5, f13, f21 \n\t"
        "fdiv.h f6, f14, f22 \n\t"
        "fdiv.h f7, f15, f23 \n\t"
        "fdiv.h f8, f16, f24 \n\t"
        "fdiv.h f9, f17, f25 \n\t"
        "fdiv.h f10, f18, f26 \n\t"
        "fdiv.h f11, f19, f27 \n\t"
        "fdiv.h f12, f20, f28 \n\t"
        "fdiv.h f13, f21, f29 \n\t"
        "fdiv.h f14, f22, f30 \n\t"
        "fdiv.h f15, f23, f31 \n\t"
        "fdiv.h f16, f24, f0 \n\t"
        "fdiv.h f17, f25, f1 \n\t"
        "fdiv.h f18, f26, f2 \n\t"
        "fdiv.h f19, f27, f3 \n\t"
        "fdiv.h f20, f28, f4 \n\t"
        "fdiv.h f21, f29, f5 \n\t"
        "fdiv.h f22, f30, f6 \n\t"
        "fdiv.h f23, f31, f7 \n\t"
        "fdiv.h f24, f0, f8 \n\t"
        "fdiv.h f25, f1, f9 \n\t"
        "fdiv.h f26, f2, f10 \n\t"
        "fdiv.h f27, f3, f11 \n\t"
        "fdiv.h f28, f4, f12 \n\t"
        "fdiv.h f29, f5, f13 \n\t"
        "fdiv.h f30, f6, f14 \n\t"
        "fdiv.h f31, f7, f15 \n\t"
        "fdiv.h f0, f8, f16 \n\t"
        "fdiv.h f1, f9, f17 \n\t"
        "fdiv.h f2, f10, f18 \n\t"
        "fdiv.h f3, f11, f19 \n\t"
        "fdiv.h f4, f12, f20 \n\t"
        "fdiv.h f5, f13, f21 \n\t"
        "fdiv.h f6, f14, f22 \n\t"
        "fdiv.h f7, f15, f23 \n\t"
        "fdiv.h f8, f16, f24 \n\t"
        "fdiv.h f9, f17, f25 \n\t"
        "fdiv.h f10, f18, f26 \n\t"
        "fdiv.h f11, f19, f27 \n\t"
        "fdiv.h f12, f20, f28 \n\t"
        "fdiv.h f13, f21, f29 \n\t"
        "fdiv.h f14, f22, f30 \n\t"
        "fdiv.h f15, f23, f31 \n\t"
        "fdiv.h f16, f24, f0 \n\t"
        "fdiv.h f17, f25, f1 \n\t"
        "fdiv.h f18, f26, f2 \n\t"
        "fdiv.h f19, f27, f3 \n\t"
        "fdiv.h f20, f28, f4 \n\t"
        "fdiv.h f21, f29, f5 \n\t"
        "fdiv.h f22, f30, f6 \n\t"
        "fdiv.h f23, f31, f7 \n\t"
        "fdiv.h f24, f0, f8 \n\t"
        "fdiv.h f25, f1, f9 \n\t"
        "fdiv.h f26, f2, f10 \n\t"
        "fdiv.h f27, f3, f11 \n\t"
        "fdiv.h f28, f4, f12 \n\t"
        "fdiv.h f29, f5, f13 \n\t"
        "fdiv.h f30, f6, f14 \n\t"
        "fdiv.h f31, f7, f15 \n\t"
        : /* no input */
        : /* no output */
        : 
        );
    }
}

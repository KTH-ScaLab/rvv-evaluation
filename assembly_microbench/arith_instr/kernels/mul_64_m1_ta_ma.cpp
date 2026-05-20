

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "16( 8(rv64gc_MUL_64b) )";
unsigned long block_inst = 8;
unsigned long lanes = 1;
unsigned long block_ops = (8*(1*lanes));
unsigned long unroll = 16;

void set_all_reg()
{
    int avl = 1;
    
    

    int64_t *mem = (int64_t *)malloc(avl * sizeof(int64_t));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( int64_t )((int)rand());
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
    asm volatile (
        "ld a0, (%0)\n\t"
        "ld a1, (%0)\n\t"
        "ld a2, (%0)\n\t"
        "ld a3, (%0)\n\t"
        "ld a4, (%0)\n\t"
        "ld a5, (%0)\n\t"
        "ld a6, (%0)\n\t"
        "ld a7, (%0)\n\t"
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
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        "mul a0, a2, a4 \n\t"
        "mul a1, a3, a5 \n\t"
        "mul a2, a4, a6 \n\t"
        "mul a3, a5, a7 \n\t"
        "mul a4, a6, a0 \n\t"
        "mul a5, a7, a1 \n\t"
        "mul a6, a0, a2 \n\t"
        "mul a7, a1, a3 \n\t"
        : /* no input */
        : /* no output */
        : "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
        );
    }
}

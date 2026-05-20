

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "16( 8(rv64gc_ADD_16b) )";
unsigned long block_inst = 8;
unsigned long lanes = 1;
unsigned long block_ops = (8*(1*lanes));
unsigned long unroll = 16;

void set_all_reg()
{
    int avl = 1;
    
    

    int16_t *mem = (int16_t *)malloc(avl * sizeof(int16_t));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( int16_t )((int)rand());
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
    asm volatile (
        "lh a0, (%0)\n\t"
        "lh a1, (%0)\n\t"
        "lh a2, (%0)\n\t"
        "lh a3, (%0)\n\t"
        "lh a4, (%0)\n\t"
        "lh a5, (%0)\n\t"
        "lh a6, (%0)\n\t"
        "lh a7, (%0)\n\t"
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
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        "add a0, a2, a4 \n\t"
        "add a1, a3, a5 \n\t"
        "add a2, a4, a6 \n\t"
        "add a3, a5, a7 \n\t"
        "add a4, a6, a0 \n\t"
        "add a5, a7, a1 \n\t"
        "add a6, a0, a2 \n\t"
        "add a7, a1, a3 \n\t"
        : /* no input */
        : /* no output */
        : "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
        );
    }
}

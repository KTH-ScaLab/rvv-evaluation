
#include <riscv_vector.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 32(rv64gcv_VSSE8.V_8b) )";
unsigned long block_inst = 32;
unsigned long lanes = ((int)8*__riscv_vlenb()*1/8);
unsigned long block_ops = (32*(1*lanes));
unsigned long unroll = 4;
static int8_t *mem = nullptr;
static int avl = 1;
static int stride = 2* 8 /8;
void init_mem()
{
    
    asm volatile ("vsetvli %0, %1, e8, m1, ta, ma": "=r"(avl): "r"(lanes): "vl", "vtype");
    //printf("The active vector length of RVV is %d\n", avl);
        
    

    mem = (int8_t *)malloc(avl * stride * sizeof(int8_t));
    srand(time(NULL));
    for (size_t i = 0; i < avl * stride; i++){
	    mem[i] = ( int8_t )((int)rand());
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
}

void kernel(unsigned long iters)
{
    
    asm volatile ("vsetvli %0, %1, e8, m1, ta, ma": "=r"(avl): "r"(lanes): "vl", "vtype");
    //printf("The active vector length of RVV is %d\n", avl);
        

    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
        "vsse8.v v0, (%[r1]), %[r2]  \n\t"
        "vsse8.v v1, (%[r1]), %[r2]  \n\t"
        "vsse8.v v2, (%[r1]), %[r2]  \n\t"
        "vsse8.v v3, (%[r1]), %[r2]  \n\t"
        "vsse8.v v4, (%[r1]), %[r2]  \n\t"
        "vsse8.v v5, (%[r1]), %[r2]  \n\t"
        "vsse8.v v6, (%[r1]), %[r2]  \n\t"
        "vsse8.v v7, (%[r1]), %[r2]  \n\t"
        "vsse8.v v8, (%[r1]), %[r2]  \n\t"
        "vsse8.v v9, (%[r1]), %[r2]  \n\t"
        "vsse8.v v10, (%[r1]), %[r2]  \n\t"
        "vsse8.v v11, (%[r1]), %[r2]  \n\t"
        "vsse8.v v12, (%[r1]), %[r2]  \n\t"
        "vsse8.v v13, (%[r1]), %[r2]  \n\t"
        "vsse8.v v14, (%[r1]), %[r2]  \n\t"
        "vsse8.v v15, (%[r1]), %[r2]  \n\t"
        "vsse8.v v16, (%[r1]), %[r2]  \n\t"
        "vsse8.v v17, (%[r1]), %[r2]  \n\t"
        "vsse8.v v18, (%[r1]), %[r2]  \n\t"
        "vsse8.v v19, (%[r1]), %[r2]  \n\t"
        "vsse8.v v20, (%[r1]), %[r2]  \n\t"
        "vsse8.v v21, (%[r1]), %[r2]  \n\t"
        "vsse8.v v22, (%[r1]), %[r2]  \n\t"
        "vsse8.v v23, (%[r1]), %[r2]  \n\t"
        "vsse8.v v24, (%[r1]), %[r2]  \n\t"
        "vsse8.v v25, (%[r1]), %[r2]  \n\t"
        "vsse8.v v26, (%[r1]), %[r2]  \n\t"
        "vsse8.v v27, (%[r1]), %[r2]  \n\t"
        "vsse8.v v28, (%[r1]), %[r2]  \n\t"
        "vsse8.v v29, (%[r1]), %[r2]  \n\t"
        "vsse8.v v30, (%[r1]), %[r2]  \n\t"
        "vsse8.v v31, (%[r1]), %[r2]  \n\t"
        "vsse8.v v0, (%[r1]), %[r2]  \n\t"
        "vsse8.v v1, (%[r1]), %[r2]  \n\t"
        "vsse8.v v2, (%[r1]), %[r2]  \n\t"
        "vsse8.v v3, (%[r1]), %[r2]  \n\t"
        "vsse8.v v4, (%[r1]), %[r2]  \n\t"
        "vsse8.v v5, (%[r1]), %[r2]  \n\t"
        "vsse8.v v6, (%[r1]), %[r2]  \n\t"
        "vsse8.v v7, (%[r1]), %[r2]  \n\t"
        "vsse8.v v8, (%[r1]), %[r2]  \n\t"
        "vsse8.v v9, (%[r1]), %[r2]  \n\t"
        "vsse8.v v10, (%[r1]), %[r2]  \n\t"
        "vsse8.v v11, (%[r1]), %[r2]  \n\t"
        "vsse8.v v12, (%[r1]), %[r2]  \n\t"
        "vsse8.v v13, (%[r1]), %[r2]  \n\t"
        "vsse8.v v14, (%[r1]), %[r2]  \n\t"
        "vsse8.v v15, (%[r1]), %[r2]  \n\t"
        "vsse8.v v16, (%[r1]), %[r2]  \n\t"
        "vsse8.v v17, (%[r1]), %[r2]  \n\t"
        "vsse8.v v18, (%[r1]), %[r2]  \n\t"
        "vsse8.v v19, (%[r1]), %[r2]  \n\t"
        "vsse8.v v20, (%[r1]), %[r2]  \n\t"
        "vsse8.v v21, (%[r1]), %[r2]  \n\t"
        "vsse8.v v22, (%[r1]), %[r2]  \n\t"
        "vsse8.v v23, (%[r1]), %[r2]  \n\t"
        "vsse8.v v24, (%[r1]), %[r2]  \n\t"
        "vsse8.v v25, (%[r1]), %[r2]  \n\t"
        "vsse8.v v26, (%[r1]), %[r2]  \n\t"
        "vsse8.v v27, (%[r1]), %[r2]  \n\t"
        "vsse8.v v28, (%[r1]), %[r2]  \n\t"
        "vsse8.v v29, (%[r1]), %[r2]  \n\t"
        "vsse8.v v30, (%[r1]), %[r2]  \n\t"
        "vsse8.v v31, (%[r1]), %[r2]  \n\t"
        "vsse8.v v0, (%[r1]), %[r2]  \n\t"
        "vsse8.v v1, (%[r1]), %[r2]  \n\t"
        "vsse8.v v2, (%[r1]), %[r2]  \n\t"
        "vsse8.v v3, (%[r1]), %[r2]  \n\t"
        "vsse8.v v4, (%[r1]), %[r2]  \n\t"
        "vsse8.v v5, (%[r1]), %[r2]  \n\t"
        "vsse8.v v6, (%[r1]), %[r2]  \n\t"
        "vsse8.v v7, (%[r1]), %[r2]  \n\t"
        "vsse8.v v8, (%[r1]), %[r2]  \n\t"
        "vsse8.v v9, (%[r1]), %[r2]  \n\t"
        "vsse8.v v10, (%[r1]), %[r2]  \n\t"
        "vsse8.v v11, (%[r1]), %[r2]  \n\t"
        "vsse8.v v12, (%[r1]), %[r2]  \n\t"
        "vsse8.v v13, (%[r1]), %[r2]  \n\t"
        "vsse8.v v14, (%[r1]), %[r2]  \n\t"
        "vsse8.v v15, (%[r1]), %[r2]  \n\t"
        "vsse8.v v16, (%[r1]), %[r2]  \n\t"
        "vsse8.v v17, (%[r1]), %[r2]  \n\t"
        "vsse8.v v18, (%[r1]), %[r2]  \n\t"
        "vsse8.v v19, (%[r1]), %[r2]  \n\t"
        "vsse8.v v20, (%[r1]), %[r2]  \n\t"
        "vsse8.v v21, (%[r1]), %[r2]  \n\t"
        "vsse8.v v22, (%[r1]), %[r2]  \n\t"
        "vsse8.v v23, (%[r1]), %[r2]  \n\t"
        "vsse8.v v24, (%[r1]), %[r2]  \n\t"
        "vsse8.v v25, (%[r1]), %[r2]  \n\t"
        "vsse8.v v26, (%[r1]), %[r2]  \n\t"
        "vsse8.v v27, (%[r1]), %[r2]  \n\t"
        "vsse8.v v28, (%[r1]), %[r2]  \n\t"
        "vsse8.v v29, (%[r1]), %[r2]  \n\t"
        "vsse8.v v30, (%[r1]), %[r2]  \n\t"
        "vsse8.v v31, (%[r1]), %[r2]  \n\t"
        "vsse8.v v0, (%[r1]), %[r2]  \n\t"
        "vsse8.v v1, (%[r1]), %[r2]  \n\t"
        "vsse8.v v2, (%[r1]), %[r2]  \n\t"
        "vsse8.v v3, (%[r1]), %[r2]  \n\t"
        "vsse8.v v4, (%[r1]), %[r2]  \n\t"
        "vsse8.v v5, (%[r1]), %[r2]  \n\t"
        "vsse8.v v6, (%[r1]), %[r2]  \n\t"
        "vsse8.v v7, (%[r1]), %[r2]  \n\t"
        "vsse8.v v8, (%[r1]), %[r2]  \n\t"
        "vsse8.v v9, (%[r1]), %[r2]  \n\t"
        "vsse8.v v10, (%[r1]), %[r2]  \n\t"
        "vsse8.v v11, (%[r1]), %[r2]  \n\t"
        "vsse8.v v12, (%[r1]), %[r2]  \n\t"
        "vsse8.v v13, (%[r1]), %[r2]  \n\t"
        "vsse8.v v14, (%[r1]), %[r2]  \n\t"
        "vsse8.v v15, (%[r1]), %[r2]  \n\t"
        "vsse8.v v16, (%[r1]), %[r2]  \n\t"
        "vsse8.v v17, (%[r1]), %[r2]  \n\t"
        "vsse8.v v18, (%[r1]), %[r2]  \n\t"
        "vsse8.v v19, (%[r1]), %[r2]  \n\t"
        "vsse8.v v20, (%[r1]), %[r2]  \n\t"
        "vsse8.v v21, (%[r1]), %[r2]  \n\t"
        "vsse8.v v22, (%[r1]), %[r2]  \n\t"
        "vsse8.v v23, (%[r1]), %[r2]  \n\t"
        "vsse8.v v24, (%[r1]), %[r2]  \n\t"
        "vsse8.v v25, (%[r1]), %[r2]  \n\t"
        "vsse8.v v26, (%[r1]), %[r2]  \n\t"
        "vsse8.v v27, (%[r1]), %[r2]  \n\t"
        "vsse8.v v28, (%[r1]), %[r2]  \n\t"
        "vsse8.v v29, (%[r1]), %[r2]  \n\t"
        "vsse8.v v30, (%[r1]), %[r2]  \n\t"
        "vsse8.v v31, (%[r1]), %[r2]  \n\t"
        : /* no input */
        : [r1] "r"(mem), [r2] "r" (stride)
        : "memory"
        );
    }

        int8_t buf[32];
        asm volatile(
            "vse8.v v0, (%0)\n\t"
            :
            : "r"(buf)
            : "t0", "t1", "memory"
        );

        printf("Check if the v0 register has been written to\n");

        for (size_t i = 0; i < avl; i++)
            printf("element%zu=%-10f\t", i, (double)buf[i]);
        printf("\n");
                         
free(mem);
}


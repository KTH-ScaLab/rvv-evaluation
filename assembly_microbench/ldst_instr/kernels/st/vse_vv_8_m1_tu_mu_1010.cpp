
#include <stdio.h>
#include <riscv_vector.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 30(rv64gcv_VSE8.V_8b) )";
unsigned long block_inst = 30;
unsigned long lanes = ((int)8*__riscv_vlenb()*1/8);
unsigned long block_ops = (30*(1*lanes))/2;
unsigned long unroll = 4;
static int8_t *mem = nullptr;
static int avl = 1;
void init_mem()
{
    
    asm volatile ("vsetvli %0, %1, e8, m1, tu, mu": "=r"(avl): "r"(lanes): "vl", "vtype");
    printf("The active vector length of RVV is %d\n", avl);
    asm volatile ("vid.v      v1\n\t"
                  "vand.vi    v1, v1, 1 \n\t"
                  "vmseq.vi   v0, v1, 0 \n\t"
                  :
                  :
                  : "v0", "v1");
        
    

    mem = (int8_t *)malloc(avl * sizeof(int8_t));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( int8_t )((int)rand());
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
}

void kernel(unsigned long iters)
{
    
    asm volatile ("vsetvli %0, %1, e8, m1, tu, mu": "=r"(avl): "r"(lanes): "vl", "vtype");
    printf("The active vector length of RVV is %d\n", avl);
    asm volatile ("vid.v      v1\n\t"
                  "vand.vi    v1, v1, 1 \n\t"
                  "vmseq.vi   v0, v1, 0 \n\t"
                  :
                  :
                  : "v0", "v1");
        

    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v3, (%0), v0.t  \n\t"
        "vse8.v v4, (%0), v0.t  \n\t"
        "vse8.v v5, (%0), v0.t  \n\t"
        "vse8.v v6, (%0), v0.t  \n\t"
        "vse8.v v7, (%0), v0.t  \n\t"
        "vse8.v v8, (%0), v0.t  \n\t"
        "vse8.v v9, (%0), v0.t  \n\t"
        "vse8.v v10, (%0), v0.t  \n\t"
        "vse8.v v11, (%0), v0.t  \n\t"
        "vse8.v v12, (%0), v0.t  \n\t"
        "vse8.v v13, (%0), v0.t  \n\t"
        "vse8.v v14, (%0), v0.t  \n\t"
        "vse8.v v15, (%0), v0.t  \n\t"
        "vse8.v v16, (%0), v0.t  \n\t"
        "vse8.v v17, (%0), v0.t  \n\t"
        "vse8.v v18, (%0), v0.t  \n\t"
        "vse8.v v19, (%0), v0.t  \n\t"
        "vse8.v v20, (%0), v0.t  \n\t"
        "vse8.v v21, (%0), v0.t  \n\t"
        "vse8.v v22, (%0), v0.t  \n\t"
        "vse8.v v23, (%0), v0.t  \n\t"
        "vse8.v v24, (%0), v0.t  \n\t"
        "vse8.v v25, (%0), v0.t  \n\t"
        "vse8.v v26, (%0), v0.t  \n\t"
        "vse8.v v27, (%0), v0.t  \n\t"
        "vse8.v v28, (%0), v0.t  \n\t"
        "vse8.v v29, (%0), v0.t  \n\t"
        "vse8.v v30, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v3, (%0), v0.t  \n\t"
        "vse8.v v4, (%0), v0.t  \n\t"
        "vse8.v v5, (%0), v0.t  \n\t"
        "vse8.v v6, (%0), v0.t  \n\t"
        "vse8.v v7, (%0), v0.t  \n\t"
        "vse8.v v8, (%0), v0.t  \n\t"
        "vse8.v v9, (%0), v0.t  \n\t"
        "vse8.v v10, (%0), v0.t  \n\t"
        "vse8.v v11, (%0), v0.t  \n\t"
        "vse8.v v12, (%0), v0.t  \n\t"
        "vse8.v v13, (%0), v0.t  \n\t"
        "vse8.v v14, (%0), v0.t  \n\t"
        "vse8.v v15, (%0), v0.t  \n\t"
        "vse8.v v16, (%0), v0.t  \n\t"
        "vse8.v v17, (%0), v0.t  \n\t"
        "vse8.v v18, (%0), v0.t  \n\t"
        "vse8.v v19, (%0), v0.t  \n\t"
        "vse8.v v20, (%0), v0.t  \n\t"
        "vse8.v v21, (%0), v0.t  \n\t"
        "vse8.v v22, (%0), v0.t  \n\t"
        "vse8.v v23, (%0), v0.t  \n\t"
        "vse8.v v24, (%0), v0.t  \n\t"
        "vse8.v v25, (%0), v0.t  \n\t"
        "vse8.v v26, (%0), v0.t  \n\t"
        "vse8.v v27, (%0), v0.t  \n\t"
        "vse8.v v28, (%0), v0.t  \n\t"
        "vse8.v v29, (%0), v0.t  \n\t"
        "vse8.v v30, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v3, (%0), v0.t  \n\t"
        "vse8.v v4, (%0), v0.t  \n\t"
        "vse8.v v5, (%0), v0.t  \n\t"
        "vse8.v v6, (%0), v0.t  \n\t"
        "vse8.v v7, (%0), v0.t  \n\t"
        "vse8.v v8, (%0), v0.t  \n\t"
        "vse8.v v9, (%0), v0.t  \n\t"
        "vse8.v v10, (%0), v0.t  \n\t"
        "vse8.v v11, (%0), v0.t  \n\t"
        "vse8.v v12, (%0), v0.t  \n\t"
        "vse8.v v13, (%0), v0.t  \n\t"
        "vse8.v v14, (%0), v0.t  \n\t"
        "vse8.v v15, (%0), v0.t  \n\t"
        "vse8.v v16, (%0), v0.t  \n\t"
        "vse8.v v17, (%0), v0.t  \n\t"
        "vse8.v v18, (%0), v0.t  \n\t"
        "vse8.v v19, (%0), v0.t  \n\t"
        "vse8.v v20, (%0), v0.t  \n\t"
        "vse8.v v21, (%0), v0.t  \n\t"
        "vse8.v v22, (%0), v0.t  \n\t"
        "vse8.v v23, (%0), v0.t  \n\t"
        "vse8.v v24, (%0), v0.t  \n\t"
        "vse8.v v25, (%0), v0.t  \n\t"
        "vse8.v v26, (%0), v0.t  \n\t"
        "vse8.v v27, (%0), v0.t  \n\t"
        "vse8.v v28, (%0), v0.t  \n\t"
        "vse8.v v29, (%0), v0.t  \n\t"
        "vse8.v v30, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        "vse8.v v3, (%0), v0.t  \n\t"
        "vse8.v v4, (%0), v0.t  \n\t"
        "vse8.v v5, (%0), v0.t  \n\t"
        "vse8.v v6, (%0), v0.t  \n\t"
        "vse8.v v7, (%0), v0.t  \n\t"
        "vse8.v v8, (%0), v0.t  \n\t"
        "vse8.v v9, (%0), v0.t  \n\t"
        "vse8.v v10, (%0), v0.t  \n\t"
        "vse8.v v11, (%0), v0.t  \n\t"
        "vse8.v v12, (%0), v0.t  \n\t"
        "vse8.v v13, (%0), v0.t  \n\t"
        "vse8.v v14, (%0), v0.t  \n\t"
        "vse8.v v15, (%0), v0.t  \n\t"
        "vse8.v v16, (%0), v0.t  \n\t"
        "vse8.v v17, (%0), v0.t  \n\t"
        "vse8.v v18, (%0), v0.t  \n\t"
        "vse8.v v19, (%0), v0.t  \n\t"
        "vse8.v v20, (%0), v0.t  \n\t"
        "vse8.v v21, (%0), v0.t  \n\t"
        "vse8.v v22, (%0), v0.t  \n\t"
        "vse8.v v23, (%0), v0.t  \n\t"
        "vse8.v v24, (%0), v0.t  \n\t"
        "vse8.v v25, (%0), v0.t  \n\t"
        "vse8.v v26, (%0), v0.t  \n\t"
        "vse8.v v27, (%0), v0.t  \n\t"
        "vse8.v v28, (%0), v0.t  \n\t"
        "vse8.v v29, (%0), v0.t  \n\t"
        "vse8.v v30, (%0), v0.t  \n\t"
        "vse8.v v2, (%0), v0.t  \n\t"
        : /* no input */
        : "r"(mem)
        : "memory"
        );
    }

        int8_t buf[32];
        asm volatile(
            "vse8.v v2, (%0)\n\t"
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



#include <riscv_vector.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 32(rv64gcv_VADD.VV_16b) )";
unsigned long block_inst = 32;
unsigned long lanes = ((int)8*__riscv_vlenb()*1/16);
unsigned long block_ops = (32*(1*lanes));
unsigned long unroll = 4;

void set_all_reg()
{
    int avl = 1;
    
    asm volatile ("vsetvli %0, %1, e16, m1, ta, ma": "=r"(avl): "r"(lanes): "vl", "vtype");
    printf("The active vector length of RVV is %d\n", avl);
        
    

    int16_t *mem = (int16_t *)malloc(avl * sizeof(int16_t));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( int16_t )((int)rand());
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
    asm volatile (
        "vle16.v v0, (%0)\n\t"
        "vle16.v v1, (%0)\n\t"
        "vle16.v v2, (%0)\n\t"
        "vle16.v v3, (%0)\n\t"
        "vle16.v v4, (%0)\n\t"
        "vle16.v v5, (%0)\n\t"
        "vle16.v v6, (%0)\n\t"
        "vle16.v v7, (%0)\n\t"
        "vle16.v v8, (%0)\n\t"
        "vle16.v v9, (%0)\n\t"
        "vle16.v v10, (%0)\n\t"
        "vle16.v v11, (%0)\n\t"
        "vle16.v v12, (%0)\n\t"
        "vle16.v v13, (%0)\n\t"
        "vle16.v v14, (%0)\n\t"
        "vle16.v v15, (%0)\n\t"
        "vle16.v v16, (%0)\n\t"
        "vle16.v v17, (%0)\n\t"
        "vle16.v v18, (%0)\n\t"
        "vle16.v v19, (%0)\n\t"
        "vle16.v v20, (%0)\n\t"
        "vle16.v v21, (%0)\n\t"
        "vle16.v v22, (%0)\n\t"
        "vle16.v v23, (%0)\n\t"
        "vle16.v v24, (%0)\n\t"
        "vle16.v v25, (%0)\n\t"
        "vle16.v v26, (%0)\n\t"
        "vle16.v v27, (%0)\n\t"
        "vle16.v v28, (%0)\n\t"
        "vle16.v v29, (%0)\n\t"
        "vle16.v v30, (%0)\n\t"
        "vle16.v v31, (%0)\n\t"
    : /* no input */
    : "r"(mem)
    : "memory"
    );


        int16_t buf[32];
        asm volatile(
            "vse16.v v0, (%0)\n\t"
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



void kernel(unsigned long iters)
{
    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
        "vadd.vv v0, v8, v16 \n\t"
        "vadd.vv v1, v9, v17 \n\t"
        "vadd.vv v2, v10, v18 \n\t"
        "vadd.vv v3, v11, v19 \n\t"
        "vadd.vv v4, v12, v20 \n\t"
        "vadd.vv v5, v13, v21 \n\t"
        "vadd.vv v6, v14, v22 \n\t"
        "vadd.vv v7, v15, v23 \n\t"
        "vadd.vv v8, v16, v24 \n\t"
        "vadd.vv v9, v17, v25 \n\t"
        "vadd.vv v10, v18, v26 \n\t"
        "vadd.vv v11, v19, v27 \n\t"
        "vadd.vv v12, v20, v28 \n\t"
        "vadd.vv v13, v21, v29 \n\t"
        "vadd.vv v14, v22, v30 \n\t"
        "vadd.vv v15, v23, v31 \n\t"
        "vadd.vv v16, v24, v0 \n\t"
        "vadd.vv v17, v25, v1 \n\t"
        "vadd.vv v18, v26, v2 \n\t"
        "vadd.vv v19, v27, v3 \n\t"
        "vadd.vv v20, v28, v4 \n\t"
        "vadd.vv v21, v29, v5 \n\t"
        "vadd.vv v22, v30, v6 \n\t"
        "vadd.vv v23, v31, v7 \n\t"
        "vadd.vv v24, v0, v8 \n\t"
        "vadd.vv v25, v1, v9 \n\t"
        "vadd.vv v26, v2, v10 \n\t"
        "vadd.vv v27, v3, v11 \n\t"
        "vadd.vv v28, v4, v12 \n\t"
        "vadd.vv v29, v5, v13 \n\t"
        "vadd.vv v30, v6, v14 \n\t"
        "vadd.vv v31, v7, v15 \n\t"
        "vadd.vv v0, v8, v16 \n\t"
        "vadd.vv v1, v9, v17 \n\t"
        "vadd.vv v2, v10, v18 \n\t"
        "vadd.vv v3, v11, v19 \n\t"
        "vadd.vv v4, v12, v20 \n\t"
        "vadd.vv v5, v13, v21 \n\t"
        "vadd.vv v6, v14, v22 \n\t"
        "vadd.vv v7, v15, v23 \n\t"
        "vadd.vv v8, v16, v24 \n\t"
        "vadd.vv v9, v17, v25 \n\t"
        "vadd.vv v10, v18, v26 \n\t"
        "vadd.vv v11, v19, v27 \n\t"
        "vadd.vv v12, v20, v28 \n\t"
        "vadd.vv v13, v21, v29 \n\t"
        "vadd.vv v14, v22, v30 \n\t"
        "vadd.vv v15, v23, v31 \n\t"
        "vadd.vv v16, v24, v0 \n\t"
        "vadd.vv v17, v25, v1 \n\t"
        "vadd.vv v18, v26, v2 \n\t"
        "vadd.vv v19, v27, v3 \n\t"
        "vadd.vv v20, v28, v4 \n\t"
        "vadd.vv v21, v29, v5 \n\t"
        "vadd.vv v22, v30, v6 \n\t"
        "vadd.vv v23, v31, v7 \n\t"
        "vadd.vv v24, v0, v8 \n\t"
        "vadd.vv v25, v1, v9 \n\t"
        "vadd.vv v26, v2, v10 \n\t"
        "vadd.vv v27, v3, v11 \n\t"
        "vadd.vv v28, v4, v12 \n\t"
        "vadd.vv v29, v5, v13 \n\t"
        "vadd.vv v30, v6, v14 \n\t"
        "vadd.vv v31, v7, v15 \n\t"
        "vadd.vv v0, v8, v16 \n\t"
        "vadd.vv v1, v9, v17 \n\t"
        "vadd.vv v2, v10, v18 \n\t"
        "vadd.vv v3, v11, v19 \n\t"
        "vadd.vv v4, v12, v20 \n\t"
        "vadd.vv v5, v13, v21 \n\t"
        "vadd.vv v6, v14, v22 \n\t"
        "vadd.vv v7, v15, v23 \n\t"
        "vadd.vv v8, v16, v24 \n\t"
        "vadd.vv v9, v17, v25 \n\t"
        "vadd.vv v10, v18, v26 \n\t"
        "vadd.vv v11, v19, v27 \n\t"
        "vadd.vv v12, v20, v28 \n\t"
        "vadd.vv v13, v21, v29 \n\t"
        "vadd.vv v14, v22, v30 \n\t"
        "vadd.vv v15, v23, v31 \n\t"
        "vadd.vv v16, v24, v0 \n\t"
        "vadd.vv v17, v25, v1 \n\t"
        "vadd.vv v18, v26, v2 \n\t"
        "vadd.vv v19, v27, v3 \n\t"
        "vadd.vv v20, v28, v4 \n\t"
        "vadd.vv v21, v29, v5 \n\t"
        "vadd.vv v22, v30, v6 \n\t"
        "vadd.vv v23, v31, v7 \n\t"
        "vadd.vv v24, v0, v8 \n\t"
        "vadd.vv v25, v1, v9 \n\t"
        "vadd.vv v26, v2, v10 \n\t"
        "vadd.vv v27, v3, v11 \n\t"
        "vadd.vv v28, v4, v12 \n\t"
        "vadd.vv v29, v5, v13 \n\t"
        "vadd.vv v30, v6, v14 \n\t"
        "vadd.vv v31, v7, v15 \n\t"
        "vadd.vv v0, v8, v16 \n\t"
        "vadd.vv v1, v9, v17 \n\t"
        "vadd.vv v2, v10, v18 \n\t"
        "vadd.vv v3, v11, v19 \n\t"
        "vadd.vv v4, v12, v20 \n\t"
        "vadd.vv v5, v13, v21 \n\t"
        "vadd.vv v6, v14, v22 \n\t"
        "vadd.vv v7, v15, v23 \n\t"
        "vadd.vv v8, v16, v24 \n\t"
        "vadd.vv v9, v17, v25 \n\t"
        "vadd.vv v10, v18, v26 \n\t"
        "vadd.vv v11, v19, v27 \n\t"
        "vadd.vv v12, v20, v28 \n\t"
        "vadd.vv v13, v21, v29 \n\t"
        "vadd.vv v14, v22, v30 \n\t"
        "vadd.vv v15, v23, v31 \n\t"
        "vadd.vv v16, v24, v0 \n\t"
        "vadd.vv v17, v25, v1 \n\t"
        "vadd.vv v18, v26, v2 \n\t"
        "vadd.vv v19, v27, v3 \n\t"
        "vadd.vv v20, v28, v4 \n\t"
        "vadd.vv v21, v29, v5 \n\t"
        "vadd.vv v22, v30, v6 \n\t"
        "vadd.vv v23, v31, v7 \n\t"
        "vadd.vv v24, v0, v8 \n\t"
        "vadd.vv v25, v1, v9 \n\t"
        "vadd.vv v26, v2, v10 \n\t"
        "vadd.vv v27, v3, v11 \n\t"
        "vadd.vv v28, v4, v12 \n\t"
        "vadd.vv v29, v5, v13 \n\t"
        "vadd.vv v30, v6, v14 \n\t"
        "vadd.vv v31, v7, v15 \n\t"
        : /* no input */
        : /* no output */
        : "v0", "v1", "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19", "v2", "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v3", "v30", "v31", "v4", "v5", "v6", "v7", "v8", "v9"
        );
    }
}

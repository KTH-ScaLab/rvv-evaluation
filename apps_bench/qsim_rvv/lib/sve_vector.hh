#include <arm_sve.h>

#define DECLARE_VECS() \
	    svfloat32_t v0, v1, v2, v3, v4, v5, v6, v7, v8;

#define SET_V(i, val)     do {          \
    switch (i) {                    \
        case 0:  v0  = (val); break; \
        case 1:  v1  = (val); break; \
        case 2:  v2  = (val); break; \
        case 3:  v3  = (val); break; \
        case 4:  v4  = (val); break; \
        case 5:  v5  = (val); break; \
        case 6:  v6  = (val); break; \
        case 7:  v7  = (val); break; \
        case 8:  v8  = (val); break; \
    }                         } while(0)    

#define V(i) ((i) == 0 ? v0 : \
              (i) == 1 ? v1 : \
              (i) == 2 ? v2 : \
              (i) == 3 ? v3 : \
              (i) == 4 ? v4 : \
              (i) == 5 ? v5 : \
              (i) == 6 ? v6 : \
              (i) == 7 ? v7 : \
              (i) == 8 ? v8 : \
              svdup_f32(0.f))

#define SET_V_ADDR(i, addr)     do {          \
    switch (i) {                    \
        case 0:  v0  = svld1_f32(svptrue_b32(), addr); break; \
        case 1:  v1  = svld1_f32(svptrue_b32(), addr); break; \
        case 2:  v2  = svld1_f32(svptrue_b32(), addr); break; \
        case 3:  v3  = svld1_f32(svptrue_b32(), addr); break; \
        case 4:  v4  = svld1_f32(svptrue_b32(), addr); break; \
        case 5:  v5  = svld1_f32(svptrue_b32(), addr); break; \
        case 6:  v6  = svld1_f32(svptrue_b32(), addr); break; \
        case 7:  v7  = svld1_f32(svptrue_b32(), addr); break; \
        case 8:  v8  = svld1_f32(svptrue_b32(), addr); break; \
    }                         } while(0)


#include <riscv_vector.h>

#if (defined(__GNUC__) && __GNUC__ >= 13) || (defined(__clang__) && __clang_major__ >= 16)

#define vfmv_v_f_f32m1 __riscv_vfmv_v_f_f32m1
#define vid_v_u32m1 __riscv_vid_v_u32m1
#define vmsltu_vx_u32m1_b32 __riscv_vmsltu_vx_u32m1_b32
#define vmset_m_b32  __riscv_vmset_m_b32
#define vse32_v_f32m1 __riscv_vse32_v_f32m1
#define vmclr_m_b32 __riscv_vmclr_m_b32
#define vadd_vx_u32m1 __riscv_vadd_vx_u32m1
#define vmv_v_x_u32m1 __riscv_vmv_v_x_u32m1
#define vand_vv_u32m1 __riscv_vand_vv_u32m1
#define vmor_mm_b32 __riscv_vmor_mm_b32
#define vcpop_m_b32  __riscv_vcpop_m_b32
#define vmerge_vvm_f32m1(p, v0, v1, vl) __riscv_vmerge_vvm_f32m1(v0, v1, p, vl)
#define vfadd_vv_f32m1 __riscv_vfadd_vv_f32m1
#define vsetvlmax_e32m1 __riscv_vsetvlmax_e32m1
#define vfmul_vv_f32m1 __riscv_vfmul_vv_f32m1
#define vfmacc_vv_f32m1_m __riscv_vfmacc_vv_f32m1_m
#define vfnmsac_vv_f32m1_m __riscv_vfnmsac_vv_f32m1_m
#define vmnot_m_b32  __riscv_vmnot_m_b32
#define vmseq_vv_u32m1_b32 __riscv_vmseq_vv_u32m1_b32
#define vfmul_vv_f32m1_m(p, v0, v1, v_res, vl) __riscv_vfmul_vv_f32m1_m(pg, v1, v_res, vl)
#define vand_vx_u32m1 __riscv_vand_vx_u32m1
#define vmseq_vx_u32m1_b32 __riscv_vmseq_vx_u32m1_b32
#define vmnand_mm_b32 __riscv_vmnand_mm_b32
#define vfredusum_vs_f32m1_f32m1_m(p, v0, v1, v_re, vl) __riscv_vfredusum_vs_f32m1_f32m1_m(pg, v0, v_re, vl)
#define vfmv_f_s_f32m1_f32 __riscv_vfmv_f_s_f32m1_f32
#define vle32_v_f32m1_m(pg, v0, base, vl) __riscv_vle32_v_f32m1_m(pg, base, vl)
#define vse32_v_f32m1_m  __riscv_vse32_v_f32m1_m

#endif
// #ifdef 
// #define __riscv_
// #endif
//

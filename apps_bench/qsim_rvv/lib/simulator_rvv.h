// Copyright 2019 Google LLC. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SIMULATOR_RVV_H_
#define SIMULATOR_RVV_H_

#include <complex>
#include <cstdint>
#include <functional>
#include <vector>

#include "simulator.h"
#include "statespace_rvv.h"
#include "statespace_basic.h"
#include "riscv_compatible.h"

extern const size_t numVals;
//#define numVals (size_t)vsetvlmax_e32m1()
#define log2val (int)log2(numVals)
#define svfloat32_t vfloat32m1_t
#define svbool_t vbool32_t
#define DEBUG 0
namespace qsim
{

  /**
   * Quantum circuit simulator without vectorization.
   */
  template <typename For, typename FP = float>
  class SimulatorRVV final : public SimulatorBase
  {
  public:
    using StateSpace = StateSpaceRVV<For, FP>;
    using State = typename StateSpace::State;
    using fp_type = typename StateSpace::fp_type;

    template <typename... ForArgs>
    explicit SimulatorRVV(ForArgs &&...args) : for_(args...) {}

    /**
     * Applies a gate using non-vectorized instructions.
     * @param qs Indices of the qubits affected by this gate.
     * @param matrix Matrix representation of the gate to be applied.
     * @param state The state of the system, to be updated by this method.
     */
    void ApplyGate(const std::vector<unsigned> &qs,
                   const fp_type *matrix, State &state) const
    {
      // Assume qs[0] < qs[1] < qs[2] < ... .

      //Since qs is ordered,
      unsigned lcount = 0;
      for (int i = 0; i < qs.size(); ++i)
      {
        if (qs[i] < log2val)
          ++lcount;
        else
          break;
      }
      unsigned L = lcount;
      unsigned H = qs.size() - lcount;
      ApplyGateH(qs, matrix, state, H, L);
    }

    /**
     * Applies a controlled gate using non-vectorized instructions.
     * @param qs Indices of the qubits affected by this gate.
     * @param cqs Indices of control qubits.
     * @param cvals Bit mask of control qubit values.
     * @param matrix Matrix representation of the gate to be applied.
     * @param state The state of the system, to be updated by this method.
     */
    void ApplyControlledGate(const std::vector<unsigned> &qs,
                             const std::vector<unsigned> &cqs, uint64_t cvals,
                             const fp_type *matrix, State &state) const
    {
      // Assume qs[0] < qs[1] < qs[2] < ... .
      
      if (cqs.size() == 0)
      {
        ApplyGate(qs, matrix, state);
        return;
      }

      
      // Since qs is ordered,
      unsigned lcount = 0;
      for (int i = 0; i < qs.size(); ++i)
      {
        if (qs[i] < log2val)
          ++lcount;
        else
          break;
      }
            
      unsigned cqs_lcount = 0;
      for (int i = 0; i < cqs.size(); ++i)
      {
        if (cqs[i] < log2val)
          ++cqs_lcount;
      }
      unsigned CL = cqs_lcount;
      unsigned L = lcount;
      unsigned H = qs.size() - lcount;
      
      ApplyControlledGateH(qs, cqs, cvals, matrix, state, H, L, CL);
      
    }

    /**
     * Computes the expectation value of an operator using non-vectorized
     * instructions.
     * @param qs Indices of the qubits the operator acts on.
     * @param matrix The operator matrix.
     * @param state The state of the system.
     * @return The computed expectation value.
     */
    std::complex<double> ExpectationValue(const std::vector<unsigned> &qs,
                                          const fp_type *matrix,
                                          const State &state) const
    {
      // Since qs is ordered,
      unsigned lcount = 0;
      for (int i = 0; i < qs.size(); ++i)
      {
        if (qs[i] < log2val)
          ++lcount;
        else
          break;
      }
      unsigned L = lcount;
      unsigned H = qs.size() - lcount;
      
      return ExpectationValueH(qs, matrix, state, H, L);
    }

    /**
     * @return The size of SIMD register if applicable.
     */
    static unsigned SIMDRegisterSize()
    {
      return numVals;
    }

  private:
    static void FillIndices_HL(unsigned num_qubits, const std::vector<unsigned> &qs,
                               uint64_t *ms, uint64_t *xss, unsigned H, unsigned L)
    {
      unsigned hsize = 1 << H;

      if (H == 0)
      {
        ms[0] = uint64_t(-1);
        xss[0] = 0;
      }
      else
      {
        std::vector<uint64_t> xs(H + 1);

        xs[0] = uint64_t{1} << (qs[L] + 1);
        ms[0] = (uint64_t{1} << qs[L]) - 1;
        for (unsigned i = 1; i < H; ++i)
        {
          xs[i] = uint64_t{1} << (qs[L + i] + 1);
          ms[i] = ((uint64_t{1} << qs[L + i]) - 1) ^ (xs[i - 1] - 1);
        }
        ms[H] = ((uint64_t{1} << num_qubits) - 1) ^ (xs[H - 1] - 1);

        for (unsigned i = 0; i < hsize; ++i)
        {
          uint64_t a = 0;
          for (unsigned k = 0; k < H; ++k)
          {
            a += xs[k] * ((i >> k) & 1);
          }
          xss[i] = a;
        }
      }
    }

    // template <unsigned H, unsigned L>
    void ApplyGateH(const std::vector<unsigned> &qs,
                    const fp_type *matrix, State &state, const unsigned H, const unsigned L) const
    {
      auto f = [](unsigned n, unsigned m, uint64_t i, const fp_type *v,
                  const uint64_t *ms, const uint64_t *xss, fp_type *rstate, const svbool_t pg, const unsigned H, const unsigned L, const uint64_t *lmask)
      {
        const unsigned gsize = 1 << (H + L);
        const unsigned hsize = 1 << H;
        const unsigned lsize = 1 << L;
        svfloat32_t ru, iu, rn, in, is, rs;
        i *= numVals;

        uint64_t ii = i & ms[0];
        for (unsigned j = 1; j <= H; ++j)
        {
          i *= 2;
          ii |= i & ms[j];
        }

        auto p0 = rstate + 2 * ii;
	
        fp_type rs_tmp[gsize * numVals];
        fp_type is_tmp[gsize * numVals];
        int count = 0;
	
	for (int i = 0; i < hsize; i++)
          for (int j = 0; j < lsize; j++)
          {
            svfloat32_t v0 = vle32_v_f32m1_m(pg, v0, p0 + xss[i] + lmask[j], numVals);           // load first 4 floats
            svfloat32_t v1 = vle32_v_f32m1_m(pg, v1, p0 + xss[i] + lmask[j] + numVals, numVals); // load next 4 floats
            vse32_v_f32m1_m(pg, rs_tmp + count * numVals, v0, numVals);                      // store first 4 floats
            vse32_v_f32m1_m(pg, is_tmp + count * numVals, v1, numVals);                      // store next 4 floats
            count = count + 1;
          }

        uint64_t j = 0;

        for (unsigned k = 0; k < hsize; ++k)
          for (unsigned s = 0; s < lsize; ++s)
          {
            // rn = rs[0] * v[j] - is[0] * v[j + 1];
            // in = rs[0] * v[j + 1] + is[0] * v[j];
            ru = vfmv_v_f_f32m1(v[j], numVals);
            iu = vfmv_v_f_f32m1(v[j + 1], numVals);
            rs = vle32_v_f32m1_m(pg, rs, rs_tmp, numVals);
            is = vle32_v_f32m1_m(pg, is, is_tmp, numVals);

            rn = vfmul_vv_f32m1_m(pg, rn, rs, ru, numVals);
            in = vfmul_vv_f32m1_m(pg, in, rs, iu, numVals);
            rn = vfnmsac_vv_f32m1_m(pg, rn, is, iu, numVals);
            in = vfmacc_vv_f32m1_m(pg, in, is, ru, numVals);

            j += 2;

            for (unsigned l = 1; l < gsize; ++l)
            {
              ru = vfmv_v_f_f32m1(v[j], numVals);
              iu = vfmv_v_f_f32m1(v[j + 1], numVals);
              rs = vle32_v_f32m1_m(pg, rs, rs_tmp + l * numVals, numVals);
              is = vle32_v_f32m1_m(pg, is, is_tmp + l * numVals, numVals);

              rn = vfmacc_vv_f32m1_m(pg, rn, ru, rs, numVals);
              in = vfmacc_vv_f32m1_m(pg, in, iu, rs, numVals);
              rn = vfnmsac_vv_f32m1_m(pg, rn, iu, is, numVals);
              in = vfmacc_vv_f32m1_m(pg, in, ru, is, numVals);
              
	      j += 2;
            }

            vse32_v_f32m1_m(pg, p0 + xss[k] + lmask[s], rn, numVals);
            vse32_v_f32m1_m(pg, p0 + xss[k] + lmask[s] + numVals, in, numVals);

          }
      };

      std::vector<uint64_t> ms((H) + 1);
      std::vector<uint64_t> xss(1 << H);
      std::vector<uint64_t> lmask(1 << L);
      FillIndices_HL(state.num_qubits(), qs, ms.data(), xss.data(), H, L);

      unsigned k = H + log2val;
      unsigned n = state.num_qubits() > k ? state.num_qubits() - k : 0;
      uint64_t size = uint64_t{1} << n;
      svbool_t pg = vmset_m_b32(numVals);
      lmask.data()[0] = 0;
      uint64_t count = 0;
      for (int i = 0; i < (1 << L); i++)
      {
        int number = 0;
        for (int j = 0; j < L; j++)
        {
          if ((i >> j) & 1)
          {
            number |= (1 << qs[j]); // Set bit qs[j]
          }
        }
        lmask.data()[i] = number;
      }
      if (L == 0)
        pg = vmset_m_b32(numVals);
      else
      {
        uint32_t tmp = 0x0;
        for (int x = 0; x < L; ++x)
        {
          tmp = tmp | (1 << qs[x]);
        }
        vuint32m1_t idex = vid_v_u32m1(numVals);
	idex = vand_vx_u32m1(idex, tmp, numVals);
        pg = vmseq_vx_u32m1_b32(idex, 0, numVals);
      }
      for_.Run(size, f, matrix, ms.data(), xss.data(), state.get(), pg, H, L, lmask.data());
    }

    void ApplyControlledGateH(const std::vector<unsigned> &qs,
                              const std::vector<unsigned> &cqs,
                              uint64_t cvals, const fp_type *matrix,
                              State &state, const unsigned H, const unsigned L, const unsigned CL) const
    {
      auto f = [](unsigned n, unsigned m, uint64_t i, const fp_type *v,
                  const uint64_t *ms, const uint64_t *xss,
                  uint64_t cvalsh, uint64_t cmaskh, fp_type *rstate, const svbool_t pg, const unsigned H, const unsigned L, const uint64_t *lmask)
      {
        const unsigned gsize = 1 << (H + L);
        const unsigned hsize = 1 << H;
        const unsigned lsize = 1 << L;

        svfloat32_t ru, iu, rn, in, is, rs;
        i *= numVals;

        uint64_t ii = i & ms[0];
        for (unsigned j = 1; j <= H; ++j)
        {
          i *= 2;
          ii |= i & ms[j];
        }


        if ((ii & cmaskh) == cvalsh)
 	{
	  
          auto p0 = rstate + 2 * ii;
          fp_type rs_tmp[gsize * numVals];
          fp_type is_tmp[gsize * numVals];
          int count = 0;


          for (int i = 0; i < hsize; i++)
            for (int j = 0; j < lsize; j++)
            {
              svfloat32_t v0 = vle32_v_f32m1_m(pg, v0, p0 + xss[i] + lmask[j], numVals);           // load first 4 floats
              svfloat32_t v1 = vle32_v_f32m1_m(pg, v1, p0 + xss[i] + lmask[j] + numVals, numVals); // load next 4 floats
              vse32_v_f32m1_m(pg, rs_tmp + count * numVals, v0, numVals);                      // store first 4 floats
              vse32_v_f32m1_m(pg, is_tmp + count * numVals, v1, numVals);                      // store next 4 floats
              count =count + 1;
	    }

          uint64_t j = 0;

          for (unsigned k = 0; k < hsize; ++k)
            for (unsigned s = 0; s < lsize; ++s)
            {
              // rn = rs[0] * v[j] - is[0] * v[j + 1];
              // in = rs[0] * v[j + 1] + is[0] * v[j];
              ru = vfmv_v_f_f32m1(v[j], numVals);
              iu = vfmv_v_f_f32m1(v[j + 1], numVals);
              rs = vle32_v_f32m1_m(pg, rs, rs_tmp, numVals);
              is = vle32_v_f32m1_m(pg, is, is_tmp, numVals);

              rn = vfmul_vv_f32m1_m(pg, rn, rs, ru, numVals);
              in = vfmul_vv_f32m1_m(pg, in, rs, iu, numVals);
              rn = vfnmsac_vv_f32m1_m(pg, rn, is, iu, numVals);
              in = vfmacc_vv_f32m1_m(pg, in, is, ru, numVals);
              
	      j += 2;

              for (unsigned l = 1; l < gsize; ++l)
              {
                ru = vfmv_v_f_f32m1(v[j], numVals);
                iu = vfmv_v_f_f32m1(v[j + 1], numVals);
                rs = vle32_v_f32m1_m(pg, rs, rs_tmp + l * numVals, numVals);
                is = vle32_v_f32m1_m(pg, is, is_tmp + l * numVals, numVals);

                rn = vfmacc_vv_f32m1_m(pg, rn, ru, rs, numVals);
                in = vfmacc_vv_f32m1_m(pg, in, iu, rs, numVals);
                rn = vfnmsac_vv_f32m1_m(pg, rn, iu, is, numVals);
                in = vfmacc_vv_f32m1_m(pg, in, ru, is, numVals);
                
		j += 2;
              }
              
	      vse32_v_f32m1_m(pg, p0 + xss[k] + lmask[s], rn, numVals);
              vse32_v_f32m1_m(pg, p0 + xss[k] + lmask[s] + numVals, in, numVals);

            }
        }
      };

      std::vector<uint64_t> ms((H) + 1);
      std::vector<uint64_t> xss(1 << H);
      std::vector<uint64_t> lmask(1 << L);
      FillIndices_HL(state.num_qubits(), qs, ms.data(), xss.data(), H, L);

      unsigned k = H + log2val;
      unsigned n = state.num_qubits() > k ? state.num_qubits() - k : 0;
      uint64_t size = uint64_t{1} << n;
      svbool_t pg = vmset_m_b32(numVals);
      lmask.data()[0] = 0;
      uint64_t count = 0;
      for (int i = 0; i < (1 << L); i++)
      {
        int number = 0;
        for (int j = 0; j < L; j++)
        {
          if ((i >> j) & 1)
          {
            number |= (1 << qs[j]); // Set bit qs[j]
          }
        }
        lmask.data()[i] = number;
      }

      
      uint32_t tmp_qs = 0x0;
      uint32_t tmp_cqs0 = 0x0;
      uint32_t tmp_cqs1 = 0x0;

      svbool_t pg_qs = vmset_m_b32(numVals);
      svbool_t pg_cqs0 = vmset_m_b32(numVals);
      svbool_t pg_cqs1 = vmset_m_b32(numVals);

      if( L != 0){  
        for (int x = 0; x < L; ++x)
           tmp_qs = tmp_qs | (1 << qs[x]);
	vuint32m1_t idex_qs = vid_v_u32m1(numVals);
        idex_qs = vand_vx_u32m1(idex_qs, tmp_qs, numVals);
        pg_qs = vmseq_vx_u32m1_b32(idex_qs, 0, numVals);
      } 
      
      if (CL != 0){
        for (int x = 0; x < CL; ++x)
	{
          if((cvals & 0x1) == 0)
                tmp_cqs0 = tmp_cqs0 | (1 << cqs[x]);
          if((cvals & 0x1) == 1)
                tmp_cqs1 = tmp_cqs1 | (1 << cqs[x]);
	  cvals = cvals >> 1;
        }
	vuint32m1_t idex_cqs = vid_v_u32m1(numVals);
        idex_cqs = vand_vx_u32m1(idex_cqs, tmp_cqs0, numVals);	    
	pg_cqs0 = vmseq_vx_u32m1_b32(idex_cqs, 0, numVals);

	idex_cqs = vid_v_u32m1(numVals);
	idex_cqs = vand_vx_u32m1(idex_cqs, tmp_cqs1, numVals);    
	pg_cqs1 = vmseq_vx_u32m1_b32(idex_cqs, tmp_cqs1, numVals);	
      }


      pg = vmnand_mm_b32(pg_cqs0, pg_cqs1, numVals);
      pg = vmnand_mm_b32(pg, pg_qs, numVals);
      
      std::vector<unsigned> cqsh;
      for (auto v : cqs) {
        if (v >= log2val) 
          cqsh.push_back(v);
      }

      
      auto m = GetMasks7(state.num_qubits(), qs, cqsh, cvals);
      for_.Run(size, f, matrix, ms.data(), xss.data(), m.cvalsh, m.cmaskh, state.get(), pg, H, L, lmask.data());
    }
//    template <unsigned H>
    std::complex<double> ExpectationValueH(const std::vector<unsigned> &qs,
                                           const fp_type *matrix,
                                           const State &state , const unsigned H, const unsigned L) const 
    {
      auto f = [](unsigned n, unsigned m, uint64_t i, const fp_type *v,
                  const uint64_t *ms, const uint64_t *xss,
                  const fp_type *rstate, const svbool_t pg, const unsigned H, const unsigned L, const uint64_t *lmask)
      {
        const unsigned gsize = 1 << (H + L);
        const unsigned hsize = 1 << H;
        const unsigned lsize = 1 << L;
        svfloat32_t ru, iu, rn, in, is, rs;
        i *= numVals;

        uint64_t ii = i & ms[0];
        for (unsigned j = 1; j <= H; ++j)
        {
          i *= 2;
          ii |= i & ms[j];
        }

        auto p0 = rstate + 2 * ii;


        fp_type rs_tmp[gsize * numVals];
        fp_type is_tmp[gsize * numVals];
        int count = 0;
        for (int i = 0; i < hsize; i++)
          for (int j = 0; j < lsize; j++)
          {
            svfloat32_t v0 = vle32_v_f32m1_m(pg, v0, p0 + xss[i] + lmask[j], numVals);           // load first 4 floats
            svfloat32_t v1 = vle32_v_f32m1_m(pg, v1, p0 + xss[i] + lmask[j] + numVals, numVals); // load next 4 floats
            vse32_v_f32m1_m(pg, rs_tmp + count * numVals, v0, numVals);                      // store first 4 floats
            vse32_v_f32m1_m(pg, is_tmp + count * numVals, v1, numVals);                      // store next 4 floats
            count = count + 1;
          }

        double re = 0;
        double im = 0;
	vfloat32m1_t re_t   = vfmv_v_f_f32m1(0.0f, numVals);
        vfloat32m1_t im_t   = vfmv_v_f_f32m1(0.0f, numVals);

        uint64_t j = 0;

        for (unsigned k = 0; k < hsize; ++k)
          for (unsigned s = 0; s < lsize; ++s)
          {
            // rn = rs[0] * v[j] - is[0] * v[j + 1];
            // in = rs[0] * v[j + 1] + is[0] * v[j];
            ru = vfmv_v_f_f32m1(v[j], numVals);
            iu = vfmv_v_f_f32m1(v[j + 1], numVals);
            rs = vle32_v_f32m1_m(pg, rs, rs_tmp, numVals);
            is = vle32_v_f32m1_m(pg, is, is_tmp, numVals);

            rn = vfmul_vv_f32m1_m(pg, rn, rs, ru, numVals);
            in = vfmul_vv_f32m1_m(pg, in, rs, iu, numVals);
            rn = vfnmsac_vv_f32m1_m(pg, rn, is, iu, numVals);
            in = vfmacc_vv_f32m1_m(pg, in, is, ru, numVals);

            j += 2;

            for (unsigned l = 1; l < gsize; ++l)
            {
              
	      ru = vfmv_v_f_f32m1(v[j], numVals);
              iu = vfmv_v_f_f32m1(v[j + 1], numVals);
              rs = vle32_v_f32m1_m(pg, rs, rs_tmp + l * numVals, numVals);
              is = vle32_v_f32m1_m(pg, is, is_tmp + l * numVals, numVals);
              
	      rn = vfmacc_vv_f32m1_m(pg, rn, ru, rs, numVals);
              in = vfmacc_vv_f32m1_m(pg, in, iu, rs, numVals);
              rn = vfnmsac_vv_f32m1_m(pg, rn, iu, is, numVals);
              in = vfmacc_vv_f32m1_m(pg, in, ru, is, numVals);
	      j += 2;

            }  
            // re += rs[k] * rn + is[k] * in;
            // im += rs[k] * in - is[k] * rn;
            
	    rs = vle32_v_f32m1_m(pg, rs, rs_tmp + (k*lsize+s) * numVals, numVals);
            is = vle32_v_f32m1_m(pg, is, is_tmp + (k*lsize+s) * numVals, numVals);
            
	    svfloat32_t v_re = vfmacc_vv_f32m1_m(pg, vfmul_vv_f32m1_m(pg, v_re, rs, rn, numVals), is, in, numVals);
            svfloat32_t v_im = vfmacc_vv_f32m1_m(pg, vfmul_vv_f32m1_m(pg, v_im, rs, in, numVals), is, rn, numVals);
            
	    //re += svaddv_f32(pg, v_re, numVals);
            //im += svaddv_f32(pg, v_im, numVals);
	    
	    re_t = vfredusum_vs_f32m1_f32m1_m(pg, re_t, re_t, v_re, numVals);
            im_t = vfredusum_vs_f32m1_f32m1_m(pg, im_t, im_t, v_im, numVals);

	    re = vfmv_f_s_f32m1_f32(re_t);
	    im = vfmv_f_s_f32m1_f32(im_t);
          }
        return std::complex<double>{re, im};
      };

      std::vector<uint64_t> ms((H) + 1);
      std::vector<uint64_t> xss(1 << H);
      std::vector<uint64_t> lmask(1 << L);
      FillIndices_HL(state.num_qubits(), qs, ms.data(), xss.data(), H, L);

      unsigned k = H + log2val;
      unsigned n = state.num_qubits() > k ? state.num_qubits() - k : 0;
      uint64_t size = uint64_t{1} << n;
      svbool_t pg = vmset_m_b32(numVals);
      lmask.data()[0] = 0;
      uint64_t count = 0;
      for (int i = 0; i < (1 << L); i++)
      {
        int number = 0;
        for (int j = 0; j < L; j++)
        {
          if ((i >> j) & 1)
          {
            number |= (1 << qs[j]); // Set bit qs[j]
          }
        }
        lmask.data()[i] = number;
      }
      if (L == 0)
        pg = vmset_m_b32(numVals);
      else
      {
        uint32_t tmp = 0x0;
        for (int x = 0; x < L; ++x)
        {
          tmp = tmp | (1 << qs[x]);
        }
        vuint32m1_t idex = vid_v_u32m1(numVals);
        idex = vand_vx_u32m1(idex, tmp, numVals);
        pg = vmseq_vx_u32m1_b32(idex, 0, numVals);
      }
      
      using Op = std::plus<std::complex<double>>;
      return for_.RunReduce(size, f, Op(), matrix, ms.data(), xss.data(), state.get(), pg, H, L, lmask.data());
    }

    For for_;
  };

} // namespace qsim

#endif // SIMULATOR_RVV_H_

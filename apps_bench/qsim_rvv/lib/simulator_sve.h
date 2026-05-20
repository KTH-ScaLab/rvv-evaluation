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

#ifndef SIMULATOR_SVE_H_
#define SIMULATOR_SVE_H_

#include <complex>
#include <cstdint>
#include <functional>
#include <vector>
#include <arm_sve.h>

#include "simulator.h"
#include "statespace_sve.h"
#include "statespace_basic.h"
#define numVals svcntw()
#define log2val (int)log2(numVals)
#define DEBUG 0
namespace qsim
{

  /**
   * Quantum circuit simulator without vectorization.
   */
  template <typename For, typename FP = float>
  class SimulatorSVE final : public SimulatorBase
  {
  public:
    using StateSpace = StateSpaceSVE<For, FP>;
    using State = typename StateSpace::State;
    using fp_type = typename StateSpace::fp_type;

    template <typename... ForArgs>
    explicit SimulatorSVE(ForArgs &&...args) : for_(args...) {}

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
            svfloat32_t v0 = svld1_f32(pg, p0 + xss[i] + lmask[j]);           // load first 4 floats
            svfloat32_t v1 = svld1_f32(pg, p0 + xss[i] + lmask[j] + numVals); // load next 4 floats
            svst1_f32(pg, rs_tmp + count * numVals, v0);                      // store first 4 floats
            svst1_f32(pg, is_tmp + count * numVals, v1);                      // store next 4 floats
            count = count + 1;
          }

        uint64_t j = 0;

        for (unsigned k = 0; k < hsize; ++k)
          for (unsigned s = 0; s < lsize; ++s)
          {
            // rn = rs[0] * v[j] - is[0] * v[j + 1];
            // in = rs[0] * v[j + 1] + is[0] * v[j];
            ru = svdup_f32(v[j]);
            iu = svdup_f32(v[j + 1]);
            rs = svld1_f32(pg, rs_tmp);
            is = svld1_f32(pg, is_tmp);

            rn = svmul_f32_z(pg, rs, ru);
            in = svmul_f32_z(pg, rs, iu);
            rn = svmls_f32_z(pg, rn, is, iu);
            in = svmla_f32_z(pg, in, is, ru);

            j += 2;

            for (unsigned l = 1; l < gsize; ++l)
            {
              ru = svdup_f32(v[j]);
              iu = svdup_f32(v[j + 1]);
              rs = svld1_f32(pg, rs_tmp + l * numVals);
              is = svld1_f32(pg, is_tmp + l * numVals);
              rn = svmla_f32_z(pg, rn, ru, rs);
              in = svmla_f32_z(pg, in, iu, rs);
              rn = svmls_f32_z(pg, rn, iu, is);
              in = svmla_f32_z(pg, in, ru, is);
              
	      j += 2;
            }

            svst1_f32(pg, p0 + xss[k] + lmask[s], rn);
            svst1_f32(pg, p0 + xss[k] + lmask[s] + numVals, in);

          }
      };

      std::vector<uint64_t> ms((H) + 1);
      std::vector<uint64_t> xss(1 << H);
      std::vector<uint64_t> lmask(1 << L);
      FillIndices_HL(state.num_qubits(), qs, ms.data(), xss.data(), H, L);

      unsigned k = H + log2val;
      unsigned n = state.num_qubits() > k ? state.num_qubits() - k : 0;
      uint64_t size = uint64_t{1} << n;
      svbool_t pg = svptrue_b32();
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
        pg = svptrue_b32();
      else
      {
        uint32_t tmp = 0x0;
        for (int x = 0; x < L; ++x)
        {
          tmp = tmp | (1 << qs[x]);
        }
        svuint32_t idex = svindex_u32(0, 1);
        idex = svand_n_u32_z(svptrue_b32(), idex, tmp);
        pg = svcmpeq_u32(svptrue_b32(), idex, svdup_n_u32(0));
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
              svfloat32_t v0 = svld1_f32(pg, p0 + xss[i] + lmask[j]);           // load first 4 floats
              svfloat32_t v1 = svld1_f32(pg, p0 + xss[i] + lmask[j] + numVals); // load next 4 floats
              svst1_f32(pg, rs_tmp + count * numVals, v0);                      // store first 4 floats
              svst1_f32(pg, is_tmp + count * numVals, v1);                      // store next 4 floats
              count = count + 1;
            }

          uint64_t j = 0;

          for (unsigned k = 0; k < hsize; ++k)
            for (unsigned s = 0; s < lsize; ++s)
            {
              // rn = rs[0] * v[j] - is[0] * v[j + 1];
              // in = rs[0] * v[j + 1] + is[0] * v[j];
              ru = svdup_f32(v[j]);
              iu = svdup_f32(v[j + 1]);
              rs = svld1_f32(pg, rs_tmp);
              is = svld1_f32(pg, is_tmp);

              rn = svmul_f32_z(pg, rs, ru);
              in = svmul_f32_z(pg, rs, iu);
              rn = svmls_f32_z(pg, rn, is, iu);
              in = svmla_f32_z(pg, in, is, ru);

              j += 2;

              for (unsigned l = 1; l < gsize; ++l)
              {
                ru = svdup_f32(v[j]);
                iu = svdup_f32(v[j + 1]);
                rs = svld1_f32(pg, rs_tmp + l * numVals);
                is = svld1_f32(pg, is_tmp + l * numVals);
                rn = svmla_f32_z(pg, rn, ru, rs);
                in = svmla_f32_z(pg, in, iu, rs);
                rn = svmls_f32_z(pg, rn, iu, is);
                in = svmla_f32_z(pg, in, ru, is);

                j += 2;
              }

              svst1_f32(pg, p0 + xss[k] + lmask[s], rn);
              svst1_f32(pg, p0 + xss[k] + lmask[s] + numVals, in);

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
      svbool_t pg = svptrue_b32();
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

      svbool_t pg_qs = svptrue_b32();
      svbool_t pg_cqs0 = svptrue_b32();
      svbool_t pg_cqs1 = svptrue_b32();

      if( L != 0){  
        for (int x = 0; x < L; ++x)
           tmp_qs = tmp_qs | (1 << qs[x]);
	svuint32_t idex_qs = svindex_u32(0, 1);
        idex_qs = svand_n_u32_z(svptrue_b32(), idex_qs, tmp_qs);
        pg_qs = svcmpeq_u32(svptrue_b32(), idex_qs, svdup_n_u32(0));
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
	svuint32_t idex_cqs = svindex_u32(0, 1);
        idex_cqs = svand_n_u32_z(svptrue_b32(), idex_cqs, tmp_cqs0);	    
	pg_cqs0 = svcmpeq_n_u32(svptrue_b32(), idex_cqs, 0);

	idex_cqs = svindex_u32(0, 1);
	idex_cqs = svand_n_u32_z(svptrue_b32(), idex_cqs, tmp_cqs1);    
	pg_cqs1 = svcmpeq_n_u32(svptrue_b32(), idex_cqs, tmp_cqs1);	
      }


      pg = svand_b_z(svptrue_b32(), pg_cqs0, pg_cqs1);
      pg = svand_b_z(svptrue_b32(), pg, pg_qs);
      
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
            svfloat32_t v0 = svld1_f32(pg, p0 + xss[i] + lmask[j]);           // load first 4 floats
            svfloat32_t v1 = svld1_f32(pg, p0 + xss[i] + lmask[j] + numVals); // load next 4 floats
            svst1_f32(pg, rs_tmp + count * numVals, v0);                      // store first 4 floats
            svst1_f32(pg, is_tmp + count * numVals, v1);                      // store next 4 floats
            count = count + 1;
          }

        double re = 0;
        double im = 0;

        uint64_t j = 0;

        for (unsigned k = 0; k < hsize; ++k)
          for (unsigned s = 0; s < lsize; ++s)
          {
            // rn = rs[0] * v[j] - is[0] * v[j + 1];
            // in = rs[0] * v[j + 1] + is[0] * v[j];
            ru = svdup_f32(v[j]);
            iu = svdup_f32(v[j + 1]);
            rs = svld1_f32(pg, rs_tmp);
            is = svld1_f32(pg, is_tmp);
            
	    rn = svmul_f32_z(pg, rs, ru);
	    in = svmul_f32_z(pg, rs, iu);
            rn = svmls_f32_z(pg, rn, is, iu);
            in = svmla_f32_z(pg, in, is, ru);

            j += 2;

            for (unsigned l = 1; l < gsize; ++l)
            {
              ru = svdup_f32(v[j]);
              iu = svdup_f32(v[j + 1]);
              rs = svld1_f32(pg, rs_tmp + l * numVals);
              is = svld1_f32(pg, is_tmp + l * numVals);
              rn = svmla_f32_z(pg, rn, ru, rs);
              in = svmla_f32_z(pg, in, iu, rs);
              rn = svmls_f32_z(pg, rn, iu, is);
              in = svmla_f32_z(pg, in, ru, is);
              
	      j += 2;

            }  
            // re += rs[k] * rn + is[k] * in;
            // im += rs[k] * in - is[k] * rn;
            rs = svld1_f32(pg, rs_tmp + (k*lsize+s) * numVals);
            is = svld1_f32(pg, is_tmp + (k*lsize+s) * numVals);
            svfloat32_t v_re = svmla_f32_x(pg, svmul_f32_x(pg, rs, rn), is, in);
            svfloat32_t v_im = svmls_f32_x(pg, svmul_f32_x(pg, rs, in), is, rn);
            re += svaddv_f32(pg, v_re);
            im += svaddv_f32(pg, v_im);
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
      svbool_t pg = svptrue_b32();
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
        pg = svptrue_b32();
      else
      {
        uint32_t tmp = 0x0;
        for (int x = 0; x < L; ++x)
        {
          tmp = tmp | (1 << qs[x]);
        }
        svuint32_t idex = svindex_u32(0, 1);
        idex = svand_n_u32_z(svptrue_b32(), idex, tmp);
        pg = svcmpeq_u32(svptrue_b32(), idex, svdup_n_u32(0));
      }
      
      using Op = std::plus<std::complex<double>>;
      return for_.RunReduce(size, f, Op(), matrix, ms.data(), xss.data(), state.get(), pg, H, L, lmask.data());
    }

    For for_;
  };

} // namespace qsim

#endif // SIMULATOR_SVE_H_

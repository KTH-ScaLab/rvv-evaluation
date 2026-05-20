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

#include "sve_vector.hh"
#include "simulator.h"
#include "statespace_sve.h"
#include "statespace_basic.h"
#include <time.h>

#define numVals svcntw()
#define DEBUG 0
namespace qsim {

/**
 * Quantum circuit simulator without vectorization.
 */
template <typename For, typename FP = float>
class SimulatorSVE final : public SimulatorBase {
 public:
  using StateSpace = StateSpaceSVE<For, FP>;
  using State = typename StateSpace::State;
  using fp_type = typename StateSpace::fp_type;


  template <typename... ForArgs>
  explicit SimulatorSVE(ForArgs&&... args) : for_(args...) {}

  /**
   * Applies a gate using non-vectorized instructions.
   * @param qs Indices of the qubits affected by this gate.
   * @param matrix Matrix representation of the gate to be applied.
   * @param state The state of the system, to be updated by this method.
   */
  void ApplyGate(const std::vector<unsigned>& qs,
                 const fp_type* matrix, State& state) const {
    // Assume qs[0] < qs[1] < qs[2] < ... .

    // switch (qs.size()) {
    // case 0:
    //   ApplyGateH<0>(qs, matrix, state);
    //   break;
    // case 1:
    //   ApplyGateH<1>(qs, matrix, state);
    //   break;
    // case 2:
    //   ApplyGateH<2>(qs, matrix, state);
    //   break;
    // case 3:
    //   ApplyGateH<3>(qs, matrix, state);
    //   break;
    // case 4:
    //   ApplyGateH<4>(qs, matrix, state);
    //   break;
    // case 5:
    //   ApplyGateH<5>(qs, matrix, state);
    //   break;
    // case 6:
    //   ApplyGateH<6>(qs, matrix, state);
    //   break;
    // default:
    //   // Not implemented.
    //   break;
    // }
    //

    int log2val = (int)log2(numVals);
    // Since qs is ordered,
    unsigned lcount = 0;
    for (int i = 0; i < qs.size(); ++i) {
        if (qs[i] < log2val)
            ++lcount;
        else
            break;
    }
    unsigned L = lcount;
    unsigned H = qs.size() - lcount;
    if(DEBUG){
      for (size_t i = 0; i < qs.size(); ++i) 
        printf("qs[%zu] = %u\n", i, qs[i]);
      printf("L:%d, H:%d \n",L,H);
    }
    ApplyGateH(qs, matrix, state,H,L);
  }

  /**
   * Applies a controlled gate using non-vectorized instructions.
   * @param qs Indices of the qubits affected by this gate.
   * @param cqs Indices of control qubits.
   * @param cvals Bit mask of control qubit values.
   * @param matrix Matrix representation of the gate to be applied.
   * @param state The state of the system, to be updated by this method.
   */
  void ApplyControlledGate(const std::vector<unsigned>& qs,
                           const std::vector<unsigned>& cqs, uint64_t cvals,
                           const fp_type* matrix, State& state) const {
    // Assume qs[0] < qs[1] < qs[2] < ... .

    if (cqs.size() == 0) {
      ApplyGate(qs, matrix, state);
      return;
    }

    switch (qs.size()) {
    case 0:
      ApplyControlledGateH<0>(qs, cqs, cvals, matrix, state);
      break;
    case 1:
      ApplyControlledGateH<1>(qs, cqs, cvals, matrix, state);
      break;
    case 2:
      ApplyControlledGateH<2>(qs, cqs, cvals, matrix, state);
      break;
    case 3:
      ApplyControlledGateH<3>(qs, cqs, cvals, matrix, state);
      break;
    case 4:
      ApplyControlledGateH<4>(qs, cqs, cvals, matrix, state);
      break;
    default:
      // Not implemented.
      break;
    }
  }

  /**
   * Computes the expectation value of an operator using non-vectorized
   * instructions.
   * @param qs Indices of the qubits the operator acts on.
   * @param matrix The operator matrix.
   * @param state The state of the system.
   * @return The computed expectation value.
   */
  std::complex<double> ExpectationValue(const std::vector<unsigned>& qs,
                                        const fp_type* matrix,
                                        const State& state) const {
    // Assume qs[0] < qs[1] < qs[2] < ... .

    switch (qs.size()) {
    case 1:
      return ExpectationValueH<1>(qs, matrix, state);
      break;
    case 2:
      return ExpectationValueH<2>(qs, matrix, state);
      break;
    case 3:
      return ExpectationValueH<3>(qs, matrix, state);
      break;
    case 4:
      return ExpectationValueH<4>(qs, matrix, state);
      break;
    case 5:
      return ExpectationValueH<5>(qs, matrix, state);
      break;
    case 6:
      return ExpectationValueH<6>(qs, matrix, state);
      break;
    default:
      // Not implemented.
      break;
    }

    return 0;
  }

  /**
   * @return The size of SIMD register if applicable.
   */
  static unsigned SIMDRegisterSize() {
    return 1;
  }

 private:

  static void FillIndices_HL(unsigned num_qubits, const std::vector<unsigned>& qs,
                            uint64_t* ms, uint64_t* xss, unsigned H, unsigned L ) {
    unsigned hsize = 1 << H;

    if (H == 0) {
      ms[0] = uint64_t(-1);
      xss[0] = 0;
    } else {
      std::vector<uint64_t> xs(H + 1); 

      xs[0] = uint64_t{1} << (qs[L] + 1);
      ms[0] = (uint64_t{1} << qs[L]) - 1;
      for (unsigned i = 1; i < H; ++i) {
        xs[i] = uint64_t{1} << (qs[L + i] + 1);
        ms[i] = ((uint64_t{1} << qs[L + i]) - 1) ^ (xs[i - 1] - 1);
      }
      ms[H] = ((uint64_t{1} << num_qubits) - 1) ^ (xs[H - 1] - 1);

      for (unsigned i = 0; i < hsize; ++i) {
        uint64_t a = 0;
        for (unsigned k = 0; k < H; ++k) {
          a += xs[k] * ((i >> k) & 1);
        }
        xss[i] = a;
      }
    }
  }

  

  //template <unsigned H, unsigned L>
  void ApplyGateH(const std::vector<unsigned>& qs,
                  const fp_type* matrix, State& state,unsigned H, unsigned L) const {
    auto f = [](unsigned n, unsigned m, uint64_t i, const fp_type* v,
                const uint64_t* ms, const uint64_t* xss, fp_type* rstate,svbool_t pg,unsigned H, unsigned L,const uint64_t* lmask) {
      unsigned gsize = 1 << (H + L);
      unsigned hsize = 1 << H;
      unsigned lsize = 1 << L;
      svfloat32_t ru, iu, rn, in, rs ,is;
      i *= numVals;
      DECLARE_VECS();
      uint64_t ii = i & ms[0];
      for (unsigned j = 1; j <= H; ++j) {
        i *= 2;
        ii |= i & ms[j];
      }

      auto p0 = rstate + 2 * ii;

      // memcpy(p1, p0, sizeof(fp_type) * 2 * copy_size);
      for (int i = 0; i < hsize; i++)
        for (int j = 0; j < lsize; j++)
        {
	  // SET_V(i+j,svld1_f32(pg, p0 + xss[i] + lmask[j]));        // load first 4 real
    	  // SET_V((i+j+gsize),svld1_f32(pg, p0 + xss[i] + lmask[j] + numVals));   // load next 4 img
          SET_V_ADDR(i+j,(p0 + xss[i] + lmask[j]));
	  SET_V_ADDR((i+j+gsize),(p0 + xss[i] + lmask[j] + numVals));
	}

      uint64_t j = 0;
      for (unsigned k = 0; k < hsize; ++k) 
	for (unsigned s = 0; s < lsize; ++s){
        // rn = rs[0] * v[j] - is[0] * v[j + 1];
        // in = rs[0] * v[j + 1] + is[0] * v[j];
          rs = V(0);
          is = V(gsize);
          ru = svdup_f32(v[j]);
          iu = svdup_f32(v[j + 1]);

	  rn = svmul_f32_z(pg, rs, ru);
          in = svmul_f32_z(pg, rs, iu);
          rn = svmls_f32_z(pg, rn, is, iu);
          in = svmla_f32_z(pg, in, is, iu);

          j += 2;

          for (unsigned h = 1; h < gsize; ++h) { 
            ru = svdup_f32(v[j]);
            iu = svdup_f32(v[j + 1]);
            rs = V(h);
            is = V(h + gsize);
                      
	    rn = svmla_f32_z(pg, rn, ru, rs);
            in = svmla_f32_z(pg, in, iu, rs);
            rn = svmls_f32_z(pg, rn, iu, is);
            in = svmla_f32_z(pg, in, ru, is);

            j += 2;
         }
         svst1_f32(pg,p0 + xss[k] + lmask[s], rn);
         svst1_f32(pg,p0 + xss[k] + lmask[s] + numVals, in);
       }
};

    std::vector<uint64_t> ms(H + 1);
    std::vector<uint64_t> xss(1 << H);
    std::vector<uint64_t> lmask(1 << L);
    FillIndices_HL(state.num_qubits(), qs, ms.data(), xss.data(),H,L);

    unsigned k = H + log2(numVals);
    unsigned n = state.num_qubits() > k ? state.num_qubits() - k : 0;
    uint64_t size = uint64_t{1} << n;
    svbool_t pg = svptrue_b32();
    lmask.data()[0] = 0;
    uint64_t count = 0;
    for (int i = 0; i < (1<<L); i++) {
        int number = 0;
        for (int j = 0; j < L; j++) {
            if ((i >> j) & 1) {
                number |= (1 << qs[j]); // Set bit qs[j]
            }
        }
	lmask.data()[i] = number;
	if(DEBUG)
          printf("lmaks[%d]=%d \n",count,lmask.data()[i]);
    }
    if(L==0)
      pg = svptrue_b32();
    else
    {
      uint32_t tmp = 0x0;
      for(int x = 0; x < L; ++x){
         tmp= tmp | (1 << qs[x]);
      }
      svuint32_t idex = svindex_u32(0, 1);
      idex = svand_n_u32_z(svptrue_b32(),idex,tmp);
      pg = svcmpeq_u32(svptrue_b32(), idex , svdup_n_u32(0));
    }
    if(DEBUG){
    for (size_t i = 0; i < (1 << H); ++i)
        printf("xss[%zu] = %u\n", i, xss[i]);

    printf("size of qubit system:%d\n",state.num_qubits());
    printf("size for loop:%d\n",size);
    }
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for_.Run(size, f, matrix, ms.data(), xss.data(), state.get(),pg,H,L,lmask.data());
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time used: %f seconds\n", cpu_time_used);

    
    }

  
  template <unsigned H>
  void ApplyControlledGateH(const std::vector<unsigned>& qs,
                            const std::vector<unsigned>& cqs,
                            uint64_t cvals, const fp_type* matrix,
                            State& state) const {
    auto f = [](unsigned n, unsigned m, uint64_t i, const fp_type* v,
                const uint64_t* ms, const uint64_t* xss,
                uint64_t cvalsh, uint64_t cmaskh, fp_type* rstate) {
      constexpr unsigned hsize = 1 << H;

      fp_type rn, in;
      fp_type rs[hsize], is[hsize];

      uint64_t ii = i & ms[0];
      for (unsigned j = 1; j <= H; ++j) {
        i *= 2;
        ii |= i & ms[j];
      }

      if ((ii & cmaskh) == cvalsh) {
        auto p0 = rstate + 2 * ii;

        for (unsigned k = 0; k < hsize; ++k) {
          rs[k] = *(p0 + xss[k]);
          is[k] = *(p0 + xss[k] + 1);
        }

        uint64_t j = 0;

        for (unsigned k = 0; k < hsize; ++k) {
          rn = rs[0] * v[j] - is[0] * v[j + 1];
          in = rs[0] * v[j + 1] + is[0] * v[j];

          j += 2;

          for (unsigned l = 1; l < hsize; ++l) {
            rn += rs[l] * v[j] - is[l] * v[j + 1];
            in += rs[l] * v[j + 1] + is[l] * v[j];

            j += 2;
          }

          *(p0 + xss[k]) = rn;
          *(p0 + xss[k] + 1) = in;
        }
      }
    };

    uint64_t ms[H + 1];
    uint64_t xss[1 << H];

    FillIndices<H>(state.num_qubits(), qs, ms, xss);

    auto m = GetMasks7(state.num_qubits(), qs, cqs, cvals);

    unsigned n = state.num_qubits() > H ? state.num_qubits() - H : 0;
    uint64_t size = uint64_t{1} << n;

    for_.Run(size, f, matrix, ms, xss, m.cvalsh, m.cmaskh, state.get());
  }

  template <unsigned H>
  std::complex<double> ExpectationValueH(const std::vector<unsigned>& qs,
                                         const fp_type* matrix,
                                         const State& state) const {
    auto f = [](unsigned n, unsigned m, uint64_t i, const fp_type* v,
                const uint64_t* ms, const uint64_t* xss,
                const fp_type* rstate) {
      constexpr unsigned hsize = 1 << H;

      fp_type rn, in;
      fp_type rs[hsize], is[hsize];

      uint64_t ii = i & ms[0];
      for (unsigned j = 1; j <= H; ++j) {
        i *= 2;
        ii |= i & ms[j];
      }

      auto p0 = rstate + 2 * ii;

      for (unsigned k = 0; k < hsize; ++k) {
        rs[k] = *(p0 + xss[k]);
        is[k] = *(p0 + xss[k] + 1);
      }

      double re = 0;
      double im = 0;

      uint64_t j = 0;

      for (unsigned k = 0; k < hsize; ++k) {
        rn = rs[0] * v[j] - is[0] * v[j + 1];
        in = rs[0] * v[j + 1] + is[0] * v[j];

        j += 2;

        for (unsigned l = 1; l < hsize; ++l) {
          rn += rs[l] * v[j] - is[l] * v[j + 1];
          in += rs[l] * v[j + 1] + is[l] * v[j];

          j += 2;
        }

        re += rs[k] * rn + is[k] * in;
        im += rs[k] * in - is[k] * rn;
      }

      return std::complex<double>{re, im};
    };

    uint64_t ms[H + 1];
    uint64_t xss[1 << H];

    FillIndices<H>(state.num_qubits(), qs, ms, xss);

    unsigned n = state.num_qubits() > H ? state.num_qubits() - H : 0;
    uint64_t size = uint64_t{1} << n;

    using Op = std::plus<std::complex<double>>;
    return for_.RunReduce(size, f, Op(), matrix, ms, xss, state.get());
  }

  For for_;
};

}  // namespace qsim

#endif  // SIMULATOR_SVE_H_

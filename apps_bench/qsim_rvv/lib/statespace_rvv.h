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

#ifndef STATESPACE_RVV_H_
#define STATESPACE_RVV_H_

#include <cmath>
#include <complex>
#include <cstdint>
#include <functional>
#include "statespace.h"
#include "util.h"
#include "vectorspace.h"

#include "riscv_compatible.h"
//const size_t numVals=vsetvlmax_e32m1()
#define log2val (int)log2(numVals)
#define svfloat32_t vfloat32m1_t
#define svbool_t vbool32_t
//#define numVals (size_t)vsetvlmax_e32m1()
static const size_t numVals = vsetvlmax_e32m1();

namespace qsim {

/**
 * Object containing context and routines for unoptimized state-vector
 * manipulations. State is a non-vectorized sequence of one real amplitude
 * followed by one imaginary amplitude.
 */
template <typename For, typename FP>
class StateSpaceRVV :
    public StateSpace<StateSpaceRVV<For, FP>, VectorSpace, For, FP> {
 private:
  using Base = StateSpace<StateSpaceRVV<For, FP>, qsim::VectorSpace, For, FP>;

 public:
  using State = typename Base::State;
  using fp_type = typename Base::fp_type;

  template <typename... ForArgs>
  explicit StateSpaceRVV(ForArgs&&... args) : Base(args...) {}

  static uint64_t MinSize(unsigned num_qubits) {
    //return 2 * (uint64_t{1} << num_qubits);
    return std::max(uint64_t{2 * numVals}, 2 * (uint64_t{1} << num_qubits));
  };

  void InternalToNormalOrder(State& state) const {
//      if (state.num_qubits() == 1) {
//      auto s = state.get();
//
//      s[2] = s[1];
//      s[1] = s[4];
//      s[3] = s[5];
//
//      for (uint64_t i = 4; i < 8; ++i) {
//        s[i] = 0;
//      }
//
//    } else {
      auto f = [](unsigned n, unsigned m, uint64_t i, fp_type* p) {
        auto s = p + 2*numVals * i;

        fp_type re[numVals-1];
        fp_type im[numVals-1];

        for (uint64_t i = 0; i < (numVals-1); ++i) {
          re[i] = s[i + 1];
          im[i] = s[i + numVals];
        }

        for (uint64_t i = 0; i < (numVals-1); ++i) {
          s[2 * i + 1] = im[i];
          s[2 * i + 2] = re[i];
        }
      };
      Base::for_.Run(MinSize(state.num_qubits()) / (2*numVals), f, state.get());
    //}
  }

  void NormalToInternalOrder(State& state) const {    
//    if (state.num_qubits() == 1) {
//      auto s = state.get();
//
//      s[numVals] = s[1];
//      s[1] = s[2];
//      s[numVals+1] = s[3];
//
//      s[2] = 0;
//      s[3] = 0;
//      s[6] = 0;
//      s[7] = 0;
//    } else {
      auto f = [](unsigned n, unsigned m, uint64_t i, fp_type* p) {
        auto s = p + (numVals*2) * i;

        fp_type re[(numVals-1)];
        fp_type im[(numVals-1)];

        for (uint64_t i = 0; i < (numVals-1); ++i) {
          im[i] = s[2 * i + 1];
          re[i] = s[2 * i + 2];
        }

        for (uint64_t i = 0; i < (numVals-1); ++i) {
          s[i + 1] = re[i];
          s[i + numVals] = im[i];
        }
    };

      Base::for_.Run(MinSize(state.num_qubits()) / (2*numVals), f, state.get());
    //}
}

  void SetAllZeros(State& state) const {
    auto f = [](unsigned n, unsigned m, uint64_t i, fp_type* p) {
    	vse32_v_f32m1(p + (2*numVals) * i, vfmv_v_f_f32m1(0, numVals), numVals);
   	vse32_v_f32m1(p + (2*numVals) * i + numVals, vfmv_v_f_f32m1(0,numVals), numVals);	
    };

    Base::for_.Run(MinSize(state.num_qubits()) / (2*numVals), f, state.get());
  }

  // Uniform superposition.
  void SetStateUniform(State& state) const {
    fp_type val = fp_type{1} / std::sqrt(uint64_t{1} << state.num_qubits());
    svbool_t pg;
    uint32_t size= uint64_t{1} << state.num_qubits();
    if(numVals > size)
    {
        vuint32m1_t idx = vid_v_u32m1(numVals);
        pg = vmsltu_vx_u32m1_b32(idx, size, numVals);
    }
    else
        pg = vmset_m_b32(numVals);
    auto f = [](unsigned n, unsigned m, uint64_t i,
                fp_type val, fp_type* p, svbool_t pg) {

        vse32_v_f32m1(p + (2*numVals) * i, vfmv_v_f_f32m1_M(pg,val,numVals),numVals);            // Store v
        vse32_v_f32m1(p + (2*numVals) * i + vfmv_v_f_f32m1(0.0f,numVals),numVals);    // Store 0s
    };

    Base::for_.Run(MinSize(state.num_qubits()) / (2*numVals), f, val, state.get(),pg);
  }
  // |0> state.
  void SetStateZero(State& state) const {
    SetAllZeros(state);
    state.get()[0] = 1;
  }

  static std::complex<fp_type> GetAmpl(const State& state, uint64_t i) {
    uint64_t p = ((2*numVals) * (i / (numVals))) + (i % (numVals));
    return std::complex<fp_type>(state.get()[p], state.get()[p + numVals]);
  }

  static void SetAmpl(
      State& state, uint64_t i, const std::complex<fp_type>& ampl) {
    uint64_t p = ((2*numVals) * (i / (numVals))) + (i % (numVals));
    state.get()[p] = std::real(ampl);
    state.get()[p + numVals] = std::imag(ampl);
  }

  static void SetAmpl(State& state, uint64_t i, fp_type re, fp_type im) {
    uint64_t p = ((2*numVals) * (i / (numVals))) + (i % (numVals));
    state.get()[p] = re;
    state.get()[p + numVals] = im;
  }

  // Sets state[i] = complex(re, im) where (i & mask) == bits.
  // if `exclude` is true then the criteria becomes (i & mask) != bits.
  void BulkSetAmpl(State& state, uint64_t mask, uint64_t bits,
                   const std::complex<fp_type>& val,
                   bool exclude = false) const {
    BulkSetAmpl(state, mask, bits, std::real(val), std::imag(val), exclude);
  }

  // Sets state[i] = complex(re, im) where (i & mask) == bits.
  // if `exclude` is true then the criteria becomes (i & mask) != bits.
  void BulkSetAmpl(State& state, uint64_t mask, uint64_t bits, fp_type re,
                 fp_type im, bool exclude = false) const {
    // Create SVE vectors with replicated values
    svfloat32_t re_reg = vfmv_v_f_f32m1(re, numVals);
    svfloat32_t im_reg = vfmv_v_f_f32m1(im, numVals);
    
    // Create exclusion mask
    svbool_t exclude_mask = vmset_m_b32(numVals);
    if (exclude) {
        exclude_mask = vmclr_m_b32(numVals); ;
    }

    auto f = [](unsigned n, unsigned m, uint64_t i, uint64_t maskv,
                uint64_t bitsv, svfloat32_t re_n, svfloat32_t im_n, 
                svbool_t exclude_n, fp_type* p) {

        vuint32m1_t indices = vid_v_u32m1(numVals);
        vuint32m1_t global_idx = vadd_vx_u32m1(indices, numVals*i, numVals);
        
        // Create mask for matching elements
        vuint32m1_t mask_vec = vmv_v_x_u32m1(maskv, numVals);
        vuint32m1_t bits_vec = vmv_v_x_u32m1(bitsv, numVals);
        svbool_t match_mask = vmseq_vv_u32m1_b32(
                                        vand_vv_u32m1(global_idx, mask_vec, numVals),
                                        vand_vv_u32m1(bits_vec, mask_vec, numVals), numVals);
        
        // Apply exclusion if needed
        svbool_t final_mask = vmnot_m_b32(vmor_mm_b32(match_mask, exclude_n,numVals),numVals);

        // Only process if any lanes are active
        if (vcpop_m_b32(final_mask, numVals)) {
            // Load real and imaginary parts
            svfloat32_t re_vals = vle32_v_f32m1(final_mask, p + 2 * numVals * i, numVals);
            svfloat32_t im_vals = vle32_v_f32m1(final_mask, p + 2 * numVals * i + numVals, numVals);
            
            // Blend values using the mask
            re_vals = vmerge_vvm_f32m1(final_mask, re_n, re_vals, numVals);
            im_vals = vmerge_vvm_f32m1(final_mask, im_n, im_vals, numVals);
            
            // Store back
            vse32_v_f32m1_m(final_mask, p + 2 * numVals * i, re_vals, numVals);
            vse32_v_f32m1_m(final_mask, p + 2 * numVals * i + numVals, im_vals, numVals);
        }
    };

    Base::for_.Run(MinSize(state.num_qubits()) / (2 * numVals), f, mask, bits, re_reg,
                   im_reg, exclude_mask, state.get());
}
  // Does the equivalent of dest += src elementwise.
  bool Add(const State& src, State& dest) const {
    if (src.num_qubits() != dest.num_qubits()) {
      return false;
    }

    auto f = [](unsigned n, unsigned m, uint64_t i,
                const fp_type* p1, fp_type* p2) {
      // p2[2 * i] += p1[2 * i];
      // p2[2 * i + 1] += p1[2 * i + 1];
      svfloat32_t re1 = vle32_v_f32m1(p1 + 2 * numVals * i, numVals);
      svfloat32_t im1 = vle32_v_f32m1(p1 + 2 * numVals * i + numVals, numVals);
      svfloat32_t re2 = vle32_v_f32m1(p2 + 2 * numVals * i, numVals);
      svfloat32_t im2 = vle32_v_f32m1(p2 + 2 * numVals * i + numVals, numVals);

      vse32_v_f32m1(p2 + 2 * numVals * i, vfadd_vv_f32m1(re1, re2, numVals), numVals);
      vse32_v_f32m1(p2 + 2 * numVals * i + numVals, vfadd_vv_f32m1(im1, im2, numVals), numVals);
    };

    Base::for_.Run(MinSize(src.num_qubits()) / (2 * numVals), f, src.get(), dest.get());

    return true;
  }

  // Does the equivalent of state *= a elementwise.
  void Multiply(fp_type a, State& state) const {
    auto f = [](unsigned n, unsigned m, uint64_t i, fp_type a, fp_type* p) {
      // p[2 * i] *= a;
      // p[2 * i + 1] *= a;
      svfloat32_t re = vle32_v_f32m1(p + 2 * numVals * i, numVals);
      svfloat32_t im = vle32_v_f32m1(p + 2 * numVals * i + numVals, numVals);
      svfloat32_t r_vec = __riscvi_vfmv_v_f_f32m1(a, numVals);  // Broadcast scalar to vector

      re = vfmul_vv_f32m1(re, r_vec, numVals);
      im = vfmul_vv_f32m1(im, r_vec, numVals);

      vse32_v_f32m1(p + 2 * numVals * i, re, numVals);
      vse32_v_f32m1(p + 2 * numVals * i + numVals, im, numVals);

    };

    Base::for_.Run(MinSize(state.num_qubits()) / (2 * numVals), f, a, state.get());
  }

  std::complex<double> InnerProduct(
      const State& state1, const State& state2) const {
    if (state1.num_qubits() != state2.num_qubits()) {
      return std::nan("");
    }

    auto f = [](unsigned n, unsigned m, uint64_t i,
                const fp_type* p1, const fp_type* p2) -> std::complex<double> {
      auto s1 = p1 + 2 * i;
      auto s2 = p2 + 2 * i;

      double re = s1[0] * s2[0] + s1[1] * s2[1];
      double im = s1[0] * s2[1] - s1[1] * s2[0];

      return std::complex<double>{re, im};
    };

    using Op = std::plus<std::complex<double>>;
    return Base::for_.RunReduce(
        MinSize(state1.num_qubits()) / 2, f, Op(), state1.get(), state2.get());
  }

  double RealInnerProduct(const State& state1, const State& state2) const {
    if (state1.num_qubits() != state2.num_qubits()) {
      return std::nan("");
    }

    auto f = [](unsigned n, unsigned m, uint64_t i,
                const fp_type* p1, const fp_type* p2) -> double {
      auto s1 = p1 + 2 * i;
      auto s2 = p2 + 2 * i;

      return s1[0] * s2[0] + s1[1] * s2[1];
    };

    using Op = std::plus<double>;
    return Base::for_.RunReduce(
        MinSize(state1.num_qubits()) / 2, f, Op(), state1.get(), state2.get());
  }

  template <typename DistrRealType = double>
  std::vector<uint64_t> Sample(
      const State& state, uint64_t num_samples, unsigned seed) const {
    std::vector<uint64_t> bitstrings;

    if (num_samples > 0) {
      double norm = 0;
      uint64_t size = MinSize(state.num_qubits()) / 2;

      const fp_type* p = state.get();

      for (uint64_t k = 0; k < size; ++k) {
        double re = p[2 * k];
        double im = p[2 * k + 1];
        norm += re * re + im * im;
      }

      auto rs = GenerateRandomValues<DistrRealType>(num_samples, seed, norm);

      uint64_t m = 0;
      double csum = 0;
      bitstrings.reserve(num_samples);

      for (uint64_t k = 0; k < size; ++k) {
        double re = p[2 * k];
        double im = p[2 * k + 1];
        csum += re * re + im * im;
        while (rs[m] < csum && m < num_samples) {
          bitstrings.emplace_back(k);
          ++m;
        }
      }

      for (; m < num_samples; ++m) {
        bitstrings.emplace_back((uint64_t{1} << state.num_qubits()) - 1);
      }
    }

    return bitstrings;
  }

  using MeasurementResult = typename Base::MeasurementResult;

  void Collapse(const MeasurementResult& mr, State& state) const {
    auto f1 = [](unsigned n, unsigned m, uint64_t i,
                 uint64_t mask, uint64_t bits, const fp_type* p) -> double {
      auto s = p + 2 * i;
      return (i & mask) == bits ? s[0] * s[0] + s[1] * s[1] : 0;
    };

    using Op = std::plus<double>;
    double norm = Base::for_.RunReduce(MinSize(state.num_qubits()) / 2, f1,
                                       Op(), mr.mask, mr.bits, state.get());

    double renorm = 1.0 / std::sqrt(norm);

    auto f2 = [](unsigned n, unsigned m, uint64_t i,
                 uint64_t mask, uint64_t bits, fp_type renorm, fp_type* p) {
      auto s = p + 2 * i;
      bool not_zero = (i & mask) == bits;

      s[0] = not_zero ? s[0] * renorm : 0;
      s[1] = not_zero ? s[1] * renorm : 0;
    };

    Base::for_.Run(MinSize(state.num_qubits()) / 2, f2,
                   mr.mask, mr.bits, renorm, state.get());
  }

  std::vector<double> PartialNorms(const State& state) const {
    auto f = [](unsigned n, unsigned m, uint64_t i,
                const fp_type* p) -> double {
      auto s = p + 2 * i;
      return s[0] * s[0] + s[1] * s[1];
    };

    using Op = std::plus<double>;
    return Base::for_.RunReduceP(
        MinSize(state.num_qubits()) / 2, f, Op(), state.get());
  }

  uint64_t FindMeasuredBits(
      unsigned m, double r, uint64_t mask, const State& state) const {
    double csum = 0;

    uint64_t k0 = Base::for_.GetIndex0(MinSize(state.num_qubits()) / 2, m);
    uint64_t k1 = Base::for_.GetIndex1(MinSize(state.num_qubits()) / 2, m);

    const fp_type* p = state.get();

    for (uint64_t k = k0; k < k1; ++k) {
      auto re = p[2 * k];
      auto im = p[2 * k + 1];
      csum += re * re + im * im;
      if (r < csum) {
        return k & mask;
      }
    }

    // Return the last bitstring in the unlikely case of underflow.
    return (k1 - 1) & mask;
  }
};

}  // namespace qsim

#endif  // STATESPACE_RVV_H_

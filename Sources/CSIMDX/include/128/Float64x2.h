// Copyright 2019 Markus Winter
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

/// The underlying vector type of 2 Float64
typedef Float64 CXFloat64x2_t __attribute__((ext_vector_type(2)));
/// The 2 x 64-bit floating point storage
typedef union { CXFloat64x2_t rawValue; } Float64x2;

// MARK: - Make

/// Loads 2 x Float64 values from unaligned memory.
/// @return `(Float64x2){ pointer[0], pointer[1] }`
CX_INLINE Float64x2 CXFloat64x2MakeLoad(Float64 const pointer[2]) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vld1q_f64(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_loadu_pd(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Float64x2){ element0, element1 }`
CX_INLINE Float64x2 CXFloat64x2Make(Float64 element0, Float64 element1) {
    Float64x2 result;
    result.rawValue = (CXFloat64x2_t){ element0, element1 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Float64x2){ value, value }`
CX_INLINE Float64x2 CXFloat64x2MakeRepeatingElement(const Float64 value) {
    Float64x2 result;
    result.rawValue = (CXFloat64x2_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Float64 CXFloat64x2GetElement(const Float64x2 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXFloat64x2SetElement(Float64x2* storage, const int index, const Float64 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Float64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Minimum(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vminq_f64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_min_pd(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(Float64x2){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Float64x2 CXFloat64x2Maximum(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vmaxq_f64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_max_pd(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Absolute values are calculated via `fabs` if intrinsics are not available
#if CX_ARM_ARCH < CX_NEON_LEVEL_AARCH64 && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <math.h>
#endif

/// Returns the negated storage (element-wise).
/// @return `(Float64x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2Negate(const Float64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vnegq_f64(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sub_pd(_mm_setzero_pd(), operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = -operand.rawValue[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Float64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2Absolute(const Float64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vabsq_f64(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { __m128d operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    result.rawValue = _mm_and_pd(operand.rawValue, Signed.operand);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = fabs(operand.rawValue[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Float64x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Add(Float64x2 lhs, Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vaddq_f64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_add_pd(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] + rhs.rawValue[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Float64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Subtract(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vsubq_f64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sub_pd(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] - rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Float64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Multiply(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vmulq_f64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_mul_pd(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] * rhs.rawValue[i];
    }
#endif
    return result;
}

/// Divides two storages (element-wise).
/// @return `(Float64x2){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Divide(const Float64x2 lhs, Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vdivq_f64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_div_pd(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] / rhs.rawValue[i];
    }
#endif
    return result;
}

/// Calculates the square root (element-wise).
/// @return `(Float64x2){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2SquareRoot(const Float64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vsqrtq_f64(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sqrt_pd(operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = sqrt(operand.rawValue[i]);
    }
#endif
    return result;
}

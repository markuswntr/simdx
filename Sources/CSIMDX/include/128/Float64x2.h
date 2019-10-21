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

/// Absolute values are calculated via `fabs` if intrinsics are not available
#if CX_ARM_ARCH < CX_NEON_LEVEL_AARCH64 && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <math.h>
#endif

/// Convenience wrapper to a single intrinsic value return
#define RETURN_RESULT(value) { Float64x2 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 2 x 64-bit floating point storage
typedef union CXFloat64x2_t {

    /// The native intrinsic type
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    float64x2_t intrinsic;
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    __m128d intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    Float64 array CX_VECTOR_SIZE(16);
#else
    Float64 array[2];
#endif
} Float64x2;

// MARK: - Make

/// Loads 2 x Float64 values from unaligned memory.
/// @return `(Float64x2){ pointer[0], pointer[1] }`
CX_INLINE Float64x2 CXFloat64x2MakeLoad(Float64 const pointer[2]) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vld1q_f64(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_loadu_pd(pointer);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Float64x2){ element0, element1 }`
CX_INLINE Float64x2 CXFloat64x2Make(Float64 element0, Float64 element1) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setr_pd(element0, element1))
#else
    Float64 values[] = { element0, element1 };
    return CXFloat64x2MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Float64x2){ value, value }`
CX_INLINE Float64x2 CXFloat64x2MakeRepeatingElement(const Float64 value) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    RETURN_RESULT(vdupq_n_f64(value))
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_set1_pd(value))
#else
    return CXFloat64x2Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0.f).
/// @return `(Float64x2){ 0.f, 0.f }`
CX_INLINE Float64x2 CXFloat64x2MakeZero(void) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setzero_pd())
#else
    return CXFloat64x2MakeRepeatingElement(0.f);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Float64 CXFloat64x2GetElement(const Float64x2 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXFloat64x2SetElement(Float64x2* storage, const int index, const Float64 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Float64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Minimum(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vminq_f64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_min_pd(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(Float64x2){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Float64x2 CXFloat64x2Maximum(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vmaxq_f64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_max_pd(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(Float64x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2Negate(const Float64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vnegq_f64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sub_pd(_mm_setzero_pd(), operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = -operand.array[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Float64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2Absolute(const Float64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vabsq_f64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { __m128d operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    result.intrinsic = _mm_and_pd(operand.intrinsic, Signed.operand);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = fabs(operand.array[i]);
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
    result.intrinsic = vaddq_f64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_add_pd(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] + rhs.array[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Float64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Subtract(const Float64x2 lhs, const Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vsubq_f64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sub_pd(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] - rhs.array[i];
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
    result.intrinsic = vmulq_f64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_mul_pd(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] * rhs.array[i];
    }
#endif
    return result;
}

/// Divides two storages (element-wise).
/// @return `(Float64x2){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE Float64x2 CXFloat64x2Divide(const Float64x2 lhs, Float64x2 rhs) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vdivq_f64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_div_pd(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] / rhs.array[i];
    }
#endif
    return result;
}

/// Calculates the square root (element-wise).
/// @return `(Float64x2){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2SquareRoot(const Float64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vsqrtq_f64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sqrt_pd(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = sqrt(operand.array[i]);
    }
#endif
    return result;
}

#undef RETURN_RESULT
#undef SIZE

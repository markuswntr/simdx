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

/// Absolute values are calculated via `abs` if neither intrinsics are available
#if CX_ARM_ARCH < CX_NEON_LEVEL_AARCH64 && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <stdlib.h>
#endif

/// Convenience wrapper to a single intrinsic value return
#define RETURN_RESULT(value) { Int64x2 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 2 x 32-bit signed integer storage
typedef union CXInt64x2_t {

    /// The native intrinsic type
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    int64x2_t intrinsic;
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    __m128i intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    Int64 array CX_VECTOR_SIZE(16);
#else
    Int64 array[2];
#endif
} Int64x2;

// MARK: - Make

/// Loads 2 x Int64 values from unaligned memory.
/// @return `(Int64x2){ pointer[0], pointer[1] }`
CX_INLINE Int64x2 CXInt64x2MakeLoad(Int64 const pointer[2]) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vld1q_s64(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_loadu_si128((const __m128i*)pointer);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Int64x2){ element0, element1 }`
CX_INLINE Int64x2 CXInt64x2Make(Int64 element0, Int64 element1) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setr_epi64(_mm_cvtsi64_m64(element0), _mm_cvtsi64_m64(element1)))
#else
    Int64 values[] = { element0, element1 };
    return CXInt64x2MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Int64x2){ value, value }`
CX_INLINE Int64x2 CXInt64x2MakeRepeatingElement(const Int64 value) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    RETURN_RESULT(vdupq_n_s64(value))
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_set1_epi64(_mm_cvtsi64_m64(value)))
#else
    return CXInt64x2Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0).
/// @return `(Int64x2){ 0, 0 }`
CX_INLINE Int64x2 CXInt64x2MakeZero(void) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setzero_si128())
#else
    return CXInt64x2MakeRepeatingElement(0);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int64 CXInt64x2GetElement(const Int64x2 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt64x2SetElement(Int64x2* storage, const int index, const Int64 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Minimum(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_min_epi64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt64x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int64x2 CXInt64x2Maximum(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_max_epi64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(Int64x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2Negate(const Int64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vnegq_s64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sub_epi64(_mm_setzero_si128(), operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = -operand.array[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2Absolute(const Int64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vabsq_s64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    uint64_t SIGN_BIT = (uint64_t)(~((1ULL) << 63));
    __m128i signs = _mm_setr_epi64(_mm_cvtsi64_m64(SIGN_BIT), _mm_cvtsi64_m64(SIGN_BIT));
    result.intrinsic = _mm_and_si128(operand.intrinsic, signs);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = abs(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Int64x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Add(Int64x2 lhs, Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vaddq_s64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_add_epi64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] + rhs.array[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Int64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Subtract(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vsubq_s64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sub_epi64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] - rhs.array[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Int64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Multiply(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] * rhs.array[i];
    }
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE Int64x2 CXInt64x2BitwiseNot(const Int64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = veorq_s64(operand.intrinsic, vdupq_n_s64(-1LL));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_xor_si128(operand.intrinsic, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = ~operand.array[i]
    }
#endif
    return result;
}

/// Bitwise And
CX_INLINE Int64x2 CXInt64x2BitwiseAnd(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vandq_s64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_and_si128(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] & rhs.array[i];
    }
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE Int64x2 CXInt64x2BitwiseAndNot(const Int64x2 lhs, const Int64x2 rhs) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_andnot_si128(lhs.intrinsic, rhs.intrinsic))
#else
    return CXInt64x2BitwiseAnd(CXInt64x2BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE Int64x2 CXInt64x2BitwiseOr(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vorrq_s64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_or_si128(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] | rhs.array[i];
    }
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE Int64x2 CXInt64x2BitwiseExclusiveOr(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = veorq_s64(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_xor_si128(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] ^ rhs.array[i];
    }
#endif
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int64x2 CXInt64x2ShiftElementWiseLeft(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vshlq_s64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs.array[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int64x2 CXInt64x2ShiftLeft(const Int64x2 lhs, const Int64 rhs) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    return CXInt64x2ShiftElementWiseLeft(lhs, CXInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_slli_epi64(lhs.intrinsic, rhs))
#else
    Int64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs;
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int64x2 CXInt64x2ShiftElementWiseRight(const Int64x2 lhs, const Int64x2 rhs)
{
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    return CXInt64x2ShiftElementWiseLeft(lhs, CXInt64x2Negate(rhs));
#else
    Int64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs.array[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int64x2 CXInt64x2ShiftRight(const Int64x2 lhs, const Int64 rhs)
{
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    return CXInt64x2ShiftElementWiseLeft(lhs, CXInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_srli_epi64(lhs.intrinsic, rhs))
#else
    Int64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs;
    }
    return result;
#endif
}

#undef RETURN_RESULT
#undef SIZE

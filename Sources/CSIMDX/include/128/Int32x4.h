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
#if !CX_ARM_ARCH && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <stdlib.h>
#endif

/// Convenience wrapper to a single intrinsic value return
#define RETURN_RESULT(value) { Int32x4 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 4 x 32-bit signed integer storage
typedef union CXInt32x4_t {

    /// The native intrinsic type
#if CX_ARM_ARCH
    int32x4_t intrinsic;
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    __m128i intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    Int32 array CX_VECTOR_SIZE(16);
#else
    Int32 array[4];
#endif
} Int32x4;

// MARK: - Make

/// Loads 4 x Int32 values from unaligned memory.
/// @return `(Int32x4){ pointer[0], pointer[1], ... }`
CX_INLINE Int32x4 CXInt32x4MakeLoad(Int32 const pointer[4]) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vld1q_s32(pointer);
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
/// @return `(Int32x4){ element0, element1, ... }`
CX_INLINE Int32x4 CXInt32x4Make(Int32 element0, Int32 element1, Int32 element2, Int32 element3) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setr_epi32(element0, element1, element2, element3))
#else
    Int32 values[] = { element0, element1, element2, element3 };
    return CXInt32x4MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Int32x4){ value, value, ... }`
CX_INLINE Int32x4 CXInt32x4MakeRepeatingElement(const Int32 value) {
#if CX_ARM_ARCH
    RETURN_RESULT(vdupq_n_s32(value))
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_set1_epi32(value))
#else
    return CXInt32x4Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0).
/// @return `(Int32x4){ 0, 0, ... }`
CX_INLINE Int32x4 CXInt32x4MakeZero(void) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setzero_si128())
#else
    return CXInt32x4MakeRepeatingElement(0);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int32 CXInt32x4GetElement(const Int32x4 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt32x4SetElement(Int32x4* storage, const int index, const Int32 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Minimum(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vminq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_min_epi32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Maximum(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vmaxq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_max_epi32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(Int32x4){ -(operand[0]), -(operand[1]), ... }`
CX_INLINE Int32x4 CXInt32x4Negate(const Int32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vnegq_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sub_epi32(_mm_setzero_si128(), operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = -operand.array[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int32x4){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE Int32x4 CXInt32x4Absolute(const Int32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vabsq_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSSE3 // Native abs on x86 has been added in SSSE3 ...
    result.intrinsic = _mm_abs_epi32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2 // ... while the type was available since SSE2
    // This is a fallback to systems not capable of SSSE3 intrinsics
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    __m128i signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
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
/// @return `(Int32x4){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Add(Int32x4 lhs, Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vaddq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_add_epi32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] + rhs.array[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Int32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Subtract(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vsubq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_sub_epi32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] - rhs.array[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Int32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Multiply(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vmulq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    // X86 intrinsics do not have a native multiply operation for 32bit ints
    __m128i temp0 = _mm_mul_epu32(lhs.intrinsic, rhs.intrinsic);
    __m128i temp1 = _mm_mul_epu32(_mm_srli_si128(lhs.intrinsic, 4), _mm_srli_si128(rhs.intrinsic, 4));
    result.intrinsic = _mm_unpacklo_epi32(
        _mm_shuffle_epi32(temp0, _MM_SHUFFLE(0, 0, 2, 0)),
        _mm_shuffle_epi32(temp1, _MM_SHUFFLE(0, 0, 2, 0))
    );
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] * rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE Int32x4 CXInt32x4BitwiseNot(const Int32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vmvnq_s32(operand.intrinsic);
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
CX_INLINE Int32x4 CXInt32x4BitwiseAnd(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vandq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_and_si128(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] & rhs.array[i];
    }
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE Int32x4 CXInt32x4BitwiseAndNot(const Int32x4 lhs, const Int32x4 rhs) {
#if CX_X86_ARCH
    RETURN_RESULT(_mm_andnot_si128(lhs.intrinsic, rhs.intrinsic))
#else
    return CXInt32x4BitwiseAnd(CXInt32x4BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE Int32x4 CXInt32x4BitwiseOr(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vorrq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_or_si128(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] | rhs.array[i];
    }
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE Int32x4 CXInt32x4BitwiseExclusiveOr(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = veorq_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
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
CX_INLINE Int32x4 CXInt32x4ShiftElementWiseLeft(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vshlq_s32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs.array[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x4 CXInt32x4ShiftLeft(const Int32x4 lhs, const Int32 rhs) {
#if CX_ARM_ARCH
    return CXInt32x4ShiftElementWiseLeft(lhs, CXInt32x4MakeRepeatingElement(rhs));
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_slli_epi32(lhs.intrinsic, rhs))
#else
    Int32x4 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs;
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x4 CXInt32x4ShiftElementWiseRight(const Int32x4 lhs, const Int32x4 rhs)
{
#if CX_ARM_ARCH
    return CXInt32x4ShiftElementWiseLeft(lhs, CXInt32x4Negate(rhs));
#else
    Int32x4 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs.array[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x4 CXInt32x4ShiftRight(const Int32x4 lhs, const Int32 rhs)
{
#if CX_ARM_ARCH
    return CXInt32x4ShiftElementWiseLeft(lhs, CXInt32x4MakeRepeatingElement(rhs));
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_srli_epi32(lhs.intrinsic, rhs))
#else
    Int32x4 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs;
    }
    return result;
#endif
}

#undef RETURN_RESULT
#undef SIZE

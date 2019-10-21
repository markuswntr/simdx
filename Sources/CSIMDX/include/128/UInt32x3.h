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
#define RETURN_RESULT(value) { UInt32x3 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 3 x 32-bit unsigned integer storage
typedef union CXUInt32x3_t {

    /// The native intrinsic type
#if CX_ARM_ARCH
    uint32x4_t intrinsic;
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    __m128i intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    UInt32 array CX_VECTOR_SIZE(12);
#else
    UInt32 array[3];
#endif
} UInt32x3;

// MARK: - Make

/// Loads 3 x UInt32 values from unaligned memory.
/// @return `(UInt32x3){ pointer[0], pointer[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3MakeLoad(UInt32 const pointer[3]) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vld1q_u32(pointer);
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
/// @return `(UInt32x3){ element0, element1, ... }`
CX_INLINE UInt32x3 CXUInt32x3Make(UInt32 element0, UInt32 element1, UInt32 element2) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setr_epi32(element0, element1, element2, 0))
#else
    UInt32 values[] = { element0, element1, element2 };
    return CXUInt32x3MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(UInt32x3){ value, value, ... }`
CX_INLINE UInt32x3 CXUInt32x3MakeRepeatingElement(const UInt32 value) {
#if CX_ARM_ARCH
    RETURN_RESULT(vdupq_n_u32(value))
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_set1_epi32(value))
#else
    return CXUInt32x3Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0).
/// @return `(UInt32x3){ 0, 0, ... }`
CX_INLINE UInt32x3 CXUInt32x3MakeZero(void) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setzero_si128())
#else
    return CXUInt32x3MakeRepeatingElement(0);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE UInt32 CXUInt32x3GetElement(const UInt32x3 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXUInt32x3SetElement(UInt32x3* storage, const int index, const UInt32 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(UInt32x3){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Minimum(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vminq_u32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_min_epu32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Maximum(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vmaxq_u32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_max_epu32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(UInt32x3){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE UInt32x3 CXUInt32x3Absolute(const UInt32x3 operand) {
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(UInt32x3){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Add(UInt32x3 lhs, UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vaddq_u32(lhs.intrinsic, rhs.intrinsic);
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
/// @return `(UInt32x3){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Subtract(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vsubq_u32(lhs.intrinsic, rhs.intrinsic);
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
/// @return `(UInt32x3){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Multiply(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vmulq_u32(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt32x3 CXUInt32x3BitwiseNot(const UInt32x3 operand) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vmvnq_u32(operand.intrinsic);
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
CX_INLINE UInt32x3 CXUInt32x3BitwiseAnd(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vandq_u32(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt32x3 CXUInt32x3BitwiseAndNot(const UInt32x3 lhs, const UInt32x3 rhs) {
#if CX_X86_ARCH
    RETURN_RESULT(_mm_andnot_si128(lhs.intrinsic, rhs.intrinsic))
#else
    return CXUInt32x3BitwiseAnd(CXUInt32x3BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE UInt32x3 CXUInt32x3BitwiseOr(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vorrq_u32(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt32x3 CXUInt32x3BitwiseExclusiveOr(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = veorq_u32(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt32x3 CXUInt32x3ShiftElementWiseLeft(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vshlq_u32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs.array[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x3 CXUInt32x3ShiftLeft(const UInt32x3 lhs, const UInt32 rhs) {
#if CX_ARM_ARCH
    return CXUInt32x3ShiftElementWiseLeft(lhs, CXUInt32x3MakeRepeatingElement(rhs));
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_slli_epi32(lhs.intrinsic, rhs))
#else
    UInt32x3 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs;
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt32x3 CXUInt32x3ShiftElementWiseRight(const UInt32x3 lhs, const UInt32x3 rhs)
{
#if CX_ARM_ARCH
    UInt32x3 rhsNegated;
    rhsNegated.intrinsic = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(rhs.intrinsic)));
    return CXUInt32x3ShiftElementWiseLeft(lhs, rhsNegated);
#else
    UInt32x3 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs.array[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x3 CXUInt32x3ShiftRight(const UInt32x3 lhs, const UInt32 rhs)
{
#if CX_ARM_ARCH
    return CXUInt32x3ShiftElementWiseLeft(lhs, CXUInt32x3MakeRepeatingElement(rhs));
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_srli_epi32(lhs.intrinsic, rhs))
#else
    UInt32x3 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs;
    }
    return result;
#endif
}

#undef RETURN_RESULT
#undef SIZE

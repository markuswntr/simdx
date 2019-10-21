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

/// Convenience wrapper to a single intrinsic value return
#define RETURN_RESULT(value) { UInt64x2 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 2 x 32-bit signed integer storage
typedef union CXUInt64x2_t {

    /// The native intrinsic type
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    uint64x2_t intrinsic;
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    __m128i intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    UInt64 array CX_VECTOR_SIZE(16);
#else
    UInt64 array[2];
#endif
} UInt64x2;

// MARK: - Make

/// Loads 2 x UInt64 values from unaligned memory.
/// @return `(UInt64x2){ pointer[0], pointer[1] }`
CX_INLINE UInt64x2 CXUInt64x2MakeLoad(UInt64 const pointer[2]) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vld1q_u64(pointer);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(UInt64x2){ element0, element1 }`
CX_INLINE UInt64x2 CXUInt64x2Make(UInt64 element0, UInt64 element1) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setr_epi64(_mm_cvtsi64_m64(element0), _mm_cvtsi64_m64(element1)))
#else
    UInt64 values[] = { element0, element1 };
    return CXUInt64x2MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(UInt64x2){ value, value }`
CX_INLINE UInt64x2 CXUInt64x2MakeRepeatingElement(const UInt64 value) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    RETURN_RESULT(vdupq_n_u64(value))
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_set1_epi64(_mm_cvtsi64_m64(value)))
#else
    return CXUInt64x2Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0).
/// @return `(UInt64x2){ 0, 0 }`
CX_INLINE UInt64x2 CXUInt64x2MakeZero(void) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_setzero_si128())
#else
    return CXUInt64x2MakeRepeatingElement(0);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE UInt64 CXUInt64x2GetElement(const UInt64x2 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXUInt64x2SetElement(UInt64x2* storage, const int index, const UInt64 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Minimum(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_min_epu64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt64x2 CXUInt64x2Maximum(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_max_epu64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return As this is an unsigned integer it returns the storage unchanged.
CX_INLINE UInt64x2 CXUInt64x2Absolute(const UInt64x2 operand) {
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(UInt64x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Add(UInt64x2 lhs, UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vaddq_u64(lhs.intrinsic, rhs.intrinsic);
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
/// @return `(UInt64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Subtract(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vsubq_u64(lhs.intrinsic, rhs.intrinsic);
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
/// @return `(UInt64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Multiply(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] * rhs.array[i];
    }
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE UInt64x2 CXUInt64x2BitwiseNot(const UInt64x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = veorq_u64(operand.intrinsic, vdupq_n_u64(-1LL));
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
CX_INLINE UInt64x2 CXUInt64x2BitwiseAnd(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vandq_u64(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt64x2 CXUInt64x2BitwiseAndNot(const UInt64x2 lhs, const UInt64x2 rhs) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_andnot_si128(lhs.intrinsic, rhs.intrinsic))
#else
    return CXUInt64x2BitwiseAnd(CXUInt64x2BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE UInt64x2 CXUInt64x2BitwiseOr(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vorrq_u64(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt64x2 CXUInt64x2BitwiseExclusiveOr(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = veorq_u64(lhs.intrinsic, rhs.intrinsic);
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
CX_INLINE UInt64x2 CXUInt64x2ShiftElementWiseLeft(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vshlq_u64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs.array[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt64x2 CXUInt64x2ShiftLeft(const UInt64x2 lhs, const UInt64 rhs) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    return CXUInt64x2ShiftElementWiseLeft(lhs, CXUInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_slli_epi64(lhs.intrinsic, rhs))
#else
    UInt64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs;
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt64x2 CXUInt64x2ShiftElementWiseRight(const UInt64x2 lhs, const UInt64x2 rhs)
{
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    UInt64x2 rhsNegated;
    rhsNegated.intrinsic = vreinterpretq_u64_s64(vnegq_s64(vreinterpretq_s64_u64(rhs.intrinsic)));
    return CXUInt64x2ShiftElementWiseLeft(lhs, rhsNegated);
#else
    UInt64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs.array[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt64x2 CXUInt64x2ShiftRight(const UInt64x2 lhs, const UInt64 rhs)
{
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    return CXUInt64x2ShiftElementWiseLeft(lhs, CXUInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    RETURN_RESULT(_mm_srli_epi64(lhs.intrinsic, rhs))
#else
    UInt64x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs;
    }
    return result;
#endif
}

#undef RETURN_RESULT
#undef SIZE

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

/// The underlying vector type of 4 x UInt32
typedef UInt32 CXUInt32x4_t __attribute__((ext_vector_type(4)));
/// The 4 x 32-bit signed integer storage
typedef union { CXUInt32x4_t rawValue; } UInt32x4;

// MARK: - Make

/// Loads 4 x UInt32 values from unaligned memory.
/// @return `(UInt32x4){ pointer[0], pointer[1], ... }`
CX_INLINE UInt32x4 CXUInt32x4MakeLoad(UInt32 const pointer[4]) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vld1q_u32(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_loadu_si128((const __m128i*)pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(UInt32x4){ element0, element1, ... }`
CX_INLINE UInt32x4 CXUInt32x4Make(UInt32 element0, UInt32 element1, UInt32 element2, UInt32 element3) {
    UInt32x4 result;
    result.rawValue = (CXUInt32x4_t){ element0, element1, element2 };
    return result;
}

/// Returns a storage type with all elements initialized to `value`.
/// @return `(UInt32x4){ value, value, ... }`
CX_INLINE UInt32x4 CXUInt32x4MakeRepeatingElement(const UInt32 value) {
    UInt32x4 result;
    result.rawValue = (CXUInt32x4_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE UInt32 CXUInt32x4GetElement(const UInt32x4 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXUInt32x4SetElement(UInt32x4* storage, const int index, const UInt32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(UInt32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt32x4 CXUInt32x4Minimum(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vminq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.rawValue = _mm_min_epu32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt32x4 CXUInt32x4Maximum(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmaxq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.rawValue = _mm_max_epu32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(UInt32x4){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE UInt32x4 CXUInt32x4Absolute(const UInt32x4 operand) {
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(UInt32x4){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE UInt32x4 CXUInt32x4Add(UInt32x4 lhs, UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vaddq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_add_epi32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] + rhs.rawValue[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(UInt32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE UInt32x4 CXUInt32x4Subtract(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vsubq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sub_epi32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] - rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(UInt32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE UInt32x4 CXUInt32x4Multiply(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmulq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    // X86 intrinsics do not have a native multiply operation for 32bit ints
    __m128i temp0 = _mm_mul_epu32(lhs.rawValue, rhs.rawValue);
    __m128i temp1 = _mm_mul_epu32(_mm_srli_si128(lhs.rawValue, 4), _mm_srli_si128(rhs.rawValue, 4));
    result.rawValue = _mm_unpacklo_epi32(
        _mm_shuffle_epi32(temp0, _MM_SHUFFLE(0, 0, 2, 0)),
        _mm_shuffle_epi32(temp1, _MM_SHUFFLE(0, 0, 2, 0))
    );
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] * rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE UInt32x4 CXUInt32x4BitwiseNot(const UInt32x4 operand) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmvnq_u32(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_xor_si128(operand.rawValue, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = ~operand.rawValue[i]
    }
#endif
    return result;
}

/// Bitwise And
CX_INLINE UInt32x4 CXUInt32x4BitwiseAnd(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vandq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH
    result.rawValue = _mm_and_si128(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] & rhs.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE UInt32x4 CXUInt32x4BitwiseAndNot(const UInt32x4 lhs, const UInt32x4 rhs) {
    return CXUInt32x4BitwiseAnd(CXUInt32x4BitwiseNot(lhs), rhs);
}

/// Bitwise Or
CX_INLINE UInt32x4 CXUInt32x4BitwiseOr(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vorrq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH
    result.rawValue = _mm_or_si128(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] | rhs.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE UInt32x4 CXUInt32x4BitwiseExclusiveOr(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = veorq_u32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH
    result.rawValue = _mm_xor_si128(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] ^ rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt32x4 CXUInt32x4ShiftElementWiseLeft(const UInt32x4 lhs, const UInt32x4 rhs) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vshlq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x4 CXUInt32x4ShiftLeft(const UInt32x4 lhs, const UInt32 rhs) {
    UInt32x4 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt32x4 CXUInt32x4ShiftElementWiseRight(const UInt32x4 lhs, const UInt32x4 rhs) {
#if CX_ARM_ARCH
    UInt32x4 rhsNegated;
    rhsNegated.rawValue = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(rhs.rawValue)));
    return CXUInt32x4ShiftElementWiseLeft(lhs, rhsNegated);
#else
    UInt32x4 result;
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x4 CXUInt32x4ShiftRight(const UInt32x4 lhs, const UInt32 rhs) {
    UInt32x4 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

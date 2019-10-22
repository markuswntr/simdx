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

/// The underlying vector type of 4 x Int32
typedef Int32 CXInt32x4_t __attribute__((ext_vector_type(4)));
/// The 4 x 32-bit signed integer storage
typedef union { CXInt32x4_t rawValue; } Int32x4;

// MARK: - Make

/// Loads 4 x Int32 values from unaligned memory.
/// @return `(Int32x4){ pointer[0], pointer[1], ... }`
CX_INLINE Int32x4 CXInt32x4MakeLoad(Int32 const pointer[4]) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vld1q_s32(pointer);
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
/// @return `(Int32x4){ element0, element1, ... }`
CX_INLINE Int32x4 CXInt32x4Make(Int32 element0, Int32 element1, Int32 element2, Int32 element3) {
    Int32x4 result;
    result.rawValue = (CXInt32x4_t){ element0, element1, element2 };
    return result;
}

/// Returns an rawValue type with all elements initialized to `value`.
/// @return `(Int32x4){ value, value, ... }`
CX_INLINE Int32x4 CXInt32x4MakeRepeatingElement(const Int32 value) {
    Int32x4 result;
    result.rawValue = (CXInt32x4_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int32 CXInt32x4GetElement(const Int32x4 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt32x4SetElement(Int32x4* storage, const int index, const Int32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Minimum(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vminq_s32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.rawValue = _mm_min_epi32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Maximum(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmaxq_s32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.rawValue = _mm_max_epi32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Absolute values are calculated via `abs` if neither rawValues are available
#if !CX_ARM_ARCH && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <stdlib.h>
#endif

/// Returns the negated storage (element-wise).
/// @return `(Int32x4){ -(operand[0]), -(operand[1]), ... }`
CX_INLINE Int32x4 CXInt32x4Negate(const Int32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vnegq_s32(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sub_epi32(_mm_setzero_si128(), operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = -operand.rawValue[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int32x4){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE Int32x4 CXInt32x4Absolute(const Int32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vabsq_s32(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSSE3 // Native abs on x86 has been added in SSSE3 ...
    result.rawValue = _mm_abs_epi32(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2 // ... while the type was available since SSE2
    // This is a fallback to systems not capable of SSSE3 rawValues
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    __m128i signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    result.rawValue = _mm_and_si128(operand.rawValue, signs);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = abs(operand.rawValue[i]);
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
    result.rawValue = vaddq_s32(lhs.rawValue, rhs.rawValue);
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
/// @return `(Int32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Subtract(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vsubq_s32(lhs.rawValue, rhs.rawValue);
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
/// @return `(Int32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE Int32x4 CXInt32x4Multiply(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmulq_s32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    // X86 rawValues do not have a native multiply operation for 32bit ints
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
CX_INLINE Int32x4 CXInt32x4BitwiseNot(const Int32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmvnq_s32(operand.rawValue);
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
CX_INLINE Int32x4 CXInt32x4BitwiseAnd(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vandq_s32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE Int32x4 CXInt32x4BitwiseAndNot(const Int32x4 lhs, const Int32x4 rhs) {
    return CXInt32x4BitwiseAnd(CXInt32x4BitwiseNot(lhs), rhs);
}

/// Bitwise Or
CX_INLINE Int32x4 CXInt32x4BitwiseOr(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vorrq_s32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE Int32x4 CXInt32x4BitwiseExclusiveOr(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = veorq_s32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE Int32x4 CXInt32x4ShiftElementWiseLeft(const Int32x4 lhs, const Int32x4 rhs) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vshlq_s32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x4 CXInt32x4ShiftLeft(const Int32x4 lhs, const Int32 rhs) {
    Int32x4 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x4 CXInt32x4ShiftElementWiseRight(const Int32x4 lhs, const Int32x4 rhs) {
#if CX_ARM_ARCH
    return CXInt32x4ShiftElementWiseLeft(lhs, CXInt32x4Negate(rhs));
#else
    Int32x4 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x4 CXInt32x4ShiftRight(const Int32x4 lhs, const Int32 rhs) {
    Int32x4 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

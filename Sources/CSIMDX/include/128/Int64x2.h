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

/// The underlying vector type of 2 x Int64
typedef Int64 CXInt64x2_t __attribute__((ext_vector_type(2)));
/// The 2 x 64-bit signed integer storage
typedef union { CXInt64x2_t rawValue; } Int64x2;

// MARK: - Make

/// Loads 2 x Int64 values from unaligned memory.
/// @return `(Int64x2){ pointer[0], pointer[1] }`
CX_INLINE Int64x2 CXInt64x2MakeLoad(Int64 const pointer[2]) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vld1q_s64(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_loadu_si128((const __m128i*)pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Int64x2){ element0, element1 }`
CX_INLINE Int64x2 CXInt64x2Make(Int64 element0, Int64 element1) {
    Int64x2 result;
    result.rawValue = (CXInt64x2_t){ element0, element1 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Int64x2){ value, value }`
CX_INLINE Int64x2 CXInt64x2MakeRepeatingElement(const Int64 value) {
    Int64x2 result;
    result.rawValue = (CXInt64x2_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int64 CXInt64x2GetElement(const Int64x2 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt64x2SetElement(Int64x2* storage, const int index, const Int64 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Minimum(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.rawValue = _mm_min_epi64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt64x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int64x2 CXInt64x2Maximum(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.rawValue = _mm_max_epi64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Absolute values are calculated via `abs` if neither intrinsics are available
#if CX_ARM_ARCH < CX_NEON_LEVEL_AARCH64 && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <stdlib.h>
#endif

/// Returns the negated storage (element-wise).
/// @return `(Int64x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2Negate(const Int64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vnegq_s64(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sub_epi64(_mm_setzero_si128(), operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = -operand.rawValue[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2Absolute(const Int64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vabsq_s64(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    uint64_t SIGN_BIT = (uint64_t)(~((1ULL) << 63));
    __m128i signs = _mm_setr_epi64(_mm_cvtsi64_m64(SIGN_BIT), _mm_cvtsi64_m64(SIGN_BIT));
    result.rawValue = _mm_and_si128(operand.rawValue, signs);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = abs(operand.rawValue[i]);
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
    result.rawValue = vaddq_s64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_add_epi64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] + rhs.rawValue[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Int64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Subtract(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vsubq_s64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_sub_epi64(lhs.rawValue, rhs.rawValue);
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
/// @return `(Int64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Int64x2 CXInt64x2Multiply(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] * rhs.rawValue[i];
    }
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE Int64x2 CXInt64x2BitwiseNot(const Int64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = veorq_s64(operand.rawValue, vdupq_n_s64(-1LL));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_xor_si128(operand.rawValue, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = ~operand.rawValue[i]
    }
#endif
    return result;
}

/// Bitwise And
CX_INLINE Int64x2 CXInt64x2BitwiseAnd(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vandq_s64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_and_si128(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] & rhs.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE Int64x2 CXInt64x2BitwiseAndNot(const Int64x2 lhs, const Int64x2 rhs) {
    return CXInt64x2BitwiseAnd(CXInt64x2BitwiseNot(lhs), rhs);
}

/// Bitwise Or
CX_INLINE Int64x2 CXInt64x2BitwiseOr(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vorrq_s64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_or_si128(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] | rhs.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE Int64x2 CXInt64x2BitwiseExclusiveOr(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = veorq_s64(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.rawValue = _mm_xor_si128(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] ^ rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int64x2 CXInt64x2ShiftElementWiseLeft(const Int64x2 lhs, const Int64x2 rhs) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vshlq_s64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int64x2 CXInt64x2ShiftLeft(const Int64x2 lhs, const Int64 rhs) {
    Int64x2 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int64x2 CXInt64x2ShiftElementWiseRight(const Int64x2 lhs, const Int64x2 rhs) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    return CXInt64x2ShiftElementWiseLeft(lhs, CXInt64x2Negate(rhs));
#else
    Int64x2 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int64x2 CXInt64x2ShiftRight(const Int64x2 lhs, const Int64 rhs) {
    Int64x2 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

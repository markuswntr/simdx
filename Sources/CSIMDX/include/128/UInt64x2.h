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

/// The underlying vector type of 2 x UInt64
typedef UInt64 CXUInt64x2_t __attribute__((ext_vector_type(2)));
/// The 2 x 64-bit signed integer storage
typedef union { CXUInt64x2_t rawValue; } UInt64x2;

// MARK: - Make

/// Loads 2 x UInt64 values from unaligned memory.
/// @return `(UInt64x2){ pointer[0], pointer[1] }`
CX_INLINE UInt64x2 CXUInt64x2MakeLoad(UInt64 const pointer[2]) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vld1q_u64(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(UInt64x2){ element0, element1 }`
CX_INLINE UInt64x2 CXUInt64x2Make(UInt64 element0, UInt64 element1) {
    UInt64x2 result;
    result.rawValue = (CXUInt64x2_t){ element0, element1 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(UInt64x2){ value, value }`
CX_INLINE UInt64x2 CXUInt64x2MakeRepeatingElement(const UInt64 value) {
    UInt64x2 result;
    result.rawValue = (CXUInt64x2_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE UInt64 CXUInt64x2GetElement(const UInt64x2 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXUInt64x2SetElement(UInt64x2* storage, const int index, const UInt64 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Minimum(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.rawValue = _mm_min_epu64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt64x2 CXUInt64x2Maximum(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.rawValue = _mm_max_epu64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
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
    result.rawValue = vaddq_u64(lhs.rawValue, rhs.rawValue);
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
/// @return `(UInt64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Subtract(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vsubq_u64(lhs.rawValue, rhs.rawValue);
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
/// @return `(UInt64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE UInt64x2 CXUInt64x2Multiply(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] * rhs.rawValue[i];
    }
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE UInt64x2 CXUInt64x2BitwiseNot(const UInt64x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = veorq_u64(operand.rawValue, vdupq_n_u64(-1LL));
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
CX_INLINE UInt64x2 CXUInt64x2BitwiseAnd(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vandq_u64(lhs.rawValue, rhs.rawValue);
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
CX_INLINE UInt64x2 CXUInt64x2BitwiseAndNot(const UInt64x2 lhs, const UInt64x2 rhs) {
    return CXUInt64x2BitwiseAnd(CXUInt64x2BitwiseNot(lhs), rhs);
}

/// Bitwise Or
CX_INLINE UInt64x2 CXUInt64x2BitwiseOr(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vorrq_u64(lhs.rawValue, rhs.rawValue);
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
CX_INLINE UInt64x2 CXUInt64x2BitwiseExclusiveOr(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = veorq_u64(lhs.rawValue, rhs.rawValue);
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
CX_INLINE UInt64x2 CXUInt64x2ShiftElementWiseLeft(const UInt64x2 lhs, const UInt64x2 rhs) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.rawValue = vshlq_u64(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt64x2 CXUInt64x2ShiftLeft(const UInt64x2 lhs, const UInt64 rhs) {
    UInt64x2 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt64x2 CXUInt64x2ShiftElementWiseRight(const UInt64x2 lhs, const UInt64x2 rhs) {
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    UInt64x2 rhsNegated;
    rhsNegated.rawValue = vreinterpretq_u64_s64(vnegq_s64(vreinterpretq_s64_u64(rhs.rawValue)));
    return CXUInt64x2ShiftElementWiseLeft(lhs, rhsNegated);
#else
    UInt64x2 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt64x2 CXUInt64x2ShiftRight(const UInt64x2 lhs, const UInt64 rhs) {
    UInt64x2 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

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

/// Absolute values are calculated via `fabsf` if intrinsics are not available (i.e. mmx or lower only)
#if !CX_ARM_ARCH && CX_X86_ARCH <= CX_X86_LEVEL_MMX
#   include <math.h>
#endif

/// The underlying vector type of 4 x Float32
typedef Float32 CXFloat32x4_t __attribute__((ext_vector_type(4)));
/// The 4 x 32-bit floating point storage
typedef union { CXFloat32x4_t rawValue; } Float32x4;

// MARK: - Make

/// Loads elements from an unaligned memory pointer.
/// @return `(Float32x4){ pointer[0], pointer[1], ... }`
CX_INLINE Float32x4 CXFloat32x4MakeLoad(Float32 const pointer[4]) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vld1q_f32(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.rawValue = _mm_loadu_ps(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Float32x4){ element0, element1, ... }`
CX_INLINE Float32x4 CXFloat32x4Make(Float32 element0, Float32 element1, Float32 element2, Float32 element3) {
    Float32x4 result;
    result.rawValue = (CXFloat32x4_t){ element0, element1, element2, element3 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Float32x4){ value, value, ... }`
CX_INLINE Float32x4 CXFloat32x4MakeRepeatingElement(const Float32 value) {
    Float32x4 result;
    result.rawValue = (CXFloat32x4_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Float32 CXFloat32x4GetElement(const Float32x4 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXFloat32x4SetElement(Float32x4* storage, const int index, const Float32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Float32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Float32x4 CXFloat32x4Minimum(const Float32x4 lhs, const Float32x4 rhs) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vminq_f32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.rawValue = _mm_min_ps(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Float32x4 CXFloat32x4Maximum(const Float32x4 lhs, const Float32x4 rhs) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vmaxq_f32(lhs.rawValue, rhs.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.rawValue = _mm_max_ps(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(Float32x4){ -(operand[0]), -(operand[1]), ... }`
CX_INLINE Float32x4 CXFloat32x4Negate(const Float32x4 operand) {
    Float32x4 result;
    result.rawValue = -(operand.rawValue);
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Float32x4){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE Float32x4 CXFloat32x4Absolute(const Float32x4 operand) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vabsq_f32(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { __m128 operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    result.rawValue = _mm_and_ps(operand.rawValue, Signed.operand);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = fabsf(operand.rawValue[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Float32x4){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE Float32x4 CXFloat32x4Add(Float32x4 lhs, Float32x4 rhs) {
    Float32x4 result;
    result.rawValue = lhs.rawValue + lhs.rawValue;
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Float32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE Float32x4 CXFloat32x4Subtract(const Float32x4 lhs, const Float32x4 rhs) {
    Float32x4 result;
    result.rawValue = lhs.rawValue - lhs.rawValue;
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Float32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE Float32x4 CXFloat32x4Multiply(const Float32x4 lhs, const Float32x4 rhs) {
    Float32x4 result;
    result.rawValue = lhs.rawValue * lhs.rawValue;
    return result;
}

/// Divides two storages (element-wise).
/// @return `(Float32x4){ lhs[0] / rhs[0], lhs[1] / rhs[1], ... }`
CX_INLINE Float32x4 CXFloat32x4Divide(const Float32x4 lhs, Float32x4 rhs) {
    Float32x4 result;
    result.rawValue = lhs.rawValue / lhs.rawValue;
    return result;
}

/// Calculates the square root (element-wise).
/// @return `(Float32x4){ sqrt(operand[0]), sqrt(operand[1]), ... }`
CX_INLINE Float32x4 CXFloat32x4SquareRoot(const Float32x4 operand) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.rawValue = vsqrtq_f32(operand.rawValue);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.rawValue = _mm_sqrt_ps(operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 4; i++) {
        result.rawValue[i] = sqrtf(operand.rawValue[i]);
    }
#endif
    return result;
}

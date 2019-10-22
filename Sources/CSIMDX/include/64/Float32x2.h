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

/// The underlying vector type of 2 Float32
typedef Float32 CXFloat32x2_t __attribute__((ext_vector_type(2)));
/// The 2 x 32-bit floating point storage
typedef union { CXFloat32x2_t rawValue; } Float32x2;

// MARK: - Make

/// Loads 2 x Float32 values from unaligned memory.
/// @return `(Float32x2){ pointer[0], pointer[1] }`
CX_INLINE Float32x2 CXFloat32x2MakeLoad(Float32 const pointer[2]) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vld1_f32(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Float32x2){ element0, element1 }`
CX_INLINE Float32x2 CXFloat32x2Make(Float32 element0, Float32 element1) {
    Float32x2 result;
    result.rawValue = (CXFloat32x2_t){ element0, element1 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Float32x2){ value, value }`
CX_INLINE Float32x2 CXFloat32x2MakeRepeatingElement(const Float32 value) {
    Float32x2 result;
    result.rawValue = (CXFloat32x2_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Float32 CXFloat32x2GetElement(const Float32x2 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXFloat32x2SetElement(Float32x2* storage, const int index, const Float32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Float32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Minimum(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vmin_f32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Float32x2 CXFloat32x2Maximum(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vmax_f32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Absolute values are calculated via `fabsf` if intrinsics are not available (i.e. mmx or lower only)
#if !CX_ARM_ARCH && CX_X86_ARCH
#   include <math.h>
#endif

/// Returns the negated storage (element-wise).
/// @return `(Float32x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2Negate(const Float32x2 operand) {
    Float32x2 result;
    result.rawValue = -(operand.rawValue);
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Float32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2Absolute(const Float32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vabs_f32(operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = fabsf(operand.rawValue[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Float32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Add(Float32x2 lhs, Float32x2 rhs) {
    Float32x2 result;
    result.rawValue = lhs.rawValue + rhs.rawValue;
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Float32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Subtract(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
    result.rawValue = lhs.rawValue - rhs.rawValue;
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Float32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Multiply(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
    result.rawValue = lhs.rawValue * rhs.rawValue;
    return result;
}

/// Divides two storages (element-wise).
/// @return `(Float32x2){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Divide(const Float32x2 lhs, Float32x2 rhs) {
    Float32x2 result;
    result.rawValue = lhs.rawValue / rhs.rawValue;
    return result;
}

/// Calculates the square root (element-wise).
/// @return `(Float32x2){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2SquareRoot(const Float32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vsqrt_f32(operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = sqrtf(operand.rawValue[i]);
    }
#endif
    return result;
}

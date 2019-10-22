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

/// The underlying vector type of 2 Int32
typedef Int32 CXInt32x2_t __attribute__((ext_vector_type(2)));
/// The 2 x 32-bit signed integer storage
typedef union { CXInt32x2_t rawValue; } Int32x2;

// MARK: - Make

/// Loads 2 x Int32 values from unaligned memory.
/// @return `(Int32x2){ pointer[0], pointer[1] }`
CX_INLINE Int32x2 CXInt32x2MakeLoad(Int32 const pointer[2]) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vld1_s32(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Int32x2){ element0, element1 }`
CX_INLINE Int32x2 CXInt32x2Make(Int32 element0, Int32 element1) {
    Int32x2 result;
    result.rawValue = (CXInt32x2_t){ element0, element1 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Int32x2){ value, value }`
CX_INLINE Int32x2 CXInt32x2MakeRepeatingElement(const Int32 value) {
    Int32x2 result;
    result.rawValue = (CXInt32x2_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int32 CXInt32x2GetElement(const Int32x2 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt32x2SetElement(Int32x2* storage, const int index, const Int32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Minimum(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vmin_s32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE Int32x2 CXInt32x2Maximum(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vmax_s32(lhs.rawValue, rhs.rawValue);
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
#if !CX_ARM_ARCH && !CX_X86_ARCH
#   include <math.h>
#endif

/// Returns the negated storage (element-wise).
/// @return `(Int32x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2Negate(const Int32x2 operand) {
    Int32x2 result;
    result.rawValue = -(operand.rawValue);
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2Absolute(const Int32x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vabs_s32(operand.rawValue);
#elif CX_X86_ARCH
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    __m64 signs = _mm_setr_pi32(SIGN_BIT, SIGN_BIT);
    result.rawValue = _mm_and_si64(operand.rawValue, signs);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = abs(operand[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Int32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Add(Int32x2 lhs, Int32x2 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue + rhs.rawValue;
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Int32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Subtract(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue - rhs.rawValue;
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Int32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Multiply(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue * rhs.rawValue;
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE Int32x2 CXInt32x2BitwiseNot(const Int32x2 operand) {
    Int32x2 result;
    result.rawValue = ~(operand.rawValue);
    return result;
}

/// Bitwise And
CX_INLINE Int32x2 CXInt32x2BitwiseAnd(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue & rhs.rawValue;
    return result;
}

/// Bitwise And Not
CX_INLINE Int32x2 CXInt32x2BitwiseAndNot(const Int32x2 lhs, const Int32x2 rhs) {
#if CX_X86_ARCH
    Int32x2 result;
    result.rawValue = _mm_andnot_si64(lhs.rawValue, rhs.rawValue);
    return result;
#else
    return CXInt32x2BitwiseAnd(CXInt32x2BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE Int32x2 CXInt32x2BitwiseOr(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue | rhs.rawValue;
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE Int32x2 CXInt32x2BitwiseExclusiveOr(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue ^ rhs.rawValue;
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x2 CXInt32x2ShiftElementWiseLeft(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vshl_s32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x2 CXInt32x2ShiftLeft(const Int32x2 lhs, const Int32 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x2 CXInt32x2ShiftElementWiseRight(const Int32x2 lhs, const Int32x2 rhs) {
#if CX_ARM_ARCH
    return CXInt32x2ShiftElementWiseLeft(lhs, CXInt32x2Negate(rhs));
#else
    Int32x2 result;
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x2 CXInt32x2ShiftRight(const Int32x2 lhs, const Int32 rhs) {
    Int32x2 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

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

/// The underlying vector type of 3 x Int32
typedef Int32 CXInt32x3_t __attribute__((ext_vector_type(3)));
/// The 3 x 32-bit signed integer storage
typedef union { CXInt32x3_t rawValue; } Int32x3;

// MARK: - Make

/// Loads 3 x Int32 values from unaligned memory.
/// @return `(Int32x3){ pointer[0], pointer[1], ... }`
CX_INLINE Int32x3 CXInt32x3MakeLoad(Int32 const pointer[3]) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vld1q_s32(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Int32x3){ element0, element1, ... }`
CX_INLINE Int32x3 CXInt32x3Make(Int32 element0, Int32 element1, Int32 element2) {
    Int32x3 result;
    result.rawValue = (CXInt32x3_t){ element0, element1, element2 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Int32x3){ value, value, ... }`
CX_INLINE Int32x3 CXInt32x3MakeRepeatingElement(const Int32 value) {
    Int32x3 result;
    result.rawValue = (CXInt32x3_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int32 CXInt32x3GetElement(const Int32x3 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt32x3SetElement(Int32x3* storage, const int index, const Int32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x3){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x3 CXInt32x3Minimum(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vminq_s32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x3 CXInt32x3Maximum(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vmaxq_s32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Absolute values are calculated via `abs` if neither intrinsics are available
#if !CX_ARM_ARCH && CX_X86_ARCH < CX_X86_LEVEL_SSE2
#   include <stdlib.h>
#endif

/// Returns the negated storage (element-wise).
/// @return `(Int32x3){ -(operand[0]), -(operand[1]), ... }`
CX_INLINE Int32x3 CXInt32x3Negate(const Int32x3 operand) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vnegq_s32(operand.rawValue);
#else
    result.rawValue = -(operand.rawValue);
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int32x3){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE Int32x3 CXInt32x3Absolute(const Int32x3 operand) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vabsq_s32(operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = abs(operand.rawValue[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Int32x3){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE Int32x3 CXInt32x3Add(Int32x3 lhs, Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vaddq_s32(lhs.rawValue, rhs.rawValue);
#else
    result.rawValue = lhs.rawValue + rhs.rawValue;
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Int32x3){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE Int32x3 CXInt32x3Subtract(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vsubq_s32(lhs.rawValue, rhs.rawValue);
#else
    result.rawValue = lhs.rawValue - rhs.rawValue;
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Int32x3){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE Int32x3 CXInt32x3Multiply(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vmulq_s32(lhs.rawValue, rhs.rawValue);
#else
    result.rawValue = lhs.rawValue * rhs.rawValue;
#endif
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE Int32x3 CXInt32x3BitwiseNot(const Int32x3 operand) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vmvnq_s32(operand.rawValue);
#else
    result.rawValue = ~(operand.rawValue);
#endif
    return result;
}

/// Bitwise And
CX_INLINE Int32x3 CXInt32x3BitwiseAnd(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vandq_s32(lhs.rawValue, rhs.rawValue);
#else
    result.rawValue = lhs.rawValue & rhs.rawValue;
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE Int32x3 CXInt32x3BitwiseAndNot(const Int32x3 lhs, const Int32x3 rhs) {
    return CXInt32x3BitwiseAnd(CXInt32x3BitwiseNot(lhs), rhs);
}

/// Bitwise Or
CX_INLINE Int32x3 CXInt32x3BitwiseOr(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vorrq_s32(lhs.rawValue, rhs.rawValue);
#else
    result.rawValue = lhs.rawValue | rhs.rawValue;
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE Int32x3 CXInt32x3BitwiseExclusiveOr(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = veorq_s32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] ^ rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x3 CXInt32x3ShiftElementWiseLeft(const Int32x3 lhs, const Int32x3 rhs) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vshlq_s32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x3 CXInt32x3ShiftLeft(const Int32x3 lhs, const Int32 rhs) {
    Int32x3 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x3 CXInt32x3ShiftElementWiseRight(const Int32x3 lhs, const Int32x3 rhs) {
#if CX_ARM_ARCH
    return CXInt32x3ShiftElementWiseLeft(lhs, CXInt32x3Negate(rhs));
#else
    Int32x3 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x3 CXInt32x3ShiftRight(const Int32x3 lhs, const Int32 rhs) {
    Int32x3 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

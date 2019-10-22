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

/// The underlying vector type of 2 UInt32
typedef UInt32 CXUInt32x2_t __attribute__((ext_vector_type(2)));
/// The 2 x 32-bit unsigned integer storage
typedef union { CXUInt32x2_t rawValue; } UInt32x2;

// MARK: - Make

/// Loads 2 x UInt32 values from unaligned memory.
/// @return `(UInt32x2){ pointer[0], pointer[1] }`
CX_INLINE UInt32x2 CXUInt32x2MakeLoad(UInt32 const pointer[2]) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vld1_u32(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(UInt32x2){ element0, element1 }`
CX_INLINE UInt32x2 CXUInt32x2Make(UInt32 element0, UInt32 element1) {
    UInt32x2 result;
    result.rawValue = (CXUInt32x2_t){ element0, element1 };
    return result;
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(UInt32x2){ value, value }`
CX_INLINE UInt32x2 CXUInt32x2MakeRepeatingElement(const UInt32 value) {
    UInt32x2 result;
    result.rawValue = (CXUInt32x2_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE UInt32 CXUInt32x2GetElement(const UInt32x2 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXUInt32x2SetElement(UInt32x2* storage, const int index, const UInt32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE UInt32x2 CXUInt32x2Minimum(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vmin_u32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE UInt32x2 CXUInt32x2Maximum(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vmax_u32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE UInt32x2 CXUInt32x2Absolute(const UInt32x2 operand) {
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(UInt32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE UInt32x2 CXUInt32x2Add(UInt32x2 lhs, UInt32x2 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue + rhs.rawValue;
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(UInt32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE UInt32x2 CXUInt32x2Subtract(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue - rhs.rawValue;
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(UInt32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE UInt32x2 CXUInt32x2Multiply(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue * rhs.rawValue;
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE UInt32x2 CXUInt32x2BitwiseNot(const UInt32x2 operand) {
    UInt32x2 result;
    result.rawValue = ~(operand.rawValue);
    return result;
}

/// Bitwise And
CX_INLINE UInt32x2 CXUInt32x2BitwiseAnd(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue & rhs.rawValue;
    return result;
}

/// Bitwise And Not
CX_INLINE UInt32x2 CXUInt32x2BitwiseAndNot(const UInt32x2 lhs, const UInt32x2 rhs) {
#if CX_X86_ARCH
    UInt32x2 result;
    result.rawValue = _mm_andnot_si64(lhs.rawValue, rhs.rawValue);
    return result;
#else
    return CXUInt32x2BitwiseAnd(CXUInt32x2BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE UInt32x2 CXUInt32x2BitwiseOr(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue | rhs.rawValue;
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE UInt32x2 CXUInt32x2BitwiseExclusiveOr(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue ^ rhs.rawValue;
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt32x2 CXUInt32x2ShiftElementWiseLeft(const UInt32x2 lhs, const UInt32x2 rhs) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.rawValue = vshl_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x2 CXUInt32x2ShiftLeft(const UInt32x2 lhs, const UInt32 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue << rhs;
    return result;
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt32x2 CXUInt32x2ShiftElementWiseRight(const UInt32x2 lhs, const UInt32x2 rhs) {
#if CX_ARM_ARCH
    UInt32x2 rhsNegated;
    rhsNegated.rawValue = vreinterpret_u32_s32(vneg_s32(vreinterpret_s32_u32(rhs.rawValue)));
    return CXUInt32x2ShiftElementWiseLeft(lhs, rhsNegated);
#else
    UInt32x2 result;
    #pragma clang loop vectorize(enable) interleave(enable)
    for (size_t i = 0; i < 2; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x2 CXUInt32x2ShiftRight(const UInt32x2 lhs, const UInt32 rhs) {
    UInt32x2 result;
    result.rawValue = lhs.rawValue >> rhs;
    return result;
}

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

/// The underlying vector type of 3 x UInt32
typedef UInt32 CXUInt32x3_t __attribute__((ext_vector_type(3)));
/// The 3 x 32-bit unsigned integer storage
typedef union { CXUInt32x3_t rawValue; } UInt32x3;

// MARK: - Make

/// Loads 3 x UInt32 values from unaligned memory.
/// @return `(UInt32x3){ pointer[0], pointer[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3MakeLoad(UInt32 const pointer[3]) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vld1q_u32(pointer);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(UInt32x3){ element0, element1, ... }`
CX_INLINE UInt32x3 CXUInt32x3Make(UInt32 element0, UInt32 element1, UInt32 element2) {
    UInt32x3 result;
    result.rawValue = (CXUInt32x3_t){ element0, element1, element2 };
    return result;
}

/// Returns an storage type with all elements initialized to `value`.
/// @return `(UInt32x3){ value, value, ... }`
CX_INLINE UInt32x3 CXUInt32x3MakeRepeatingElement(const UInt32 value) {
    UInt32x3 result;
    result.rawValue = (CXUInt32x3_t)( value );
    return result;
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE UInt32 CXUInt32x3GetElement(const UInt32x3 storage, const int index) {
    return storage.rawValue[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXUInt32x3SetElement(UInt32x3* storage, const int index, const UInt32 value) {
    (*storage).rawValue[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(UInt32x3){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Minimum(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vminq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] < rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Maximum(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vmaxq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] > rhs.rawValue[i] ? lhs.rawValue[i] : rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(UInt32x3){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE UInt32x3 CXUInt32x3Absolute(const UInt32x3 operand) {
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(UInt32x3){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Add(UInt32x3 lhs, UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vaddq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] + rhs.rawValue[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(UInt32x3){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Subtract(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vsubq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] - rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(UInt32x3){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE UInt32x3 CXUInt32x3Multiply(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vmulq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] * rhs.rawValue[i];
    }
#endif
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE UInt32x3 CXUInt32x3BitwiseNot(const UInt32x3 operand) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vmvnq_u32(operand.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
         result.rawValue[i] = ~operand.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise And
CX_INLINE UInt32x3 CXUInt32x3BitwiseAnd(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vandq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] & rhs.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE UInt32x3 CXUInt32x3BitwiseAndNot(const UInt32x3 lhs, const UInt32x3 rhs) {
    return CXUInt32x3BitwiseAnd(CXUInt32x3BitwiseNot(lhs), rhs);
}

/// Bitwise Or
CX_INLINE UInt32x3 CXUInt32x3BitwiseOr(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vorrq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] | rhs.rawValue[i];
    }
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE UInt32x3 CXUInt32x3BitwiseExclusiveOr(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = veorq_u32(lhs.rawValue, rhs.rawValue);
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
CX_INLINE UInt32x3 CXUInt32x3ShiftElementWiseLeft(const UInt32x3 lhs, const UInt32x3 rhs) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.rawValue = vshlq_u32(lhs.rawValue, rhs.rawValue);
#else
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs.rawValue[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x3 CXUInt32x3ShiftLeft(const UInt32x3 lhs, const UInt32 rhs) {
#if CX_ARM_ARCH
    return CXUInt32x3ShiftElementWiseLeft(lhs, CXUInt32x3MakeRepeatingElement(rhs));
#else
    UInt32x3 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] << rhs;
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE UInt32x3 CXUInt32x3ShiftElementWiseRight(const UInt32x3 lhs, const UInt32x3 rhs)
{
#if CX_ARM_ARCH
    UInt32x3 rhsNegated;
    rhsNegated.rawValue = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(rhs.rawValue)));
    return CXUInt32x3ShiftElementWiseLeft(lhs, rhsNegated);
#else
    UInt32x3 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs.rawValue[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE UInt32x3 CXUInt32x3ShiftRight(const UInt32x3 lhs, const UInt32 rhs)
{
#if CX_ARM_ARCH
    return CXUInt32x3ShiftElementWiseLeft(lhs, CXUInt32x3MakeRepeatingElement(rhs));
#else
    UInt32x3 result;
    #pragma clang loop vectorize(enable) interleave(enable)
     for (size_t i = 0; i < 3; i++) {
        result.rawValue[i] = lhs.rawValue[i] >> rhs;
    }
    return result;
#endif
}

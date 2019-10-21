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

/// Absolute values are calculated via `abs` if neither intrinsics are available
#if !CX_ARM_ARCH && !CX_X86_ARCH
#   include <math.h>
#endif

/// Convenience wrapper to a single intrinsic value return
#define RETURN_RESULT(value) { Int32x2 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 2 x 32-bit signed integer storage
typedef union CXInt32x2_t {

    /// The native intrinsic type
#if CX_ARM_ARCH
    int32x2_t intrinsic;
#elif CX_X86_ARCH
    __m64 intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    Int32 array CX_VECTOR_SIZE(8);
#else
    Int32 array[2];
#endif
} Int32x2;

// MARK: - Make

/// Loads 2 x Int32 values from unaligned memory.
/// @return `(Int32x2){ pointer[0], pointer[1] }`
CX_INLINE Int32x2 CXInt32x2MakeLoad(Int32 const pointer[2]) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vld1_s32(pointer);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Int32x2){ element0, element1 }`
CX_INLINE Int32x2 CXInt32x2Make(Int32 element0, Int32 element1) {
#if CX_X86_ARCH
    RETURN_RESULT(_mm_setr_pi32(element0, element1))
#else
    Int32 values[] = { element0, element1 };
    return CXInt32x2MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Int32x2){ value, value }`
CX_INLINE Int32x2 CXInt32x2MakeRepeatingElement(const Int32 value) {
#if CX_ARM_ARCH
    RETURN_RESULT(vdup_n_s32(value))
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_set1_pi32(value))
#else
    return CXInt32x2Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0).
/// @return `(Int32x2){ 0, 0 }`
CX_INLINE Int32x2 CXInt32x2MakeZero(void) {
#if CX_X86_ARCH
    RETURN_RESULT(_mm_setzero_si64())
#else
    return CXInt32x2MakeRepeatingElement(0);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Int32 CXInt32x2GetElement(const Int32x2 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXInt32x2SetElement(Int32x2* storage, const int index, const Int32 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Int32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Minimum(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmin_s32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Int32x2 CXInt32x2Maximum(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmax_s32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(Int32x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2Negate(const Int32x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vneg_s32(operand.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_sub_pi32(_mm_setzero_si64(), operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = -operand.array[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Int32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2Absolute(const Int32x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vabs_s32(operand.intrinsic);
#elif CX_X86_ARCH
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    __m64 signs = _mm_setr_pi32(SIGN_BIT, SIGN_BIT);
    result.intrinsic = _mm_and_si64(operand.intrinsic, signs);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = abs(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Int32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Add(Int32x2 lhs, Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vadd_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_add_pi32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] + rhs.array[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Int32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Subtract(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vsub_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_sub_pi32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] - rhs.array[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Int32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Int32x2 CXInt32x2Multiply(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmul_s32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] * rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE Int32x2 CXInt32x2BitwiseNot(const Int32x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmvn_s32(operand.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_xor_si64(operand.intrinsic, _mm_cvtsi64_m64(-1LL));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = ~operand.array[i]
    }
#endif
    return result;
}

/// Bitwise And
CX_INLINE Int32x2 CXInt32x2BitwiseAnd(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vand_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_and_si64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] & rhs.array[i];
    }
#endif
    return result;
}

/// Bitwise And Not
CX_INLINE Int32x2 CXInt32x2BitwiseAndNot(const Int32x2 lhs, const Int32x2 rhs) {
#if CX_X86_ARCH
    RETURN_RESULT(_mm_andnot_si64(lhs.intrinsic, rhs.intrinsic))
#else
    return CXInt32x2BitwiseAnd(CXInt32x2BitwiseNot(lhs), rhs);
#endif
}

/// Bitwise Or
CX_INLINE Int32x2 CXInt32x2BitwiseOr(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vorr_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_or_si64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] | rhs.array[i];
    }
#endif
    return result;
}

/// Bitwise Exclusive Or
CX_INLINE Int32x2 CXInt32x2BitwiseExclusiveOr(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = veor_s32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH
    result.intrinsic = _mm_xor_si64(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] ^ rhs.array[i];
    }
#endif
    return result;
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x2 CXInt32x2ShiftElementWiseLeft(const Int32x2 lhs, const Int32x2 rhs) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vshl_s32(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs.array[i];
    }
#endif
    return result;
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x2 CXInt32x2ShiftLeft(const Int32x2 lhs, const Int32 rhs) {
#if CX_ARM_ARCH
    return CXInt32x2ShiftElementWiseLeft(lhs, CXInt32x2MakeRepeatingElement(rhs));
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_slli_pi32(lhs.intrinsic, rhs))
#else
    Int32x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] << rhs;
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE Int32x2 CXInt32x2ShiftElementWiseRight(const Int32x2 lhs, const Int32x2 rhs)
{
#if CX_ARM_ARCH
    return CXInt32x2ShiftElementWiseLeft(lhs, CXInt32x2Negate(rhs));
#else
    Int32x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs.array[i];
    }
    return result;
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE Int32x2 CXInt32x2ShiftRight(const Int32x2 lhs, const Int32 rhs)
{
#if CX_ARM_ARCH
    return CXInt32x2ShiftElementWiseLeft(lhs, CXInt32x2MakeRepeatingElement(rhs));
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_srli_pi32(lhs.intrinsic, rhs))
#else
    Int32x2 result;
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] >> rhs;
    }
    return result;
#endif
}

#undef RETURN_RESULT
#undef SIZE

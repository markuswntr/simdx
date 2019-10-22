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

#include <stdlib.h>
#include "types.h"
#include "CXUInt32x3.h"

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXInt32x3){ element0, element1, element2 }`
CX_INLINE(CXInt32x3) CXInt32x3Make(Int32 element0, Int32 element1, Int32 element2)
{
#if CX_NEON_128
    return (CXInt32x3){ element0, element1, element2, 0 }; // last 32-bits are unused
#elif CX_X86_128
    return _mm_setr_epi32(element0, element1, element2, 0);
#elif CX_EXT_VECTOR
    return (CXInt32x3){ element0, element1, element2 };
#else
    CXInt32x3 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    storage.val[2] = element2;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXInt32x3){ pointer[0], pointer[1], pointer[2] }`
CX_INLINE(CXInt32x3) CXInt32x3MakeLoad(const Int32* pointer)
{
#if CX_NEON_128
    CXInt32x3 storage = vld1q_s32(pointer);
    vsetq_lane_s32(0, storage, 3);
    return storage;
#elif CX_X86_128
    CXInt32x3 storage = _mm_loadu_si128((const __m128i*)pointer);
    ((Int32*)&storage)[3] = 0;
    return storage;
#else
    return CXInt32x3Make( pointer[0], pointer[1], pointer[2] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXInt32x3){ value, value, value }`
CX_INLINE(CXInt32x3) CXInt32x3MakeRepeatingElement(const Int32 value)
{
#if CX_NEON_128
    CXInt32x3 storage = vdupq_n_s32(value);
    vsetq_lane_s32(0, storage, 3);
    return storage;
#elif CX_X86_128
    return CXInt32x3Make(value, value, value);
#elif CX_EXT_VECTOR
    return (CXInt32x3)(value);
#else
    return CXInt32x3Make( value, value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXInt32x3){ 0, 0, 0 }`
CX_INLINE(CXInt32x3) CXInt32x3MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXInt32x3MakeRepeatingElement(0);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Int32) CXInt32x3GetElement(const CXInt32x3 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((Int32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXInt32x3SetElement(CXInt32x3* storage, const int index, const Int32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Int32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x3){ (Int32)(operand[0]), (Int32)(operand[1]), (Int32)(operand[2]) }`
CX_INLINE(CXInt32x3) CXInt32x3FromCXFloat32x3(CXFloat32x3 operand)
{
#if CX_NEON_128
    return vcvtq_s32_f32(operand);
#elif CX_X86_128
    return _mm_cvtps_epi32(operand);
#elif CX_EXT_VECTOR
    return CXInt32x3Make(
        (Int32)nearbyintf(operand[0]),
        (Int32)nearbyintf(operand[1]),
        (Int32)nearbyintf(operand[2])
    );
#else
    return CXInt32x3Make(
        (Int32)nearbyintf(operand.val[0]),
        (Int32)nearbyintf(operand.val[1]),
        (Int32)nearbyintf(operand.val[2])
    );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x3){ (Int32)(operand[0]), (Int32)(operand[1]), (Int32)(operand[2]) }`
CX_INLINE(CXInt32x3) CXInt32x3FromCXUInt32x3(CXUInt32x3 operand)
{
#if CX_NEON_128
    return vreinterpretq_s32_u32(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt32x3);
#else
    return CXInt32x3Make(
        (Int32)(operand.val[0]),
        (Int32)(operand.val[1]),
        (Int32)(operand.val[2])
    );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXInt32x3) CXInt32x3Minimum(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vminq_s32(lhs, rhs);
//#elif CX_X86_128
//    // TODO: SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtps_epi32(_mm_min_ps(_mm_cvtepi32_ps(lhs), _mm_cvtepi32_ps(rhs)));
#else
    Int32 lhs0 = CXInt32x3GetElement(lhs, 0), rhs0 = CXInt32x3GetElement(rhs, 0);
    Int32 lhs1 = CXInt32x3GetElement(lhs, 1), rhs1 = CXInt32x3GetElement(rhs, 1);
    Int32 lhs2 = CXInt32x3GetElement(lhs, 2), rhs2 = CXInt32x3GetElement(rhs, 2);
    return CXInt32x3Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1,
        lhs2 < rhs2 ? lhs2 : rhs2
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXInt32x3) CXInt32x3Maximum(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vmaxq_s32(lhs, rhs);
//#elif CX_X86_128
//    // TODO: SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtps_epi32(_mm_max_ps(_mm_cvtepi32_ps(lhs), _mm_cvtepi32_ps(rhs)));
#else
    Int32 lhs0 = CXInt32x3GetElement(lhs, 0), rhs0 = CXInt32x3GetElement(rhs, 0);
    Int32 lhs1 = CXInt32x3GetElement(lhs, 1), rhs1 = CXInt32x3GetElement(rhs, 1);
    Int32 lhs2 = CXInt32x3GetElement(lhs, 2), rhs2 = CXInt32x3GetElement(rhs, 2);
    return CXInt32x3Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1,
        lhs2 > rhs2 ? lhs2 : rhs2
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXInt32x3){ -(operand[0]), -(operand[1]), -(operand[2]) }`
CX_INLINE(CXInt32x3) CXInt32x3Negate(const CXInt32x3 operand)
{
#if CX_NEON_128
    return vnegq_s32(operand);
#elif CX_X86_128
    return _mm_sub_epi32(CXInt32x3MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXInt32x3Make(
        -CXInt32x3GetElement(operand, 0),
        -CXInt32x3GetElement(operand, 1),
        -CXInt32x3GetElement(operand, 2)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt32x3){ abs(operand[0]), abs(operand[1]), abs(operand[2]) }`
CX_INLINE(CXUInt32x3) CXInt32x3Absolute(const CXInt32x3 operand)
{
#if CX_NEON_128
    return vreinterpretq_u32_s32(vabsq_s32(operand));
#elif CX_X86_128
    #define CXInt32x3BitwiseExclusiveOr(operand) _mm_xor_si128(operand, CXInt32x3MakeRepeatingElement(0xFFFFFFFF))
    #define CXInt32x3CompareEqual(lhs, rhs) CXInt32x3BitwiseExclusiveOr(_mm_cmplt_epi32(lhs, rhs))
    CXInt32x3 comparison = CXInt32x3CompareEqual(operand, CXInt32x3MakeZero());
    CXInt32x3 negated = CXInt32x3Negate(operand);
    return _mm_xor_si128(negated, _mm_and_si128(comparison, _mm_xor_si128(operand, negated)));
    #undef CXInt32x3CompareEqual
    #undef CXInt32x3BitwiseExclusiveOr
#else
    return CXUInt32x3Make(
        abs(CXInt32x3GetElement(operand, 0)),
        abs(CXInt32x3GetElement(operand, 1)),
        abs(CXInt32x3GetElement(operand, 2))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXInt32x3){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE(CXInt32x3) CXInt32x3Add(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vaddq_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) + CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) + CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) + CXInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXInt32x3){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE(CXInt32x3) CXInt32x3Subtract(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vsubq_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) - CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) - CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) - CXInt32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXInt32x3){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE(CXInt32x3) CXInt32x3Multiply(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vmulq_s32(lhs, rhs);
#elif CX_X86_128
    // SSE2 does not have a native multiply operation for 32bit ints
    __m128i temp0 = _mm_mul_epu32(lhs, rhs);
    __m128i temp1 = _mm_mul_epu32(_mm_srli_si128(lhs, 4), _mm_srli_si128(rhs, 4));
    return _mm_unpacklo_epi32(
        _mm_shuffle_epi32(temp0, _MM_SHUFFLE(0, 0, 2, 0)),
        _mm_shuffle_epi32(temp1, _MM_SHUFFLE(0, 0, 2, 0))
    );
#elif CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) * CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) * CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) * CXInt32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXInt32x3) CXInt32x3BitwiseNot(const CXInt32x3 operand)
{
#if CX_NEON_128
    CXInt32x3 storage = vmvnq_s32(operand);
    CXInt32x3SetElement(&storage, 3, 0);
    return storage;
#elif CX_X86_128
    CXInt32x3 storage = _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
    CXInt32x3SetElement(&storage, 3, 0);
    return storage;
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXInt32x3Make(
        ~CXInt32x3GetElement(operand, 0),
        ~CXInt32x3GetElement(operand, 1),
        ~CXInt32x3GetElement(operand, 2)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXInt32x3) CXInt32x3BitwiseAnd(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vandq_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) & CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) & CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) & CXInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXInt32x3) CXInt32x3BitwiseAndNot(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return CXInt32x3BitwiseAnd(CXInt32x3BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXInt32x3BitwiseNot(lhs) & rhs;
#else
    CXInt32x3 notLhs = CXInt32x3BitwiseNot(lhs);
    return CXInt32x3Make(
        CXInt32x3GetElement(notLhs, 0) & CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(notLhs, 1) & CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(notLhs, 2) & CXInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXInt32x3) CXInt32x3BitwiseOr(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vorrq_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) | CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) | CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) | CXInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXInt32x3) CXInt32x3BitwiseExclusiveOr(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return veorq_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) ^ CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) ^ CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) ^ CXInt32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt32x3) CXInt32x3ShiftElementWiseLeft(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return vshlq_s32(lhs, rhs);
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) << CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) << CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) << CXInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt32x3) CXInt32x3ShiftLeft(const CXInt32x3 lhs, const Int32 rhs)
{
#if CX_NEON_128
    return CXInt32x3ShiftElementWiseLeft(lhs, CXInt32x3MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) << rhs,
        CXInt32x3GetElement(lhs, 1) << rhs,
        CXInt32x3GetElement(lhs, 2) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt32x3) CXInt32x3ShiftElementWiseRight(const CXInt32x3 lhs, const CXInt32x3 rhs)
{
#if CX_NEON_128
    return CXInt32x3ShiftElementWiseLeft(lhs, CXInt32x3Negate(rhs));
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) >> CXInt32x3GetElement(rhs, 0),
        CXInt32x3GetElement(lhs, 1) >> CXInt32x3GetElement(rhs, 1),
        CXInt32x3GetElement(lhs, 2) >> CXInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt32x3) CXInt32x3ShiftRight(const CXInt32x3 lhs, const Int32 rhs)
{
#if CX_NEON_128
    return CXInt32x3ShiftElementWiseRight(lhs, CXInt32x3MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srai_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXInt32x3Make(
        CXInt32x3GetElement(lhs, 0) >> rhs,
        CXInt32x3GetElement(lhs, 1) >> rhs,
        CXInt32x3GetElement(lhs, 2) >> rhs
    );
#endif
}

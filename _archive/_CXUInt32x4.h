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

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXUInt32x4){ element0, element1, element2, element3 }`
CX_INLINE(CXUInt32x4) CXUInt32x4Make(UInt32 element0, UInt32 element1, UInt32 element2, UInt32 element3)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXUInt32x4){ element0, element1, element2, element3 };
#elif CX_X86_128
    return _mm_setr_epi32(element0, element1, element2, element3);
#else
    CXUInt32x4 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    storage.val[2] = element2;
    storage.val[3] = element3;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXUInt32x4){ pointer[0], pointer[1], pointer[2], pointer[3] }`
CX_INLINE(CXUInt32x4) CXUInt32x4MakeLoad(const UInt32* pointer)
{
#if CX_NEON_128
    return vld1q_u32(pointer);
#elif CX_X86_128
    return _mm_loadu_si128((const __m128i*)pointer);
#else
    return CXUInt32x4Make( pointer[0], pointer[1], pointer[2], pointer[3] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXUInt32x4){ value, value, value, value }`
CX_INLINE(CXUInt32x4) CXUInt32x4MakeRepeatingElement(const UInt32 value)
{
#if CX_NEON_128
    return vdupq_n_u32(value);
#elif CX_X86_128
    return _mm_set1_epi32(value);
#elif CX_EXT_VECTOR
    return (CXUInt32x4)(value);
#else
    return CXUInt32x4Make( value, value, value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXUInt32x4){ 0, 0, 0, 0 }`
CX_INLINE(CXUInt32x4) CXUInt32x4MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXUInt32x4MakeRepeatingElement(0);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(UInt32) CXUInt32x4GetElement(const CXUInt32x4 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((UInt32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXUInt32x4SetElement(CXUInt32x4* storage, const int index, const UInt32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((UInt32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x4){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]), (UInt32)(operand[3]) }`
CX_INLINE(CXUInt32x4) CXUInt32x4FromCXFloat32x4(CXFloat32x4 operand)
{
#if CX_NEON_128
    return vcvtq_u32_f32(operand);
#elif CX_X86_128
    return _mm_cvtps_epi32(operand);
#elif CX_EXT_VECTOR
    return CXUInt32x4Make(
        (UInt32)nearbyintf(operand[0]),
        (UInt32)nearbyintf(operand[1]),
        (UInt32)nearbyintf(operand[2]),
        (UInt32)nearbyintf(operand[3])
    );
#else
    return CXUInt32x4Make(
        (UInt32)nearbyintf(operand.val[0]),
        (UInt32)nearbyintf(operand.val[1]),
        (UInt32)nearbyintf(operand.val[2]),
        (UInt32)nearbyintf(operand.val[3])
    );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x4){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]), (UInt32)(operand[3]) }`
CX_INLINE(CXUInt32x4) CXUInt32x4FromCXInt32x4(CXInt32x4 operand)
{
#if CX_NEON_128
    return vreinterpretq_u32_s32(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt32x4);
#else
    return CXUInt32x4Make(
        (UInt32)(operand.val[0]),
        (UInt32)(operand.val[1]),
        (UInt32)(operand.val[2]),
        (UInt32)(operand.val[3])
    );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXUInt32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXUInt32x4) CXUInt32x4Minimum(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vminq_u32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_min_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    UInt32 lhs0 = CXUInt32x4GetElement(lhs, 0), rhs0 = CXUInt32x4GetElement(rhs, 0);
    UInt32 lhs1 = CXUInt32x4GetElement(lhs, 1), rhs1 = CXUInt32x4GetElement(rhs, 1);
    UInt32 lhs2 = CXUInt32x4GetElement(lhs, 2), rhs2 = CXUInt32x4GetElement(rhs, 2);
    UInt32 lhs3 = CXUInt32x4GetElement(lhs, 3), rhs3 = CXUInt32x4GetElement(rhs, 3);
    return CXUInt32x4Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1,
        lhs2 < rhs2 ? lhs2 : rhs2,
        lhs3 < rhs3 ? lhs3 : rhs3
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXUInt32x4) CXUInt32x4Maximum(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vmaxq_u32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_max_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    UInt32 lhs0 = CXUInt32x4GetElement(lhs, 0), rhs0 = CXUInt32x4GetElement(rhs, 0);
    UInt32 lhs1 = CXUInt32x4GetElement(lhs, 1), rhs1 = CXUInt32x4GetElement(rhs, 1);
    UInt32 lhs2 = CXUInt32x4GetElement(lhs, 2), rhs2 = CXUInt32x4GetElement(rhs, 2);
    UInt32 lhs3 = CXUInt32x4GetElement(lhs, 3), rhs3 = CXUInt32x4GetElement(rhs, 3);
    return CXUInt32x4Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1,
        lhs2 > rhs2 ? lhs2 : rhs2,
        lhs3 > rhs3 ? lhs3 : rhs3
    );
#endif
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt32x4){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE(CXUInt32x4) CXUInt32x4Absolute(const CXUInt32x4 operand)
{
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXUInt32x4){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE(CXUInt32x4) CXUInt32x4Add(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vaddq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXUInt32x4Make(
         CXUInt32x4GetElement(lhs, 0) + CXUInt32x4GetElement(rhs, 0),
         CXUInt32x4GetElement(lhs, 1) + CXUInt32x4GetElement(rhs, 1),
         CXUInt32x4GetElement(lhs, 2) + CXUInt32x4GetElement(rhs, 2),
         CXUInt32x4GetElement(lhs, 3) + CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXUInt32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE(CXUInt32x4) CXUInt32x4Subtract(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vsubq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXUInt32x4Make(
         CXUInt32x4GetElement(lhs, 0) - CXUInt32x4GetElement(rhs, 0),
         CXUInt32x4GetElement(lhs, 1) - CXUInt32x4GetElement(rhs, 1),
         CXUInt32x4GetElement(lhs, 2) - CXUInt32x4GetElement(rhs, 2),
         CXUInt32x4GetElement(lhs, 3) - CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXUInt32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE(CXUInt32x4) CXUInt32x4Multiply(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vmulq_u32(lhs, rhs);
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
    return CXUInt32x4Make(
         CXUInt32x4GetElement(lhs, 0) * CXUInt32x4GetElement(rhs, 0),
         CXUInt32x4GetElement(lhs, 1) * CXUInt32x4GetElement(rhs, 1),
         CXUInt32x4GetElement(lhs, 2) * CXUInt32x4GetElement(rhs, 2),
         CXUInt32x4GetElement(lhs, 3) * CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXUInt32x4) CXUInt32x4BitwiseNot(const CXUInt32x4 operand)
{
#if CX_NEON_128
    return vmvnq_u32(operand);
#elif CX_X86_128
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXUInt32x4Make(
         ~CXUInt32x4GetElement(operand, 0),
         ~CXUInt32x4GetElement(operand, 1),
         ~CXUInt32x4GetElement(operand, 2),
         ~CXUInt32x4GetElement(operand, 3)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXUInt32x4) CXUInt32x4BitwiseAnd(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vandq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXUInt32x4Make(
         CXUInt32x4GetElement(lhs, 0) & CXUInt32x4GetElement(rhs, 0),
         CXUInt32x4GetElement(lhs, 1) & CXUInt32x4GetElement(rhs, 1),
         CXUInt32x4GetElement(lhs, 2) & CXUInt32x4GetElement(rhs, 2),
         CXUInt32x4GetElement(lhs, 3) & CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXUInt32x4) CXUInt32x4BitwiseAndNot(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return CXUInt32x4BitwiseAnd(CXUInt32x4BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXUInt32x4BitwiseNot(lhs) & rhs;
#else
    CXUInt32x4 notLhs = CXUInt32x4BitwiseNot(lhs);
    return CXUInt32x4Make(
         CXUInt32x4GetElement(notLhs, 0) & CXUInt32x4GetElement(rhs, 0),
         CXUInt32x4GetElement(notLhs, 1) & CXUInt32x4GetElement(rhs, 1),
         CXUInt32x4GetElement(notLhs, 2) & CXUInt32x4GetElement(rhs, 2),
         CXUInt32x4GetElement(notLhs, 3) & CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXUInt32x4) CXUInt32x4BitwiseOr(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vorrq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXUInt32x4Make(
         CXUInt32x4GetElement(lhs, 0) | CXUInt32x4GetElement(rhs, 0),
         CXUInt32x4GetElement(lhs, 1) | CXUInt32x4GetElement(rhs, 1),
         CXUInt32x4GetElement(lhs, 2) | CXUInt32x4GetElement(rhs, 2),
         CXUInt32x4GetElement(lhs, 3) | CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXUInt32x4) CXUInt32x4BitwiseExclusiveOr(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return veorq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXUInt32x4Make(
        CXUInt32x4GetElement(lhs, 0) ^ CXUInt32x4GetElement(rhs, 0),
        CXUInt32x4GetElement(lhs, 1) ^ CXUInt32x4GetElement(rhs, 1),
        CXUInt32x4GetElement(lhs, 2) ^ CXUInt32x4GetElement(rhs, 2),
        CXUInt32x4GetElement(lhs, 3) ^ CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt32x4) CXUInt32x4ShiftElementWiseLeft(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return vshlq_s32(lhs, rhs);
#else
    return CXUInt32x4Make(
        CXUInt32x4GetElement(lhs, 0) << CXUInt32x4GetElement(rhs, 0),
        CXUInt32x4GetElement(lhs, 1) << CXUInt32x4GetElement(rhs, 1),
        CXUInt32x4GetElement(lhs, 2) << CXUInt32x4GetElement(rhs, 2),
        CXUInt32x4GetElement(lhs, 3) << CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt32x4) CXUInt32x4ShiftLeft(const CXUInt32x4 lhs, const UInt32 rhs)
{
#if CX_NEON_128
    return CXUInt32x4ShiftElementWiseLeft(lhs, CXUInt32x4MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXUInt32x4Make(
        CXUInt32x4GetElement(lhs, 0) << rhs,
        CXUInt32x4GetElement(lhs, 1) << rhs,
        CXUInt32x4GetElement(lhs, 2) << rhs,
        CXUInt32x4GetElement(lhs, 3) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt32x4) CXUInt32x4ShiftElementWiseRight(const CXUInt32x4 lhs, const CXUInt32x4 rhs)
{
#if CX_NEON_128
    return CXUInt32x4ShiftElementWiseLeft(lhs, vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(rhs))));
#else
    return CXUInt32x4Make(
        CXUInt32x4GetElement(lhs, 0) >> CXUInt32x4GetElement(rhs, 0),
        CXUInt32x4GetElement(lhs, 1) >> CXUInt32x4GetElement(rhs, 1),
        CXUInt32x4GetElement(lhs, 2) >> CXUInt32x4GetElement(rhs, 2),
        CXUInt32x4GetElement(lhs, 3) >> CXUInt32x4GetElement(rhs, 3)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt32x4) CXUInt32x4ShiftRight(const CXUInt32x4 lhs, const UInt32 rhs)
{
#if CX_NEON_128
    return CXUInt32x4ShiftElementWiseRight(lhs, CXUInt32x4MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srai_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXUInt32x4Make(
        CXUInt32x4GetElement(lhs, 0) >> rhs,
        CXUInt32x4GetElement(lhs, 1) >> rhs,
        CXUInt32x4GetElement(lhs, 2) >> rhs,
        CXUInt32x4GetElement(lhs, 3) >> rhs
    );
#endif
}

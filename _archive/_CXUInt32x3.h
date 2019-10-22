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
/// @return `(CXUInt32x3){ element0, element1, element2 }`
CX_INLINE(CXUInt32x3) CXUInt32x3Make(UInt32 element0, UInt32 element1, UInt32 element2)
{
#if CX_NEON_128
    return (CXUInt32x3){ element0, element1, element2, 0 }; // last 32-bits are unused
#elif CX_X86_128
    return _mm_setr_epi32(element0, element1, element2, 0); // last 32-bits are unused
#elif CX_EXT_VECTOR
    return (CXUInt32x3){ element0, element1, element2 };
#else
    CXUInt32x3 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    storage.val[2] = element2;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXUInt32x3){ pointer[0], pointer[1], pointer[2] }`
CX_INLINE(CXUInt32x3) CXUInt32x3MakeLoad(const UInt32* pointer)
{
#if CX_NEON_128
    CXUInt32x3 storage = vld1q_u32(pointer);
    vsetq_lane_u32(0, storage, 3);
    return storage;
#elif CX_X86_128
    CXUInt32x3 storage = _mm_loadu_si128((const __m128i*)pointer);
    ((UInt32*)&storage)[3] = 0;
    return storage;
#else
    return CXUInt32x3Make( pointer[0], pointer[1], pointer[2] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXUInt32x3){ value, value, value }`
CX_INLINE(CXUInt32x3) CXUInt32x3MakeRepeatingElement(const UInt32 value)
{
#if CX_NEON_128
    CXUInt32x3 storage = vdupq_n_u32(value);
    vsetq_lane_u32(0, storage, 3);
    return storage;
#elif CX_X86_128
    return CXUInt32x3Make(value, value, value);
#elif CX_EXT_VECTOR
    return (CXUInt32x3)(value);
#else
    return CXUInt32x3Make( value, value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXUInt32x3){ 0, 0, 0 }`
CX_INLINE(CXUInt32x3) CXUInt32x3MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXUInt32x3MakeRepeatingElement(0);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(UInt32) CXUInt32x3GetElement(const CXUInt32x3 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((UInt32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXUInt32x3SetElement(CXUInt32x3* storage, const int index, const UInt32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((UInt32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x3){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]) }`
CX_INLINE(CXUInt32x3) CXUInt32x3FromCXFloat32x3(CXFloat32x3 operand)
{
#if CX_NEON_128
    return vcvtq_u32_f32(operand);
#elif CX_X86_128
    return _mm_cvtps_epi32(operand);
#elif CX_EXT_VECTOR
    return CXUInt32x3Make(
        (UInt32)nearbyintf(operand[0]),
        (UInt32)nearbyintf(operand[1]),
        (UInt32)nearbyintf(operand[2])
    );
#else
    return CXUInt32x3Make(
        (UInt32)nearbyintf(operand.val[0]),
        (UInt32)nearbyintf(operand.val[1]),
        (UInt32)nearbyintf(operand.val[2])
    );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x3){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]) }`
CX_INLINE(CXUInt32x3) CXUInt32x3FromCXInt32x3(CXInt32x3 operand)
{
#if CX_NEON_128
    return vreinterpretq_u32_s32(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt32x3);
#else
    return CXUInt32x3Make(
        (UInt32)(operand.val[0]),
        (UInt32)(operand.val[1]),
        (UInt32)(operand.val[2])
    );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXUInt32x3){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXUInt32x3) CXUInt32x3Minimum(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vminq_u32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_min_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    UInt32 lhs0 = CXUInt32x3GetElement(lhs, 0), rhs0 = CXUInt32x3GetElement(rhs, 0);
    UInt32 lhs1 = CXUInt32x3GetElement(lhs, 1), rhs1 = CXUInt32x3GetElement(rhs, 1);
    UInt32 lhs2 = CXUInt32x3GetElement(lhs, 2), rhs2 = CXUInt32x3GetElement(rhs, 2);
    return CXUInt32x3Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1,
        lhs2 < rhs2 ? lhs2 : rhs2
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXUInt32x3) CXUInt32x3Maximum(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vmaxq_s32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_max_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    UInt32 lhs0 = CXUInt32x3GetElement(lhs, 0), rhs0 = CXUInt32x3GetElement(rhs, 0);
    UInt32 lhs1 = CXUInt32x3GetElement(lhs, 1), rhs1 = CXUInt32x3GetElement(rhs, 1);
    UInt32 lhs2 = CXUInt32x3GetElement(lhs, 2), rhs2 = CXUInt32x3GetElement(rhs, 2);
    return CXUInt32x3Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1,
        lhs2 > rhs2 ? lhs2 : rhs2
    );
#endif
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt32x3){ abs(operand[0]), abs(operand[1]), abs(operand[2]) }`
CX_INLINE(CXUInt32x3) CXUInt32x3Absolute(const CXUInt32x3 operand)
{
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXUInt32x3){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE(CXUInt32x3) CXUInt32x3Add(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vaddq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) + CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) + CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) + CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXUInt32x3){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE(CXUInt32x3) CXUInt32x3Subtract(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vsubq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) - CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) - CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) - CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXUInt32x3){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE(CXUInt32x3) CXUInt32x3Multiply(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
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
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) * CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) * CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) * CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXUInt32x3) CXUInt32x3BitwiseNot(const CXUInt32x3 operand)
{
#if CX_NEON_128
    CXUInt32x3 storage = vmvnq_u32(operand);
    CXUInt32x3SetElement(&storage, 3, 0);
    return storage;
#elif CX_X86_128
    CXUInt32x3 storage = _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
    CXUInt32x3SetElement(&storage, 3, 0);
    return storage;
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXUInt32x3Make(
        ~CXUInt32x3GetElement(operand, 0),
        ~CXUInt32x3GetElement(operand, 1),
        ~CXUInt32x3GetElement(operand, 2)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXUInt32x3) CXUInt32x3BitwiseAnd(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vandq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXUInt32x3Make(
         CXUInt32x3GetElement(lhs, 0) & CXUInt32x3GetElement(rhs, 0),
         CXUInt32x3GetElement(lhs, 1) & CXUInt32x3GetElement(rhs, 1),
         CXUInt32x3GetElement(lhs, 2) & CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXUInt32x3) CXUInt32x3BitwiseAndNot(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return CXUInt32x3BitwiseAnd(CXUInt32x3BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXUInt32x3BitwiseNot(lhs) & rhs;
#else
    CXUInt32x3 notLhs = CXUInt32x3BitwiseNot(lhs);
    return CXUInt32x3Make(
        CXUInt32x3GetElement(notLhs, 0) & CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(notLhs, 1) & CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(notLhs, 2) & CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXUInt32x3) CXUInt32x3BitwiseOr(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vorrq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) | CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) | CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) | CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXUInt32x3) CXUInt32x3BitwiseExclusiveOr(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return veorq_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) ^ CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) ^ CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) ^ CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt32x3) CXUInt32x3ShiftElementWiseLeft(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return vshlq_s32(lhs, rhs);
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) << CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) << CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) << CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt32x3) CXUInt32x3ShiftLeft(const CXUInt32x3 lhs, const UInt32 rhs)
{
#if CX_NEON_128
    return CXUInt32x3ShiftElementWiseLeft(lhs, CXUInt32x3MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) << rhs,
        CXUInt32x3GetElement(lhs, 1) << rhs,
        CXUInt32x3GetElement(lhs, 2) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt32x3) CXUInt32x3ShiftElementWiseRight(const CXUInt32x3 lhs, const CXUInt32x3 rhs)
{
#if CX_NEON_128
    return CXUInt32x3ShiftElementWiseLeft(lhs, vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(rhs))));
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) >> CXUInt32x3GetElement(rhs, 0),
        CXUInt32x3GetElement(lhs, 1) >> CXUInt32x3GetElement(rhs, 1),
        CXUInt32x3GetElement(lhs, 2) >> CXUInt32x3GetElement(rhs, 2)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt32x3) CXUInt32x3ShiftRight(const CXUInt32x3 lhs, const UInt32 rhs)
{
#if CX_NEON_128
    return CXUInt32x3ShiftElementWiseRight(lhs, CXUInt32x3MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srai_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXUInt32x3Make(
        CXUInt32x3GetElement(lhs, 0) >> rhs,
        CXUInt32x3GetElement(lhs, 1) >> rhs,
        CXUInt32x3GetElement(lhs, 2) >> rhs
    );
#endif
}

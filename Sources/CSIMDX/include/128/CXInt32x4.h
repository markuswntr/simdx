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

#include "Types.h"
#if defined(CX_NEON_128)
#include <arm_neon.h>
#elif defined(CX_X86_128)
#include <emmintrin.h>
#else
#include <stdlib.h>
#endif

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXInt32x4){ element0, element1, element2, element3 }`
CX_INLINE(CXInt32x4) CXInt32x4Make(Int32 element0, Int32 element1, Int32 element2, Int32 element3)
{
#if defined(CX_NEON_128) || defined(CX_EXT_VECTOR)
    return (CXInt32x4){ element0, element1, element2, element3 };
#elif defined(CX_X86_128)
    return _mm_setr_epi32(element0, element1, element2, element3);
#else
    return (CXInt32x4){ .val = [ element0, element1, element2, element2 ] };
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXInt32x4){ pointer[0], pointer[1], pointer[2], pointer[3] }`
CX_INLINE(CXInt32x4) CXInt32x4MakeLoad(const Int32* pointer)
{
#if defined(CX_NEON_128)
    return vld1q_s32(pointer);
#elif defined(CX_X86_128)
    return _mm_loadu_si128((const __m128i*)pointer);
#elif defined(CX_EXT_VECTOR)
    return (CXInt32x4){ pointer[0], pointer[1], pointer[2], pointer[3] };
#else
    return (CXInt32x4){ .val = [ pointer[0], pointer[1], pointer[2], pointer[3] ] };
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXInt32x4){ value, value, value, value }`
CX_INLINE(CXInt32x4) CXInt32x4MakeRepeatingElement(const Int32 value)
{
#if defined(CX_NEON_128)
    return vdupq_n_s32(value);
#elif defined(CX_X86_128)
    return _mm_set1_epi32(value);
#elif defined(CX_EXT_VECTOR)
    return (CXInt32x4)(value);
#else
    return (CXInt32x4){ .val = [ value, value, value, value ] };
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXInt32x4){ 0, 0, 0, 0 }`
CX_INLINE(CXInt32x4) CXInt32x4MakeZero(void)
{
#if defined(CX_X86_128)
    return _mm_setzero_si128();
#else
    return CXInt32x4MakeRepeatingElement(0);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Int32) CXInt32x4GetElement(const CXInt32x4 storage, const int index)
{
#if defined(CX_NEON_128) || defined(CX_X86_128) || defined(CX_EXT_VECTOR)
    return ((Int32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXInt32x4SetElement(CXInt32x4* storage, const int index, const Int32 value)
{
#if defined(CX_NEON_128) || defined(CX_X86_128) || defined(CX_EXT_VECTOR)
    ((Int32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x4){ (Int32)(operand[0]), (Int32)(operand[1]), (Int32)(operand[2]), (Int32)(operand[3]) }`
CX_INLINE(CXInt32x4) CXInt32x4FromCXFloat32x4(CXFloat32x4 operand)
{
#if defined(CX_NEON_128)
    return vcvtq_s32_f32(operand);
#elif defined(CX_X86_128)
    return _mm_cvtps_epi32(operand);
#elif defined(CX_EXT_VECTOR)
    return __builtin_convertvector(operand, CXInt32x4);
#else
    return (CXInt32x4){ .val = [
        (Int32)(operand.val[0]),
        (Int32)(operand.val[1]),
        (Int32)(operand.val[2]),
        (Int32)(operand.val[3])
    ]};
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x4){ (Int32)(operand[0]), (Int32)(operand[1]), (Int32)(operand[2]), (Int32)(operand[3]) }`
CX_INLINE(CXInt32x4) CXInt32x4FromCXUInt32x4(CXUInt32x4 operand)
{
#if defined(CX_NEON_128)
    return vreinterpretq_s32_u32(operand);
#elif defined(CX_X86_128)
    return operand;
#elif defined(CX_EXT_VECTOR)
    return __builtin_convertvector(operand, CXInt32x4);
#else
    return (CXInt32x4){ .val = [
        (Int32)(operand.val[0]),
        (Int32)(operand.val[1]),
        (Int32)(operand.val[2]),
        (Int32)(operand.val[3])
    ]};
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXInt32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXInt32x4) CXInt32x4Minimum(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vminq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    // SSE2 has no min on Int32, but on Float64 so use it temporarily
    return _mm_cvtpd_epi32(_mm_min_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    Int64 lhs0 = CXInt32x4GetElement(lhs, 0), rhs0 = CXInt32x4GetElement(rhs, 0);
    Int64 lhs1 = CXInt32x4GetElement(lhs, 1), rhs1 = CXInt32x4GetElement(rhs, 1);
    Int64 lhs2 = CXInt32x4GetElement(lhs, 2), rhs2 = CXInt32x4GetElement(rhs, 2);
    Int64 lhs3 = CXInt32x4GetElement(lhs, 3), rhs3 = CXInt32x4GetElement(rhs, 3);
    #if defined(CX_EXT_VECTOR)
        return (CXInt32x4){
            lhs0 < rhs0 ? lhs0 : rhs0,
            lhs1 < rhs1 ? lhs1 : rhs1,
            lhs2 < rhs2 ? lhs2 : rhs2,
            lhs3 < rhs3 ? lhs3 : rhs3
        };
    #else
        return (CXInt32x4){ .val = [
            lhs0 < rhs0 ? lhs0 : rhs0,
            lhs1 < rhs1 ? lhs1 : rhs1,
            lhs2 < rhs2 ? lhs2 : rhs2,
            lhs3 < rhs3 ? lhs3 : rhs3
        ]};
    #endif
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXInt32x4) CXInt32x4Maximum(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vmaxq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    // SSE2 has no min on Int32, but on Float64 so use it temporarily
    return _mm_cvtpd_epi32(_mm_max_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    Int64 lhs0 = CXInt32x4GetElement(lhs, 0), rhs0 = CXInt32x4GetElement(rhs, 0);
    Int64 lhs1 = CXInt32x4GetElement(lhs, 1), rhs1 = CXInt32x4GetElement(rhs, 1);
    Int64 lhs2 = CXInt32x4GetElement(lhs, 2), rhs2 = CXInt32x4GetElement(rhs, 2);
    Int64 lhs3 = CXInt32x4GetElement(lhs, 3), rhs3 = CXInt32x4GetElement(rhs, 3);
    #if defined(CX_EXT_VECTOR)
        return (CXInt32x4){
            lhs0 > rhs0 ? lhs0 : rhs0,
            lhs1 > rhs1 ? lhs1 : rhs1,
            lhs2 > rhs2 ? lhs2 : rhs2,
            lhs3 > rhs3 ? lhs3 : rhs3
        };
    #else
        return (CXInt32x4){ .val = [
            lhs0 > rhs0 ? lhs0 : rhs0,
            lhs1 > rhs1 ? lhs1 : rhs1,
            lhs2 > rhs2 ? lhs2 : rhs2,
            lhs3 > rhs3 ? lhs3 : rhs3
        ]};
    #endif
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXInt32x4){ -(operand[0]), -(operand[1]), ... }`
CX_INLINE(CXInt32x4) CXInt32x4Negate(const CXInt32x4 operand)
{
#if defined(CX_NEON_128)
    return vnegq_s32(operand);
#elif defined(CX_X86_128)
    return _mm_sub_epi32(CXInt32x4MakeZero(), operand);
#elif defined(CX_EXT_VECTOR)
    return -(operand);
#else
    return (CXInt32x4){ .val = [
        -CXInt32x4GetElement(operand, 0),
        -CXInt32x4GetElement(operand, 1),
        -CXInt32x4GetElement(operand, 2),
        -CXInt32x4GetElement(operand, 3)
    ]};
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt32x4){ abs(operand[0]), abs(operand[1]), ... }`
CX_INLINE(CXUInt32x4) CXInt32x4Absolute(const CXInt32x4 operand)
{
#if defined(CX_NEON_128)
    return vreinterpretq_u32_s32(vabsq_s32(operand));
#elif defined(CX_X86_128)
    #define CXInt32x4BitwiseExclusiveOr(operand) _mm_xor_si128(operand, CXInt32x4MakeRepeatingElement(0xFFFFFFFF))
    #define CXInt32x4CompareEqual(lhs, rhs) CXInt32x4BitwiseExclusiveOr(_mm_cmplt_epi32(lhs, rhs))
    CXInt32x4 comparison = CXInt32x4CompareEqual(operand, CXInt32x4MakeZero());
    CXInt32x4 negated = CXInt32x4Negate(operand);
    return _mm_xor_si128(negated, _mm_and_si128(comparison, _mm_xor_si128(operand, negated)));
    #undef CXInt32x4CompareEqual
    #undef CXInt32x4BitwiseExclusiveOr
#elif defined(CX_EXT_VECTOR)
    return (CXUInt32x4){
        abs(CXInt32x4GetElement(operand, 0)),
        abs(CXInt32x4GetElement(operand, 1)),
        abs(CXInt32x4GetElement(operand, 2)),
        abs(CXInt32x4GetElement(operand, 3))
    };
#else
    return (CXUInt32x4){ .val = [
        abs(CXInt32x4GetElement(operand, 0)),
        abs(CXInt32x4GetElement(operand, 1)),
        abs(CXInt32x4GetElement(operand, 2)),
        abs(CXInt32x4GetElement(operand, 3))
    ]};
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXInt32x4){ lhs[0] + rhs[0], lhs[1] + rhs[1], ... }`
CX_INLINE(CXInt32x4) CXInt32x4Add(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vaddq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    return _mm_add_epi32(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return lhs + rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) + CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) + CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) + CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) + CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXInt32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1], ... }`
CX_INLINE(CXInt32x4) CXInt32x4Subtract(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vsubq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    return _mm_sub_epi32(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return lhs - rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) - CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) - CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) - CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) - CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXInt32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1], ... }`
CX_INLINE(CXInt32x4) CXInt32x4Multiply(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vmulq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    // SSE2 does not have a native multiply operation for 32bit ints
    __m128i temp0 = _mm_mul_epu32(lhs, rhs);
    __m128i temp1 = _mm_mul_epu32(_mm_srli_si128(lhs, 4), _mm_srli_si128(rhs, 4));
    return _mm_unpacklo_epi32(
        _mm_shuffle_epi32(temp0, _MM_SHUFFLE(0, 0, 2, 0)),
        _mm_shuffle_epi32(temp1, _MM_SHUFFLE(0, 0, 2, 0))
    );
#elif defined(CX_EXT_VECTOR)
    return lhs * rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) * CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) * CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) * CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) * CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXInt32x4) CXInt32x4BitwiseNot(const CXInt32x4 operand)
{
#if defined(CX_NEON_128)
    return vmvnq_s32(operand);
#elif defined(CX_X86_128)
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#elif defined(CX_EXT_VECTOR)
    return ~operand;
#else
    return (CXInt32x4){ .val = [
         ~CXInt32x4GetElement(operand, 0),
         ~CXInt32x4GetElement(operand, 1),
         ~CXInt32x4GetElement(operand, 2),
         ~CXInt32x4GetElement(operand, 3)
    ]};
#endif
}

/// Bitwise And
CX_INLINE(CXInt32x4) CXInt32x4BitwiseAnd(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vandq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    return _mm_and_si128(rhs, lhs);
#elif defined(CX_EXT_VECTOR)
    return lhs & rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) & CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) & CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) & CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) & CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

/// Bitwise And Not
CX_INLINE(CXInt32x4) CXInt32x4BitwiseAndNot(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return CXInt32x4BitwiseAnd(CXInt32x4BitwiseNot(lhs), rhs);
#elif defined(CX_X86_128)
    return _mm_andnot_si128(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return CXInt32x4BitwiseNot(lhs) & rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4BitwiseNot(CXInt32x4GetElement(lhs, 0)) & CXInt32x4GetElement(rhs, 0),
         CXInt32x4BitwiseNot(CXInt32x4GetElement(lhs, 1)) & CXInt32x4GetElement(rhs, 1),
         CXInt32x4BitwiseNot(CXInt32x4GetElement(lhs, 2)) & CXInt32x4GetElement(rhs, 2),
         CXInt32x4BitwiseNot(CXInt32x4GetElement(lhs, 3)) & CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

/// Bitwise Or
CX_INLINE(CXInt32x4) CXInt32x4BitwiseOr(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vorrq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    return _mm_or_si128(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return lhs | rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) | CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) | CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) | CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) | CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXInt32x4) CXInt32x4BitwiseExclusiveOr(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return veorq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    return _mm_xor_si128(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return lhs ^ rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) ^ CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) ^ CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) ^ CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) ^ CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt32x4) CXInt32x4ShiftElementWiseLeft(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return vshlq_s32(lhs, rhs);
#elif defined(CX_X86_128)
    return _mm_sll_epi32(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return (CXInt32x4){
         CXInt32x4GetElement(lhs, 0) << CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) << CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) << CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) << CXInt32x4GetElement(rhs, 3)
    };
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) << CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) << CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) << CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) << CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt32x4) CXInt32x4ShiftLeft(const CXInt32x4 lhs, const Int32 rhs)
{
#if defined(CX_NEON_128) || defined(CX_X86_128)
    return CXInt32x4ShiftElementWiseLeft(lhs, CXInt32x4MakeRepeatingElement(rhs));
#elif defined(CX_EXT_VECTOR)
    return lhs << rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) << rhs,
         CXInt32x4GetElement(lhs, 1) << rhs,
         CXInt32x4GetElement(lhs, 2) << rhs,
         CXInt32x4GetElement(lhs, 3) << rhs
    ]};
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt32x4) CXInt32x4ShiftElementWiseRight(const CXInt32x4 lhs, const CXInt32x4 rhs)
{
#if defined(CX_NEON_128)
    return CXInt32x4ShiftElementWiseLeft(lhs, CXInt32x4Negate(rhs));
#elif defined(CX_X86_128)
    return _mm_sra_epi32(lhs, rhs);
#elif defined(CX_EXT_VECTOR)
    return (CXInt32x4){
         CXInt32x4GetElement(lhs, 0) >> CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) >> CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) >> CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) >> CXInt32x4GetElement(rhs, 3)
    };
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) >> CXInt32x4GetElement(rhs, 0),
         CXInt32x4GetElement(lhs, 1) >> CXInt32x4GetElement(rhs, 1),
         CXInt32x4GetElement(lhs, 2) >> CXInt32x4GetElement(rhs, 2),
         CXInt32x4GetElement(lhs, 3) >> CXInt32x4GetElement(rhs, 3)
    ]};
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt32x4) CXInt32x4ShiftRight(const CXInt32x4 lhs, const Int32 rhs)
{
#if defined(CX_NEON_128) || defined(CX_X86_128)
    return CXInt32x4ShiftElementWiseRight(lhs, CXInt32x4MakeRepeatingElement(rhs));
#elif defined(CX_EXT_VECTOR)
    return lhs >> rhs;
#else
    return (CXInt32x4){ .val = [
         CXInt32x4GetElement(lhs, 0) >> rhs,
         CXInt32x4GetElement(lhs, 1) >> rhs,
         CXInt32x4GetElement(lhs, 2) >> rhs,
         CXInt32x4GetElement(lhs, 3) >> rhs
    ]};
#endif
}

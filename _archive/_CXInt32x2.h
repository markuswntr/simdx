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
#include "CXUInt32x2.h"


/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXInt32x2){ element0, element1 }`
CX_INLINE(CXInt32x2) CXInt32x2Make(Int32 element0, Int32 element1)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXInt32x2){ element0, element1 };
#elif CX_X86_128
    return _mm_setr_epi32(element0, element1, 0, 0);
#else
    CXInt32x2 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXInt32x2){ pointer[0], pointer[1] }`
CX_INLINE(CXInt32x2) CXInt32x2MakeLoad(const Int32* pointer)
{
#if CX_NEON_128
    return vld1_s32(pointer);
#elif CX_X86_128
    return _mm_loadu_si64(pointer); // Loads only the first 64 bits using given pointer and zeros the rest.
#else
    return CXInt32x2Make( pointer[0], pointer[1] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXInt32x2){ value, value }`
CX_INLINE(CXInt32x2) CXInt32x2MakeRepeatingElement(const Int32 value)
{
#if CX_NEON_128
    return vdup_n_s32(value);
#elif CX_X86_128
    return CXInt32x2Make(value, value);
#elif CX_EXT_VECTOR
    return (CXInt32x2)(value);
#else
    return CXInt32x2Make( value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXInt32x2){ 0, 0 }`
CX_INLINE(CXInt32x2) CXInt32x2MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXInt32x2MakeRepeatingElement(0);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Int32) CXInt32x2GetElement(const CXInt32x2 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((Int32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXInt32x2SetElement(CXInt32x2* storage, const int index, const Int32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Int32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE(CXInt32x2) CXInt32x2FromCXFloat32x2(CXFloat32x2 operand)
{
#if CX_NEON_128
    return vcvt_s32_f32(operand);
#elif CX_X86_128
    return _mm_cvtps_epi32(operand);
#elif CX_EXT_VECTOR
    return CXInt32x2Make( (Int32)nearbyintf(operand[0]), (Int32)nearbyintf(operand[1]) );
#else
    return CXInt32x2Make( (Int32)nearbyintf(operand.val[0]), (Int32)nearbyintf(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE(CXInt32x2) CXInt32x2FromCXUInt32x2(CXUInt32x2 operand)
{
#if CX_NEON_128
    return vreinterpret_s32_u32(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt32x2);
#else
    return CXInt32x2Make( (Int32)(operand.val[0]), (Int32)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE(CXInt32x2) CXInt32x2FromCXFloat64x2(CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvt_s32_f32(vcvt_f32_f64(operand));
#elif CX_X86_128
    return _mm_cvtpd_epi32(operand);
#elif CX_EXT_VECTOR
    return CXInt32x2Make( (Int32)nearbyint(operand[0]), (Int32)nearbyint(operand[1]) );
#else
    return CXInt32x2Make( (Int32)nearbyint(operand.val[0]), (Int32)nearbyint(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE(CXInt32x2) CXInt32x2FromCXInt64x2(CXInt64x2 operand)
{
#if CX_NEON_128
    return vmovn_s64(operand);
#elif CX_X86_128
    // There is no int64 to int32 conversion, so we are going int64 - (hacky way) -> float64 -> int32
    // This is so stupid... Maybe move to an unpack solution later on.
    //  Only works for inputs in the range: [-2^51, 2^51]
    /// https://stackoverflow.com/questions/41144668/how-to-efficiently-perform-double-int64-conversions-with-sse-avx
    CXInt64x2 temp = _mm_add_epi64(operand, _mm_castpd_si128(_mm_set1_pd(0x0018000000000000)));
    CXFloat64x2 converted = _mm_sub_pd(_mm_castsi128_pd(temp), _mm_set1_pd(0x0018000000000000));
    return CXInt32x2FromCXFloat64x2(converted);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt32x2);
#else
    return CXInt32x2Make( (Int32)(operand.val[0]), (Int32)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE(CXInt32x2) CXInt32x2FromCXUInt64x2(CXUInt64x2 operand)
{
#if CX_NEON_128
    return vreinterpret_s32_u32(vmovn_u64(operand));
#elif CX_X86_128
    // There is no int64 to int32 conversion, so we are going uint64 - (hacky way) -> float64 -> int32
    // This is so stupid... Maybe move to an unpack solution later on.
    //  Only works for inputs in the range: [-2^51, 2^51]
    /// https://stackoverflow.com/questions/41144668/how-to-efficiently-perform-double-int64-conversions-with-sse-avx
    CXInt64x2 temp = _mm_or_si128(operand, _mm_castpd_si128(_mm_set1_pd(0x0010000000000000)));
    CXFloat64x2 converted = _mm_sub_pd(_mm_castsi128_pd(temp), _mm_set1_pd(0x0010000000000000));
    return CXInt32x2FromCXFloat64x2(converted);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt32x2);
#else
    return CXInt32x2Make( (Int32)(operand.val[0]), (Int32)(operand.val[1]) );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXInt32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXInt32x2) CXInt32x2Minimum(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vmin_s32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_min_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    Int32 lhs0 = CXInt32x2GetElement(lhs, 0);
    Int32 rhs0 = CXInt32x2GetElement(rhs, 0);
    Int32 lhs1 = CXInt32x2GetElement(lhs, 1);
    Int32 rhs1 = CXInt32x2GetElement(rhs, 1);
    return CXInt32x2Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXInt32x2) CXInt32x2Maximum(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vmax_s32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_max_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    Int32 lhs0 = CXInt32x2GetElement(lhs, 0);
    Int32 rhs0 = CXInt32x2GetElement(rhs, 0);
    Int32 lhs1 = CXInt32x2GetElement(lhs, 1);
    Int32 rhs1 = CXInt32x2GetElement(rhs, 1);
    return CXInt32x2Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXInt32x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE(CXInt32x2) CXInt32x2Negate(const CXInt32x2 operand)
{
#if CX_NEON_128
    return vneg_s32(operand);
#elif CX_X86_128
    return _mm_sub_epi32(CXInt32x2MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXInt32x2Make(
        -CXInt32x2GetElement(operand, 0),
        -CXInt32x2GetElement(operand, 1)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXUInt32x2) CXInt32x2Absolute(const CXInt32x2 operand)
{
#if CX_NEON_128
    return vreinterpret_u32_s32(vabs_s32(operand));
#elif CX_X86_128
    #define CXInt32x2BitwiseExclusiveOr(operand) _mm_xor_si128(operand, CXInt32x2MakeRepeatingElement(0xFFFFFFFF))
    #define CXInt32x2CompareEqual(lhs, rhs) CXInt32x2BitwiseExclusiveOr(_mm_cmplt_epi32(lhs, rhs))
    CXInt32x2 comparison = CXInt32x2CompareEqual(operand, CXInt32x2MakeZero());
    CXInt32x2 negated = CXInt32x2Negate(operand);
    return _mm_xor_si128(negated, _mm_and_si128(comparison, _mm_xor_si128(operand, negated)));
    #undef CXInt32x2CompareEqual
    #undef CXInt32x2BitwiseExclusiveOr
#else
    return CXUInt32x2Make(
        abs(CXInt32x2GetElement(operand, 0)),
        abs(CXInt32x2GetElement(operand, 1))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXInt32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXInt32x2) CXInt32x2Add(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vadd_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) + CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) + CXInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXInt32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXInt32x2) CXInt32x2Subtract(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vsub_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) - CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) - CXInt32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXInt32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXInt32x2) CXInt32x2Multiply(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vmul_s32(lhs, rhs);
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
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) * CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) * CXInt32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXInt32x2) CXInt32x2BitwiseNot(const CXInt32x2 operand)
{
#if CX_NEON_128
    return vmvn_s32(operand);
#elif CX_X86_128
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXInt32x2Make(
        ~CXInt32x2GetElement(operand, 0),
        ~CXInt32x2GetElement(operand, 1)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXInt32x2) CXInt32x2BitwiseAnd(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vand_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) & CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) & CXInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXInt32x2) CXInt32x2BitwiseAndNot(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return CXInt32x2BitwiseAnd(CXInt32x2BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXInt32x2BitwiseNot(lhs) & rhs;
#else
    CXInt32x2 notLhs = CXInt32x2BitwiseNot(lhs);
    return CXInt32x2Make(
        CXInt32x2GetElement(notLhs, 0) & CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(notLhs, 1) & CXInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXInt32x2) CXInt32x2BitwiseOr(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vorr_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) | CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) | CXInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXInt32x2) CXInt32x2BitwiseExclusiveOr(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return veor_s32(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) ^ CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) ^ CXInt32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt32x2) CXInt32x2ShiftElementWiseLeft(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return vshl_s32(lhs, rhs);
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) << CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) << CXInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt32x2) CXInt32x2ShiftLeft(const CXInt32x2 lhs, const Int32 rhs)
{
#if CX_NEON_128
    return CXInt32x2ShiftElementWiseLeft(lhs, CXInt32x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) << rhs,
        CXInt32x2GetElement(lhs, 1) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt32x2) CXInt32x2ShiftElementWiseRight(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
#if CX_NEON_128
    return CXInt32x2ShiftElementWiseLeft(lhs, CXInt32x2Negate(rhs));
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) >> CXInt32x2GetElement(rhs, 0),
        CXInt32x2GetElement(lhs, 1) >> CXInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt32x2) CXInt32x2ShiftRight(const CXInt32x2 lhs, const Int32 rhs)
{
#if CX_NEON_128
    return CXInt32x2ShiftElementWiseRight(lhs, CXInt32x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srai_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXInt32x2Make(
        CXInt32x2GetElement(lhs, 0) >> rhs,
        CXInt32x2GetElement(lhs, 1) >> rhs
    );
#endif
}

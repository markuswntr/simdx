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
/// @return `(CXUInt32x2){ element0, element1 }`
CX_INLINE(CXUInt32x2) CXUInt32x2Make(UInt32 element0, UInt32 element1)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXUInt32x2){ element0, element1 };
#elif CX_X86_128
    return _mm_setr_epi32(element0, element1, 0, 0);
#else
    CXUInt32x2 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXUInt32x2){ pointer[0], pointer[1] }`
CX_INLINE(CXUInt32x2) CXUInt32x2MakeLoad(const UInt32* pointer)
{
#if CX_NEON_128
    return vld1_u32(pointer);
#elif CX_X86_128
    return _mm_loadu_si64(pointer); // Loads only the first 64 bits using given pointer and zeros the rest.
#else
    return CXUInt32x2Make( pointer[0], pointer[1] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXUInt32x2){ value, value }`
CX_INLINE(CXUInt32x2) CXUInt32x2MakeRepeatingElement(const UInt32 value)
{
#if CX_NEON_128
    return vdup_n_u32(value);
#elif CX_X86_128
    return CXUInt32x2Make(value, value);
#elif CX_EXT_VECTOR
    return (CXUInt32x2)(value);
#else
    return CXUInt32x2Make( value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXUInt32x2){ 0, 0 }`
CX_INLINE(CXUInt32x2) CXUInt32x2MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXUInt32x2MakeRepeatingElement(0);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(UInt32) CXUInt32x2GetElement(const CXUInt32x2 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((UInt32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXUInt32x2SetElement(CXUInt32x2* storage, const int index, const UInt32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Int32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE(CXUInt32x2) CXUInt32x2FromCXFloat32x2(CXFloat32x2 operand)
{
#if CX_NEON_128
    return vcvt_u32_f32(operand);
#elif CX_X86_128
    return _mm_cvtps_epi32(operand);
#elif CX_EXT_VECTOR
    return CXUInt32x2Make( (UInt32)nearbyintf(operand[0]), (UInt32)nearbyintf(operand[1]) );
#else
    return CXUInt32x2Make( (UInt32)nearbyintf(operand.val[0]), (UInt32)nearbyintf(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE(CXUInt32x2) CXUInt32x2FromCXInt32x2(CXInt32x2 operand)
{
#if CX_NEON_128
    return vreinterpret_u32_s32(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt32x2);
#else
    return CXUInt32x2Make( (UInt32)(operand.val[0]), (UInt32)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE(CXUInt32x2) CXUInt32x2FromCXFloat64x2(CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvt_u32_f32(vcvt_f32_f64(operand));
#elif CX_X86_128
    return _mm_cvtpd_epi32(operand);
#elif CX_EXT_VECTOR
    return CXUInt32x2Make( (UInt32)nearbyint(operand[0]), (UInt32)nearbyint(operand[1]) );
#else
    return CXUInt32x2Make( (UInt32)nearbyint(operand.val[0]), (UInt32)nearbyint(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE(CXUInt32x2) CXUInt32x2FromCXInt64x2(CXInt64x2 operand)
{
#if CX_NEON_128
    return vreinterpret_u32_s32(vmovn_s64(operand));
#elif CX_X86_128
    // There is no int64 to int32 conversion, so we are going int64 - (hacky way) -> float64 -> int32
    // This is so stupid... Maybe move to an unpack solution later on.
    //  Only works for inputs in the range: [-2^51, 2^51]
    /// https://stackoverflow.com/questions/41144668/how-to-efficiently-perform-double-int64-conversions-with-sse-avx
    CXInt64x2 temp = _mm_add_epi64(operand, _mm_castpd_si128(_mm_set1_pd(0x0018000000000000)));
    CXFloat64x2 converted = _mm_sub_pd(_mm_castsi128_pd(temp), _mm_set1_pd(0x0018000000000000));
    return CXUInt32x2FromCXFloat64x2(converted);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt32x2);
#else
    return CXUInt32x2Make( (UInt32)(operand.val[0]), (UInt32)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE(CXUInt32x2) CXUInt32x2FromCXUInt64x2(CXUInt64x2 operand)
{
#if CX_NEON_128
    return vmovn_u64(operand);
#elif CX_X86_128
    // There is no int64 to int32 conversion, so we are going uint64 - (hacky way) -> float64 -> int32
    // This is so stupid... Maybe move to an unpack solution later on.
    //  Only works for inputs in the range: [-2^51, 2^51]
    /// https://stackoverflow.com/questions/41144668/how-to-efficiently-perform-double-int64-conversions-with-sse-avx
    CXInt64x2 temp = _mm_or_si128(operand, _mm_castpd_si128(_mm_set1_pd(0x0010000000000000)));
    CXFloat64x2 converted = _mm_sub_pd(_mm_castsi128_pd(temp), _mm_set1_pd(0x0010000000000000));
    return CXUInt32x2FromCXFloat64x2(converted);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt32x2);
#else
    return CXUInt32x2Make( (UInt32)(operand.val[0]), (UInt32)(operand.val[1]) );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXUInt32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXUInt32x2) CXUInt32x2Minimum(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vmin_s32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_min_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    UInt32 lhs0 = CXUInt32x2GetElement(lhs, 0), rhs0 = CXUInt32x2GetElement(rhs, 0);
    UInt32 lhs1 = CXUInt32x2GetElement(lhs, 1), rhs1 = CXUInt32x2GetElement(rhs, 1);
    return CXUInt32x2Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXUInt32x2) CXUInt32x2Maximum(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vmax_s32(lhs, rhs);
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_cvtpd_epi32(_mm_max_pd(_mm_cvtepi32_pd(lhs), _mm_cvtepi32_pd(rhs)));
#else
    UInt32 lhs0 = CXUInt32x2GetElement(lhs, 0), rhs0 = CXUInt32x2GetElement(rhs, 0);
    UInt32 lhs1 = CXUInt32x2GetElement(lhs, 1), rhs1 = CXUInt32x2GetElement(rhs, 1);
    return CXUInt32x2Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1
    );
#endif
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXUInt32x2) CXUInt32x2Absolute(const CXUInt32x2 operand)
{
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXUInt32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXUInt32x2) CXUInt32x2Add(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vadd_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) + CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) + CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXUInt32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXUInt32x2) CXUInt32x2Subtract(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vsub_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) - CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) - CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXUInt32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXUInt32x2) CXUInt32x2Multiply(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vmul_u32(lhs, rhs);
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
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) * CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) * CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXUInt32x2) CXUInt32x2BitwiseNot(const CXUInt32x2 operand)
{
#if CX_NEON_128
    return vmvn_u32(operand);
#elif CX_X86_128
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXUInt32x2Make(
        ~CXUInt32x2GetElement(operand, 0),
        ~CXUInt32x2GetElement(operand, 1)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXUInt32x2) CXUInt32x2BitwiseAnd(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vand_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) & CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) & CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXUInt32x2) CXUInt32x2BitwiseAndNot(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return CXUInt32x2BitwiseAnd(CXUInt32x2BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXUInt32x2BitwiseNot(lhs) & rhs;
#else
    CXUInt32x2 notLhs = CXUInt32x2BitwiseNot(lhs);
    return CXUInt32x2Make(
        CXUInt32x2GetElement(notLhs, 0) & CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(notLhs, 1) & CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXUInt32x2) CXUInt32x2BitwiseOr(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vorr_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) | CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) | CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXUInt32x2) CXUInt32x2BitwiseExclusiveOr(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return veor_u32(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) ^ CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) ^ CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt32x2) CXUInt32x2ShiftElementWiseLeft(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return vshl_u32(lhs, rhs);
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) << CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) << CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt32x2) CXUInt32x2ShiftLeft(const CXUInt32x2 lhs, const UInt32 rhs)
{
#if CX_NEON_128
    return CXUInt32x2ShiftElementWiseLeft(lhs, CXUInt32x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) << rhs,
        CXUInt32x2GetElement(lhs, 1) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt32x2) CXUInt32x2ShiftElementWiseRight(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
#if CX_NEON_128
    return CXUInt32x2ShiftElementWiseLeft(lhs, vreinterpret_u32_s32(vneg_s32(vreinterpret_s32_u32(rhs))));
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) >> CXUInt32x2GetElement(rhs, 0),
        CXUInt32x2GetElement(lhs, 1) >> CXUInt32x2GetElement(rhs, 1)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt32x2) CXUInt32x2ShiftRight(const CXUInt32x2 lhs, const UInt32 rhs)
{
#if CX_NEON_128
    return CXUInt32x2ShiftElementWiseRight(lhs, CXUInt32x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srai_epi32(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXUInt32x2Make(
        CXUInt32x2GetElement(lhs, 0) >> rhs,
        CXUInt32x2GetElement(lhs, 1) >> rhs
    );
#endif
}

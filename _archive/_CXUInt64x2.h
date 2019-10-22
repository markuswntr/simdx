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

#include "types.h"

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXUInt64x2){ element0, element1 }`
CX_INLINE(CXUInt64x2) CXUInt64x2Make(UInt64 element0, UInt64 element1)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXUInt64x2){ element0, element1 };
#elif CX_X86_128
    return _mm_setr_epi64((__m64)(element0), (__m64)(element1)); // _mm_cvtsi64_m64
#else
    CXUInt64x2 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXUInt64x2){ pointer[0], pointer[1] }`
CX_INLINE(CXUInt64x2) CXUInt64x2MakeLoad(const UInt64* pointer)
{
#if CX_NEON_128
    return vld1q_u64(pointer);
#elif CX_X86_128
    return _mm_loadu_si128((const __m128i*)pointer);
#else
    return CXUInt64x2Make( pointer[0], pointer[1] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXUInt64x2){ value, value }`
CX_INLINE(CXUInt64x2) CXUInt64x2MakeRepeatingElement(const UInt64 value)
{
#if CX_NEON_128
    return vdupq_n_u64(value);
#elif CX_X86_128
    return _mm_set1_epi64((__m64)(value));
#elif CX_EXT_VECTOR
    return (CXUInt64x2)(value);
#else
    return CXUInt64x2Make( value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0).
/// @return `(CXUInt64x2){ 0, 0 }`
CX_INLINE(CXUInt64x2) CXUInt64x2MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXUInt64x2MakeRepeatingElement(0L);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(UInt64) CXUInt64x2GetElement(const CXUInt64x2 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((UInt64*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXUInt64x2SetElement(CXUInt64x2* storage, const int index, const UInt64 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((UInt64*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE(CXUInt64x2) CXUInt64x2FromCXFloat32x2(CXFloat32x2 operand)
{
#if CX_NEON_128
    return vmovl_u32(vcvt_u32_f32(operand));
#elif CX_X86_128
    return (__m128i)__builtin_convertvector( // TODO: NO intel intrinsic?
        __builtin_shufflevector((__v4sf)operand, (__v4sf)operand, 0, 1), __v2di);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt64x2);
#else
    return CXUInt64x2Make( (UInt64)(operand.val[0]), (UInt64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE(CXUInt64x2) CXUInt64x2FromCXInt32x2(CXInt32x2 operand)
{
#if CX_NEON_128
    return vreinterpretq_u64_s64(vmovl_s32(operand));
#elif CX_X86_128
    return (__m128i)__builtin_convertvector( // TODO: NO intel intrinsic?
        __builtin_shufflevector((__v4si)operand, (__v4si)operand, 0, 1), __v2di);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt64x2);
#else
    return CXUInt64x2Make( (UInt64)(operand.val[0]), (UInt64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE(CXUInt64x2) CXUInt64x2FromCXUInt32x2(CXUInt32x2 operand)
{
#if CX_NEON_128
    return vmovl_u32(operand);
#elif CX_X86_128
    return CXUInt64x2FromCXInt32x2(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt64x2);
#else
    return CXUInt64x2Make( (UInt64)(operand.val[0]), (UInt64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE(CXUInt64x2) CXUInt64x2FromCXFloat64x2(CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_u64_f64(operand);
#elif CX_EXT_VECTOR || CX_X86_128
    return CXUInt64x2Make( (UInt64)nearbyint(operand[0]), (UInt64)nearbyint(operand[1]) );
#else
    return CXUInt64x2Make( (UInt64)nearbyint(operand.val[0]), (UInt64)nearbyint(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXUInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE(CXUInt64x2) CXUInt64x2FromCXInt64x2(CXInt64x2 operand)
{
#if CX_NEON_128
    return vreinterpretq_u64_s64(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXUInt64x2);
#else
    return CXUInt64x2Make( (UInt64)(operand.val[0]), (UInt64)(operand.val[1]) );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXUInt64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXUInt64x2) CXUInt64x2Minimum(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_u64_f64(vminq_f64(vcvtq_f64_u64(lhs), vcvtq_f64_u64(rhs)));
#elif CX_X86_128
    return _mm_castpd_si128(_mm_min_pd(_mm_castsi128_pd(lhs), _mm_castsi128_pd(rhs)));
#else
    UInt64 lhs0 = CXUInt64x2GetElement(lhs, 0), rhs0 = CXUInt64x2GetElement(rhs, 0);
    UInt64 lhs1 = CXUInt64x2GetElement(lhs, 1), rhs1 = CXUInt64x2GetElement(rhs, 1);
    return CXUInt64x2Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXUInt64x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXUInt64x2) CXUInt64x2Maximum(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_u64_f64(vmaxq_f64(vcvtq_f64_u64(lhs), vcvtq_f64_u64(rhs)));
#elif CX_X86_128
    return _mm_castpd_si128(_mm_max_pd(_mm_castsi128_pd(lhs), _mm_castsi128_pd(rhs)));
#else
    UInt64 lhs0 = CXUInt64x2GetElement(lhs, 0), rhs0 = CXUInt64x2GetElement(rhs, 0);
    UInt64 lhs1 = CXUInt64x2GetElement(lhs, 1), rhs1 = CXUInt64x2GetElement(rhs, 1);
    return CXUInt64x2Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1
    );
#endif
}

// MARK: - Arithmetics

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXUInt64x2) CXUInt64x2Absolute(const CXUInt64x2 operand)
{
    return operand;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXUInt64x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXUInt64x2) CXUInt64x2Add(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return vaddq_u64(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) + CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) + CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXUInt64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXUInt64x2) CXUInt64x2Subtract(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return vsubq_u64(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) - CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) - CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXUInt64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXUInt64x2) CXUInt64x2Multiply(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) * CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) * CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXUInt64x2) CXUInt64x2BitwiseNot(const CXUInt64x2 operand)
{
#if CX_NEON_128
    return veorq_u64(operand, CXUInt64x2MakeRepeatingElement(-1LL));
#elif CX_X86_128
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXUInt64x2Make(
        ~CXUInt64x2GetElement(operand, 0),
        ~CXUInt64x2GetElement(operand, 1)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXUInt64x2) CXUInt64x2BitwiseAnd(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return vandq_u64(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXUInt64x2Make(
         CXUInt64x2GetElement(lhs, 0) & CXUInt64x2GetElement(rhs, 0),
         CXUInt64x2GetElement(lhs, 1) & CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXUInt64x2) CXUInt64x2BitwiseAndNot(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return CXUInt64x2BitwiseAnd(CXUInt64x2BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXUInt64x2BitwiseNot(lhs) & rhs;
#else
    CXUInt64x2 notLhs = CXUInt64x2BitwiseNot(lhs);
    return CXUInt64x2Make(
        CXUInt64x2GetElement(notLhs, 0) & CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(notLhs, 1) & CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXUInt64x2) CXUInt64x2BitwiseOr(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return vorrq_u64(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) | CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) | CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXUInt64x2) CXUInt64x2BitwiseExclusiveOr(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return veorq_u64(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) ^ CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) ^ CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt64x2) CXUInt64x2ShiftElementWiseLeft(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return vshlq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_sll_epi64(lhs, rhs);
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) << CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) << CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt64x2) CXUInt64x2ShiftLeft(const CXUInt64x2 lhs, const UInt64 rhs)
{
#if CX_NEON_128
    return CXUInt64x2ShiftElementWiseLeft(lhs, CXUInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) << rhs,
        CXUInt64x2GetElement(lhs, 1) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXUInt64x2) CXUInt64x2ShiftElementWiseRight(const CXUInt64x2 lhs, const CXUInt64x2 rhs)
{
#if CX_NEON_128
    return CXUInt64x2ShiftElementWiseLeft(lhs, vreinterpretq_u64_s64(vnegq_s64(vreinterpretq_s64_u64(rhs))));
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) >> CXUInt64x2GetElement(rhs, 0),
        CXUInt64x2GetElement(lhs, 1) >> CXUInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXUInt64x2) CXUInt64x2ShiftRight(const CXUInt64x2 lhs, const UInt64 rhs)
{
#if CX_NEON_128
    return CXUInt64x2ShiftElementWiseRight(lhs, CXUInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srli_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXUInt64x2Make(
        CXUInt64x2GetElement(lhs, 0) >> rhs,
        CXUInt64x2GetElement(lhs, 1) >> rhs
    );
#endif
}

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
#include "CXUInt64x2.h"

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXInt64x2){ element0, element1 }`
CX_INLINE(CXInt64x2) CXInt64x2Make(Int64 element0, Int64 element1)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXInt64x2){ element0, element1 };
#elif CX_X86_128
    return _mm_setr_epi64(_mm_cvtsi64_m64(element0), _mm_cvtsi64_m64(element1));
#else
    CXInt64x2 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXInt64x2){ pointer[0], pointer[1] }`
CX_INLINE(CXInt64x2) CXInt64x2MakeLoad(const Int64* pointer)
{
#if CX_NEON_128
    return vld1q_s64(pointer);
#elif CX_X86_128
    return _mm_loadu_si128((const __m128i*)pointer);
#else
    return CXInt64x2Make( pointer[0], pointer[1] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXInt64x2){ value, value }`
CX_INLINE(CXInt64x2) CXInt64x2MakeRepeatingElement(const Int64 value)
{
#if CX_NEON_128
    return vdupq_n_s64(value);
#elif CX_X86_128
    return _mm_set1_epi64(_mm_cvtsi64_m64(value));
#elif CX_EXT_VECTOR
    return (CXInt64x2)(value);
#else
    return CXInt64x2Make( value, value );
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0).
/// @return `(CXInt64x2){ 0, 0 }`
CX_INLINE(CXInt64x2) CXInt64x2MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_si128();
#else
    return CXInt64x2MakeRepeatingElement(0L);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Int64) CXInt64x2GetElement(const CXInt64x2 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((Int64*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXInt64x2SetElement(CXInt64x2* storage, const int index, const Int64 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Int64*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE(CXInt64x2) CXInt64x2FromCXFloat32x2(CXFloat32x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_s64_f64(vcvt_f64_f32(operand));
#elif CX_X86_128
    return (__m128i)__builtin_convertvector( // TODO: NO intel intrinsic?
        __builtin_shufflevector((__v4sf)operand, (__v4sf)operand, 0, 1), __v2di);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt64x2);
#else
    return CXInt64x2Make( (Int64)(operand.val[0]), (Int64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE(CXInt64x2) CXInt64x2FromCXInt32x2(CXInt32x2 operand)
{
#if CX_NEON_128
    return vmovl_s32(operand);
#elif CX_X86_128
    return (__m128i)__builtin_convertvector( // TODO: NO intel intrinsic?
        __builtin_shufflevector((__v4si)operand, (__v4si)operand, 0, 1), __v2di);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt64x2);
#else
    return CXInt64x2Make( (Int64)(operand.val[0]), (Int64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE(CXInt64x2) CXInt64x2FromCXUInt32x2(CXUInt32x2 operand)
{
#if CX_NEON_128
    return vmovl_u32(operand);
#elif CX_X86_128
    return CXInt64x2FromCXInt32x2(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt64x2);
#else
    return CXInt64x2Make( (Int64)(operand.val[0]), (Int64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE(CXInt64x2) CXInt64x2FromCXFloat64x2(CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_s64_f64(operand);
#elif CX_EXT_VECTOR || CX_X86_128
    return __builtin_convertvector(operand, CXInt64x2);
#else
    return CXInt64x2Make( (Int64)(operand.val[0]), (Int64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXInt64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE(CXInt64x2) CXInt64x2FromCXUInt64x2(CXUInt64x2 operand)
{
#if CX_NEON_128
    return vreinterpretq_s64_u64(operand);
#elif CX_X86_128
    return operand;
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXInt64x2);
#else
    return CXInt64x2Make( (Int64)(operand.val[0]), (Int64)(operand.val[1]) );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXInt64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXInt64x2) CXInt64x2Minimum(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_s64_f64(vminq_f64(vcvtq_f64_s64(lhs), vcvtq_f64_s64(rhs)));
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_castpd_si128(_mm_min_pd(_mm_castsi128_pd(lhs), _mm_castsi128_pd(rhs)));
#else
    Int64 lhs0 = CXInt64x2GetElement(lhs, 0), rhs0 = CXInt64x2GetElement(rhs, 0);
    Int64 lhs1 = CXInt64x2GetElement(lhs, 1), rhs1 = CXInt64x2GetElement(rhs, 1);
    return CXInt64x2Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXInt64x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXInt64x2) CXInt64x2Maximum(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_s64_f64(vmaxq_f64(vcvtq_f64_s64(lhs), vcvtq_f64_s64(rhs)));
//#elif CX_X86_128
//    // SSE2 has no min on Int32, but on Float64 so use it temporarily
//    return _mm_castpd_si128(_mm_max_pd(_mm_castsi128_pd(lhs), _mm_castsi128_pd(rhs)));
#else
    Int64 lhs0 = CXInt64x2GetElement(lhs, 0), rhs0 = CXInt64x2GetElement(rhs, 0);
    Int64 lhs1 = CXInt64x2GetElement(lhs, 1), rhs1 = CXInt64x2GetElement(rhs, 1);
    return CXInt64x2Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXInt64x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE(CXInt64x2) CXInt64x2Negate(const CXInt64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vnegq_s64(operand);
#elif CX_X86_128
    return _mm_sub_epi64(CXInt64x2MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXInt64x2Make(
        -CXInt64x2GetElement(operand, 0),
        -CXInt64x2GetElement(operand, 1)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXUInt64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXUInt64x2) CXInt64x2Absolute(const CXInt64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vreinterpretq_u64_s64(vabsq_s64(operand));
#else
    return CXUInt64x2Make(
        llabs(CXInt64x2GetElement(operand, 0)),
        llabs(CXInt64x2GetElement(operand, 1))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXInt64x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXInt64x2) CXInt64x2Add(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return vaddq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_add_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) + CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) + CXInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXInt64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXInt64x2) CXInt64x2Subtract(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return vsubq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) - CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) - CXInt64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXInt64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXInt64x2) CXInt64x2Multiply(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) * CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) * CXInt64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: - Bitwise

/// Bitwise Not
CX_INLINE(CXInt64x2) CXInt64x2BitwiseNot(const CXInt64x2 operand)
{
#if CX_NEON_128
    return veorq_s64(operand, CXInt64x2MakeRepeatingElement(-1LL));
#elif CX_X86_128
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
#elif CX_EXT_VECTOR
    return ~operand;
#else
    return CXInt64x2Make(
        ~CXInt64x2GetElement(operand, 0),
        ~CXInt64x2GetElement(operand, 1)
    );
#endif
}

/// Bitwise And
CX_INLINE(CXInt64x2) CXInt64x2BitwiseAnd(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return vandq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_and_si128(rhs, lhs);
#elif CX_EXT_VECTOR
    return lhs & rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) & CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) & CXInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise And Not
CX_INLINE(CXInt64x2) CXInt64x2BitwiseAndNot(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return CXInt64x2BitwiseAnd(CXInt64x2BitwiseNot(lhs), rhs);
#elif CX_X86_128
    return _mm_andnot_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return CXInt64x2BitwiseNot(lhs) & rhs;
#else
    CXInt64x2 notLhs = CXInt64x2BitwiseNot(lhs);
    return CXInt64x2Make(
        CXInt64x2GetElement(notLhs, 0) & CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(notLhs, 1) & CXInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Or
CX_INLINE(CXInt64x2) CXInt64x2BitwiseOr(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return vorrq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_or_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs | rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) | CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) | CXInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Bitwise Exclusive Or
CX_INLINE(CXInt64x2) CXInt64x2BitwiseExclusiveOr(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return veorq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_xor_si128(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs ^ rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) ^ CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) ^ CXInt64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Shifting

/// Left-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt64x2) CXInt64x2ShiftElementWiseLeft(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return vshlq_s64(lhs, rhs);
#elif CX_X86_128
    return _mm_sll_epi64(lhs, rhs);
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) << CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) << CXInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Left-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt64x2) CXInt64x2ShiftLeft(const CXInt64x2 lhs, const Int64 rhs)
{
#if CX_NEON_128
    return CXInt64x2ShiftElementWiseLeft(lhs, CXInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_slli_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs << rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) << rhs,
        CXInt64x2GetElement(lhs, 1) << rhs
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits in each lane of rhs.
CX_INLINE(CXInt64x2) CXInt64x2ShiftElementWiseRight(const CXInt64x2 lhs, const CXInt64x2 rhs)
{
#if CX_NEON_128
    return CXInt64x2ShiftElementWiseLeft(lhs, CXInt64x2Negate(rhs));
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) >> CXInt64x2GetElement(rhs, 0),
        CXInt64x2GetElement(lhs, 1) >> CXInt64x2GetElement(rhs, 1)
    );
#endif
}

/// Right-shifts each element in the storage operand by the specified number of bits.
CX_INLINE(CXInt64x2) CXInt64x2ShiftRight(const CXInt64x2 lhs, const Int64 rhs)
{
#if CX_NEON_128
    return CXInt64x2ShiftElementWiseRight(lhs, CXInt64x2MakeRepeatingElement(rhs));
#elif CX_X86_128
    return _mm_srli_epi64(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs >> rhs;
#else
    return CXInt64x2Make(
        CXInt64x2GetElement(lhs, 0) >> rhs,
        CXInt64x2GetElement(lhs, 1) >> rhs
    );
#endif
}

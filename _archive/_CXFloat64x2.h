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
#include <math.h>

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXFloat64x2){ element0, element1 }`
CX_INLINE(CXFloat64x2) CXFloat64x2Make(Float64 element0, Float64 element1)
{
#if CX_NEON_128_WITH_AARCH64 || CX_EXT_VECTOR
    return (CXFloat64x2){ element0, element1 };
#elif CX_X86_128
    return _mm_setr_pd(element0, element1);
#else
    CXFloat64x2 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXFloat64x2){ pointer[0], pointer[1] }`
CX_INLINE(CXFloat64x2) CXFloat64x2MakeLoad(const Float64* pointer)
{
#if CX_NEON_128_WITH_AARCH64
    return vld1q_f64(pointer);
#elif CX_X86_128
    return _mm_loadu_pd(pointer);
#else
    return CXFloat64x2Make( pointer[0], pointer[1] );
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXFloat64x2){ value, value }`
CX_INLINE(CXFloat64x2) CXFloat64x2MakeRepeatingElement(const Float64 value)
{
#if CX_NEON_128_WITH_AARCH64
    return vdupq_n_f64(value);
#elif CX_X86_128
    return _mm_set1_pd(value);
#elif CX_EXT_VECTOR
    return (CXFloat64x2)(value);
#else
    return CXFloat64x2Make( value, value );
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0.f).
/// @return `(CXFloat64x2){ 0.f, 0.f }`
CX_INLINE(CXFloat64x2) CXFloat64x2MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_pd();
#else
    return CXFloat64x2MakeRepeatingElement(0.f);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Float64) CXFloat64x2GetElement(const CXFloat64x2 storage, const int index)
{
#if CX_NEON_128_WITH_AARCH64 || CX_X86_128 || CX_EXT_VECTOR
    return ((Float64*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXFloat64x2SetElement(CXFloat64x2* storage, const int index, const Float64 value)
{
#if CX_NEON_128_WITH_AARCH64 || CX_X86_128 || CX_EXT_VECTOR
    ((Float64*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2FromCXFloat32x2(CXFloat32x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvt_f64_f32(operand);
#elif CX_X86_128
    return _mm_cvtps_pd(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat64x2);
#else
    return CXFloat64x2Make( (Float64)(operand.val[0]), (Float64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2FromCXInt32x2(CXInt32x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return CXFloat64x2FromCXFloat32x2(vreinterpret_f32_s32(operand));
#elif CX_X86_128
    return _mm_cvtepi32_pd(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat64x2);
#else
    return CXFloat64x2Make( (Float64)(operand.val[0]), (Float64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2FromCXUInt32x2(CXUInt32x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return CXFloat64x2FromCXFloat32x2(vreinterpret_f32_u32(operand));
#elif CX_X86_128
    return _mm_cvtepi32_pd(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat64x2);
#else
    return CXFloat64x2Make( (Float64)(operand.val[0]), (Float64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2FromCXInt64x2(CXInt64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_f64_s64(operand);
#elif CX_X86_128
//    return _mm_castsi128_pd(operand);
    return __builtin_convertvector(operand, CXFloat64x2);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat64x2);
#else
    return CXFloat64x2Make( (Float64)(operand.val[0]), (Float64)(operand.val[1]) );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2FromCXUInt64x2(CXUInt64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvtq_f64_u64(operand);
#elif CX_X86_128
//    return _mm_castsi128_pd(operand);
    return __builtin_convertvector(operand, CXFloat64x2);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat64x2);
#else
    return CXFloat64x2Make( (Float64)(operand.val[0]), (Float64)(operand.val[1]) );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXFloat64x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXFloat64x2) CXFloat64x2Minimum(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vminq_f64(lhs, rhs);
#elif CX_X86_128
    return _mm_min_pd(lhs, rhs);
#else
    Float64 lhs0 = CXFloat64x2GetElement(lhs, 0), rhs0 = CXFloat64x2GetElement(rhs, 0);
    Float64 lhs1 = CXFloat64x2GetElement(lhs, 1), rhs1 = CXFloat64x2GetElement(rhs, 1);

    return CXFloat64x2Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat64x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXFloat64x2) CXFloat64x2Maximum(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vmaxq_f64(lhs, rhs);
#elif CX_X86_128
    return _mm_max_pd(lhs, rhs);
#else
    Float64 lhs0 = CXFloat64x2GetElement(lhs, 0), rhs0 = CXFloat64x2GetElement(rhs, 0);
    Float64 lhs1 = CXFloat64x2GetElement(lhs, 1), rhs1 = CXFloat64x2GetElement(rhs, 1);

    return CXFloat64x2Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXFloat64x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2Negate(const CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vnegq_f64(operand);
#elif CX_X86_128
    return _mm_sub_pd(CXFloat64x2MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXFloat64x2Make(
        -CXFloat64x2GetElement(operand, 0),
        -CXFloat64x2GetElement(operand, 1)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXFloat64x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2Absolute(const CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vabsq_f64(operand);
#elif CX_X86_128
    __m64 SIGN_BIT = _mm_cvtsi64_m64(~(1LL << 63));
    union { CXFloat64x2 operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi64(SIGN_BIT, SIGN_BIT);
    return _mm_and_ps(operand, Signed.operand);
#else
    return CXFloat64x2Make(
        fabs(CXFloat64x2GetElement(operand, 0)),
        fabs(CXFloat64x2GetElement(operand, 1))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXFloat64x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXFloat64x2) CXFloat64x2Add(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vaddq_f64(lhs, rhs);
#elif CX_X86_128
    return _mm_add_pd(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXFloat64x2Make(
        CXFloat64x2GetElement(lhs, 0) + CXFloat64x2GetElement(rhs, 0),
        CXFloat64x2GetElement(lhs, 1) + CXFloat64x2GetElement(rhs, 1)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXFloat64x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXFloat64x2) CXFloat64x2Subtract(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vsubq_f64(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_pd(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXFloat64x2Make(
        CXFloat64x2GetElement(lhs, 0) - CXFloat64x2GetElement(rhs, 0),
        CXFloat64x2GetElement(lhs, 1) - CXFloat64x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXFloat64x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXFloat64x2) CXFloat64x2Multiply(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vmulq_f64(lhs, rhs);
#elif CX_X86_128
    return _mm_mul_pd(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXFloat64x2Make(
        CXFloat64x2GetElement(lhs, 0) * CXFloat64x2GetElement(rhs, 0),
        CXFloat64x2GetElement(lhs, 1) * CXFloat64x2GetElement(rhs, 1)
    );
#endif
}

/// Divides two storages (element-wise).
/// @return `(CXFloat64x2){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE(CXFloat64x2) CXFloat64x2Divide(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vdivq_f64(lhs, rhs);
#elif CX_X86_128
    return _mm_div_pd(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs / rhs;
#else
    return CXFloat64x2Make(
        CXFloat64x2GetElement(lhs, 0) / CXFloat64x2GetElement(rhs, 0),
        CXFloat64x2GetElement(lhs, 1) / CXFloat64x2GetElement(rhs, 1)
    );
#endif
}

/// Calculates the square root (element-wise).
/// @return `(CXFloat64x2){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE(CXFloat64x2) CXFloat64x2SquareRoot(const CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vsqrtq_f64(operand);
#elif CX_X86_128
    return _mm_sqrt_pd(operand);
#else
    return CXFloat64x2Make(
       sqrt(CXFloat64x2GetElement(operand, 0)),
       sqrt(CXFloat64x2GetElement(operand, 1))
    );
#endif
}

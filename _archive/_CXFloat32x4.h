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
/// @return `(CXFloat32x4){ element0, element1, element2, element3 }`
CX_INLINE(CXFloat32x4)
CXFloat32x4Make(Float32 element0, Float32 element1, Float32 element2, Float32 element3)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXFloat32x4){ element0, element1, element2, element3 };
#elif CX_X86_128
    return _mm_setr_ps(element0, element1, element2, element3);
#else
    CXFloat32x4 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    storage.val[2] = element2;
    storage.val[3] = element3;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXFloat32x4){ pointer[0], pointer[1], pointer[2], pointer[3] }`
CX_INLINE(CXFloat32x4) CXFloat32x4MakeLoad(const Float32* pointer)
{
#if CX_NEON_128
    return vld1q_f32(pointer);
#elif CX_X86_128
    return _mm_loadu_ps(pointer);
#else
    return CXFloat32x4Make(pointer[0], pointer[1], pointer[2], pointer[3]);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXFloat32x4){ value, value, value, value }`
CX_INLINE(CXFloat32x4) CXFloat32x4MakeRepeatingElement(const Float32 value)
{
#if CX_NEON_128
    return vdupq_n_f32(value);
#elif CX_X86_128
    return _mm_set1_ps(value);
#elif CX_EXT_VECTOR
    return (CXFloat32x4)(value);
#else
    return CXFloat32x4Make(value, value, value, value);
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0.f).
/// @return `(CXFloat32x4){ 0.f, 0.f, 0.f, 0.f }`
CX_INLINE(CXFloat32x4) CXFloat32x4MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_ps();
#else
    return CXFloat32x4MakeRepeatingElement(0.f);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Float32) CXFloat32x4GetElement(const CXFloat32x4 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((Float32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXFloat32x4SetElement(CXFloat32x4* storage, const int index, const Float32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Float32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat32x4){ (Float32)(operand[0]), (Float32)(operand[1]), (Float32)(operand[2]), ... }`
CX_INLINE(CXFloat32x4) CXFloat32x4FromCXInt32x4(CXInt32x4 operand)
{
#if CX_NEON_128
    return vcvtq_f32_s32(operand);
#elif CX_X86_128
    return _mm_cvtepi32_ps(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x4);
#else
    return CXFloat32x4Make(
        (Float32)(operand.val[0]),
        (Float32)(operand.val[1]),
        (Float32)(operand.val[2]),
        (Float32)(operand.val[3])
    );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat32x4){ (Float32)(operand[0]), (Float32)(operand[1]), (Float32)(operand[2]), ... }`
CX_INLINE(CXFloat32x4) CXFloat32x4FromCXUInt32x4(CXUInt32x4 operand)
{
#if CX_NEON_128
    return vcvtq_f32_u32(operand);
#elif CX_X86_128
    return _mm_cvtepi32_ps(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x4);
#else
    return CXFloat32x4Make(
        (Float32)(operand.val[0]),
        (Float32)(operand.val[1]),
        (Float32)(operand.val[2]),
        (Float32)(operand.val[3])
    );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXFloat32x4){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXFloat32x4) CXFloat32x4Minimum(const CXFloat32x4 lhs, const CXFloat32x4 rhs)
{
#if CX_NEON_128
    return vminq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_min_ps(lhs, rhs);
#else
    Float32 lhs0 = CXFloat32x4GetElement(lhs, 0), rhs0 = CXFloat32x4GetElement(rhs, 0);
    Float32 lhs1 = CXFloat32x4GetElement(lhs, 1), rhs1 = CXFloat32x4GetElement(rhs, 1);
    Float32 lhs2 = CXFloat32x4GetElement(lhs, 2), rhs2 = CXFloat32x4GetElement(rhs, 2);
    Float32 lhs3 = CXFloat32x4GetElement(lhs, 3), rhs3 = CXFloat32x4GetElement(rhs, 3);
    return CXFloat32x4Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1,
        lhs2 < rhs2 ? lhs2 : rhs2,
        lhs3 < rhs3 ? lhs3 : rhs3
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat32x4){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXFloat32x4) CXFloat32x4Maximum(const CXFloat32x4 lhs, const CXFloat32x4 rhs)
{
#if CX_NEON_128
    return vmaxq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_max_ps(lhs, rhs);
#else
    Float32 lhs0 = CXFloat32x4GetElement(lhs, 0), rhs0 = CXFloat32x4GetElement(rhs, 0);
    Float32 lhs1 = CXFloat32x4GetElement(lhs, 1), rhs1 = CXFloat32x4GetElement(rhs, 1);
    Float32 lhs2 = CXFloat32x4GetElement(lhs, 2), rhs2 = CXFloat32x4GetElement(rhs, 2);
    Float32 lhs3 = CXFloat32x4GetElement(lhs, 3), rhs3 = CXFloat32x4GetElement(rhs, 3);
    return CXFloat32x4Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1,
        lhs2 > rhs2 ? lhs2 : rhs2,
        lhs3 > rhs3 ? lhs3 : rhs3
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXFloat32x4){ -(operand[0]), -(operand[1]), -(operand[2]), -(operand[3]) }`
CX_INLINE(CXFloat32x4) CXFloat32x4Negate(const CXFloat32x4 operand)
{
#if CX_NEON_128
    return vnegq_f32(operand);
#elif CX_X86_128
    return _mm_sub_ps(CXFloat32x4MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXFloat32x4Make(
        -CXFloat32x4GetElement(operand, 0),
        -CXFloat32x4GetElement(operand, 1),
        -CXFloat32x4GetElement(operand, 2),
        -CXFloat32x4GetElement(operand, 3)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXFloat32x4){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXFloat32x4) CXFloat32x4Absolute(const CXFloat32x4 operand)
{
#if CX_NEON_128
    return vabsq_f32(operand);
#elif CX_X86_128
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { CXFloat32x4 operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    return _mm_and_ps(operand, Signed.operand);
#else
    return CXFloat32x4Make(
        fabsf(CXFloat32x4GetElement(operand, 0)),
        fabsf(CXFloat32x4GetElement(operand, 1)),
        fabsf(CXFloat32x4GetElement(operand, 2)),
        fabsf(CXFloat32x4GetElement(operand, 3))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXFloat32x4){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXFloat32x4) CXFloat32x4Add(const CXFloat32x4 lhs, const CXFloat32x4 rhs)
{
#if CX_NEON_128
    return vaddq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXFloat32x4Make(
        CXFloat32x4GetElement(lhs, 0) + CXFloat32x4GetElement(rhs, 0),
        CXFloat32x4GetElement(lhs, 1) + CXFloat32x4GetElement(rhs, 1),
        CXFloat32x4GetElement(lhs, 2) + CXFloat32x4GetElement(rhs, 2),
        CXFloat32x4GetElement(lhs, 3) + CXFloat32x4GetElement(rhs, 3)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXFloat32x4){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXFloat32x4) CXFloat32x4Subtract(const CXFloat32x4 lhs, const CXFloat32x4 rhs)
{
#if CX_NEON_128
    return vsubq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXFloat32x4Make(
        CXFloat32x4GetElement(lhs, 0) - CXFloat32x4GetElement(rhs, 0),
        CXFloat32x4GetElement(lhs, 1) - CXFloat32x4GetElement(rhs, 1),
        CXFloat32x4GetElement(lhs, 2) - CXFloat32x4GetElement(rhs, 2),
        CXFloat32x4GetElement(lhs, 3) - CXFloat32x4GetElement(rhs, 3)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXFloat32x4){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXFloat32x4) CXFloat32x4Multiply(const CXFloat32x4 lhs, const CXFloat32x4 rhs)
{
#if CX_NEON_128
    return vmulq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_mul_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXFloat32x4Make(
        CXFloat32x4GetElement(lhs, 0) * CXFloat32x4GetElement(rhs, 0),
        CXFloat32x4GetElement(lhs, 1) * CXFloat32x4GetElement(rhs, 1),
        CXFloat32x4GetElement(lhs, 2) * CXFloat32x4GetElement(rhs, 2),
        CXFloat32x4GetElement(lhs, 3) * CXFloat32x4GetElement(rhs, 3)
    );
#endif
}

/// Divides two storages (element-wise).
/// @return `(CXFloat32x4){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE(CXFloat32x4) CXFloat32x4Divide(const CXFloat32x4 lhs, const CXFloat32x4 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    return vdivq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_div_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs / rhs;
#else
    return CXFloat32x4Make(
        CXFloat32x4GetElement(lhs, 0) / CXFloat32x4GetElement(rhs, 0),
        CXFloat32x4GetElement(lhs, 1) / CXFloat32x4GetElement(rhs, 1),
        CXFloat32x4GetElement(lhs, 2) / CXFloat32x4GetElement(rhs, 2),
        CXFloat32x4GetElement(lhs, 3) / CXFloat32x4GetElement(rhs, 3)
    );
#endif
}

/// Calculates the square root (element-wise).
/// @return `(CXFloat32x4){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE(CXFloat32x4) CXFloat32x4SquareRoot(const CXFloat32x4 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vsqrtq_f32(operand);
#elif CX_X86_128
    return _mm_sqrt_ps(operand);
#else
    return CXFloat32x4Make(
       sqrtf(CXFloat32x4GetElement(operand, 0)),
       sqrtf(CXFloat32x4GetElement(operand, 1)),
       sqrtf(CXFloat32x4GetElement(operand, 2)),
       sqrtf(CXFloat32x4GetElement(operand, 3))
    );
#endif
}

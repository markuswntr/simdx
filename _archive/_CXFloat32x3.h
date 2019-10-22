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
/// @return `(CXFloat32x3){ element0, element1, element2 }`
CX_INLINE(CXFloat32x3) CXFloat32x3Make(Float32 element0, Float32 element1, Float32 element2)
{
#if CX_NEON_128
    return (CXFloat32x3){ element0, element1, element2, 0.f };
#elif CX_X86_128
    return _mm_setr_ps(element0, element1, element2, 0.f);
#elif CX_EXT_VECTOR
    return (CXFloat32x3){ element0, element1, element2 };
#else
    CXFloat32x3 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    storage.val[2] = element2;
    return storage;
#endif
}

/// Loads elements from an unaligned memory pointer.
/// @return `(CXFloat32x3){ pointer[0], pointer[1], pointer[2] }`
CX_INLINE(CXFloat32x3) CXFloat32x3MakeLoad(const Float32* pointer)
{
#if CX_NEON_128
    CXFloat32x3 storage = vld1q_f32(pointer);
    vsetq_lane_u32(0.f, storage, 3);
    return storage;
#elif CX_X86_128
    CXFloat32x4 storage =  _mm_loadu_ps(pointer);
    storage[3] = 0.f; // zero-out the last float32
    return storage;
#else
    return CXFloat32x3Make(pointer[0], pointer[1], pointer[2]);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXFloat32x3){ value, value, value }`
CX_INLINE(CXFloat32x3) CXFloat32x3MakeRepeatingElement(const Float32 value)
{
#if CX_NEON_128
    CXFloat32x3 storage = vdupq_n_f32(value);
    vsetq_lane_u32(0.f, storage, 3);
    return storage;
#elif CX_X86_128
    CXFloat32x3 storage = _mm_set1_ps(value);
    storage[3] = 0.f; // zero-out the last float32
    return storage;
#elif CX_EXT_VECTOR
    return (CXFloat32x3)(value);
#else
    return CXFloat32x3Make(value, value, value);
#endif
}

/// Returns an intrinsic type with all lanes initialized to zero (0.f).
/// @return `(CXFloat32x3){ 0.f, 0.f, 0.f }`
CX_INLINE(CXFloat32x3) CXFloat32x3MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_ps();
#else
    return CXFloat32x3MakeRepeatingElement(0.f);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Float32) CXFloat32x3GetElement(const CXFloat32x3 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((Float32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXFloat32x3SetElement(CXFloat32x3* storage, const int index, const Float32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Float32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat32x3){ (Float32)(operand[0]), (Float32)(operand[1]), (Float32)(operand[2]) }`
CX_INLINE(CXFloat32x3) CXFloat32x3FromCXInt32x3(CXInt32x3 operand)
{
#if CX_NEON_128
    return vcvtq_f32_s32(operand);
#elif CX_X86_128
    return _mm_cvtepi32_ps(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x3);
#else
    return CXFloat32x3Make(
        (Float32)(operand.val[0]),
        (Float32)(operand.val[1]),
        (Float32)(operand.val[2])
    );
#endif
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(CXFloat32x3){ (Float32)(operand[0]), (Float32)(operand[1]), (Float32)(operand[2]) }`
CX_INLINE(CXFloat32x3) CXFloat32x3FromCXUInt32x3(CXUInt32x3 operand)
{
#if CX_NEON_128
    return vcvtq_f32_u32(operand);
#elif CX_X86_128
    return _mm_cvtepi32_ps(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x3);
#else
    return CXFloat32x3Make(
        (Float32)(operand.val[0]),
        (Float32)(operand.val[1]),
        (Float32)(operand.val[2])
    );
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXFloat32x3){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXFloat32x3) CXFloat32x3Minimum(const CXFloat32x3 lhs, const CXFloat32x3 rhs)
{
#if CX_NEON_128
    return vminq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_min_ps(lhs, rhs);
#else
    Float32 lhs0 = CXFloat32x3GetElement(lhs, 0), rhs0 = CXFloat32x3GetElement(rhs, 0);
    Float32 lhs1 = CXFloat32x3GetElement(lhs, 1), rhs1 = CXFloat32x3GetElement(rhs, 1);
    Float32 lhs2 = CXFloat32x3GetElement(lhs, 2), rhs2 = CXFloat32x3GetElement(rhs, 2);
    return CXFloat32x3Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1,
        lhs2 < rhs2 ? lhs2 : rhs2
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXFloat32x3) CXFloat32x3Maximum(const CXFloat32x3 lhs, const CXFloat32x3 rhs)
{
#if CX_NEON_128
    return vmaxq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_max_ps(lhs, rhs);
#else
    Float32 lhs0 = CXFloat32x3GetElement(lhs, 0), rhs0 = CXFloat32x3GetElement(rhs, 0);
    Float32 lhs1 = CXFloat32x3GetElement(lhs, 1), rhs1 = CXFloat32x3GetElement(rhs, 1);
    Float32 lhs2 = CXFloat32x3GetElement(lhs, 2), rhs2 = CXFloat32x3GetElement(rhs, 2);
    return CXFloat32x3Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1,
        lhs2 > rhs2 ? lhs2 : rhs2
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXFloat32x3){ -(operand[0]), -(operand[1]), -(operand[2]) }`
CX_INLINE(CXFloat32x3) CXFloat32x3Negate(const CXFloat32x3 operand)
{
#if CX_NEON_128
    return vnegq_f32(operand);
#elif CX_X86_128
    return _mm_sub_ps(CXFloat32x3MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXFloat32x3Make(
        -CXFloat32x3GetElement(operand, 0),
        -CXFloat32x3GetElement(operand, 1),
        -CXFloat32x3GetElement(operand, 2)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXFloat32x3){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXFloat32x3) CXFloat32x3Absolute(const CXFloat32x3 operand)
{
#if CX_NEON_128
    return vabsq_f32(operand);
#elif CX_X86_128
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { CXFloat32x4 operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    return _mm_and_ps(operand, Signed.operand);
#else
    return CXFloat32x3Make(
        fabsf(CXFloat32x3GetElement(operand, 0)),
        fabsf(CXFloat32x3GetElement(operand, 1)),
        fabsf(CXFloat32x3GetElement(operand, 2))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXFloat32x3){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXFloat32x3) CXFloat32x3Add(const CXFloat32x3 lhs, const CXFloat32x3 rhs)
{
#if CX_NEON_128
    return vaddq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXFloat32x3Make(
        CXFloat32x3GetElement(lhs, 0) + CXFloat32x3GetElement(rhs, 0),
        CXFloat32x3GetElement(lhs, 1) + CXFloat32x3GetElement(rhs, 1),
        CXFloat32x3GetElement(lhs, 2) + CXFloat32x3GetElement(rhs, 2)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXFloat32x3){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXFloat32x3) CXFloat32x3Subtract(const CXFloat32x3 lhs, const CXFloat32x3 rhs)
{
#if CX_NEON_128
    return vsubq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXFloat32x3Make(
        CXFloat32x3GetElement(lhs, 0) - CXFloat32x3GetElement(rhs, 0),
        CXFloat32x3GetElement(lhs, 1) - CXFloat32x3GetElement(rhs, 1),
        CXFloat32x3GetElement(lhs, 2) - CXFloat32x3GetElement(rhs, 2)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXFloat32x3){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXFloat32x3) CXFloat32x3Multiply(const CXFloat32x3 lhs, const CXFloat32x3 rhs)
{
#if CX_NEON_128
    return vmulq_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_mul_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXFloat32x3Make(
        CXFloat32x3GetElement(lhs, 0) * CXFloat32x3GetElement(rhs, 0),
        CXFloat32x3GetElement(lhs, 1) * CXFloat32x3GetElement(rhs, 1),
        CXFloat32x3GetElement(lhs, 2) * CXFloat32x3GetElement(rhs, 2)
    );
#endif
}

/// Divides two storages (element-wise).
/// @return `(CXFloat32x3){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE(CXFloat32x3) CXFloat32x3Divide(const CXFloat32x3 lhs, CXFloat32x3 rhs)
{
#if CX_NEON_128_WITH_AARCH64
    vsetq_lane_f32(1.f, rhs, 3);
    return vdivq_f32(lhs, rhs);
#elif CX_X86_128
    CXFloat32x3SetElement(&rhs, 3, 1.f); // Avoid division by zero, but 1 instead
    return _mm_div_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs / rhs;
#else
    return CXFloat32x3Make(
        CXFloat32x3GetElement(lhs, 0) / CXFloat32x3GetElement(rhs, 0),
        CXFloat32x3GetElement(lhs, 1) / CXFloat32x3GetElement(rhs, 1),
        CXFloat32x3GetElement(lhs, 2) / CXFloat32x3GetElement(rhs, 2)
    );
#endif
}

/// Calculates the square root (element-wise).
/// @return `(CXFloat32x3){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE(CXFloat32x3) CXFloat32x3SquareRoot(const CXFloat32x3 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vsqrtq_f32(operand);
#elif CX_X86_128
    return _mm_sqrt_ps(operand);
#else
    return CXFloat32x3Make(
       sqrtf(CXFloat32x3GetElement(operand, 0)),
       sqrtf(CXFloat32x3GetElement(operand, 1)),
       sqrtf(CXFloat32x3GetElement(operand, 2))
    );
#endif
}

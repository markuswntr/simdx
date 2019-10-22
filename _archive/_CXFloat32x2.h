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

#include <math.h>
#include "types.h"

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(CXFloat32x2){ element0, element1 }`
CX_INLINE(CXFloat32x2) CXFloat32x2Make(Float32 element0, Float32 element1)
{
#if CX_NEON_128 || CX_EXT_VECTOR
    return (CXFloat32x2){ element0, element1 };
#elif CX_X86_128
    return _mm_setr_ps(element0, element1, 0.f, 0.f);
#else
    CXFloat32x2 storage;
    storage.val[0] = element0;
    storage.val[1] = element1;
    return storage;
#endif
}

/// Loads 2 x Float32 values from unaligned memory.
/// @return `(CXFloat32x2){ pointer[0], pointer[1] }`
CX_INLINE(CXFloat32x2) CXFloat32x2MakeLoad(const Float32* pointer)
{
#if CX_NEON_128
    return vld1_f32(pointer);
#elif CX_X86_128
    CXFloat32x4 storage = _mm_loadu_ps(pointer);
    storage[2] = storage[3] = 0.f; // zero-out the last 2 x float32 bits
    return storage;
#else
    return CXFloat32x2Make(pointer[0], pointer[1]);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(CXFloat32x2){ value, value }`
CX_INLINE(CXFloat32x2) CXFloat32x2MakeRepeatingElement(const Float32 value)
{
#if CX_NEON_128
    return vdup_n_f32(value);
#elif CX_X86_128
    CXFloat32x4 storage = _mm_set1_ps(value);
    storage[2] = storage[3] = 0.f; // zero-out the last 2 x float32 bits
    return storage;
#elif CX_EXT_VECTOR
    return (CXFloat32x2)(value);
#else
    return CXFloat32x2Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0.f).
/// @return `(CXFloat32x2){ 0.f, 0.f }`
CX_INLINE(CXFloat32x2) CXFloat32x2MakeZero(void)
{
#if CX_X86_128
    return _mm_setzero_ps();
#else
    return CXFloat32x2MakeRepeatingElement(0.f);
#endif
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE(Float32) CXFloat32x2GetElement(const CXFloat32x2 storage, const int index)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    return ((Float32*)&(storage))[index];
#else
    return storage.val[index];
#endif
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE(void) CXFloat32x2SetElement(CXFloat32x2* storage, const int index, const Float32 value)
{
#if CX_NEON_128 || CX_X86_128 || CX_EXT_VECTOR
    ((Float32*)storage)[index] = value;
#else
    (*storage).val[index] = value;
#endif
}

// MARK: - Conversion

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(CXFloat32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2FromCXFloat64x2(CXFloat64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return vcvt_f32_f64(operand);
#elif CX_X86_128
    return _mm_cvtpd_ps(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x2);
#else
    return CXFloat32x2Make((Float32)(operand.val[0]), (Float32)(operand.val[1]));
#endif
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(CXFloat32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2FromCXInt32x2(CXInt32x2 operand)
{
#if CX_NEON_128
    return vcvt_f32_s32(operand);
#elif CX_X86_128
    return _mm_cvtepi32_ps(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x2);
#else
    return CXFloat32x2Make((Float32)(operand.val[0]), (Float32)(operand.val[1]));
#endif
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(CXFloat32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2FromCXUInt32x2(CXUInt32x2 operand)
{
#if CX_NEON_128
    return vcvt_f32_u32(operand);
#elif CX_X86_128
    return CXFloat32x2FromCXInt32x2(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x2);
#else
    return CXFloat32x2Make((Float32)(operand.val[0]), (Float32)(operand.val[1]));
#endif
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(CXFloat32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2FromCXInt64x2(CXInt64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return CXFloat32x2FromCXFloat64x2(vreinterpretq_f64_s64(operand));
#elif CX_X86_128
    //  Only works for inputs in the range: [-2^51, 2^51]
    /// https://stackoverflow.com/questions/41144668/how-to-efficiently-perform-double-int64-conversions-with-sse-avx
    CXInt64x2 temp = _mm_add_epi64(operand, _mm_castpd_si128(_mm_set1_pd(0x0018000000000000)));
    CXFloat64x2 converted = _mm_sub_pd(_mm_castsi128_pd(temp), _mm_set1_pd(0x0018000000000000));
    return CXFloat32x2FromCXFloat64x2(converted);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x2);
#else
    return CXFloat32x2Make((Float32)(operand.val[0]), (Float32)(operand.val[1]));
#endif
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(CXFloat32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2FromCXUInt64x2(CXUInt64x2 operand)
{
#if CX_NEON_128_WITH_AARCH64
    return CXFloat32x2FromCXFloat64x2(vreinterpretq_f64_u64(operand));
#elif CX_X86_128
    return CXFloat32x2FromCXInt64x2(operand);
#elif CX_EXT_VECTOR
    return __builtin_convertvector(operand, CXFloat32x2);
#else
    return CXFloat32x2Make((Float32)(operand.val[0]), (Float32)(operand.val[1]));
#endif
}

// MARK: Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(CXFloat32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE(CXFloat32x2) CXFloat32x2Minimum(const CXFloat32x2 lhs, const CXFloat32x2 rhs)
{
#if CX_NEON_128
    return vmin_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_min_ps(lhs, rhs);
#else
    Float32 lhs0 = CXFloat32x2GetElement(lhs, 0);
    Float32 rhs0 = CXFloat32x2GetElement(rhs, 0);
    Float32 lhs1 = CXFloat32x2GetElement(lhs, 1);
    Float32 rhs1 = CXFloat32x2GetElement(rhs, 1);
    return CXFloat32x2Make(
        lhs0 < rhs0 ? lhs0 : rhs0,
        lhs1 < rhs1 ? lhs1 : rhs1
    );
#endif
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE(CXFloat32x2) CXFloat32x2Maximum(const CXFloat32x2 lhs, const CXFloat32x2 rhs)
{
#if CX_NEON_128
    return vmax_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_max_ps(lhs, rhs);
#else
    Float32 lhs0 = CXFloat32x2GetElement(lhs, 0);
    Float32 rhs0 = CXFloat32x2GetElement(rhs, 0);
    Float32 lhs1 = CXFloat32x2GetElement(lhs, 1);
    Float32 rhs1 = CXFloat32x2GetElement(rhs, 1);
    return CXFloat32x2Make(
        lhs0 > rhs0 ? lhs0 : rhs0,
        lhs1 > rhs1 ? lhs1 : rhs1
    );
#endif
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(CXFloat32x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2Negate(const CXFloat32x2 operand)
{
#if CX_NEON_128
    return vneg_f32(operand);
#elif CX_X86_128
    return _mm_sub_ps(CXFloat32x2MakeZero(), operand);
#elif CX_EXT_VECTOR
    return -(operand);
#else
    return CXFloat32x2Make(
        -CXFloat32x2GetElement(operand, 0),
        -CXFloat32x2GetElement(operand, 1)
    );
#endif
}

/// Returns the absolute storage (element-wise).
/// @return `(CXFloat32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2Absolute(const CXFloat32x2 operand)
{
#if CX_NEON_128
    return vabs_f32(operand);
#elif CX_X86_128
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { CXFloat32x4 operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    return _mm_and_ps(operand, Signed.operand);
#else
    return CXFloat32x2Make(
        fabsf(CXFloat32x2GetElement(operand, 0)),
        fabsf(CXFloat32x2GetElement(operand, 1))
    );
#endif
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(CXFloat32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE(CXFloat32x2) CXFloat32x2Add(const CXFloat32x2 lhs, const CXFloat32x2 rhs)
{
#if CX_NEON_128
    return vadd_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_add_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs + rhs;
#else
    return CXFloat32x2Make(
        CXFloat32x2GetElement(lhs, 0) + CXFloat32x2GetElement(rhs, 0),
        CXFloat32x2GetElement(lhs, 1) + CXFloat32x2GetElement(rhs, 1)
    );
#endif
}

/// Subtracts a storage from another (element-wise).
/// @return `(CXFloat32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE(CXFloat32x2) CXFloat32x2Subtract(const CXFloat32x2 lhs, const CXFloat32x2 rhs)
{
#if CX_NEON_128
    return vsub_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_sub_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs - rhs;
#else
    return CXFloat32x2Make(
        CXFloat32x2GetElement(lhs, 0) - CXFloat32x2GetElement(rhs, 0),
        CXFloat32x2GetElement(lhs, 1) - CXFloat32x2GetElement(rhs, 1)
    );
#endif
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(CXFloat32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE(CXFloat32x2) CXFloat32x2Multiply(const CXFloat32x2 lhs, const CXFloat32x2 rhs)
{
#if CX_NEON_128
    return vmul_f32(lhs, rhs);
#elif CX_X86_128
    return _mm_mul_ps(lhs, rhs);
#elif CX_EXT_VECTOR
    return lhs * rhs;
#else
    return CXFloat32x2Make(
        CXFloat32x2GetElement(lhs, 0) * CXFloat32x2GetElement(rhs, 0),
        CXFloat32x2GetElement(lhs, 1) * CXFloat32x2GetElement(rhs, 1)
    );
#endif
}

/// Divides two storages (element-wise).
/// @return `(CXFloat32x2){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE(CXFloat32x2) CXFloat32x2Divide(const CXFloat32x2 lhs, CXFloat32x2 rhs)
{
#if CX_NEON_128_WITH_AARCH
    return vdiv_f32(lhs, rhs);
#elif CX_X86_128
    CXFloat32x2SetElement(&rhs, 2, 1.f); // Prepare rhs value, to avoid a ...
    CXFloat32x2SetElement(&rhs, 3, 1.f); // ... division by zero, but 1 instead
    return _mm_div_ps(lhs, rhs);
#elif CX_EXT_VECTOR || CX_NEON_128 // TODO: No aarch64 nor extended vector support - improve
    return lhs / rhs;
#else
    return CXFloat32x2Make(
        CXFloat32x2GetElement(lhs, 0) / CXFloat32x2GetElement(rhs, 0),
        CXFloat32x2GetElement(lhs, 1) / CXFloat32x2GetElement(rhs, 1)
    );
#endif
}

/// Calculates the square root (element-wise).
/// @return `(CXFloat32x2){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE(CXFloat32x2) CXFloat32x2SquareRoot(const CXFloat32x2 operand)
{
#if CX_NEON_128_WITH_AARCH
    return vsqrt_f32(operand);
#elif CX_X86_128
    return _mm_sqrt_ps(operand);
#else
    return CXFloat32x2Make(
        sqrtf(CXFloat32x2GetElement(operand, 0)),
        sqrtf(CXFloat32x2GetElement(operand, 1))
    );
#endif
}

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

/// Absolute values are calculated via `fabsf` if intrinsics are not available (i.e. mmx or lower only)
#if !CX_ARM_ARCH && CX_X86_ARCH < CX_X86_LEVEL_SSE
#   include <math.h>
#endif

/// Convenience wrapper to a single intrinsic value return
#define RETURN_RESULT(value) { Float32x2 result; result.intrinsic = value; return result; }

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

// MARK: - Type

/// The 2 x 32-bit floating point storage
typedef union CXFloat32x2_t {

    /// The native intrinsic type
#if CX_ARM_ARCH
    float32x2_t intrinsic;
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    __m128 intrinsic; // __m128 has way more native operations than __m64. Last 64bit are unused.
#elif CX_X86_ARCH
    __m64 intrinsic;
#endif

    /// The array or vector extension equivalent to the intrinsic
#if CX_HAS_EXT_VECTOR
    Float32 array CX_VECTOR_SIZE(8);
#else
    Float32 array[2];
#endif
} Float32x2;

// MARK: - Make

/// Loads 2 x Float32 values from unaligned memory.
/// @return `(Float32x2){ pointer[0], pointer[1] }`
CX_INLINE Float32x2 CXFloat32x2MakeLoad(Float32 const pointer[2]) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vld1_f32(pointer);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_loadu_ps(pointer);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = pointer[i];
    }
#endif
    return result;
}

/// Initializes a storage to given elements, from least-significant to most-significant bits.
/// @return `(Float32x2){ element0, element1 }`
CX_INLINE Float32x2 CXFloat32x2Make(Float32 element0, Float32 element1) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE
    RETURN_RESULT(_mm_setr_ps(element0, element1, 0.f, 0.f))
#else
    Float32 values[] = { element0, element1 };
    return CXFloat32x2MakeLoad(values);
#endif
}

/// Returns an intrinsic type with all elements initialized to `value`.
/// @return `(Float32x2){ value, value }`
CX_INLINE Float32x2 CXFloat32x2MakeRepeatingElement(const Float32 value) {
#if CX_ARM_ARCH
    RETURN_RESULT(vdup_n_f32(value))
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    RETURN_RESULT(_mm_set1_ps(value))
#else
    return CXFloat32x2Make(value, value);
#endif
}

/// Returns an intrinsic type with all elements initialized to zero (0.f).
/// @return `(Float32x2){ 0.f, 0.f }`
CX_INLINE Float32x2 CXFloat32x2MakeZero(void) {
#if CX_X86_ARCH >= CX_X86_LEVEL_SSE
    RETURN_RESULT(_mm_setzero_ps())
#elif CX_X86_ARCH
    RETURN_RESULT(_mm_setzero_si64())
#else
    return CXFloat32x2MakeRepeatingElement(0.f);
#endif
}

// MARK: - Get/Set

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @return `storage[index]`
CX_INLINE Float32 CXFloat32x2GetElement(const Float32x2 storage, const int index) {
    return storage.array[index];
}

/// Sets the element at `index` from `storage` to given value, i.e. `(*storage)[index] = value;`
CX_INLINE void CXFloat32x2SetElement(Float32x2* storage, const int index, const Float32 value) {
    storage->array[index] = value;
}

// MARK: - Minimum & Maximum

/// Performs element-by-element comparison of both storages and returns the lesser of each pair in the result.
/// @return `(Float32x2){ lhs[0] < rhs[0] ? lhs[0] : rhs[0], lhs[1] < rhs[1] ? lhs[1] : rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Minimum(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmin_f32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_min_ps(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] < rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

/// Performs element-by-element comparison of both storages and returns the greater of each pair in the result.
/// @return `(CXFloat32x3){ lhs[0] > rhs[0] ? lhs[0] : rhs[0], lhs[1] > rhs[1] ? lhs[1] : rhs[1], ... }`
CX_INLINE Float32x2 CXFloat32x2Maximum(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmax_f32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_max_ps(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] > rhs.array[i] ? lhs.array[i] : rhs.array[i];
    }
#endif
    return result;
}

// MARK: - Arithmetics

/// Returns the negated storage (element-wise).
/// @return `(Float32x2){ -(operand[0]), -(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2Negate(const Float32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vneg_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_sub_ps(_mm_setzero_ps(), operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = -operand.array[i];
    }
#endif
    return result;
}

/// Returns the absolute storage (element-wise).
/// @return `(Float32x2){ abs(operand[0]), abs(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2Absolute(const Float32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vabs_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    uint32_t SIGN_BIT = (uint32_t)(~(1 << 31));
    union { __m128 operand; __m128i signs; } Signed;
    Signed.signs = _mm_setr_epi32(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    result.intrinsic = _mm_and_ps(operand.intrinsic, Signed.operand);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = fabsf(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Additive

/// Adds two storages (element-wise).
/// @return `(Float32x2){ lhs[0] + rhs[0], lhs[1] + rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Add(Float32x2 lhs, Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vadd_f32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_add_ps(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] + rhs.array[i];
    }
#endif
    return result;
}

/// Subtracts a storage from another (element-wise).
/// @return `(Float32x2){ lhs[0] - rhs[0], lhs[1] - rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Subtract(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vsub_f32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_sub_ps(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] - rhs.array[i];
    }
#endif
    return result;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise).
/// @return `(Float32x2){ lhs[0] * rhs[0], lhs[1] * rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Multiply(const Float32x2 lhs, const Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmul_f32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_mul_ps(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] * rhs.array[i];
    }
#endif
    return result;
}

/// Divides two storages (element-wise).
/// @return `(Float32x2){ lhs[0] / rhs[0], lhs[1] / rhs[1] }`
CX_INLINE Float32x2 CXFloat32x2Divide(const Float32x2 lhs, Float32x2 rhs) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vdiv_f32(lhs.intrinsic, rhs.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    CXFloat32x2SetElement(&rhs, 2, 1.f); // Prepare rhs values, to avoid a ...
    CXFloat32x2SetElement(&rhs, 3, 1.f); // ... division by zero, but 1 instead
    result.intrinsic = _mm_div_ps(lhs.intrinsic, rhs.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = lhs.array[i] / rhs.array[i];
    }
#endif
    return result;
}

/// Calculates the square root (element-wise).
/// @return `(Float32x2){ sqrt(operand[0]), sqrt(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2SquareRoot(const Float32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vsqrt_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_sqrt_ps(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = sqrtf(operand.array[i]);
    }
#endif
    return result;
}

#undef RETURN_RESULT
#undef SIZE

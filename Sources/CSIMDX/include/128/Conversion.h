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

/// Returns the number of numerics in given array
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

#if !CX_ARM_ARCH && CX_X86_ARCH < CX_X86_LEVEL_AVX_512
#   include <math.h>
#endif

// MARK: - Float32x3

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float32x3){ (Float32)(operand[0]), (Float32)(operand[1]), ... }`
CX_INLINE Float32x3 CXFloat32x3FromInt32x3(Int32x3 operand) {
    Float32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_f32_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtepi32_ps(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float32x3){ (Float32)(operand[0]), (Float32)(operand[1]), ... }`
CX_INLINE Float32x3 CXFloat32x3FromUInt32x3(UInt32x3 operand) {
    Float32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_f32_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtepi32_ps(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Int32x3

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x3){ (Int32)(operand[0]), (Int32)(operand[1]), ... }`
CX_INLINE Int32x3 CXInt32x3FromFloat32x3(Float32x3 operand) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_s32_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtps_epi32(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)nearbyintf(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x3){ (Int32)(operand[0]), (Int32)(operand[1]), ... }`
CX_INLINE Int32x3 CXInt32x3FromUInt32x3(UInt32x3 operand) {
    Int32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpretq_s32_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: UInt32x3

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x3){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]) }`
CX_INLINE UInt32x3 CXUInt32x3FromFloat32x3(Float32x3 operand) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_u32_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtps_epi32(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)nearbyintf(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x3){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]) }`
CX_INLINE UInt32x3 CXUInt32x3FromInt32x3(Int32x3 operand) {
    UInt32x3 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpretq_u32_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: - Float32x4

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float32x4){ (Float32)(operand[0]), (Float32)(operand[1]), ... }`
CX_INLINE Float32x4 CXFloat32x4FromInt32x4(Int32x4 operand) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_f32_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtepi32_ps(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float32x4){ (Float32)(operand[0]), (Float32)(operand[1]), ... }`
CX_INLINE Float32x4 CXFloat32x4FromUInt32x4(UInt32x4 operand) {
    Float32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_f32_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtepi32_ps(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Int32x4

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x4){ (Int32)(operand[0]), (Int32)(operand[1]), ... }`
CX_INLINE Int32x4 CXInt32x4FromFloat32x4(Float32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_s32_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtps_epi32(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)nearbyintf(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x4){ (Int32)(operand[0]), (Int32)(operand[1]), ... }`
CX_INLINE Int32x4 CXInt32x4FromUInt32x4(UInt32x4 operand) {
    Int32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpretq_s32_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: UInt32x4

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x4){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]) }`
CX_INLINE UInt32x4 CXUInt32x4FromFloat32x4(Float32x4 operand) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_u32_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtps_epi32(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)nearbyintf(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x4){ (UInt32)(operand[0]), (UInt32)(operand[1]), (UInt32)(operand[2]) }`
CX_INLINE UInt32x4 CXUInt32x4FromInt32x4(Int32x4 operand) {
    UInt32x4 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpretq_u32_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: - Float64x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float64x2){ (Float64)(operand[0]), (Float64)(operand[1]), ... }`
CX_INLINE Float64x2 CXFloat64x2FromInt64x2(Int64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_f64_s64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtepi64_pd(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float64)(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float64x2){ (Float64)(operand[0]), (Float64)(operand[1]), ... }`
CX_INLINE Float64x2 CXFloat64x2FromUInt64x2(UInt64x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_f64_u64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtepu64_pd(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float64)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Int64x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int64x2){ (Int64)(operand[0]), (Int64)(operand[1]), ... }`
CX_INLINE Int64x2 CXInt64x2FromFloat64x2(Float64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_s64_f64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtpd_epi64(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int64)nearbyint(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int64x2){ (Int64)(operand[0]), (Int64)(operand[1]), ... }`
CX_INLINE Int64x2 CXInt64x2FromUInt64x2(UInt64x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpretq_s64_u64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int64)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: UInt64x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]), (UInt64)(operand[2]) }`
CX_INLINE UInt64x2 CXUInt64x2FromFloat64x2(Float64x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvtq_u64_f64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtpd_epi64(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt64)nearbyint(operand.array[i]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]), (UInt64)(operand[2]) }`
CX_INLINE UInt64x2 CXUInt64x2FromInt64x2(Int64x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpretq_u64_s64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt64)(operand.array[i]);
    }
#endif
    return result;
}

#undef SIZE

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

#if CX_ARM_ARCH < CX_NEON_LEVEL_AARCH64 && CX_X86_ARCH
#   include <math.h>
#endif

// MARK: - 64x2 to 32x2 Bits

// MARK: Float32x2

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(Float32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2FromFloat64x2(Float64x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvt_f32_f64(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_cvtpd_ps(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[0]);
    }
#endif
    return result;
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(Float32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2FromInt64x2(Int64x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvt_f32_f64(vreinterpretq_f64_s64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[0]);
    }
#endif
    return result;
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(Float32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2FromUInt64x2(UInt64x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvt_f32_f64(vreinterpretq_f64_u64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[0]);
    }
#endif
    return result;
}

// MARK: Int32x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2FromFloat64x2(Float64x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvt_s32_f32(vcvt_f32_f64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)nearbyint(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2FromInt64x2(Int64x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmovn_s64(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2FromUInt64x2(UInt64x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpret_s32_u32(vmovn_u64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)(operand.array[0]);
    }
#endif
    return result;
}

// MARK: UInt32x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE UInt32x2 CXUInt32x2FromFloat64x2(Float64x2 operand) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvt_u32_f32(vcvt_f32_f64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)nearbyint(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE UInt32x2 CXUInt32x2FromInt64x2(Int64x2 operand) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpret_u32_s32(vmovn_s64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE UInt32x2 CXUInt32x2FromUInt64x2(UInt64x2 operand) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vmovn_u64(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)(operand.array[0]);
    }
#endif
    return result;
}

// MARK: - 32x2 to 64x2 Bits

// MARK: Float64x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2FromFloat32x2(Float32x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvt_f64_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_cvtps_pd(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float64)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2FromInt32x2(Int32x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvt_f64_f32(vreinterpret_f32_s32(operand.intrinsic));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE2
    result.intrinsic = _mm_cvtpi32_pd(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float64)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Float64x2){ (Float64)(operand[0]), (Float64)(operand[1]) }`
CX_INLINE Float64x2 CXFloat64x2FromUInt32x2(UInt32x2 operand) {
    Float64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvt_f64_f32(vreinterpret_f32_u32(operand.intrinsic));
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtepu32_pd(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float64)(operand.array[0]);
    }
#endif
    return result;
}

// MARK: Int64x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2FromFloat32x2(Float32x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvtq_s64_f64(vcvt_f64_f32(operand.intrinsic));
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtps_epi64(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int64)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2FromInt32x2(Int32x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vmovl_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_cvtepi32_epi64(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int64)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int64x2){ (Int64)(operand[0]), (Int64)(operand[1]) }`
CX_INLINE Int64x2 CXInt64x2FromUInt32x2(UInt32x2 operand) {
    Int64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vreinterpretq_s64_u64(vmovl_u32(operand.intrinsic));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_cvtepi32_epi64(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int64)(operand.array[0]);
    }
#endif
    return result;
}

// MARK: UInt64x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE UInt64x2 CXUInt64x2FromFloat32x2(Float32x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vcvtq_u64_f64(vcvt_f64_f32(operand.intrinsic));
#elif CX_X86_ARCH >= CX_X86_LEVEL_AVX_512
    result.intrinsic = _mm_cvtps_epu64(operand.intrinsic);
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt64)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE UInt64x2 CXUInt64x2FromInt32x2(Int32x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vreinterpretq_u64_s64(vmovl_s32(operand.intrinsic));
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_cvtepi32_epi64(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt64)(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt64x2){ (UInt64)(operand[0]), (UInt64)(operand[1]) }`
CX_INLINE UInt64x2 CXUInt64x2FromUInt32x2(UInt32x2 operand) {
    UInt64x2 result;
#if CX_ARM_ARCH >= CX_NEON_LEVEL_AARCH64
    result.intrinsic = vmovl_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE4_1
    result.intrinsic = _mm_cvtepi32_epi64(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt64)(operand.array[0]);
    }
#endif
    return result;
}

#undef SIZE

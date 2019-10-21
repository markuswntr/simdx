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

#if !CX_ARM_ARCH && CX_X86_ARCH < CX_X86_LEVEL_SSE
#   include <math.h>
#endif

// MARK: Float32x2

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(Float32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2FromInt32x2(Int32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvt_f32_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtepi32_ps(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[i]);
    }
#endif
    return result;
}

/// Convert the elements of `operand`, load them in the new storage and return the result.
/// @returns `(Float32x2){ (Float32)(operand[0]), (Float32)(operand[1]) }`
CX_INLINE Float32x2 CXFloat32x2FromUInt32x2(UInt32x2 operand) {
    Float32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvt_f32_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_cvtepi32_ps(_mm_movpi64_epi64(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Float32)(operand.array[i]);
    }
#endif
    return result;
}

// MARK: Int32x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2FromFloat32x2(Float32x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvt_s32_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_movepi64_pi64(_mm_cvtps_epi32(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)nearbyintf(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(Int32x2){ (Int32)(operand[0]), (Int32)(operand[1]) }`
CX_INLINE Int32x2 CXInt32x2FromUInt32x2(UInt32x2 operand) {
    Int32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpret_s32_u32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (Int32)operand.array[0];
    }
#endif
    return result;
}

// MARK: UInt32x2

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE UInt32x2 CXUInt32x2FromFloat32x2(Float32x2 operand) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vcvt_u32_f32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = _mm_movepi64_pi64(_mm_cvtps_epi32(operand.intrinsic));
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)nearbyintf(operand.array[0]);
    }
#endif
    return result;
}

/// Converts the elements of `operand`, load them in the new storage and returns the result.
/// @returns `(UInt32x2){ (UInt32)(operand[0]), (UInt32)(operand[1]) }`
CX_INLINE UInt32x2 CXUInt32x2FromInt32x2(Int32x2 operand) {
    UInt32x2 result;
#if CX_ARM_ARCH
    result.intrinsic = vreinterpret_u32_s32(operand.intrinsic);
#elif CX_X86_ARCH >= CX_X86_LEVEL_SSE
    result.intrinsic = operand.intrinsic;
#else
    CX_VECTORIZE for (size_t i = 0; i < SIZE(result.array); i++) {
        result.array[i] = (UInt32)operand.array[0];
    }
#endif
    return result;
}
#undef SIZE

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

#include <stdint.h>
#include "common/numeric.h"

#if CX_NEON_128
#include <arm_neon.h>
#elif CX_X86_128
#include <emmintrin.h>
#endif

/// The 2 x 32-bit floating point storage
#if CX_NEON_128
typedef float32x2_t __attribute((aligned(16))) CXFloat32x2;
#elif CX_X86_128
typedef __m128 CXFloat32x2; // Most significant 64 bits are unused (zero)
#elif CX_EXT_VECTOR
typedef Float32 CXFloat32x2 __attribute__((ext_vector_type(2)));
#else
typedef struct CXFloat32x2_t { Float32 val[2]; } CXFloat32x2;
#endif

/// The 3 x 32-bit floating point storage
#if CX_NEON_128
typedef float32x4_t __attribute((aligned(16))) CXFloat32x3; // Most significant 32 bits are unused (zero)
#elif CX_X86_128
typedef __m128 CXFloat32x3; // Most significant 32 bits are unused (zero)
#elif CX_EXT_VECTOR
typedef Float32 CXFloat32x3 __attribute__((ext_vector_type(3)));
#else
typedef struct CXFloat32x3_t { Float32 val[3]; } CXFloat32x3;
#endif

/// The 4 x 32-bit floating point storage
#if CX_NEON_128
typedef float32x4_t __attribute((aligned(16))) CXFloat32x4;
#elif CX_X86_128
typedef __m128 CXFloat32x4;
#elif CX_EXT_VECTOR
typedef Float32 CXFloat32x4 __attribute__((ext_vector_type(4)));
#else
typedef struct CXFloat32x4_t { Float32 val[4]; } CXFloat32x4;
#endif

/// The 2 x 64-bit floating point storage
#if CX_NEON_128_WITH_AARCH64 // NOTE: float64x2_t requires aarch64
typedef float64x2_t __attribute((aligned(16))) CXFloat64x2;
#elif CX_X86_128
typedef __m128d CXFloat64x2;
#elif CX_EXT_VECTOR
typedef Float64 CXFloat64x2 __attribute__((ext_vector_type(2)));
#else
typedef struct CXFloat64x2_t { Float64 val[2]; } CXFloat64x2;
#endif

/// The 2 x 32-bit signed integer storage
#if CX_NEON_128
typedef int32x2_t __attribute((aligned(16))) CXInt32x2;
#elif CX_X86_128
typedef __m128i CXInt32x2; // Most significant 2 x 32 bits are unused (zero)
#elif CX_EXT_VECTOR
typedef Int32 CXInt32x2 __attribute__((ext_vector_type(2)));
#else
typedef struct CXInt32x2_t { Int32 val[2]; } CXInt32x2;
#endif

/// The 3 x 32-bit signed integer storage
#if CX_NEON_128
typedef int32x4_t __attribute((aligned(16))) CXInt32x3; // Most significant 32 bits are unused (zero)
#elif CX_X86_128
typedef __m128i CXInt32x3; // Most significant 32 bits are unused (zero)
#elif CX_EXT_VECTOR
typedef Int32 CXInt32x3 __attribute__((ext_vector_type(3)));
#else
typedef struct CXInt32x3_t { Int32 val[3]; } CXInt32x3;
#endif

/// The 4 x 32-bit signed integer storage
#if CX_NEON_128
typedef int32x4_t __attribute((aligned(16))) CXInt32x4;
#elif CX_X86_128
typedef __m128i CXInt32x4;
#elif CX_EXT_VECTOR
typedef Int32 CXInt32x4 __attribute__((ext_vector_type(4)));
#else
typedef struct CXInt32x4_t { Int32 val[4]; } CXInt32x4;
#endif

/// The 2 x 64-bit signed integer storage
#if CX_NEON_128
typedef int64x2_t __attribute((aligned(16))) CXInt64x2;
#elif CX_X86_128
typedef __m128i CXInt64x2;
#elif CX_EXT_VECTOR
typedef Int64 CXInt64x2 __attribute__((ext_vector_type(2)));
#else
typedef struct CXInt64x2_t { Int64 val[2]; } CXInt64x2;
#endif

/// The 2 x 32-bit unsigned integer storage
#if CX_NEON_128
typedef uint32x2_t __attribute((aligned(16))) CXUInt32x2;
#elif CX_X86_128
typedef __m128i CXUInt32x2; // Most significant 2 x 32 bits are unused (zero)
#elif CX_EXT_VECTOR
typedef UInt32 CXUInt32x2 __attribute__((ext_vector_type(2)));
#else
typedef struct CXUInt32x2_t { UInt32 val[2]; } CXUInt32x2;
#endif

/// The 3 x 32-bit unsigned integer storage
#if CX_NEON_128
typedef uint32x4_t __attribute((aligned(16))) CXUInt32x3; // Most significant 32 bits are unused (zero)
#elif CX_X86_128
typedef __m128i CXUInt32x3; // Most significant 32 bits are unused (zero)
#elif CX_EXT_VECTOR
typedef UInt32 CXUInt32x3 __attribute__((ext_vector_type(3)));
#else
typedef struct CXUInt32x3_t { UInt32 val[3]; } CXUInt32x3;
#endif

/// The 4 x 32-bit unsigned integer storage
#if CX_NEON_128
typedef uint32x4_t __attribute((aligned(16))) CXUInt32x4;
#elif CX_X86_128
typedef __m128i CXUInt32x4;
#elif CX_EXT_VECTOR
typedef UInt32 CXUInt32x4 __attribute__((ext_vector_type(4)));
#else
typedef struct CXUInt32x4_t { UInt32 val[4]; } CXUInt32x4;
#endif

/// The 2 x 64-bit unsigned integer storage
#if CX_NEON_128
typedef uint64x2_t __attribute((aligned(16))) CXUInt64x2;
#elif CX_X86_128
typedef __m128i CXUInt64x2;
#elif CX_EXT_VECTOR
typedef UInt64 CXUInt64x2 __attribute__((ext_vector_type(2)));
#else
typedef struct CXUInt64x2_t { UInt64 val[2]; } CXUInt64x2;
#endif

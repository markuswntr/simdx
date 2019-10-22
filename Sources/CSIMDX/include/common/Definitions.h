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

// MARK: Clang
// == References ========================================
//  - https://clang.llvm.org/docs/LanguageExtensions.html
// ======================================================

/// Function pre-fix to make it static and to try force inlining it by the compiler.
#define CX_INLINE __inline__ __attribute__((__always_inline__, __nodebug__)) static

/// Whether the target hardware supports the builtin convertvector
#define CX_HAS_CONVERTVECTOR __has_builtin(__builtin_convertvector)

/// Whether the target hardware supports the builtin shufflevector
#define CX_HAS_SHUFFLEVECTOR __has_builtin(__builtin_shufflevector)

// MARK: Arm
// == References ===============================================================================
//  - https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics
//  - http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0472k/chr1359125040547.html
// =============================================================================================

#define CX_NEON_LEVEL_NONE      0
#define CX_NEON_LEVEL_ARM32     10
#define CX_NEON_LEVEL_AARCH64   20

/// Use the Arm Neon intrinsic path
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#   include <arm_neon.h>
#   if defined(__aarch64__)
#       define CX_ARM_ARCH CX_NEON_LEVEL_AARCH64 /// Use the extend neon intrinsics in aarch64
#   else
#       define CX_ARM_ARCH CX_NEON_LEVEL_ARM32 /// Use the standard neon intrinsics
#   endif
#else
#   define CX_ARM_ARCH CX_NEON_LEVEL_NONE
#endif

// MARK: x86
// == References ========================================================================
//  - https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_m128&techs=SSE2
//  - https://clang.llvm.org/doxygen/emmintrin_8h_source.html
// ======================================================================================

#define CX_X86_LEVEL_NONE       0
#define CX_X86_LEVEL_MMX        10
#define CX_X86_LEVEL_SSE        20
#define CX_X86_LEVEL_SSE2       25
#define CX_X86_LEVEL_SSE3       30
#define CX_X86_LEVEL_SSSE3      35
#define CX_X86_LEVEL_SSE4_1     40
#define CX_X86_LEVEL_SSE4_2     45
#define CX_X86_LEVEL_AVX        50
#define CX_X86_LEVEL_AVX_2      55

// TBD
#define CX_X86_LEVEL_AVX_512    1000 
//#define __AVX512CD__ 1
//#define __AVX512ER__ 1
//#define __AVX512F__ 1
//#define __AVX512PF__ 1
//#define __AVX512BW__ 1
//#define __AVX512DQ__ 1
//#define __AVX512VL__ 1

/// Use the x86 intrinsic path
#if defined(__AVX2__)
#   include <immintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_AVX_2
#elif defined(__AVX__)
#   include <immintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_AVX
#elif defined(__SSE4_2__)
#   include <nmmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_SSE4_2
#elif defined(__SSE4_1__)
#   include <smmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_SSE4_1
#elif defined(__SSSE3__)
#   include <tmmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_SSSE3
#elif defined(__SSE3__)
#   include <pmmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_SSE3
#elif defined(__SSE2__)
#   include <emmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_SSE2
#elif defined(__SSE__)
#   include <xmmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_SSE
#elif defined(__MMX__)
#   include <mmintrin.h>
#   define CX_X86_ARCH CX_X86_LEVEL_MMX
#else
#   define CX_X86_ARCH CX_X86_LEVEL_NONE
#endif

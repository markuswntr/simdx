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

// == References ===============================================================================
//  - https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics
//  - http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0472k/chr1359125040547.html
// =============================================================================================

/// Use the neon intrinsic path
#define CX_NEON_128 1

/// Additionally, use the extend set of aarch64 if available
#ifdef __aarch64__
#define CX_NEON_128_WITH_AARCH64 1
#endif

// Some instructions may fallback to clang provided extended vector support if available.
#if defined(__has_extension) && __has_extension(attribute_ext_vector_type)
#define CX_EXT_VECTOR 1
#endif

/// Function pre-fix to make it static and to try force inlining by the compiler. Takes the return value as input.
/// Adds the default function attributes for intrinsic wrapping functions.
#define CX_INLINE(returnType) static __inline__ returnType __attribute__((__always_inline__, __nodebug__))

/// Include the 128 bit instructions
#include "128/128.h"

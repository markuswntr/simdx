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

// == References ================================================================================
//  - https://clang.llvm.org/doxygen/emmintrin_8h_source.html
//  - https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_m128&techs=SSE2
//  - https://thinkingandcomputing.com/posts/using-avx-instructions-in-matrix-multiplication.html
// ==============================================================================================

/// Use the x86 intrinsic path
#define CX_X86_128 1

/// The default function attributes for intrinsic wrapping functions
#define DEFAULT_FUNCTION_ATTRIBUTES  __attribute__((__always_inline__, __nodebug__, __min_vector_width__(128)))

/// Function pre-fix to make it static and to try force inlining it by the compiler. Takes the return value as input.
#define CX_INLINE(returnType) static __inline__ returnType DEFAULT_FUNCTION_ATTRIBUTES

/// Include the 128 bit instructions
#include <emmintrin.h>
#include "128/128.h"

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

#if !__has_extension(attribute_ext_vector_type)
#error This library requires the extended vector types of clang to be available on the target hardware
#endif

#include "common/Definitions.h"
#include "common/Numerics.h"

#include "64/64.h"
#include "128/128.h"

#include "Conversion.h"

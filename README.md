# SIMDX

*SIMDX* provides a unified implementation for built-in vector and matrix intrinsics, such as **SSE/AVX on x86** and
**Neon on Arm**, in C (see `CSIMDX` target) and exposes them to Swift as generic types (see `SIMDX` target).
Furthermore, *SIMDX* provides a fast and portable implementation of SIMD like intrinsics on **hardware which doesn't
natively support** them, making *SIMDX* independent of the target hardware. Therefore it allows vector and matrix
calculations on any (Swift supporting) hardware and automatically inlines the appropriate intrinsic functions of the
target hardware.

> **Warning.** Not meant to be used in production, created for learning purposes!
> <br/><br/> See [**Stumbling on SIMD**](https://blog.wntr.me/posts/001-the-properties-of-space/) series to learn how
this project came to be. If you want to report a bug or an unexpected behaviour, feel free to open an issue. If have
suggestions or really anything else that helps evolving the library and/or are interested in the details feel free to
contact me on [Twitter](https://twitter.com/markuswntr).

## Coding example

Given a `color` that should be modified in `brightness` on each channel separately and then multiplied by a `scale` factor on each lanes equally.

**Example 1.1:**

```swift
// A color to be modify
let color: [Float] = [0.11, 0.2, 0.64, 1.0] // RGBA

// Add a modification value to each channel seperatly
let brightness: [Float] = [0.25, 0.3, -0.35, 0.0]

// Scale the resulting color on each channel equally
let scale: Float = 0.8

var newColor: [Float] = .init(repeating: 0.0, count: 4)
for index in 0...3 {
    // operation on each element
    newColor[index] = (color[index] + brightness[index]) * scale 
}

print(newColor)
// [0.288, 0.4, 0,232, 1.0]
```

The *SIMDX* library allows to rewrite the example:

**Example 1.2:**

```swift
import SIMDX

// A color to be modify
let color: SIMDX4<Float> = [0.11, 0.2, 0.64, 1.0]

// Add a modification value to each channel seperatly
let brightness: SIMDX4<Float> = [0.25, 0.3, -0.35, 0.0]

// Scale the resulting color on each channel equally
let scale: Float = 0.8

// Do all operations on SIMD in parallel using SIMDX
let newColor = (color + brightness) * scale

print(newColor)
// [0.288, 0.4, 0,232, 1.0]
```

Example 1.2 does the same as example 1.1, but more efficiently because it utilises SIMD instructions that do four
additions and four multiplications in a single instruction. Today, modern CPU's have these instructions which may give
you a throughput of four floating point additions and four multiplications per clock cycle. A good compiler may 
actually convert example 1.1 automatically to use the SIMD instructions, but in more complicated cases you cannot be
sure that the compiler is able to vectorise your code in an optimal way.

### How it works

The type `SIMDX4<Float>` in example 1.2 is a struct that encapsulates a 128-bit intrinsic type holding 4 floating point
numbers of 32 bits each. The operators `+` and `*` represent the SIMD instruction for adding and multiplying the
intrinsic types. These operators are inlined so that no extra code is generated other than the SIMD instructions. More
specifically, the type `SIMDX4<Float>` masks a `__m128` intrinsic type on x86 with SSE2 or a `float32x4_t` intrinsic
type on Arm with Neon. If neither of both are available, the module instructs the compiler to optimise the vector code.
If this is not possible on the target hardware, the library provides a fallback to a C-array of float type and fixed
length, i.e. `float array[4]`.

## Features

- [x] Conform to common `Numeric` protocols functions (see upcoming blog post #link )
- [x] 64-bit storage
- [x] 128-bit storage
- [x] Int32, UInt32, Float32 and Float64 storable

|         | Int8 | UInt8 | Int16 | UInt16 | Float16 | Int32 | UInt32 | Float32 | Int64 | UInt64 | Float64 |
|--------:|------|-------|-------|--------|---------|-------|--------|---------|-------|--------|---------|
|  64 bit  |      |       |       |        |         |       |        |         |       |        |         |
| 128 bit |      |       |       |        |         |       |        |         |       |        |         |
| 256 bit |      |       |       |        |         |       |        |         |       |        |         |
| 512 bit |      |       |       |        |         |       |        |         |       |        |         |

## TODO

Move TODOs to Issues and/or a Project at some point

- [x] Make `count` on SIMDX static
- [ ] Extension on Array `init(SIMDX)` that uses native intrinsics store
- [ ] Horizontal sum
- [ ] Remove BitwiseAndNot from csimdx (no intrinsic anymore)
- [ ] Documentation
- [ ] Comparison (Equal, GreaterThan, LowerThan, ...)
- [ ] Move Storable structs into C and Swift Shim only
- [ ] Int8, UInt8, Int16 and UInt16 storable
- [ ] Boolean storage
- [ ] 256-bit storage
- [ ] Multi-dimensional storage (Matrix)
- [ ] Extend conformance to the `Numeric` protocols
    - [ ] Handle overflows properly
    - [ ] Handle floating point rounding modes
    - [ ] Instance from RandomNumberGenerator
    - [ ] Cast most vector types natively using intrinsics
- [ ] 512-bit storage
- [ ] Evaluate Float16
- [x] Remove the ARM 64 requirement and any other platform restriction in Package.swift
- [ ] Edge case tests
- [ ] Not all intrinsics route through the fastest way possible. Re-visit and improve.
- [ ] Re-evaluate the necessity of SIMDX being ExpressibleByArrayLiteral
- [ ] The generic implementation is not the fastest it could be. Do some more magic.
- [ ] OpenMP & OpenCL

## References 

I started with almost zero knowledge of SIMD/Intrinsics or builtin clang functions and was DuckDuckGoing (is that a
thing?) alot prior to started writing this lib. The following references contain some of the most useful instructions
I found across the internet. I gathered them while writing this library, and I am pretty sure I will need them and
re-visit them quite a lot so I leave them here.

### Intrinsic functions

- https://clang.llvm.org/docs/LanguageExtensions.html
- https://software.intel.com/sites/landingpage/IntrinsicsGuide/
- https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics

### Other Libraries

- https://github.com/QuantStack/xsimd
- https://github.com/nemequ/simde

### Things to come

- https://thinkingandcomputing.com/posts/using-avx-instructions-in-matrix-multiplication.html

## License

The **SIMDX** library is licensed under the Apache License, version 2.0.

You may not use the files except in compliance with this License.  
You may obtain a copy of the license at www.apache.org/licenses/LICENSE-2.0

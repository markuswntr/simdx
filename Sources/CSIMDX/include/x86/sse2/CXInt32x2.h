#pragma once

#include "CXTypes_t.h"

/// Returns an intrinsic initialized to the 2 given values, from least- to most-significant bits.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2Make(Int32 value0, Int32 value1)
{
    return CXInt32x4Make(value0, value1, 0, 0);
}

/// Loads 2 x Int32 values from unaligned memory.
/// @param pointer Unaligned memory pointer to 2 x Int32 values
/// @return CXInt32x2(pointer[0], pointer[1])
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2MakeLoad(const Int32* pointer)
{
    return _mm_loadu_si64(pointer); // Loads only the first 64 bits using given pointer and zeros the rest.
}

/// Returns an intrinsic type with all lanes initialized to `value`.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2MakeRepeatingElement(const Int32 value)
{
    return CXInt32x2Make(value, value);
}

/// Returns an intrinsic type with all lanes initialized to zero (0.f).
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2MakeZero(void)
{
    return CXInt32x4MakeZero();
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @param storage The storage to read values from
/// @param index The index of the value to return
STATIC_INLINE_INTRINSIC(Int32) CXInt32x2GetElement(const CXInt32x2 storage, const int index)
{
    return CXInt32x4GetElement(storage, index);
}

/// Sets the element at `index` from `storage` to given value.
/// @param storage The storage to look up values
/// @param index The index of the value to change.
/// @param value The value to set at storage[index].
STATIC_INLINE_INTRINSIC(void) CXInt32x2SetElement(CXInt32x2* storage, const int index, const Int32 value)
{
    CXInt32x4SetElement(storage, index, value);
}

// MARK: - Conversion

/// Converts a storage of 2 x Float32 into a storage of 2 x Int32.
/// @param operand A storage of 2 x Float32.
/// @returns A storage of 2 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2FromCXFloat32x2(CXFloat32x2 operand)
{
    return CXInt32x4FromCXFloat32x4(operand);
}

/// Converts a storage of 2 x Float32 into a storage of 2 x Int32,
/// truncating (rounded towards zero) the result when it is inexact.
///
/// @param operand A storage of 2 x Float32.
/// @returns A storage of 2 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2FromCXFloat32x2AllowTruncating(CXFloat32x2 operand)
{
    return CXInt32x4FromCXFloat32x4AllowTruncating(operand);
}

/// Converts the two double-precision floating-point elements of a 128-bit
/// storage of 2 x Float32 into two signed 32-bit integer values.
/// @param operand A storage of 2 x Float64.
/// @returns A storage of 2 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2FromCXFloat64x2(CXFloat64x2 operand)
{
    return _mm_cvtpd_epi32(operand);
}

/// Converts a storage of 2 x Float32 into a storage of 2 x Int32,
/// truncating (rounded towards zero) the result when it is inexact.
///
/// @param operand A storage of 2 x Float64.
/// @returns A storage of 2 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2FromCXFloat64x2AllowTruncating(CXFloat64x2 operand)
{
    return _mm_cvttpd_epi32(operand);
}

// MARK: - Arithmetics

/// Returns the negated value (element-wise).
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2Negate(const CXInt32x2 storage)
{
    return CXInt32x4Negate(storage);
}

/// Returns the absolute value (element-wise).
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXInt32x2Absolute(const CXInt32x2 storage)
{
    return CXInt32x4Absolute(storage);
}

// MARK: Additive

/// Adds two storages (element-wise) and returns the result.
/// @param lhs Left-hand side operator
/// @param rhs Right-hand side operator
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2Add(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return CXInt32x4Add(lhs, rhs);
}

/// Subtracts a storage from another (element-wise) and returns the result.
/// @param lhs Left-hand side operator
/// @param rhs Right-hand side operator
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2Subtract(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return CXInt32x4Subtract(lhs, rhs);
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise) and returns the result.
/// @param lhs Left-hand side operator
/// @param rhs Right-hand side operator
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2Multiply(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return CXInt32x4Multiply(lhs, rhs);
}

// MARK: - Bitwise

/// Bitwise Not
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2BitwiseNot(const CXInt32x2 operand)
{
    return _mm_xor_si128(operand, _mm_set1_epi64(_mm_cvtsi64_m64(-1LL)));
}

/// Bitwise And
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2BitwiseAnd(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return _mm_and_si128(rhs, lhs);
}

/// Bitwise And Not
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2BitwiseAndNot(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return _mm_andnot_si128(lhs, rhs);
}

/// Bitwise Or
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2BitwiseOr(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return _mm_or_si128(lhs, rhs);
}

/// Bitwise Exclusive Or
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2BitwiseExclusiveOr(const CXInt32x2 lhs, const CXInt32x2 rhs)
{
    return _mm_xor_si128(lhs, rhs);
}

// MARK: Shifting

/// Left-shifts each 64-bit value in the 128-bit integer storage operand by the specified number of bits.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2ShiftLeft(const CXInt32x2 lhs, const Int32 rhs)
{
    return _mm_sll_epi32(lhs, _mm_cvtsi32_si128(rhs));
}

/// Right-shifts each 64-bit value in the 128-bit integer storage operand by the specified number of bits.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2ShiftRight(const CXInt32x2 lhs, const Int32 rhs)
{
    return _mm_sra_epi32(lhs, _mm_cvtsi32_si128(rhs));
}

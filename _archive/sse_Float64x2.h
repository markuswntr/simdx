// MARK: - Comparison

/// Compares each of the corresponding double-precision values of the 128-bit storages of 2 x Float64 for equality.
///
/// Each comparison yields 0x0 for false, 0xFFFFFFFFFFFFFFFF for true.
///
/// @param lhs A 128-bit storage of 2 x Float64.
/// @param rhs A 128-bit storage of 2 x Float64.
/// @returns A 128-bit storage containing the comparison results.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2CompareEqual(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_cmpeq_pd(lhs, rhs);
}

/// Compares each of the corresponding double-precision values of the 128-bit storages of 2 x Float64 to determine
/// if the values in the first operand are unequal to those in the second operand.
///
/// Each comparison yields 0x0 for false, 0xFFFFFFFFFFFFFFFF for true.
///
/// @param lhs A 128-bit storage of 2 x Float64.
/// @param rhs A 128-bit storage of 2 x Float64.
/// @returns A 128-bit storage containing the comparison results.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2CompareNotEqual(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_cmpneq_pd(lhs, rhs);
}

/// Compares each of the corresponding double-precision values of the 128-bit storages of 2 x Float64
/// to determine if the values in the first operand are less than those in the second operand.
///
/// Each comparison yields 0x0 for false, 0xFFFFFFFFFFFFFFFF for true.
///
/// @param lhs A 128-bit storage of 2 x Float64.
/// @param rhs A 128-bit storage of 2 x Float64.
/// @returns A 128-bit storage containing the comparison results.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2CompareLessThan(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_cmplt_pd(lhs, rhs);
}

/// Compares each of the corresponding double-precision values of the 128-bit storages of 2 x Float64 to
/// determine if the values in the first operand are less than or equal to those in the second operand.
///
/// Each comparison yields 0x0 for false, 0xFFFFFFFFFFFFFFFF for true.
///
/// @param lhs A 128-bit storage of 2 x Float64.
/// @param rhs A 128-bit storage of 2 x Float64.
/// @returns A 128-bit storage containing the comparison results.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2CompareLessThanOrEqual(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_cmple_pd(lhs, rhs);
}

/// Compares each of the corresponding double-precision values of the 128-bit storages of 2 x Float64
/// to determine if the values in the first operand are greater than those in the second operand.
///
/// Each comparison yields 0x0 for false, 0xFFFFFFFFFFFFFFFF for true.
///
/// @param lhs A 128-bit storage of 2 x Float64.
/// @param rhs A 128-bit storage of 2 x Float64.
/// @returns A 128-bit storage containing the comparison results.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2CompareGreaterThan(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_cmpgt_pd(lhs, rhs);
}

/// Compares each of the corresponding double-precision values of the 128-bit storages of 2 x Float64 to
/// determine if the values in the first operand are greater than or equal to those in the second operand.
///
/// Each comparison yields 0x0 for false, 0xFFFFFFFFFFFFFFFF for true.
///
/// @param lhs A 128-bit storage of 2 x Float64.
/// @param rhs A 128-bit storage of 2 x Float64.
/// @returns A 128-bit storage containing the comparison results.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2CompareGreaterThanOrEqual(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_cmpge_pd(lhs, rhs);
}

// MARK: Bitwise

/// Performs a bitwise AND of two 128-bit storages of 2 x Float64.
/// @param lhs A 128-bit storage of 2 x Float64 containing one of the source operands.
/// @param rhs A 128-bit storage of 2 x Float64 containing one of the source operands.
/// @returns A 128-bit storage of 2 x Float64 containing the bitwise AND of the values between both operands.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2BitwiseAnd(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_and_pd(lhs, rhs);
}

/// Performs a bitwise AND of two 128-bit storages of 2 x Float64, using the one's
/// complement of the values contained in the first source operand.
/// @param lhs A 128-bit storage of 2 x Float64 containing the left source operand.
///            The one's complement of this value is used in the bitwise AND.
/// @param rhs A 128-bit storage of 2 x Float64 containing the right source operand.
/// @returns A 128-bit storage of 2 x Float64 containing the bitwise AND of the values in the second operand
///          and the one's complement of the first operand.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2BitwiseAndNot(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_andnot_pd(lhs, rhs);
}

/// Performs a bitwise OR of two 128-bit storages of 2 x Float64.
/// @param lhs A 128-bit storage of 2 x Float64 containing one of the source operands.
/// @param rhs A 128-bit storage of 2 x Float64 containing one of the source operands.
/// @returns A 128-bit storage of 2 x Float64 containing the bitwise OR of the values between both operands.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2BitwiseOr(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_or_pd(lhs, rhs);
}

/// Performs a bitwise XOR of two 128-bit storages of 2 x Float64.
/// @param lhs A 128-bit storage of 2 x Float64 containing one of the source operands.
/// @param rhs A 128-bit storage of 2 x Float64 containing one of the source operands.
/// @returns A 128-bit storage of 2 x Float64 containing the bitwise XOR of the values between both operands.
STATIC_INLINE_INTRINSIC(CXFloat64x2) CXFloat64x2BitwiseExclusiveOr(const CXFloat64x2 lhs, const CXFloat64x2 rhs)
{
    return _mm_xor_pd(lhs, rhs);
}

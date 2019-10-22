/// Converts a vector of 4 x Float32 into a vector of 4 x Int32, truncating the result when it is inexact.
/// @param operand A 128-bit vector of 4 x Float32.
/// @returns A 128-bit vector of 4 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x4) CXInt32x4FromCXFloat32x4AllowTruncating(CXFloat32x4 operand)
{
    return _mm_cvttps_epi32(operand);
}

/// Converts a vector of 3 x Float32 into a vector of 3 x Int32, truncating the result when it is inexact.
/// @param operand A 128-bit vector of 3 x Float32.
/// @returns A 128-bit vector of 3 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x3) CXInt32x3FromCXFloat32x3AllowTruncating(CXFloat32x3 operand)
{
    return _mm_cvttps_epi32(operand);
}

/// Converts a storage of 2 x Float32 into a storage of 2 x Int32,
/// truncating (rounded towards zero) the result when it is inexact.
///
/// @param operand A storage of 2 x Float32.
/// @returns A storage of 2 x Int32 containing the converted values.
STATIC_INLINE_INTRINSIC(CXInt32x2) CXInt32x2FromCXFloat32x2AllowTruncating(CXFloat32x2 operand)
{
    return _mm_cvttps_epi32(operand);
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

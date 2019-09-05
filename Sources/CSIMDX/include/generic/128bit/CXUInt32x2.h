#pragma once

#include <stdlib.h>
#include "CXUInt32_t.h"

// MARK: Designated Initializers

/// Returns an intrinsic initialized to the 2 given values, from least- to most-significant bits.
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2Make(UInt32 value0, UInt32 value1)
{
    CXUInt32x2 storage;
#if __has_extension(attribute_ext_vector_type)
    storage.internalElements = (CEXStorageUInt32x2){ value0, value1 };
#else
    storage.elements = [value0, value1];
#endif
    return storage;
}

/// Loads 2 x UInt32 values from unaligned memory.
/// @param pointer Unaligned memory pointer to 2 x UInt32 values
/// @return CXUInt32x2(pointer[0], pointer[1])
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2MakeLoad(const UInt32* pointer)
{
    CXUInt32x2 storage;
#if __has_extension(attribute_ext_vector_type)
    storage.internalElements = (CEXStorageUInt32x2){ pointer[0], pointer[1] };
#else
    storage.elements = (CEXStorageUInt32x2)[ pointer[0], pointer[1] ];
#endif
    return storage;
}

/// Returns an intrinsic type with all lanes initialized to `value`.
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2MakeRepeatingElement(const UInt32 value)
{
    CXUInt32x2 storage;
#if __has_extension(attribute_ext_vector_type)
    storage.internalElements = (CEXStorageUInt32x2){ value, value };
#else
    storage.elements = (CEXStorageUInt32x2)[ value, value ];
#endif
    return storage;
}

/// Returns an intrinsic type with all lanes initialized to zero (0.f).
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2MakeZero(void)
{
    return CXUInt32x2MakeRepeatingElement(0.f);
}

// MARK: - Getter/Setter

/// Returns the element at `index` of `storage` (`storage[index]`).
/// @param storage The storage to read values from
/// @param index The index of the value to return
STATIC_INLINE_INTRINSIC(UInt32) CXUInt32x2GetElement(const CXUInt32x2 storage, const int index)
{
#if __has_extension(attribute_ext_vector_type)
    return storage.internalElements[index];
#else
    return storage.elements[index];
#endif
}

/// Sets the element at `index` from `storage` to given value.
/// @param storage The storage to look up values
/// @param index The index of the value to change.
/// @param value The value to set at storage[index].
STATIC_INLINE_INTRINSIC(void) CXUInt32x2SetElement(CXUInt32x2* storage, const int index, const UInt32 value)
{
#if __has_extension(attribute_ext_vector_type)
    (*storage).internalElements[index] = value;
#else
    (*storage).elements[index] = value;
#endif
}

// MARK: - Arithmetics

/// Returns the given storage unchanged. Allows for symmetry with signed values.
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2Absolute(const CXUInt32x2 storage)
{
    return storage;
}

// MARK: Additive

/// Adds two storages (element-wise) and returns the result.
/// @param lhs Left-hand side operator
/// @param rhs Right-hand side operator
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2Add(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
    CXUInt32x2 storage;
#if __has_extension(attribute_ext_vector_type)
    storage.internalElements = lhs.internalElements + rhs.internalElements;
#else
    storage.elements = (CXUInt32x2)[
         CXUInt32x2GetElement(lhs, 0) + CXUInt32x2GetElement(rhs, 0),
         CXUInt32x2GetElement(lhs, 1) + CXUInt32x2GetElement(rhs, 1)
     ];
#endif
    return storage;
}

/// Subtracts a storage from another (element-wise) and returns the result.
/// @param lhs Left-hand side operator
/// @param rhs Right-hand side operator
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2Subtract(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
    CXUInt32x2 storage;
#if __has_extension(attribute_ext_vector_type)
    storage.internalElements = lhs.internalElements - rhs.internalElements;
#else
    storage.elements = (CXUInt32x2)[
         CXUInt32x2GetElement(lhs, 0) - CXUInt32x2GetElement(rhs, 0),
         CXUInt32x2GetElement(lhs, 1) - CXUInt32x2GetElement(rhs, 1)
     ];
#endif
    return storage;
}

// MARK: Multiplicative

/// Multiplies two storages (element-wise) and returns the result.
/// @param lhs Left-hand side operator
/// @param rhs Right-hand side operator
STATIC_INLINE_INTRINSIC(CXUInt32x2) CXUInt32x2Multiply(const CXUInt32x2 lhs, const CXUInt32x2 rhs)
{
    CXUInt32x2 storage;
#if __has_extension(attribute_ext_vector_type)
    storage.internalElements = lhs.internalElements * rhs.internalElements;
#else
    storage.elements = (CXUInt32x2)[
         CXUInt32x2GetElement(lhs, 0) * CXUInt32x2GetElement(rhs, 0),
         CXUInt32x2GetElement(lhs, 1) * CXUInt32x2GetElement(rhs, 1)
     ];
#endif
    return storage;
}
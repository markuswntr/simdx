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

import Foundation

/// A wrapper type to raw SIMD intrinsic instructions. TODO: More documentation.
public protocol SIMDX: RandomAccessCollection, MutableCollection, ExpressibleByArrayLiteral, RawRepresentable
where Element == Storage.Element, Index: RawRepresentable, Index.RawValue == Storage.Index {

    /// The raw SIMD storage type to use for instances of this type.
    associatedtype Storage: RawStorage

    /// The raw SIMD storage to operate on when calling this instance.
    var rawValue: Storage { get set }

    /// Initializes a new instance of `Self` backed by given storage.
    /// - Parameter rawValue: The value to use as storage for this instance.
    init(rawValue: Storage)
}

// MARK: Common Interface

extension SIMDX {

    /// The number of elements in the storage
    @inlinable public var count: Int { rawValue.count }

    /// Allows read and write access to individual elements in the storage.
    @inlinable public subscript(index: Index) -> Element {
        @inlinable set { rawValue[index.rawValue] = newValue }
        @inlinable get { rawValue[index.rawValue] }
    }

    /// Initialize to a SIMDX collection with all elements to `value`.
    @inlinable public init(repeating value: Element) {
        self.init(rawValue: .init(value))
    }

    /// Initialize to a SIMDX collection with elements taken from `other` sequence.
    /// - Precondition: `other` must have the exact same count as the SIMDX collection.
    @inlinable public init<Other>(other: Other) where Other: Sequence, Element == Other.Element {
        self.init(rawValue: .init(other))
    }

    /// Creates an instance initialized with the given elements.
    @inlinable public init(arrayLiteral elements: Element...) {
        self.init(other: elements)
    }
}

// MARK: - Down the Numeric Road

extension SIMDX where Storage: EquatableStorage {

    /// Returns a Boolean value indicating whether the values of two storages are element-wise equal.
    @inlinable public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.rawValue == rhs.rawValue
    }
}

// MARK: Additive Arithmetic

extension SIMDX where Storage: AdditiveArithmeticStorage {

    /// The zero value.
    @inlinable public static var zero: Self { .init(rawValue: .zero) }

    /// Returns the given storage unchanged.
    ///
    /// You can use the unary plus operator (`+`) to provide symmetry in your
    /// code for positive storages when also using the unary minus operator.
    @inlinable prefix public static func + (operand: Self) -> Self { operand }

    /// Adds two storages and produces their element-wise sums.
    /// - Parameter lhs: The left hand side operand.
    /// - Parameter rhs: The right hand side operand.
    @inlinable public static func + (lhs: Self, rhs: Self) -> Self {
        .init(rawValue: lhs.rawValue + rhs.rawValue)
    }

    /// Adds two storages and stores their element-wise sums in the lhs variable.
    @inlinable public static func += (lhs: inout Self, rhs: Self) {
        lhs.rawValue += rhs.rawValue
    }

    /// Subtracts one storage from another and produces their element-wise differences.
    /// - Parameter lhs: A numeric storage.
    /// - Parameter rhs: The numeric storage to subtract from `lhs`.
    @inlinable public static func - (lhs: Self, rhs: Self) -> Self {
        .init(rawValue: lhs.rawValue - rhs.rawValue)
    }

    /// Subtracts the second storage from the first and stores the element-wise differences in the lhs variable.
    @inlinable public static func -= (lhs: inout Self, rhs: Self) {
        lhs.rawValue -= rhs.rawValue
    }
}

// MARK: Numeric

extension SIMDX where Storage: NumericStorage {

    /// Creates a new instance from the given integer storage, if all can be represented exactly.
    @inlinable public init?<Source>(
        exactly value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryInteger {
        guard let rawValue = Storage.init(exactly: value.rawValue) else { return nil }
        self.init(rawValue: rawValue)
    }

    /// Multiplies two storages and produces their element-wise products.
    @inlinable public static func * (lhs: Self, rhs: Self) -> Self {
        .init(rawValue: lhs.rawValue * rhs.rawValue)
    }

    /// Multiplies two storages element-wise and stores their products in the left-hand-side variable.
    @inlinable public static func *= (lhs: inout Self, rhs: Self) {
        lhs.rawValue *= rhs.rawValue
    }

    /// Multiplies the storage element-wise by given element, and returns their products.
    @inlinable public static func * (storage: Self, element: Element) -> Self {
        .init(rawValue: storage.rawValue * element)
    }

    /// Replaces the storage with the products of current storage elements multiplied element-wise by given element.
    @inlinable public static func *= (storage: inout Self, element: Element) {
        storage.rawValue *= element
    }
}

// MARK: Signed Numeric

extension SIMDX where Storage: SignedNumericStorage {

    /// Returns the additive inverse of the specified storage.
    @inlinable prefix public static func - (operand: Self) -> Self {
       .init(rawValue: -(operand.rawValue))
    }

    /// Replaces this value with its additive inverse.
    @inlinable public mutating func negate() {
        self.rawValue = -(self.rawValue)
    }
}

// MARK: Binary Integer

extension SIMDX where Storage: BinaryIntegerStorage {

    /// A Boolean value indicating whether this storage contains signed integer types.
    @inlinable public static var isSigned: Bool { Storage.isSigned }

    @inlinable public init?<Source>(
        exactly value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryFloatingPoint {
        guard let rawValue = Storage.init(exactly: value.rawValue) else { return nil }
        self.init(rawValue: rawValue)
    }

    @inlinable public init<Source>(
        _ value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryFloatingPoint {
        self.init(rawValue: Storage.init(value.rawValue))
    }

    @inlinable public init<Source>(
        _ value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryInteger {
        self.init(rawValue: Storage.init(value.rawValue))
    }

    @inlinable public init<Source>(
        truncatingIfNeeded value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryInteger {
        self.init(rawValue: Storage.init(truncatingIfNeeded: value.rawValue))
    }

    @inlinable public init<Source>(
        clamping value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryInteger {
        self.init(rawValue: Storage.init(clamping: value.rawValue))
    }

    @inlinable public prefix static func ~ (operand: Self) -> Self {
        .init(rawValue: ~(operand.rawValue))
    }

    @inlinable public static func & (lhs: Self, rhs: Self) -> Self {
        .init(rawValue: lhs.rawValue & rhs.rawValue)
    }

    @inlinable public static func &= (lhs: inout Self, rhs: Self) {
        lhs.rawValue &= rhs.rawValue
    }

    @inlinable public static func | (lhs: Self, rhs: Self) -> Self {
        .init(rawValue: lhs.rawValue | rhs.rawValue)
    }

    @inlinable public static func |= (lhs: inout Self, rhs: Self) {
        lhs.rawValue |= rhs.rawValue
    }

    @inlinable public static func ^ (lhs: Self, rhs: Self) -> Self {
        .init(rawValue: lhs.rawValue ^ rhs.rawValue)
    }

    @inlinable public static func ^= (lhs: inout Self, rhs: Self) {
        lhs.rawValue ^= rhs.rawValue
    }

    @inlinable public static func >> <RHS>(lhs: Self, rhs: RHS) -> Self where RHS : BinaryInteger {
        .init(rawValue: lhs.rawValue >> rhs)
    }

    @inlinable public static func >>= <RHS>(lhs: inout Self, rhs: RHS) where RHS : BinaryInteger {
        lhs.rawValue >>= rhs
    }

    @inlinable public static func << <RHS>(lhs: Self, rhs: RHS) -> Self where RHS : BinaryInteger {
        .init(rawValue: lhs.rawValue << rhs)
    }

    @inlinable public static func <<= <RHS>(lhs: inout Self, rhs: RHS) where RHS : BinaryInteger {
        lhs.rawValue <<= rhs
    }
}

// MARK: Fixed Width Integer

extension SIMDX where Storage: FixedWidthIntegerStorage {

    /// The maximum representable integer in this type.
    ///
    /// For unsigned integer types, this value is `(2 ** bitWidth) - 1`, where
    /// `**` is exponentiation. For signed integer types, this value is `(2 ** (bitWidth - 1)) - 1`.
    @inlinable public static var max: Self { .init(rawValue: .max) }

    /// The minimum representable integer in this type.
    ///
    /// For unsigned integer types, this value is always `0`. For signed integer
    /// types, this value is `-(2 ** (bitWidth - 1))`, where `**` is exponentiation.
    @inlinable public static var min: Self { .init(rawValue: .min) }
}

// MARK: Floating Point

extension SIMDX where Storage: FloatingPointStorage {

//    @inlinable public init<Source>(
//        _ value: Source
//    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryInteger {
//        self.init(rawValue: Storage.init(value.rawValue))
//    }
//
//    @inlinable public init?<Source>(
//        exactly value: Source
//    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryInteger {
//        guard let rawValue = Storage.init(exactly: value.rawValue) else { return nil }
//        self.init(rawValue: rawValue)
//    }

    @inlinable public static var infinity: Self {
        .init(rawValue: .infinity)
    }

    @inlinable public static var greatestFiniteMagnitude: Self {
        .init(rawValue: .greatestFiniteMagnitude)
    }

    @inlinable public static var pi: Self {
        .init(rawValue: .pi)
    }

    @inlinable public static var ulpOfOne: Self {
        .init(rawValue: .ulpOfOne)
    }

    @inlinable public static var leastNormalMagnitude: Self {
        .init(rawValue: .leastNormalMagnitude)
    }

    @inlinable public static var leastNonzeroMagnitude: Self {
        .init(rawValue: .leastNonzeroMagnitude)
    }

    @inlinable public static func / (lhs: Self, rhs: Self) -> Self  {
        .init(rawValue: lhs.rawValue / rhs.rawValue)
    }

    @inlinable public static func /= (lhs: inout Self, rhs: Self) {
        lhs.rawValue /= rhs.rawValue
    }

    @inlinable public static func / (storage: Self, element: Element) -> Self  {
        .init(rawValue: storage.rawValue / element)
    }

    @inlinable public static func /= (lhs: inout Self, rhs: Element) {
        lhs.rawValue /= rhs
    }

    @inlinable public func squareRoot() -> Self {
        .init(rawValue: rawValue.squareRoot())
    }

    /// Replaces this storage with its square root, rounded to a representable value.
    @inlinable public mutating func formSquareRoot() {
        rawValue.formSquareRoot()
    }

    @inlinable public static func minimum(_ lhs: Self, _ rhs: Self) -> Self {
        .init(rawValue: .minimum(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func maximum(_ lhs: Self, _ rhs: Self) -> Self {
        .init(rawValue: .maximum(lhs.rawValue, rhs.rawValue))
    }
}

// MARK: Binary FloatingPoint

extension SIMDX where Storage: BinaryFloatingPointStorage {

    @inlinable public init<Source>(
        _ value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryFloatingPoint {
        self.init(rawValue: Storage.init(value.rawValue))
    }

    @inlinable public init?<Source>(
        exactly value: Source
    ) where Source: SIMDX, Source.Storage: NumericStorage, Source.Storage.Element: BinaryFloatingPoint {
        guard let rawValue = Storage.init(exactly: value.rawValue) else { return nil }
        self.init(rawValue: rawValue)
    }
}

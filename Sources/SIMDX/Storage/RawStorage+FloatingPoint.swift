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

/// A floating-point numeric storage type.
public protocol FloatingPointStorage: SignedNumericStorage where Element: FloatingPoint {

//    /// Creates a new storage with all elements rounded to the closest possible representation.
//    ///
//    /// - Parameter value: The integer storage to convert to a floating-point storage.
//    init<Source>(_ value: Source) where Source: NumericStorage, Source.Element: BinaryInteger

//    /// Creates a new storage, if the given integer storage can be represented exactly.
//    ///
//    /// If the given integer storage cannot be represented exactly, the result is `nil`.
//    ///
//    /// - Parameter value: The integer storage to convert to a floating-point storage.
//    init?<Source>(exactly value: Source) where Source: NumericStorage, Source.Element: BinaryInteger

    /// Positive infinity.
    ///
    /// Infinity compares greater than all finite numbers and equal to other infinite values.
    static var infinity: Self { get }

    /// The greatest finite number representable by elements in the storage.
    ///
    /// This value compares greater than or equal to all finite numbers, but less than `infinity`.
    static var greatestFiniteMagnitude: Self { get }

    /// The mathematical constant **pi** on all lanes.
    static var pi: Self { get }

    /// The unit in the last place of 1.0 on all lanes.
    ///
    /// The positive difference between 1.0 and the next greater representable number.
    /// `ulpOfOne` corresponds to the value represented by the C macros `FLT_EPSILON`,
    /// `DBL_EPSILON`, etc, and is sometimes called *epsilon* or *machine epsilon*.
    static var ulpOfOne: Self { get }

    /// The least positive normal number on all lanes.
    static var leastNormalMagnitude: Self { get }

    /// The least positive number on all lanes.
    /// otherwise they are equal.
    static var leastNonzeroMagnitude: Self { get }

    /// Returns the quotient of dividing the first storage by the second, rounded to a representable value.
    ///
    /// - Parameters:
    ///   - lhs: The storage to divide.
    ///   - rhs: The storage to divide `lhs` by.
    static func / (lhs: Self, rhs: Self) -> Self

    /// Divides the first storage by the second and stores the quotient in the left-hand-side variable.
    ///
    /// - Parameters:
    ///   - lhs: The storage to divide.
    ///   - rhs: The storage to divide `lhs` by.
    static func /= (lhs: inout Self, rhs: Self)

    /// Returns the quotient of dividing each element of the `storage` by `element`,
    /// rounded to a representable value.
    ///
    /// - Parameters:
    ///   - lhs: The storage to divide.
    ///   - rhs: The storage to divide `lhs` by.
    static func / (storage: Self, element: Element) -> Self

    /// Divides each element of the `storage` by `element` and stores the quotient in the left-hand-side variable.
    ///
    /// - Parameters:
    ///   - lhs: The storage to divide.
    ///   - rhs: The storage to divide `lhs` by.
    static func /= (storage: inout Self, element: Element)

    /// Returns the square roots of each value, rounded to a representable value.
    ///
    /// - Returns: The square roots of the value.
    func squareRoot() -> Self

    /// Replaces this storage with its square root, rounded to a representable value.
    mutating func formSquareRoot()

    /// Performs element-by-element comparison and returns the lesser of each comparison in a new storage.
    static func minimum(_ lhs: Self, _ rhs: Self) -> Self

    /// Performs element-by-element comparison and returns the greater of each comparison in a new storage.
    static func maximum(_ lhs: Self, _ rhs: Self) -> Self
}

extension FloatingPointStorage {

//    @inlinable public init<Source>(_ source: Source) where Source: NumericStorage, Source.Element: BinaryInteger {
//        self.init(source.map(Element.init))
//    }

//    @inlinable public init?<Source>(exactly: Source) where Source: NumericStorage, Source.Element: BinaryInteger {
//        let convertedElements = exactly.compactMap(Element.init(exactly:))
//        guard convertedElements.count == exactly.count else { return nil }
//        self.init(convertedElements)
//    }

    @inlinable public static var infinity: Self { .init(.infinity) }

    @inlinable public static var greatestFiniteMagnitude: Self { .init(.greatestFiniteMagnitude) }

    @inlinable public static var pi: Self { .init(.pi) }

    @inlinable public static var ulpOfOne: Self { .init(.ulpOfOne) }

    @inlinable public static var leastNormalMagnitude: Self { .init(.leastNormalMagnitude) }

    @inlinable public static var leastNonzeroMagnitude: Self { .init(.leastNonzeroMagnitude) }

    @inlinable public static func /= (lhs: inout Self, rhs: Self) { lhs = lhs / rhs }

    @inlinable public static func / (lhs: Self, rhs: Element) -> Self { lhs / .init(rhs) }

    @inlinable public static func /= (lhs: inout Self, rhs: Element) { lhs =  lhs / .init(rhs) }

    @inlinable public mutating func formSquareRoot() { self = squareRoot() }
}

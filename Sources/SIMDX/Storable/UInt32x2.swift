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
import CSIMDX

public struct UInt32x2: RawStorage2, FixedWidthIntegerStorage, UnsignedIntegerStorage {

    // MARK: Collection

    @inlinable public var count: Int { 2 }
    @inlinable public var startIndex: Int32 { 0 }
    @inlinable public var endIndex: Int32 { 2 }

    // MARK: RawValue

    public var rawValue: CXUInt32x2

    @inlinable public subscript(index: Int32) -> UInt32 {
        @inlinable set { CXUInt32x2SetElement(&rawValue, index, newValue) }
        @inlinable get { CXUInt32x2GetElement(rawValue, index) }
    }

    @inlinable public init(rawValue: CXUInt32x2) {
        self.rawValue = rawValue
    }
}

// MARK: - Initializer
extension UInt32x2 {

    @inlinable public init(_ repeatingElement: UInt32) {
        self.init(rawValue: CXUInt32x2MakeRepeatingElement(repeatingElement))
    }

    @inlinable public init(_ index0: UInt32, _ index1: UInt32) {
        self.init(rawValue: CXUInt32x2Make(index0, index1))
    }

    @inlinable public init(_ array: [UInt32]) {
        var array = array
        self.init(rawValue: CXUInt32x2MakeLoad(&array))
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == UInt32 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension UInt32x2 {

    @inlinable public static func minimum(_ lhs: UInt32x2, _ rhs: UInt32x2) -> UInt32x2 {
        .init(rawValue: CXUInt32x2Minimum(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func maximum(_ lhs: UInt32x2, _ rhs: UInt32x2) -> UInt32x2 {
        .init(rawValue: CXUInt32x2Maximum(lhs.rawValue, rhs.rawValue))
    }
}

// MARK: - Bitwise
extension UInt32x2 {

    @inlinable public  prefix static func ~ (operand: UInt32x2) -> UInt32x2 {
        return .init(rawValue: CXUInt32x2BitwiseNot(operand.rawValue))
    }

    @inlinable public static func & (lhs: UInt32x2, rhs: UInt32x2) -> UInt32x2 {
        return .init(rawValue: CXUInt32x2BitwiseAnd(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func | (lhs: UInt32x2, rhs: UInt32x2) -> UInt32x2 {
        return .init(rawValue: CXUInt32x2BitwiseOr(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func ^ (lhs: UInt32x2, rhs: UInt32x2) -> UInt32x2 {
        return .init(rawValue: CXUInt32x2BitwiseExclusiveOr(lhs.rawValue, rhs.rawValue))
    }

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: UInt32x2, rhs: RHS) -> UInt32x2 where RHS: BinaryInteger {
        return .init(rawValue: CXUInt32x2ShiftLeft(lhs.rawValue, numericCast(rhs)))
    }

    @inlinable public static func << <RHS>(lhs: UInt32x2, rhs: RHS) -> UInt32x2 where RHS: BinaryInteger {
        return .init(rawValue: CXUInt32x2ShiftRight(lhs.rawValue, numericCast(rhs)))
    }
}

// MARK: - Arithmetics
extension UInt32x2 {

    @inlinable public static var zero: UInt32x2 { .init(rawValue: CXUInt32x2MakeZero()) }

    @inlinable public var magnitude: UInt32x2 { .init(rawValue: CXUInt32x2Absolute(rawValue)) }

    // MARK: Additive

    @inlinable public static func + (lhs: UInt32x2, rhs: UInt32x2) -> UInt32x2 {
        .init(rawValue: CXUInt32x2Add(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func - (lhs: UInt32x2, rhs: UInt32x2) -> UInt32x2  {
        .init(rawValue: CXUInt32x2Subtract(lhs.rawValue, rhs.rawValue))
    }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: UInt32x2, rhs: UInt32x2) -> UInt32x2  {
        .init(rawValue: CXUInt32x2Multiply(lhs.rawValue, rhs.rawValue))
    }
}

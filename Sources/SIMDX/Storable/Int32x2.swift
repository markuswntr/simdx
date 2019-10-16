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

public struct Int32x2: RawStorage2, FixedWidthIntegerStorage, SignedIntegerStorage {

    // MARK: Collection

    @inlinable public var count: Int { 2 }
    @inlinable public var startIndex: Int32 { 0 }
    @inlinable public var endIndex: Int32 { 2 }

    // MARK: RawValue

    public var rawValue: CXInt32x2

    @inlinable public subscript(index: Int32) -> Int32 {
        @inlinable set { CXInt32x2SetElement(&rawValue, index, newValue) }
        @inlinable get { CXInt32x2GetElement(rawValue, index) }
    }

    @inlinable public init(rawValue: CXInt32x2) {
        self.rawValue = rawValue
    }
}

// MARK: - Initializer
extension Int32x2 {

    @inlinable public init(_ repeatingElement: Int32) {
        self.init(rawValue: CXInt32x2MakeRepeatingElement(repeatingElement))
    }

    @inlinable public init(_ index0: Int32, _ index1: Int32) {
        self.init(rawValue: CXInt32x2Make(index0, index1))
    }

    @inlinable public init(_ array: [Int32]) {
        var array = array
        self.init(rawValue: CXInt32x2MakeLoad(&array))
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Int32 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension Int32x2 {

    @inlinable public static func minimum(_ lhs: Int32x2, _ rhs: Int32x2) -> Int32x2 {
        .init(rawValue: CXInt32x2Minimum(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func maximum(_ lhs: Int32x2, _ rhs: Int32x2) -> Int32x2 {
        .init(rawValue: CXInt32x2Maximum(lhs.rawValue, rhs.rawValue))
    }
}

// MARK: - Bitwise
extension Int32x2 {

    @inlinable public  prefix static func ~ (operand: Int32x2) -> Int32x2 {
        return .init(rawValue: CXInt32x2BitwiseNot(operand.rawValue))
    }

    @inlinable public static func & (lhs: Int32x2, rhs: Int32x2) -> Int32x2 {
        return .init(rawValue: CXInt32x2BitwiseAnd(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func | (lhs: Int32x2, rhs: Int32x2) -> Int32x2 {
        return .init(rawValue: CXInt32x2BitwiseOr(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func ^ (lhs: Int32x2, rhs: Int32x2) -> Int32x2 {
        return .init(rawValue: CXInt32x2BitwiseExclusiveOr(lhs.rawValue, rhs.rawValue))
    }

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: Int32x2, rhs: RHS) -> Int32x2 where RHS: BinaryInteger {
        return .init(rawValue: CXInt32x2ShiftLeft(lhs.rawValue, numericCast(rhs)))
    }

    @inlinable public static func << <RHS>(lhs: Int32x2, rhs: RHS) -> Int32x2 where RHS: BinaryInteger {
        return .init(rawValue: CXInt32x2ShiftRight(lhs.rawValue, numericCast(rhs)))
    }
}

// MARK: - Arithmetics
extension Int32x2 {

    @inlinable public static var zero: Int32x2 { .init(rawValue: CXInt32x2MakeZero()) }

    @inlinable public var magnitude: UInt32x2 { .init(rawValue: CXInt32x2Absolute(rawValue)) }

    // MARK: Additive

    @inlinable public static func + (lhs: Int32x2, rhs: Int32x2) -> Int32x2 {
        .init(rawValue: CXInt32x2Add(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func - (lhs: Int32x2, rhs: Int32x2) -> Int32x2  {
        .init(rawValue: CXInt32x2Subtract(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static prefix func - (operand: Int32x2) -> Int32x2  {
        .init(rawValue: CXInt32x2Negate(operand.rawValue))
    }

    @inlinable public mutating func negate() {
        rawValue = CXInt32x2Negate(rawValue)
    }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: Int32x2, rhs: Int32x2) -> Int32x2  {
        .init(rawValue: CXInt32x2Multiply(lhs.rawValue, rhs.rawValue))
    }
}
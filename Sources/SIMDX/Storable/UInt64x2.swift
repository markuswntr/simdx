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

extension UInt64x2: RawStorage2, FixedWidthIntegerStorage, UnsignedIntegerStorage {

    @inlinable public subscript(index: Int32) -> UInt64 {
        @inlinable set { CXUInt64x2SetElement(&self, index, newValue) }
        @inlinable get { CXUInt64x2GetElement(self, index) }
    }
}

// MARK: - Initializer
extension UInt64x2 {

    @inlinable public init(_ repeatingElement: UInt64) {
        self = CXUInt64x2MakeRepeatingElement(repeatingElement)
    }

    @inlinable public init(_ index0: UInt64, _ index1: UInt64) {
        self = CXUInt64x2Make(index0, index1)
    }

    @inlinable public init(_ array: [UInt64]) {
        var array = array
        self = CXUInt64x2MakeLoad(&array)
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == UInt64 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension UInt64x2 {

    @inlinable public static func minimum(_ lhs: UInt64x2, _ rhs: UInt64x2) -> UInt64x2 { CXUInt64x2Minimum(lhs, rhs) }

    @inlinable public static func maximum(_ lhs: UInt64x2, _ rhs: UInt64x2) -> UInt64x2 { CXUInt64x2Maximum(lhs, rhs) }
}

// MARK: - Bitwise
extension UInt64x2 {

    @inlinable public  prefix static func ~ (operand: UInt64x2) -> UInt64x2 { CXUInt64x2BitwiseNot(operand) }

    @inlinable public static func & (lhs: UInt64x2, rhs: UInt64x2) -> UInt64x2 { CXUInt64x2BitwiseAnd(lhs, rhs) }

    @inlinable public static func | (lhs: UInt64x2, rhs: UInt64x2) -> UInt64x2 { CXUInt64x2BitwiseOr(lhs, rhs) }

    @inlinable public static func ^ (lhs: UInt64x2, rhs: UInt64x2) -> UInt64x2 { CXUInt64x2BitwiseExclusiveOr(lhs, rhs)}

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: UInt64x2, rhs: RHS) -> UInt64x2 where RHS: BinaryInteger {
        CXUInt64x2ShiftLeft(lhs, numericCast(rhs))
    }

    @inlinable public static func << <RHS>(lhs: UInt64x2, rhs: RHS) -> UInt64x2 where RHS: BinaryInteger {
        CXUInt64x2ShiftRight(lhs, numericCast(rhs))
    }
}

// MARK: - Arithmetics
extension UInt64x2 {

    @inlinable public static var zero: UInt64x2 { CXUInt64x2MakeZero() }

    @inlinable public var magnitude: UInt64x2 { CXUInt64x2Absolute(self) }

    // MARK: Additive

    @inlinable public static func + (lhs: UInt64x2, rhs: UInt64x2) -> UInt64x2 { CXUInt64x2Add(lhs, rhs) }

    @inlinable public static func - (lhs: UInt64x2, rhs: UInt64x2) -> UInt64x2  { CXUInt64x2Subtract(lhs, rhs) }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: UInt64x2, rhs: UInt64x2) -> UInt64x2  { CXUInt64x2Multiply(lhs, rhs) }
}

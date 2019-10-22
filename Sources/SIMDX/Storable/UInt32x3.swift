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

extension UInt32x3: RawStorage3, FixedWidthIntegerStorage, UnsignedIntegerStorage {

    @inlinable public subscript(index: Int32) -> UInt32 {
        @inlinable set { CXUInt32x3SetElement(&self, index, newValue) }
        @inlinable get { CXUInt32x3GetElement(self, index) }
    }
}

// MARK: - Initializer
extension UInt32x3 {

    @inlinable public init(_ repeatingElement: UInt32) {
        self = CXUInt32x3MakeRepeatingElement(repeatingElement)
    }

    @inlinable public init(_ index0: UInt32, _ index1: UInt32, _ index2: UInt32) {
        self = CXUInt32x3Make(index0, index1, index2)
    }

    @inlinable public init(_ array: [UInt32]) {
        var array = array
        self = CXUInt32x3MakeLoad(&array)
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == UInt32 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension UInt32x3 {

    @inlinable public static func minimum(_ lhs: UInt32x3, _ rhs: UInt32x3) -> UInt32x3 { CXUInt32x3Minimum(lhs, rhs) }

    @inlinable public static func maximum(_ lhs: UInt32x3, _ rhs: UInt32x3) -> UInt32x3 { CXUInt32x3Maximum(lhs, rhs) }
}

// MARK: - Bitwise
extension UInt32x3 {

    @inlinable public  prefix static func ~ (operand: UInt32x3) -> UInt32x3 { CXUInt32x3BitwiseNot(operand) }

    @inlinable public static func & (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 { CXUInt32x3BitwiseAnd(lhs, rhs) }

    @inlinable public static func | (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 { CXUInt32x3BitwiseOr(lhs, rhs) }

    @inlinable public static func ^ (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 { CXUInt32x3BitwiseExclusiveOr(lhs, rhs)}

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: UInt32x3, rhs: RHS) -> UInt32x3 where RHS: BinaryInteger {
        CXUInt32x3ShiftLeft(lhs, numericCast(rhs))
    }

    @inlinable public static func << <RHS>(lhs: UInt32x3, rhs: RHS) -> UInt32x3 where RHS: BinaryInteger {
        CXUInt32x3ShiftRight(lhs, numericCast(rhs))
    }
}

// MARK: - Arithmetics
extension UInt32x3 {

    @inlinable public static var zero: UInt32x3 { .init(0) }

    @inlinable public var magnitude: UInt32x3 { CXUInt32x3Absolute(self) }

    // MARK: Additive

    @inlinable public static func + (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 { CXUInt32x3Add(lhs, rhs) }

    @inlinable public static func - (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3  { CXUInt32x3Subtract(lhs, rhs) }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3  { CXUInt32x3Multiply(lhs, rhs) }
}

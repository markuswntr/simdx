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

extension Int64x2: RawStorage2, FixedWidthIntegerStorage, SignedIntegerStorage {

    @inlinable public subscript(index: Int32) -> Int64 {
        @inlinable set { CXInt64x2SetElement(&self, index, newValue) }
        @inlinable get { CXInt64x2GetElement(self, index) }
    }
}

// MARK: - Initializer
extension Int64x2 {

    @inlinable public init(_ repeatingElement: Int64) {
        self = CXInt64x2MakeRepeatingElement(repeatingElement)
    }

    @inlinable public init(_ index0: Int64, _ index1: Int64) {
        self = CXInt64x2Make(index0, index1)
    }

    @inlinable public init(_ array: [Int64]) {
        var array = array
        self = CXInt64x2MakeLoad(&array)
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Int64 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension Int64x2 {

    @inlinable public static func minimum(_ lhs: Int64x2, _ rhs: Int64x2) -> Int64x2 { CXInt64x2Minimum(lhs, rhs) }

    @inlinable public static func maximum(_ lhs: Int64x2, _ rhs: Int64x2) -> Int64x2 { CXInt64x2Maximum(lhs, rhs) }
}

// MARK: - Bitwise
extension Int64x2 {

    @inlinable public  prefix static func ~ (operand: Int64x2) -> Int64x2 { CXInt64x2BitwiseNot(operand) }

    @inlinable public static func & (lhs: Int64x2, rhs: Int64x2) -> Int64x2 { CXInt64x2BitwiseAnd(lhs, rhs) }

    @inlinable public static func | (lhs: Int64x2, rhs: Int64x2) -> Int64x2 { CXInt64x2BitwiseOr(lhs, rhs) }

    @inlinable public static func ^ (lhs: Int64x2, rhs: Int64x2) -> Int64x2 { CXInt64x2BitwiseExclusiveOr(lhs, rhs) }

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: Int64x2, rhs: RHS) -> Int64x2 where RHS: BinaryInteger {
        CXInt64x2ShiftLeft(lhs, numericCast(rhs))
    }

    @inlinable public static func << <RHS>(lhs: Int64x2, rhs: RHS) -> Int64x2 where RHS: BinaryInteger {
        CXInt64x2ShiftRight(lhs, numericCast(rhs))
    }
}

// MARK: - Arithmetics
extension Int64x2 {

    @inlinable public static var zero: Int64x2 { .init(0) }

    @inlinable public var magnitude: UInt64x2 { CXUInt64x2FromInt64x2(CXInt64x2Absolute(self)) }

    // MARK: Additive

    @inlinable public static func + (lhs: Int64x2, rhs: Int64x2) -> Int64x2 { CXInt64x2Add(lhs, rhs) }

    @inlinable public static func - (lhs: Int64x2, rhs: Int64x2) -> Int64x2  { CXInt64x2Subtract(lhs, rhs) }

    @inlinable public static prefix func - (operand: Int64x2) -> Int64x2  { CXInt64x2Negate(operand) }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: Int64x2, rhs: Int64x2) -> Int64x2  { CXInt64x2Multiply(lhs, rhs) }
}

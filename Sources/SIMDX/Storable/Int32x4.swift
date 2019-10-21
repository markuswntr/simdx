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

extension Int32x4: RawStorage4, FixedWidthIntegerStorage, SignedIntegerStorage {

    @inlinable public subscript(index: Int32) -> Int32 {
        @inlinable set { CXInt32x4SetElement(&self, index, newValue) }
        @inlinable get { CXInt32x4GetElement(self, index) }
    }
}

// MARK: - Initializer
extension Int32x4 {

    @inlinable public init(_ repeatingElement: Int32) {
        self = CXInt32x4MakeRepeatingElement(repeatingElement)
    }

    @inlinable public init(_ index0: Int32, _ index1: Int32, _ index2: Int32, _ index3: Int32) {
        self = CXInt32x4Make(index0, index1, index2, index3)
    }

    @inlinable public init(_ array: [Int32]) {
        var array = array
        self = CXInt32x4MakeLoad(&array)
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Int32 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension Int32x4 {

    @inlinable public static func minimum(_ lhs: Int32x4, _ rhs: Int32x4) -> Int32x4 { CXInt32x4Minimum(lhs, rhs) }

    @inlinable public static func maximum(_ lhs: Int32x4, _ rhs: Int32x4) -> Int32x4 { CXInt32x4Maximum(lhs, rhs) }
}

// MARK: - Bitwise
extension Int32x4 {

    @inlinable public  prefix static func ~ (operand: Int32x4) -> Int32x4 { CXInt32x4BitwiseNot(operand) }

    @inlinable public static func & (lhs: Int32x4, rhs: Int32x4) -> Int32x4 { CXInt32x4BitwiseAnd(lhs, rhs) }

    @inlinable public static func | (lhs: Int32x4, rhs: Int32x4) -> Int32x4 { CXInt32x4BitwiseOr(lhs, rhs) }

    @inlinable public static func ^ (lhs: Int32x4, rhs: Int32x4) -> Int32x4 { CXInt32x4BitwiseExclusiveOr(lhs, rhs) }

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: Int32x4, rhs: RHS) -> Int32x4 where RHS: BinaryInteger {
        CXInt32x4ShiftLeft(lhs, numericCast(rhs))
    }

    @inlinable public static func << <RHS>(lhs: Int32x4, rhs: RHS) -> Int32x4 where RHS: BinaryInteger {
        CXInt32x4ShiftRight(lhs, numericCast(rhs))
    }
}

// MARK: - Arithmetics
extension Int32x4 {

    @inlinable public static var zero: Int32x4 { CXInt32x4MakeZero() }

    @inlinable public var magnitude: UInt32x4 { CXUInt32x4FromInt32x4(CXInt32x4Absolute(self)) }

    // MARK: Additive

    @inlinable public static func + (lhs: Int32x4, rhs: Int32x4) -> Int32x4 { CXInt32x4Add(lhs, rhs) }

    @inlinable public static func - (lhs: Int32x4, rhs: Int32x4) -> Int32x4  { CXInt32x4Subtract(lhs, rhs) }

    @inlinable public static prefix func - (operand: Int32x4) -> Int32x4  { CXInt32x4Negate(operand) }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: Int32x4, rhs: Int32x4) -> Int32x4  { CXInt32x4Multiply(lhs, rhs) }
}

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

extension Int32x3: RawStorage3, FixedWidthIntegerStorage, SignedIntegerStorage {

    @inlinable public subscript(index: Int32) -> Int32 {
        @inlinable set { CXInt32x3SetElement(&self, index, newValue) }
        @inlinable get { CXInt32x3GetElement(self, index) }
    }
}

// MARK: - Initializer
extension Int32x3 {

    @inlinable public init(_ repeatingElement: Int32) {
        self = CXInt32x3MakeRepeatingElement(repeatingElement)
    }

    @inlinable public init(_ index0: Int32, _ index1: Int32, _ index2: Int32) {
        self = CXInt32x3Make(index0, index1, index2)
    }

    @inlinable public init(_ array: [Int32]) {
        var array = array
        self = CXInt32x3MakeLoad(&array)
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Int32 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension Int32x3 {

    @inlinable public static func minimum(_ lhs: Int32x3, _ rhs: Int32x3) -> Int32x3 { CXInt32x3Minimum(lhs, rhs) }

    @inlinable public static func maximum(_ lhs: Int32x3, _ rhs: Int32x3) -> Int32x3 { CXInt32x3Maximum(lhs, rhs) }
}

// MARK: - Bitwise
extension Int32x3 {
    @inlinable public  prefix static func ~ (operand: Int32x3) -> Int32x3 { CXInt32x3BitwiseNot(operand) }

    @inlinable public static func & (lhs: Int32x3, rhs: Int32x3) -> Int32x3 { CXInt32x3BitwiseAnd(lhs, rhs) }

    @inlinable public static func | (lhs: Int32x3, rhs: Int32x3) -> Int32x3 { CXInt32x3BitwiseOr(lhs, rhs) }

    @inlinable public static func ^ (lhs: Int32x3, rhs: Int32x3) -> Int32x3 { CXInt32x3BitwiseExclusiveOr(lhs, rhs) }

    // MARK: Shifting

    @inlinable public static func >> <RHS>(lhs: Int32x3, rhs: RHS) -> Int32x3 where RHS: BinaryInteger {
        CXInt32x3ShiftLeft(lhs, numericCast(rhs))
    }

    @inlinable public static func << <RHS>(lhs: Int32x3, rhs: RHS) -> Int32x3 where RHS: BinaryInteger {
        CXInt32x3ShiftRight(lhs, numericCast(rhs))
    }
}

// MARK: - Arithmetics
extension Int32x3 {

    @inlinable public static var zero: Int32x3 { .init(0) }

    @inlinable public var magnitude: UInt32x3 { CXUInt32x3FromInt32x3(CXInt32x3Absolute(self)) }

    // MARK: Additive

    @inlinable public static func + (lhs: Int32x3, rhs: Int32x3) -> Int32x3 { CXInt32x3Add(lhs, rhs) }

    @inlinable public static func - (lhs: Int32x3, rhs: Int32x3) -> Int32x3  { CXInt32x3Subtract(lhs, rhs) }

    @inlinable public static prefix func - (operand: Int32x3) -> Int32x3  { CXInt32x3Negate(operand) }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: Int32x3, rhs: Int32x3) -> Int32x3  { CXInt32x3Multiply(lhs, rhs) }
}

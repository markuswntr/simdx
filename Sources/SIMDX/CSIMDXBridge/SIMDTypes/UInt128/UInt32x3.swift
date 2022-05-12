//// Copyright 2019 Markus Winter
////
//// Licensed under the Apache License, Version 2.0 (the "License");
//// you may not use this file except in compliance with the License.
//// You may obtain a copy of the License at
////
////     http://www.apache.org/licenses/LICENSE-2.0
////
//// Unless required by applicable law or agreed to in writing, software
//// distributed under the License is distributed on an "AS IS" BASIS,
//// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//// See the License for the specific language governing permissions and
//// limitations under the License.
//
//import Foundation
//import CSIMDX
//
//public struct UInt32x3: RawStorage3, FixedWidthIntegerStorage, UnsignedIntegerStorage {
//
//    // MARK: Collection
//
//    @inlinable public var count: Int { 3 }
//    @inlinable public var startIndex: Int32 { 0 }
//    @inlinable public var endIndex: Int32 { 3 }
//
//    // MARK: RawValue
//
//    public var rawValue: CXUInt32x3
//
//    @inlinable public subscript(index: Int32) -> UInt32 {
//        @inlinable set { CXUInt32x3SetElement(&rawValue, index, newValue) }
//        @inlinable get { CXUInt32x3GetElement(rawValue, index) }
//    }
//
//    @inlinable public init(rawValue: CXUInt32x3) {
//        self.rawValue = rawValue
//    }
//}
//
//// MARK: - Initializer
//extension UInt32x3 {
//
//    @inlinable public init(_ repeatingElement: UInt32) {
//        self.init(rawValue: CXUInt32x3MakeRepeatingElement(repeatingElement))
//    }
//
//    @inlinable public init(_ index0: UInt32, _ index1: UInt32, _ index2: UInt32) {
//        self.init(rawValue: CXUInt32x3Make(index0, index1, index2))
//    }
//
//    @inlinable public init(_ array: [UInt32]) {
//        var array = array
//        self.init(rawValue: CXUInt32x3MakeLoad(&array))
//    }
//
//    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == UInt32 {
//        self.init(Array(sequence))
//    }
//}
//
//// MARK: - Comparison
//extension UInt32x3 {
//
//    @inlinable public static func minimum(_ lhs: UInt32x3, _ rhs: UInt32x3) -> UInt32x3 {
//        .init(rawValue: CXUInt32x3Minimum(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func maximum(_ lhs: UInt32x3, _ rhs: UInt32x3) -> UInt32x3 {
//        .init(rawValue: CXUInt32x3Maximum(lhs.rawValue, rhs.rawValue))
//    }
//}
//
//// MARK: - Bitwise
//extension UInt32x3 {
//
//    @inlinable public  prefix static func ~ (operand: UInt32x3) -> UInt32x3 {
//        return .init(rawValue: CXUInt32x3BitwiseNot(operand.rawValue))
//    }
//
//    @inlinable public static func & (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 {
//        return .init(rawValue: CXUInt32x3BitwiseAnd(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func | (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 {
//        return .init(rawValue: CXUInt32x3BitwiseOr(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func ^ (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 {
//        return .init(rawValue: CXUInt32x3BitwiseExclusiveOr(lhs.rawValue, rhs.rawValue))
//    }
//
//    // MARK: Shifting
//
//    @inlinable public static func >> <RHS>(lhs: UInt32x3, rhs: RHS) -> UInt32x3 where RHS: BinaryInteger {
//        return .init(rawValue: CXUInt32x3ShiftLeft(lhs.rawValue, numericCast(rhs)))
//    }
//
//    @inlinable public static func << <RHS>(lhs: UInt32x3, rhs: RHS) -> UInt32x3 where RHS: BinaryInteger {
//        return .init(rawValue: CXUInt32x3ShiftRight(lhs.rawValue, numericCast(rhs)))
//    }
//}
//
//// MARK: - Arithmetics
//extension UInt32x3 {
//
//    @inlinable public static var zero: UInt32x3 { .init(rawValue: CXUInt32x3MakeZero()) }
//
//    @inlinable public var magnitude: UInt32x3 { .init(rawValue: CXUInt32x3Absolute(rawValue)) }
//
//    // MARK: Additive
//
//    @inlinable public static func + (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 {
//        .init(rawValue: CXUInt32x3Add(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func - (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3  {
//        .init(rawValue: CXUInt32x3Subtract(lhs.rawValue, rhs.rawValue))
//    }
//
//    // MARK: Multiplicative
//
//    @inlinable public static func * (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3  {
//        .init(rawValue: CXUInt32x3Multiply(lhs.rawValue, rhs.rawValue))
//    }
//}

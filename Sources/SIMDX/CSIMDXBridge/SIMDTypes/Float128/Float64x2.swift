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
//public struct Float64x2: RawStorage2, BinaryFloatingPointStorage {
//
//    // MARK: Collection
//
//    @inlinable public var count: Int { 2 }
//    @inlinable public var startIndex: Int32 { 0 }
//    @inlinable public var endIndex: Int32 { 2 }
//
//    // MARK: RawValue
//
//    public var rawValue: CXFloat64x2
//
//    @inlinable public subscript(index: Int32) -> Float64 {
//        @inlinable set { CXFloat64x2SetElement(&rawValue, index, newValue) }
//        @inlinable get { CXFloat64x2GetElement(rawValue, index) }
//    }
//
//    @inlinable public init(rawValue: CXFloat64x2) {
//        self.rawValue = rawValue
//    }
//}
//
//// MARK: - Initializer
//extension Float64x2 {
//
//    @inlinable public init(_ repeatingElement: Float64) {
//        self.init(rawValue: CXFloat64x2MakeRepeatingElement(repeatingElement))
//    }
//
//    @inlinable public init(_ index0: Float64, _ index1: Float64) {
//        self.init(rawValue: CXFloat64x2Make(index0, index1))
//    }
//
//    @inlinable public init(_ array: [Float64]) {
//        var array = array
//        self.init(rawValue: CXFloat64x2MakeLoad(&array))
//    }
//
//    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Float64 {
//        self.init(Array(sequence))
//    }
//}
//
//// MARK: - Comparison
//extension Float64x2 {
//
//    @inlinable public static func minimum(_ lhs: Float64x2, _ rhs: Float64x2) -> Float64x2 {
//        .init(rawValue: CXFloat64x2Minimum(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func maximum(_ lhs: Float64x2, _ rhs: Float64x2) -> Float64x2 {
//        .init(rawValue: CXFloat64x2Maximum(lhs.rawValue, rhs.rawValue))
//    }
//}
//
//// MARK: - Arithmetics
//extension Float64x2 {
//
//    @inlinable public static var zero: Float64x2 { .init(rawValue: CXFloat64x2MakeZero()) }
//
//    @inlinable public var magnitude: Float64x2 { .init(rawValue: CXFloat64x2Absolute(rawValue)) }
//
//    // MARK: Additive
//
//    @inlinable public static func + (lhs: Float64x2, rhs: Float64x2) -> Float64x2 {
//        .init(rawValue: CXFloat64x2Add(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func - (lhs: Float64x2, rhs: Float64x2) -> Float64x2  {
//        .init(rawValue: CXFloat64x2Subtract(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static prefix func - (operand: Float64x2) -> Float64x2  {
//        .init(rawValue: CXFloat64x2Negate(operand.rawValue))
//    }
//
//    @inlinable public mutating func negate() {
//        rawValue = CXFloat64x2Negate(rawValue)
//    }
//
//    // MARK: Multiplicative
//
//    @inlinable public static func * (lhs: Float64x2, rhs: Float64x2) -> Float64x2  {
//        .init(rawValue: CXFloat64x2Multiply(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public static func / (lhs: Float64x2, rhs: Float64x2) -> Float64x2  {
//        .init(rawValue: CXFloat64x2Divide(lhs.rawValue, rhs.rawValue))
//    }
//
//    @inlinable public func squareRoot() -> Float64x2 {
//        .init(rawValue: CXFloat64x2SquareRoot(rawValue))
//    }
//}

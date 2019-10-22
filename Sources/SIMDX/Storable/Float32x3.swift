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

extension Float32x3: RawStorage3, BinaryFloatingPointStorage {

    @inlinable public subscript(index: Int32) -> Float32 {
        @inlinable set { CXFloat32x3SetElement(&self, index, newValue) }
        @inlinable get { CXFloat32x3GetElement(self, index) }
    }
}

// MARK: - Initializer
extension Float32x3 {

    @inlinable public init(_ repeatingElement: Float32) {
        self = CXFloat32x3MakeRepeatingElement(repeatingElement)
    }

    @inlinable public init(_ index0: Float32, _ index1: Float32, _ index2: Float32) {
        self = CXFloat32x3Make(index0, index1, index2)
    }

    @inlinable public init(_ array: [Float32]) {
        var array = array
        self = CXFloat32x3MakeLoad(&array)
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Float32 {
        self.init(Array(sequence))
    }
}

// MARK: - Comparison
extension Float32x3 {

    @inlinable public static func minimum(_ lhs: Float32x3, _ rhs: Float32x3) -> Float32x3 {
        CXFloat32x3Minimum(lhs, rhs)
    }

    @inlinable public static func maximum(_ lhs: Float32x3, _ rhs: Float32x3) -> Float32x3 {
        CXFloat32x3Maximum(lhs, rhs)
    }
}

// MARK: - Arithmetics
extension Float32x3 {

    @inlinable public static var zero: Float32x3 { .init(0) }

    @inlinable public var magnitude: Float32x3 { CXFloat32x3Absolute(self) }

    // MARK: Additive

    @inlinable public static func + (lhs: Float32x3, rhs: Float32x3) -> Float32x3 { CXFloat32x3Add(lhs, rhs) }

    @inlinable public static func - (lhs: Float32x3, rhs: Float32x3) -> Float32x3  { CXFloat32x3Subtract(lhs, rhs) }

    @inlinable public static prefix func - (operand: Float32x3) -> Float32x3  { CXFloat32x3Negate(operand) }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: Float32x3, rhs: Float32x3) -> Float32x3  { CXFloat32x3Multiply(lhs, rhs) }

    @inlinable public static func / (lhs: Float32x3, rhs: Float32x3) -> Float32x3  { CXFloat32x3Divide(lhs, rhs) }

    @inlinable public func squareRoot() -> Float32x3 { CXFloat32x3SquareRoot(self) }
}

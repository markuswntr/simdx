import Foundation
import CSIMDX

public struct UInt32x3: RawStorage3, FixedWidthIntegerRawStorage, UnsignedIntegerRawStorage {

    // MARK: Collection Conformance

    public let startIndex: Int32 = 0

    public let endIndex: Int32 = 3

    // MARK: Raw Value Access

    public var rawValue: CXUInt32x3

    @inlinable public subscript(index: Int32) -> UInt32 {
        @inlinable set { CXUInt32x3SetElement(&rawValue, index, newValue) }
        @inlinable get { CXUInt32x3GetElement(rawValue, index) }
    }

    @inlinable public init(rawValue: CXUInt32x3) {
        self.rawValue = rawValue
    }
}

// MARK: - Designated Initializer
extension UInt32x3 {

    @inlinable public init(_ repeatingElement: UInt32) {
        self.init(rawValue: CXUInt32x3MakeRepeatingElement(repeatingElement))
    }

    @inlinable public init(_ index0: UInt32, _ index1: UInt32, _ index2: UInt32) {
        self.init(rawValue: CXUInt32x3Make(index0, index1, index2))
    }

    @inlinable public init(_ array: [UInt32]) {
        var array = array
        self.init(rawValue: CXUInt32x3MakeLoad(&array))
    }

    @inlinable public init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == UInt32 {
        self.init(Array(sequence))
    }
}

// MARK: - Arithmetics
extension UInt32x3 {

    @inlinable public static var zero: UInt32x3 { .init(rawValue: CXUInt32x3MakeZero()) }

    @inlinable public var magnitude: UInt32x3 { .init(rawValue: CXUInt32x3Absolute(rawValue)) }

    // MARK: Additive

    @inlinable public static func + (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3 {
        .init(rawValue: CXUInt32x3Add(lhs.rawValue, rhs.rawValue))
    }

    @inlinable public static func - (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3  {
        .init(rawValue: CXUInt32x3Subtract(lhs.rawValue, rhs.rawValue))
    }

    // MARK: Multiplicative

    @inlinable public static func * (lhs: UInt32x3, rhs: UInt32x3) -> UInt32x3  {
        .init(rawValue: CXUInt32x3Multiply(lhs.rawValue, rhs.rawValue))
    }
}

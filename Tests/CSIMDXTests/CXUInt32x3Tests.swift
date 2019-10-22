import XCTest
@testable import CSIMDX

final class CXUInt32x3Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXUInt32x3Make(1, 2, 3)

        XCTAssertEqual(CXUInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 2), 3)
    }

    func testMakeLoad() {
        var array: [UInt32] = [1, 2, 3]
        let collection = CXUInt32x3MakeLoad(&array)

        XCTAssertEqual(CXUInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 2), 3)
    }

    func testMakeRepeatingElement() {
        let collection = CXUInt32x3MakeRepeatingElement(3)

        XCTAssertEqual(CXUInt32x3GetElement(collection, 0), 3)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 1), 3)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 2), 3)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXUInt32x3Make(1, 2, 3)

        XCTAssertEqual(CXUInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 2), 3)
    }

    func testSetElement() {
        var collection = CXUInt32x3Make(1, 2, 3)

        XCTAssertEqual(CXUInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 2), 3)

        CXUInt32x3SetElement(&collection, 0, 5)
        CXUInt32x3SetElement(&collection, 1, 6)
        CXUInt32x3SetElement(&collection, 2, 7)

        XCTAssertEqual(CXUInt32x3GetElement(collection, 0), 5)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 1), 6)
        XCTAssertEqual(CXUInt32x3GetElement(collection, 2), 7)
    }

    // MARK: Conversion

    func testConvertFromFloat32x3() {
        let fromStorage = CXFloat32x3Make(2.49, 0.51, 1.5)
        let storage = CXUInt32x3FromFloat32x3(fromStorage)

        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 1)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), 2)
    }

    func testConvertFromInt32x3() {
        let fromStorage = CXInt32x3Make(2, 5, .max)
        let storage = CXUInt32x3FromInt32x3(fromStorage)

        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 5)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), UInt32(Int32.max))
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXUInt32x3Make(34, 12, .max)
        let rhs = CXUInt32x3Make(34, 24, .min)
        let storage = CXUInt32x3Minimum(lhs, rhs)

        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 12)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), .min)
    }

    func testMaximum() {
        let lhs = CXUInt32x3Make(34, 12, .max)
        let rhs = CXUInt32x3Make(34, 24, .min)
        let storage = CXUInt32x3Maximum(lhs, rhs)

        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 24)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), .max)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXUInt32x3Make(1, 0, 3)
        let absolute = CXUInt32x3Absolute(normal)

        XCTAssertEqual(CXUInt32x3GetElement(absolute, 0), 1)
        XCTAssertEqual(CXUInt32x3GetElement(absolute, 1), 0)
        XCTAssertEqual(CXUInt32x3GetElement(absolute, 2), 3)
    }

    func testAdd() {
        let lhs = CXUInt32x3Make(1, 2, 3)
        let rhs = CXUInt32x3Make(4, 3, 2)
        let sum = CXUInt32x3Add(lhs, rhs)

        XCTAssertEqual(CXUInt32x3GetElement(sum, 0), 5)
        XCTAssertEqual(CXUInt32x3GetElement(sum, 1), 5)
        XCTAssertEqual(CXUInt32x3GetElement(sum, 2), 5)
    }

    func testSubtract() {
        let lhs = CXUInt32x3Make(4, 3, 5)
        let rhs = CXUInt32x3Make(1, 1, 5)
        let sum = CXUInt32x3Subtract(lhs, rhs)

        XCTAssertEqual(CXUInt32x3GetElement(sum, 0), 3)
        XCTAssertEqual(CXUInt32x3GetElement(sum, 1), 2)
        XCTAssertEqual(CXUInt32x3GetElement(sum, 2), 0)
    }

    func testMultiply() {
        let lhs = CXUInt32x3Make(1, 2, 3)
        let rhs = CXUInt32x3Make(4, 3, 2)
        let product = CXUInt32x3Multiply(lhs, rhs)

        XCTAssertEqual(CXUInt32x3GetElement(product, 0), 4)
        XCTAssertEqual(CXUInt32x3GetElement(product, 1), 6)
        XCTAssertEqual(CXUInt32x3GetElement(product, 2), 6)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXUInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let storage = CXUInt32x3BitwiseNot(operand)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), ~(0b1111_0000))
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), ~(0b0011_1100))
    }

    func testBitwiseAnd() {
        let lhs = CXUInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXUInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXUInt32x3BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), 0b0011_1100 & 0b1100_0011)
    }

    func testBitwiseAndNot() {
        let lhs = CXUInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXUInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXUInt32x3BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), (~0b0011_1100) & 0b1100_0011)
    }

    func testBitwiseOr() {
        let lhs = CXUInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXUInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXUInt32x3BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), 0b0011_1100 | 0b1100_0011)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXUInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXUInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXUInt32x3BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), 0b0011_1100 ^ 0b1100_0011)
    }

    func testShiftLeft() {
        let lhs = CXUInt32x3Make(0b1111_1111, 0b0000_1111, 0b0011_1100)
        let storage = CXUInt32x3ShiftLeft(lhs, 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 0b0000_1111 << 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), 0b0011_1100 << 2)
    }

    func testShiftRight() {
        let lhs = CXUInt32x3Make(0b1111_1111, 0b0000_1111, 0b0011_1100)
        let storage = CXUInt32x3ShiftRight(lhs, 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 1), 0b0000_1111 >> 2)
        XCTAssertEqual(CXUInt32x3GetElement(storage, 2), 0b0011_1100 >> 2)
    }

    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat32x3", testConvertFromFloat32x3),
        ("testConvertFromInt32x3", testConvertFromInt32x3),
        ("testMinimum", testMinimum),
        ("testMaximum", testMaximum),
        ("testAbsolute", testAbsolute),
        ("testAdd", testAdd),
        ("testSubtract", testSubtract),
        ("testMultiply", testMultiply),
        ("testBitwiseNot", testBitwiseNot),
        ("testBitwiseAnd", testBitwiseAnd),
        ("testBitwiseAndNot", testBitwiseAndNot),
        ("testBitwiseOr", testBitwiseOr),
        ("testBitwiseExclusiveOr", testBitwiseExclusiveOr),
        ("testShiftLeft", testShiftLeft),
        ("testShiftRight", testShiftRight),
    ]
}

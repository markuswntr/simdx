import XCTest
import CSIMDX

final class CXInt32x3Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXInt32x3Make(1, 2, 3)

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 3)
    }

    func testMakeLoad() {
        var array: [Int32] = [1, 2, 3]
        let collection = CXInt32x3MakeLoad(&array)

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 3)
    }

    func testMakeRepeatingElement() {
        let collection = CXInt32x3MakeRepeatingElement(3)

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 3)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 3)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 3)
    }

    func testMakeZero() {
        let collection = CXInt32x3MakeZero()

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 0)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 0)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 0)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXInt32x3Make(1, 2, 3)

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 3)
    }

    func testSetElement() {
        var collection = CXInt32x3Make(1, 2, 3)

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 3)

        CXInt32x3SetElement(&collection, 0, 5)
        CXInt32x3SetElement(&collection, 1, 6)
        CXInt32x3SetElement(&collection, 2, 7)

        XCTAssertEqual(CXInt32x3GetElement(collection, 0), 5)
        XCTAssertEqual(CXInt32x3GetElement(collection, 1), 6)
        XCTAssertEqual(CXInt32x3GetElement(collection, 2), 7)
    }

    // MARK: Conversion

    func testConvertFromFloat32x3() {
        let fromStorage = CXFloat32x3Make(2.49, 0.51, 1.5)
        let storage = CXInt32x3FromFloat32x3(fromStorage)

        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 1)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), 2)
    }

    func testConvertFromUInt32x3() {
        let fromStorage = CXUInt32x3Make(2, 5, .max)
        let storage = CXInt32x3FromUInt32x3(fromStorage)

        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 5)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), -1)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXInt32x3Make(34, 12, .max)
        let rhs = CXInt32x3Make(-34, 24, .min)
        let storage = CXInt32x3Minimum(lhs, rhs)

        XCTAssertEqual(CXInt32x3GetElement(storage, 0), -34)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 12)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), .min)
    }

    func testMaximum() {
        let lhs = CXInt32x3Make(34, 12, .max)
        let rhs = CXInt32x3Make(-34, 24, .min)
        let storage = CXInt32x3Maximum(lhs, rhs)

        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 34)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 24)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), .max)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXInt32x3Make(-1, 0, 3)
        let absolute = CXInt32x3Absolute(normal)

        XCTAssertEqual(CXInt32x3GetElement(absolute, 0), 1)
        XCTAssertEqual(CXInt32x3GetElement(absolute, 1), 0)
        XCTAssertEqual(CXInt32x3GetElement(absolute, 2), 3)
    }

    func testNegate() {
        let normal = CXInt32x3Make(-1, 0, 3)
        let negate = CXInt32x3Negate(normal)

        XCTAssertEqual(CXInt32x3GetElement(negate, 0), 1)
        XCTAssertEqual(CXInt32x3GetElement(negate, 1), 0)
        XCTAssertEqual(CXInt32x3GetElement(negate, 2), -3)
    }

    func testAdd() {
        let lhs = CXInt32x3Make(1, 2, 3)
        let rhs = CXInt32x3Make(4, 3, 2)
        let sum = CXInt32x3Add(lhs, rhs)

        XCTAssertEqual(CXInt32x3GetElement(sum, 0), 5)
        XCTAssertEqual(CXInt32x3GetElement(sum, 1), 5)
        XCTAssertEqual(CXInt32x3GetElement(sum, 2), 5)
    }

    func testSubtract() {
        let lhs = CXInt32x3Make(1, 2, 3)
        let rhs = CXInt32x3Make(4, 3, 2)
        let sum = CXInt32x3Subtract(lhs, rhs)

        XCTAssertEqual(CXInt32x3GetElement(sum, 0), -3)
        XCTAssertEqual(CXInt32x3GetElement(sum, 1), -1)
        XCTAssertEqual(CXInt32x3GetElement(sum, 2), 1)
    }

    func testMultiply() {
        let lhs = CXInt32x3Make(1, -2, -3)
        let rhs = CXInt32x3Make(4, -3, 2)
        let product = CXInt32x3Multiply(lhs, rhs)

        XCTAssertEqual(CXInt32x3GetElement(product, 0), 4)
        XCTAssertEqual(CXInt32x3GetElement(product, 1), 6)
        XCTAssertEqual(CXInt32x3GetElement(product, 2), -6)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let storage = CXInt32x3BitwiseNot(operand)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), ~(0b1111_0000))
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), ~(0b0011_1100))
    }

    func testBitwiseAnd() {
        let lhs = CXInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXInt32x3BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), 0b0011_1100 & 0b1100_0011)
    }

    func testBitwiseAndNot() {
        let lhs = CXInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXInt32x3BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), (~0b0011_1100) & 0b1100_0011)
    }

    func testBitwiseOr() {
        let lhs = CXInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXInt32x3BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), 0b0011_1100 | 0b1100_0011)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXInt32x3Make(0b0000_1111, 0b1111_0000, 0b0011_1100)
        let rhs = CXInt32x3Make(0b1111_1111, 0b0000_0000, 0b1100_0011)
        let storage = CXInt32x3BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), 0b0011_1100 ^ 0b1100_0011)
    }

    func testShiftLeft() {
        let lhs = CXInt32x3Make(0b1111_1111, 0b0000_1111, 0b0011_1100)
        let storage = CXInt32x3ShiftLeft(lhs, 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 0b0000_1111 << 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), 0b0011_1100 << 2)
    }

    func testShiftRight() {
        let lhs = CXInt32x3Make(0b1111_1111, 0b0000_1111, 0b0011_1100)
        let storage = CXInt32x3ShiftRight(lhs, 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 1), 0b0000_1111 >> 2)
        XCTAssertEqual(CXInt32x3GetElement(storage, 2), 0b0011_1100 >> 2)
    }

    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testMakeZero", testMakeZero),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat32x3", testConvertFromFloat32x3),
        ("testConvertFromUInt32x3", testConvertFromUInt32x3),
        ("testMinimum", testMinimum),
        ("testMaximum", testMaximum),
        ("testNegate", testNegate),
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

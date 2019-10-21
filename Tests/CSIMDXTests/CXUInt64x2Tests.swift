import XCTest
import CSIMDX

final class CXUInt64x2Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXUInt64x2Make(1, 2)

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 2)
    }

    func testMakeLoad() {
        var array: [UInt64] = [1, 2]
        let collection = CXUInt64x2MakeLoad(&array)

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 2)
    }

    func testMakeRepeatingElement() {
        let collection = CXUInt64x2MakeRepeatingElement(3)

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 3)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 3)
    }

    func testMakeZero() {
        let collection = CXUInt64x2MakeZero()

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 0)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 0)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXUInt64x2Make(1, 2)

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 2)
    }

    func testSetElement() {
        var collection = CXUInt64x2Make(1, 2)

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 2)

        CXUInt64x2SetElement(&collection, 0, 5)
        CXUInt64x2SetElement(&collection, 1, 6)

        XCTAssertEqual(CXUInt64x2GetElement(collection, 0), 5)
        XCTAssertEqual(CXUInt64x2GetElement(collection, 1), 6)
    }

    // MARK: Conversion

    func testConvertFromFloat32x2() {
        let fromStorage = CXFloat32x2Make(2.49, 0.51)
        let storage = CXUInt64x2FromFloat32x2(fromStorage)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 0)
    }

    func testConvertFromInt32x2() {
        let fromStorage = CXInt32x2Make(-34, 12)
        let storage = CXUInt64x2FromInt32x2(fromStorage)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), UInt64.max - 34 + 1) // Overflow
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 12)
    }

    func testConvertFromUInt32x2() {
        let fromStorage = CXUInt32x2Make(2, 5)
        let storage = CXUInt64x2FromUInt32x2(fromStorage)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 5)
    }

    func testConvertFromFloat64x2() {
        let fromStorage = CXFloat64x2Make(2.0000000, 1.9999999)
        let storage = CXUInt64x2FromFloat64x2(fromStorage)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 2)
    }

    func testConvertFromInt64x2() {
        let fromStorage = CXInt64x2Make(34, 12)
        let storage = CXUInt64x2FromInt64x2(fromStorage)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 12)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXUInt64x2Make(34, 12)
        let rhs = CXUInt64x2Make(35, 24)
        let storage = CXUInt64x2Minimum(lhs, rhs)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 12)
    }

    func testMaximum() {
        let lhs = CXUInt64x2Make(34, 12)
        let rhs = CXUInt64x2Make(34, 24)
        let storage = CXUInt64x2Maximum(lhs, rhs)

        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 24)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXUInt64x2Make(1, 3)
        let absolute = CXUInt64x2Absolute(normal)

        XCTAssertEqual(CXUInt64x2GetElement(absolute, 0), 1)
        XCTAssertEqual(CXUInt64x2GetElement(absolute, 1), 3)
    }

    func testAdd() {
        let lhs = CXUInt64x2Make(1, 2)
        let rhs = CXUInt64x2Make(4, 3)
        let sum = CXUInt64x2Add(lhs, rhs)

        XCTAssertEqual(CXUInt64x2GetElement(sum, 0), 5)
        XCTAssertEqual(CXUInt64x2GetElement(sum, 1), 5)
    }

    func testSubtract() {
        let lhs = CXUInt64x2Make(10, 5)
        let rhs = CXUInt64x2Make(5, 3)
        let sum = CXUInt64x2Subtract(lhs, rhs)

        XCTAssertEqual(CXUInt64x2GetElement(sum, 0), 5)
        XCTAssertEqual(CXUInt64x2GetElement(sum, 1), 2)

    }

    func testMultiply() {
        let lhs = CXUInt64x2Make(1, 2)
        let rhs = CXUInt64x2Make(4, 3)
        let product = CXUInt64x2Multiply(lhs, rhs)

        XCTAssertEqual(CXUInt64x2GetElement(product, 0), 4)
        XCTAssertEqual(CXUInt64x2GetElement(product, 1), 6)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXUInt64x2Make(0b0000_1111, 0b1111_0000)
        let storage = CXUInt64x2BitwiseNot(operand)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), ~(0b1111_0000))
    }

    func testBitwiseAnd() {
        let lhs = CXUInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXUInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXUInt64x2BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
    }

    func testBitwiseAndNot() {
        let lhs = CXUInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXUInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXUInt64x2BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
    }

    func testBitwiseOr() {
        let lhs = CXUInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXUInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXUInt64x2BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXUInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXUInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXUInt64x2BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
    }

    func testShiftLeft() {
        let lhs = CXUInt64x2Make(0b1111_1111, 0b0000_1111)
        let storage = CXUInt64x2ShiftLeft(lhs, 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 0b0000_1111 << 2)
    }

    func testShiftRight() {
        let lhs = CXUInt64x2Make(0b1111_1111, 0b0000_1111)
        let storage = CXUInt64x2ShiftRight(lhs, 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXUInt64x2GetElement(storage, 1), 0b0000_1111 >> 2)
    }


    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testMakeZero", testMakeZero),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat32x2", testConvertFromFloat32x2),
        ("testConvertFromInt32x2", testConvertFromInt32x2),
        ("testConvertFromUInt32x2", testConvertFromUInt32x2),
        ("testConvertFromFloat64x2", testConvertFromFloat64x2),
        ("testConvertFromUInt64x2", testConvertFromInt64x2),
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

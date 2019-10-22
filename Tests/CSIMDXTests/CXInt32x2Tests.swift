import XCTest
@testable import CSIMDX

final class CXInt32x2Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXInt32x2Make(1, 2)

        XCTAssertEqual(CXInt32x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x2GetElement(collection, 1), 2)
    }

    func testMakeLoad() {
        var array: [Int32] = [1, 2]
        let collection = CXInt32x2MakeLoad(&array)

        XCTAssertEqual(CXInt32x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x2GetElement(collection, 1), 2)
    }

    func testMakeRepeatingElement() {
        let collection = CXInt32x2MakeRepeatingElement(3)

        XCTAssertEqual(CXInt32x2GetElement(collection, 0), 3)
        XCTAssertEqual(CXInt32x2GetElement(collection, 1), 3)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXInt32x2Make(1, 2)

        XCTAssertEqual(CXInt32x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x2GetElement(collection, 1), 2)
    }

    func testSetElement() {
        var collection = CXInt32x2Make(1, 2)

        XCTAssertEqual(CXInt32x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x2GetElement(collection, 1), 2)

        CXInt32x2SetElement(&collection, 0, 5)
        CXInt32x2SetElement(&collection, 1, 6)

        XCTAssertEqual(CXInt32x2GetElement(collection, 0), 5)
        XCTAssertEqual(CXInt32x2GetElement(collection, 1), 6)
    }

    // MARK: Conversion

    func testConvertFromFloat32x2() {
        let fromStorage = CXFloat32x2Make(2.49, 0.51)
        let storage = CXInt32x2FromFloat32x2(fromStorage)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 1)
    }

    func testConvertFromUInt32x2() {
        let fromStorage = CXUInt32x2Make(2, 5)
        let storage = CXInt32x2FromUInt32x2(fromStorage)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 5)
    }

    func testConvertFromFloat64x2() {
        let fromStorage = CXFloat64x2Make(2.0000000, 1.9999999)
        let storage = CXInt32x2FromFloat64x2(fromStorage)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 2)
    }

    func testConvertFromInt64x2() {
        let fromStorage = CXInt64x2Make(-34, 12)
        let storage = CXInt32x2FromInt64x2(fromStorage)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 12)
    }

    func testConvertFromUInt64x2() {
        let fromStorage = CXUInt64x2Make(34, 12)
        let storage = CXInt32x2FromUInt64x2(fromStorage)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 12)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXInt32x2Make(34, 12)
        let rhs = CXInt32x2Make(-34, 24)
        let storage = CXInt32x2Minimum(lhs, rhs)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 12)
    }

    func testMaximum() {
        let lhs = CXInt32x2Make(34, 12)
        let rhs = CXInt32x2Make(-34, 24)
        let storage = CXInt32x2Maximum(lhs, rhs)

        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 24)
    }

    // MARK: Arithmetic

    func testNegate() {
        let normal = CXInt32x2Make(-1, 3)
        let negate = CXInt32x2Negate(normal)

        XCTAssertEqual(CXInt32x2GetElement(negate, 0), 1)
        XCTAssertEqual(CXInt32x2GetElement(negate, 1), -3)
    }

    func testAbsolute() {
        let normal = CXInt32x2Make(-1, 3)
        let absolute = CXInt32x2Absolute(normal)

        XCTAssertEqual(CXInt32x2GetElement(absolute, 0), 1)
        XCTAssertEqual(CXInt32x2GetElement(absolute, 1), 3)
    }

    func testAdd() {
        let lhs = CXInt32x2Make(1, 2)
        let rhs = CXInt32x2Make(4, 3)
        let sum = CXInt32x2Add(lhs, rhs)

        XCTAssertEqual(CXInt32x2GetElement(sum, 0), 5)
        XCTAssertEqual(CXInt32x2GetElement(sum, 1), 5)
    }

    func testSubtract() {
        let lhs = CXInt32x2Make(1, 2)
        let rhs = CXInt32x2Make(4, 3)
        let sum = CXInt32x2Subtract(lhs, rhs)

        XCTAssertEqual(CXInt32x2GetElement(sum, 0), -3)
        XCTAssertEqual(CXInt32x2GetElement(sum, 1), -1)

    }

    func testMultiply() {
        let lhs = CXInt32x2Make(1, -2)
        let rhs = CXInt32x2Make(-4, -3)
        let product = CXInt32x2Multiply(lhs, rhs)

        XCTAssertEqual(CXInt32x2GetElement(product, 0), -4)
        XCTAssertEqual(CXInt32x2GetElement(product, 1), 6)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXInt32x2Make(0b0000_1111, 0b1111_0000)
        let storage = CXInt32x2BitwiseNot(operand)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), ~(0b1111_0000))
    }

    func testBitwiseAnd() {
        let lhs = CXInt32x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt32x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt32x2BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
    }

    func testBitwiseAndNot() {
        let lhs = CXInt32x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt32x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt32x2BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
    }

    func testBitwiseOr() {
        let lhs = CXInt32x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt32x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt32x2BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXInt32x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt32x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt32x2BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
    }

    func testShiftLeft() {
        let lhs = CXInt32x2Make(0b1111_1111, 0b0000_1111)
        let storage = CXInt32x2ShiftLeft(lhs, 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 0b0000_1111 << 2)
    }

    func testShiftRight() {
        let lhs = CXInt32x2Make(0b1111_1111, 0b0000_1111)
        let storage = CXInt32x2ShiftRight(lhs, 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXInt32x2GetElement(storage, 1), 0b0000_1111 >> 2)
    }

    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat32x2", testConvertFromFloat32x2),
        ("testConvertFromUInt32x2", testConvertFromUInt32x2),
        ("testConvertFromFloat64x2", testConvertFromFloat64x2),
        ("testConvertFromInt64x2", testConvertFromInt64x2),
        ("testConvertFromUInt64x2", testConvertFromUInt64x2),
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

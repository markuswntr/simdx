import XCTest
import CSIMDX

final class CXInt64x2Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXInt64x2Make(1, 2)

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 2)
    }

    func testMakeLoad() {
        var array: [Int64] = [1, 2]
        let collection = CXInt64x2MakeLoad(&array)

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 2)
    }

    func testMakeRepeatingElement() {
        let collection = CXInt64x2MakeRepeatingElement(3)

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 3)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 3)
    }

    func testMakeZero() {
        let collection = CXInt64x2MakeZero()

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 0)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 0)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXInt64x2Make(1, 2)

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 2)
    }

    func testSetElement() {
        var collection = CXInt64x2Make(1, 2)

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 2)

        CXInt64x2SetElement(&collection, 0, 5)
        CXInt64x2SetElement(&collection, 1, 6)

        XCTAssertEqual(CXInt64x2GetElement(collection, 0), 5)
        XCTAssertEqual(CXInt64x2GetElement(collection, 1), 6)
    }

    // MARK: Conversion

    func testConvertFromFloat32x2() {
        let fromStorage = CXFloat32x2Make(2.49, 0.51)
        let storage = CXInt64x2FromFloat32x2(fromStorage)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 0)
    }

    func testConvertFromInt32x2() {
        let fromStorage = CXInt32x2Make(-34, 12)
        let storage = CXInt64x2FromInt32x2(fromStorage)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 12)
    }

    func testConvertFromUInt32x2() {
        let fromStorage = CXUInt32x2Make(2, 5)
        let storage = CXInt64x2FromUInt32x2(fromStorage)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 5)
    }

    func testConvertFromFloat64x2() {
        let fromStorage = CXFloat64x2Make(2.0000000, 1.9999999)
        let storage = CXInt64x2FromFloat64x2(fromStorage)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 1)
    }

    func testConvertFromUInt64x2() {
        let fromStorage = CXUInt64x2Make(34, 12)
        let storage = CXInt64x2FromUInt64x2(fromStorage)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 12)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXInt64x2Make(34, 12)
        let rhs = CXInt64x2Make(-34, 24)
        let storage = CXInt64x2Minimum(lhs, rhs)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 12)
    }

    func testMaximum() {
        let lhs = CXInt64x2Make(34, 12)
        let rhs = CXInt64x2Make(-34, 24)
        let storage = CXInt64x2Maximum(lhs, rhs)

        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 24)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXInt64x2Make(-1, 3)
        let absolute = CXInt64x2Absolute(normal)

        XCTAssertEqual(CXInt64x2GetElement(absolute, 0), 1)
        XCTAssertEqual(CXInt64x2GetElement(absolute, 1), 3)
    }

    func testNegate() {
        let normal = CXInt64x2Make(-1, 3)
        let negate = CXInt64x2Negate(normal)

        XCTAssertEqual(CXInt64x2GetElement(negate, 0), 1)
        XCTAssertEqual(CXInt64x2GetElement(negate, 1), -3)
    }

    func testAdd() {
        let lhs = CXInt64x2Make(1, 2)
        let rhs = CXInt64x2Make(4, 3)
        let sum = CXInt64x2Add(lhs, rhs)

        XCTAssertEqual(CXInt64x2GetElement(sum, 0), 5)
        XCTAssertEqual(CXInt64x2GetElement(sum, 1), 5)
    }

    func testSubtract() {
        let lhs = CXInt64x2Make(1, 2)
        let rhs = CXInt64x2Make(4, 3)
        let sum = CXInt64x2Subtract(lhs, rhs)

        XCTAssertEqual(CXInt64x2GetElement(sum, 0), -3)
        XCTAssertEqual(CXInt64x2GetElement(sum, 1), -1)

    }

    func testMultiply() {
        let lhs = CXInt64x2Make(1, -2)
        let rhs = CXInt64x2Make(-4, -3)
        let product = CXInt64x2Multiply(lhs, rhs)

        XCTAssertEqual(CXInt64x2GetElement(product, 0), -4)
        XCTAssertEqual(CXInt64x2GetElement(product, 1), 6)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXInt64x2Make(0b0000_1111, 0b1111_0000)
        let storage = CXInt64x2BitwiseNot(operand)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), ~(0b1111_0000))
    }

    func testBitwiseAnd() {
        let lhs = CXInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt64x2BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
    }

    func testBitwiseAndNot() {
        let lhs = CXInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt64x2BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
    }

    func testBitwiseOr() {
        let lhs = CXInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt64x2BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXInt64x2Make(0b0000_1111, 0b1111_0000)
        let rhs = CXInt64x2Make(0b1111_1111, 0b0000_0000)
        let storage = CXInt64x2BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
    }

    func testShiftLeft() {
        let lhs = CXInt64x2Make(0b1111_1111, 0b0000_1111)
        let storage = CXInt64x2ShiftLeft(lhs, 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 0b0000_1111 << 2)
    }

    func testShiftRight() {
        let lhs = CXInt64x2Make(0b1111_1111, 0b0000_1111)
        let storage = CXInt64x2ShiftRight(lhs, 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXInt64x2GetElement(storage, 1), 0b0000_1111 >> 2)
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

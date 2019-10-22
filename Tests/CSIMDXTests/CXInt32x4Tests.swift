import XCTest
@testable import CSIMDX

final class CXInt32x4Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXInt32x4Make(1, 2, 3, 4)
        
        XCTAssertEqual(CXInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 3), 4)
    }

    func testMakeLoad() {
        var array: [Int32] = [1, 2, 3, 4]
        let collection = CXInt32x4MakeLoad(&array)

        XCTAssertEqual(CXInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 3), 4)
    }

    func testMakeRepeatingElement() {
        let collection = CXInt32x4MakeRepeatingElement(3)

        XCTAssertEqual(CXInt32x4GetElement(collection, 0), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 1), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 3), 3)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXInt32x4Make(1, 2, 3, 4)
        
        XCTAssertEqual(CXInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 3), 4)
    }
    
    func testSetElement() {
        var collection = CXInt32x4Make(1, 2, 3, 4)

        XCTAssertEqual(CXInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXInt32x4GetElement(collection, 3), 4)

        CXInt32x4SetElement(&collection, 0, 5)
        CXInt32x4SetElement(&collection, 1, 6)
        CXInt32x4SetElement(&collection, 2, 7)
        CXInt32x4SetElement(&collection, 3, 8)

        XCTAssertEqual(CXInt32x4GetElement(collection, 0), 5)
        XCTAssertEqual(CXInt32x4GetElement(collection, 1), 6)
        XCTAssertEqual(CXInt32x4GetElement(collection, 2), 7)
        XCTAssertEqual(CXInt32x4GetElement(collection, 3), 8)
    }

    // MARK: Conversion

    func testConvertFromFloat32x4() {
        let fromStorage = CXFloat32x4Make(2.49, 0.51, 1.5, .greatestFiniteMagnitude)
        let storage = CXInt32x4FromFloat32x4(fromStorage)

        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 1)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), .min) // Overflow
    }

    func testConvertFromUInt32x4() {
        let fromStorage = CXUInt32x4Make(2, 5, .max, .min)
        let storage = CXInt32x4FromUInt32x4(fromStorage)

        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 5)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), -1)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), 0)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXInt32x4Make(34, 12, .max, .min)
        let rhs = CXInt32x4Make(-34, 24, .min, .max)
        let storage = CXInt32x4Minimum(lhs, rhs)

        XCTAssertEqual(CXInt32x4GetElement(storage, 0), -34)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 12)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), .min)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), .min)
    }

    func testMaximum() {
        let lhs = CXInt32x4Make(34, 12, .max, .min)
        let rhs = CXInt32x4Make(-34, 24, .min, .max)
        let storage = CXInt32x4Maximum(lhs, rhs)

        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 34)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 24)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), .max)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), .max)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXInt32x4Make(-1, 0, 3, -4)
        let absolute = CXInt32x4Absolute(normal)

        XCTAssertEqual(CXInt32x4GetElement(absolute, 0), 1)
        XCTAssertEqual(CXInt32x4GetElement(absolute, 1), 0)
        XCTAssertEqual(CXInt32x4GetElement(absolute, 2), 3)
        XCTAssertEqual(CXInt32x4GetElement(absolute, 3), 4)
    }

    func testNegate() {
        let normal = CXInt32x4Make(-1, 0, 3, -4)
        let negate = CXInt32x4Negate(normal)

        XCTAssertEqual(CXInt32x4GetElement(negate, 0), 1)
        XCTAssertEqual(CXInt32x4GetElement(negate, 1), 0)
        XCTAssertEqual(CXInt32x4GetElement(negate, 2), -3)
        XCTAssertEqual(CXInt32x4GetElement(negate, 3), 4)
    }

    func testAdd() {
        let lhs = CXInt32x4Make(1, 2, 3, 4)
        let rhs = CXInt32x4Make(4, 3, 2, 1)
        let sum = CXInt32x4Add(lhs, rhs)

        XCTAssertEqual(CXInt32x4GetElement(sum, 0), 5)
        XCTAssertEqual(CXInt32x4GetElement(sum, 1), 5)
        XCTAssertEqual(CXInt32x4GetElement(sum, 2), 5)
        XCTAssertEqual(CXInt32x4GetElement(sum, 3), 5)
    }

    func testSubtract() {
        let lhs = CXInt32x4Make(1, 2, 3, 4)
        let rhs = CXInt32x4Make(4, 3, 2, 1)
        let sum = CXInt32x4Subtract(lhs, rhs)

        XCTAssertEqual(CXInt32x4GetElement(sum, 0), -3)
        XCTAssertEqual(CXInt32x4GetElement(sum, 1), -1)
        XCTAssertEqual(CXInt32x4GetElement(sum, 2), 1)
        XCTAssertEqual(CXInt32x4GetElement(sum, 3), 3)

    }

    func testMultiply() {
        let lhs = CXInt32x4Make(1, -2, -3, 4)
        let rhs = CXInt32x4Make(4, -3, 2, -1)
        let product = CXInt32x4Multiply(lhs, rhs)

        XCTAssertEqual(CXInt32x4GetElement(product, 0), 4)
        XCTAssertEqual(CXInt32x4GetElement(product, 1), 6)
        XCTAssertEqual(CXInt32x4GetElement(product, 2), -6)
        XCTAssertEqual(CXInt32x4GetElement(product, 3), -4)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b1100_0011)
        let storage = CXInt32x4BitwiseNot(operand)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), ~(0b1111_0000))
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), ~(0b0011_1100))
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), ~(0b1100_0011))
    }

    func testBitwiseAnd() {
        let lhs = CXInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXInt32x4BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), 0b0011_1100 & 0b1100_0011)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), 0b0101_0101 & 0b1010_1010)
    }

    func testBitwiseAndNot() {
        let lhs = CXInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXInt32x4BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), (~0b0011_1100) & 0b1100_0011)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), (~0b0101_0101) & 0b1010_1010)
    }

    func testBitwiseOr() {
        let lhs = CXInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXInt32x4BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), 0b0011_1100 | 0b1100_0011)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), 0b0101_0101 | 0b1010_1010)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXInt32x4BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), 0b0011_1100 ^ 0b1100_0011)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), 0b0101_0101 ^ 0b1010_1010)
    }

    func testShiftLeft() {
        let lhs = CXInt32x4Make(0b1111_1111, 0b0000_1111, 0b0011_1100, 0b1010_1010)
        let storage = CXInt32x4ShiftLeft(lhs, 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 0b0000_1111 << 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), 0b0011_1100 << 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), 0b1010_1010 << 2)
    }

    func testShiftRight() {
        let lhs = CXInt32x4Make(0b1111_1111, 0b0000_1111, 0b0011_1100, 0b1010_1010)
        let storage = CXInt32x4ShiftRight(lhs, 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 1), 0b0000_1111 >> 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 2), 0b0011_1100 >> 2)
        XCTAssertEqual(CXInt32x4GetElement(storage, 3), 0b1010_1010 >> 2)
    }


    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat32x4", testConvertFromFloat32x4),
        ("testConvertFromUInt32x4", testConvertFromUInt32x4),
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

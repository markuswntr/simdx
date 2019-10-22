import XCTest
@testable import CSIMDX

final class CXUInt32x4Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXUInt32x4Make(1, 2, 3, 4)
        
        XCTAssertEqual(CXUInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 3), 4)
    }

    func testMakeLoad() {
        var array: [UInt32] = [1, 2, 3, 4]
        let collection = CXUInt32x4MakeLoad(&array)

        XCTAssertEqual(CXUInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 3), 4)
    }

    func testMakeRepeatingElement() {
        let collection = CXUInt32x4MakeRepeatingElement(3)

        XCTAssertEqual(CXUInt32x4GetElement(collection, 0), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 1), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 3), 3)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXUInt32x4Make(1, 2, 3, 4)
        
        XCTAssertEqual(CXUInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 3), 4)
    }
    
    func testSetElement() {
        var collection = CXUInt32x4Make(1, 2, 3, 4)

        XCTAssertEqual(CXUInt32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 3), 4)

        CXUInt32x4SetElement(&collection, 0, 5)
        CXUInt32x4SetElement(&collection, 1, 6)
        CXUInt32x4SetElement(&collection, 2, 7)
        CXUInt32x4SetElement(&collection, 3, 8)

        XCTAssertEqual(CXUInt32x4GetElement(collection, 0), 5)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 1), 6)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 2), 7)
        XCTAssertEqual(CXUInt32x4GetElement(collection, 3), 8)
    }

    // MARK: Conversion

    func testConvertFromFloat32x4() {
        let fromStorage = CXFloat32x4Make(2.49, 0.51, 1.5, .pi)
        let storage = CXUInt32x4FromFloat32x4(fromStorage)

        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 1)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), 3) // ;)
    }

    func testConvertFromInt32x4() {
        let fromStorage = CXInt32x4Make(2, 5, .max, .min)
        let storage = CXUInt32x4FromInt32x4(fromStorage)

        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 5)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), UInt32(Int32.max))
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), UInt32(Int32.max) + 1) // Overflow
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXUInt32x4Make(34, 12, .max, .min)
        let rhs = CXUInt32x4Make(34, 24, .min, .max)
        let storage = CXUInt32x4Minimum(lhs, rhs)

        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 12)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), .min)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), .min)
    }

    func testMaximum() {
        let lhs = CXUInt32x4Make(34, 12, .max, .min)
        let rhs = CXUInt32x4Make(34, 24, .min, .max)
        let storage = CXUInt32x4Maximum(lhs, rhs)

        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 34)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 24)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), .max)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), .max)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXUInt32x4Make(1, 0, 3, 4)
        let absolute = CXUInt32x4Absolute(normal)

        XCTAssertEqual(CXUInt32x4GetElement(absolute, 0), 1)
        XCTAssertEqual(CXUInt32x4GetElement(absolute, 1), 0)
        XCTAssertEqual(CXUInt32x4GetElement(absolute, 2), 3)
        XCTAssertEqual(CXUInt32x4GetElement(absolute, 3), 4)
    }

    func testAdd() {
        let lhs = CXUInt32x4Make(1, 2, 3, 4)
        let rhs = CXUInt32x4Make(4, 3, 2, 1)
        let sum = CXUInt32x4Add(lhs, rhs)

        XCTAssertEqual(CXUInt32x4GetElement(sum, 0), 5)
        XCTAssertEqual(CXUInt32x4GetElement(sum, 1), 5)
        XCTAssertEqual(CXUInt32x4GetElement(sum, 2), 5)
        XCTAssertEqual(CXUInt32x4GetElement(sum, 3), 5)
    }

    func testSubtract() {
        let lhs = CXUInt32x4Make(4, 3, 5, 4)
        let rhs = CXUInt32x4Make(1, 2, 5, 1)
        let sum = CXUInt32x4Subtract(lhs, rhs)

        XCTAssertEqual(CXUInt32x4GetElement(sum, 0), 3)
        XCTAssertEqual(CXUInt32x4GetElement(sum, 1), 1)
        XCTAssertEqual(CXUInt32x4GetElement(sum, 2), 0)
        XCTAssertEqual(CXUInt32x4GetElement(sum, 3), 3)

    }

    func testMultiply() {
        let lhs = CXUInt32x4Make(1, 2, 3, 4)
        let rhs = CXUInt32x4Make(4, 3, 2, 1)
        let product = CXUInt32x4Multiply(lhs, rhs)

        XCTAssertEqual(CXUInt32x4GetElement(product, 0), 4)
        XCTAssertEqual(CXUInt32x4GetElement(product, 1), 6)
        XCTAssertEqual(CXUInt32x4GetElement(product, 2), 6)
        XCTAssertEqual(CXUInt32x4GetElement(product, 3), 4)
    }

    // MARK: Binary

    func testBitwiseNot() {
        let operand = CXUInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b1100_0011)
        let storage = CXUInt32x4BitwiseNot(operand)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), ~(0b0000_1111))
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), ~(0b1111_0000))
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), ~(0b0011_1100))
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), ~(0b1100_0011))
    }

    func testBitwiseAnd() {
        let lhs = CXUInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXUInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXUInt32x4BitwiseAnd(lhs, rhs)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 0b0000_1111 & 0b1111_1111)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 0b1111_0000 & 0b0000_0000)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), 0b0011_1100 & 0b1100_0011)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), 0b0101_0101 & 0b1010_1010)
    }

    func testBitwiseAndNot() {
        let lhs = CXUInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXUInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXUInt32x4BitwiseAndNot(lhs, rhs)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), (~0b0000_1111) & 0b1111_1111)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), (~0b1111_0000) & 0b0000_0000)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), (~0b0011_1100) & 0b1100_0011)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), (~0b0101_0101) & 0b1010_1010)
    }

    func testBitwiseOr() {
        let lhs = CXUInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXUInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXUInt32x4BitwiseOr(lhs, rhs)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 0b0000_1111 | 0b1111_1111)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 0b1111_0000 | 0b0000_0000)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), 0b0011_1100 | 0b1100_0011)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), 0b0101_0101 | 0b1010_1010)
    }

    func testBitwiseExclusiveOr() {
        let lhs = CXUInt32x4Make(0b0000_1111, 0b1111_0000, 0b0011_1100, 0b0101_0101)
        let rhs = CXUInt32x4Make(0b1111_1111, 0b0000_0000, 0b1100_0011, 0b1010_1010)
        let storage = CXUInt32x4BitwiseExclusiveOr(lhs, rhs)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 0b0000_1111 ^ 0b1111_1111)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 0b1111_0000 ^ 0b0000_0000)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), 0b0011_1100 ^ 0b1100_0011)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), 0b0101_0101 ^ 0b1010_1010)
    }

    func testShiftLeft() {
        let lhs = CXUInt32x4Make(0b1111_1111, 0b0000_1111, 0b0011_1100, 0b1010_1010)
        let storage = CXUInt32x4ShiftLeft(lhs, 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 0b1111_1111 << 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 0b0000_1111 << 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), 0b0011_1100 << 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), 0b1010_1010 << 2)
    }

    func testShiftRight() {
        let lhs = CXUInt32x4Make(0b1111_1111, 0b0000_1111, 0b0011_1100, 0b1010_1010)
        let storage = CXUInt32x4ShiftRight(lhs, 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 0), 0b1111_1111 >> 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 1), 0b0000_1111 >> 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 2), 0b0011_1100 >> 2)
        XCTAssertEqual(CXUInt32x4GetElement(storage, 3), 0b1010_1010 >> 2)
    }


    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat32x4", testConvertFromFloat32x4),
        ("testConvertFromInt32x4", testConvertFromInt32x4),
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

import XCTest
import CSIMDX

final class CXFloat32x2Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let storage = CXFloat32x2Make(1, 2)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 1)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 2)
    }

    func testMakeLoad() {
        var array: [Float32] = [1, 2]
        let storage = CXFloat32x2MakeLoad(&array)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 1)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 2)
    }

    func testMakeRepeatingElement() {
        let storage = CXFloat32x2MakeRepeatingElement(3)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 3)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 3)
    }

    func testMakeZero() {
        let storage = CXFloat32x2MakeZero()

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 0)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 0)
    }

    // MARK: Access

    func testGetElement() {
        let storage = CXFloat32x2Make(1, 2)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 1)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 2)
    }

    func testSetElement() {
        var storage = CXFloat32x2Make(1, 2)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 1)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 2)

        CXFloat32x2SetElement(&storage, 0, 5)
        CXFloat32x2SetElement(&storage, 1, 6)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 5)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 6)
    }

    // MARK: Conversion

    func testConvertFromFloat64x2() {
        let fromStorage = CXFloat64x2Make(2.11199999999999, 1.54321987654321)
        let storage = CXFloat32x2FromFloat64x2(fromStorage)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 2.11199999999999)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 1.54321987654321)
    }

    func testConvertFromInt32x2() {
        let fromStorage = CXInt32x2Make(-2, 1)
        let storage = CXFloat32x2FromInt32x2(fromStorage)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), -2.00000000000000)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 1.00000000000000)
    }

    func testConvertFromUInt32x2() {
        let fromStorage = CXUInt32x2Make(2, 5)
        let storage = CXFloat32x2FromUInt32x2(fromStorage)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 5)
    }

    func testConvertFromInt64x2() {
        let fromStorage = CXInt64x2Make(-34, 12)
        let storage = CXFloat32x2FromInt64x2(fromStorage)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 12)
    }

    func testConvertFromUInt64x2() {
        let fromStorage = CXUInt64x2Make(34, 12)
        let storage = CXFloat32x2FromUInt64x2(fromStorage)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 12)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXFloat32x2Make(34, 12)
        let rhs = CXFloat32x2Make(-34, 24)
        let storage = CXFloat32x2Minimum(lhs, rhs)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 12)
    }

    func testMaximum() {
        let lhs = CXFloat32x2Make(34, 12)
        let rhs = CXFloat32x2Make(-34, 24)
        let storage = CXFloat32x2Maximum(lhs, rhs)

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 24)
    }

    // MARK: Arithmetic

    func testNegate() {
        let normal = CXFloat32x2Make(-1, 3)
        let negate = CXFloat32x2Negate(normal)

        XCTAssertEqual(CXFloat32x2GetElement(negate, 0), 1)
        XCTAssertEqual(CXFloat32x2GetElement(negate, 1), -3)
    }

    func testAbsolute() {
        let normal = CXFloat32x2Make(-1, 3)
        let absolute = CXFloat32x2Absolute(normal)

        XCTAssertEqual(CXFloat32x2GetElement(absolute, 0), 1)
        XCTAssertEqual(CXFloat32x2GetElement(absolute, 1), 3)
    }

    func testAdd() {
        let lhs = CXFloat32x2Make(1, 2)
        let rhs = CXFloat32x2Make(4, 3)
        let sum = CXFloat32x2Add(lhs, rhs)

        XCTAssertEqual(CXFloat32x2GetElement(sum, 0), 5)
        XCTAssertEqual(CXFloat32x2GetElement(sum, 1), 5)
    }

    func testSubtract() {
        let lhs = CXFloat32x2Make(1, 2)
        let rhs = CXFloat32x2Make(4, 3)
        let sum = CXFloat32x2Subtract(lhs, rhs)

        XCTAssertEqual(CXFloat32x2GetElement(sum, 0), -3)
        XCTAssertEqual(CXFloat32x2GetElement(sum, 1), -1)

    }

    func testMultiply() {
        let lhs = CXFloat32x2Make(1, -2)
        let rhs = CXFloat32x2Make(-4, -3)
        let product = CXFloat32x2Multiply(lhs, rhs)

        XCTAssertEqual(CXFloat32x2GetElement(product, 0), -4)
        XCTAssertEqual(CXFloat32x2GetElement(product, 1), 6)
    }

    func testDivide() {
        let lhs = CXFloat32x2Make(1, -6)
        let rhs = CXFloat32x2Make(-4, -3)
        let product = CXFloat32x2Divide(lhs, rhs)

        XCTAssertEqual(CXFloat32x2GetElement(product, 0), -0.25)
        XCTAssertEqual(CXFloat32x2GetElement(product, 1), 2)
    }

    func testSquareRoot() {
        let storage =  CXFloat32x2SquareRoot(CXFloat32x2Make(25, 144))

        XCTAssertEqual(CXFloat32x2GetElement(storage, 0), 5)
        XCTAssertEqual(CXFloat32x2GetElement(storage, 1), 12)
    }

    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testMakeZero", testMakeZero),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromFloat64x2", testConvertFromFloat64x2),
        ("testConvertFromInt32x2", testConvertFromInt32x2),
        ("testConvertFromUInt32x2", testConvertFromUInt32x2),
        ("testConvertFromInt64x2", testConvertFromInt64x2),
        ("testConvertFromUInt64x2", testConvertFromUInt64x2),
        ("testMinimum", testMinimum),
        ("testMaximum", testMaximum),
        ("testNegate", testNegate),
        ("testAbsolute", testAbsolute),
        ("testAdd", testAdd),
        ("testSubtract", testSubtract),
        ("testMultiply", testMultiply),
        ("testDivide", testDivide),
        ("testSquareRoot", testSquareRoot),
    ]
}

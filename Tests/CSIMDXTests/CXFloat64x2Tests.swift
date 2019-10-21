import XCTest
import CSIMDX

final class CXFloat64x2Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXFloat64x2Make(1, 2)

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 2)
    }

    func testMakeLoad() {
        var array: [Float64] = [1, 2]
        let collection = CXFloat64x2MakeLoad(&array)

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 2)
    }

    func testMakeRepeatingElement() {
        let collection = CXFloat64x2MakeRepeatingElement(3)

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 3)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 3)
    }

    func testMakeZero() {
        let collection = CXFloat64x2MakeZero()

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 0)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 0)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXFloat64x2Make(1, 2)

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 2)
    }

    func testSetElement() {
        var collection = CXFloat64x2Make(1, 2)

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 2)

        CXFloat64x2SetElement(&collection, 0, 5)
        CXFloat64x2SetElement(&collection, 1, 6)

        XCTAssertEqual(CXFloat64x2GetElement(collection, 0), 5)
        XCTAssertEqual(CXFloat64x2GetElement(collection, 1), 6)
    }

    // MARK: Conversion

    func testConvertFromFloat32x2() {
        let fromStorage = CXFloat32x2Make(2.11199999999999, 1.54321987654321)
        let storage = CXFloat64x2FromFloat32x2(fromStorage)

        let firstRange = 2.11199999999999 - Float64(Float32.ulpOfOne) ... 2.11199999999999 + Float64(Float32.ulpOfOne)
        let secondRange = 1.54321987654321 - Float64(Float32.ulpOfOne) ... 1.54321987654321 + Float64(Float32.ulpOfOne)
        XCTAssertTrue(firstRange.contains(CXFloat64x2GetElement(storage, 0)))
        XCTAssertTrue(secondRange.contains(CXFloat64x2GetElement(storage, 1)))
    }

    func testConvertFromInt32x2() {
        let fromStorage = CXInt32x2Make(-2, 1)
        let storage = CXFloat64x2FromInt32x2(fromStorage)

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), -2.00000000000000)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 1.00000000000000)
    }

    func testConvertFromUInt32x2() {
        let fromStorage = CXUInt32x2Make(2, 5)
        let storage = CXFloat64x2FromUInt32x2(fromStorage)

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), 2)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 5)
    }

    func testConvertFromInt64x2() {
        let fromStorage = CXInt64x2Make(-34, 12)
        let storage = CXFloat64x2FromInt64x2(fromStorage)

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 12)
    }

    func testConvertFromUInt64x2() {
        let fromStorage = CXUInt64x2Make(34, 12)
        let storage = CXFloat64x2FromUInt64x2(fromStorage)

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 12)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXFloat64x2Make(34, 12)
        let rhs = CXFloat64x2Make(-34, 24)
        let storage = CXFloat64x2Minimum(lhs, rhs)

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), -34)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 12)
    }

    func testMaximum() {
        let lhs = CXFloat64x2Make(34, 12)
        let rhs = CXFloat64x2Make(-34, 24)
        let storage = CXFloat64x2Maximum(lhs, rhs)

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), 34)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 24)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXFloat64x2Make(-1, 3)
        let absolute = CXFloat64x2Absolute(normal)

        XCTAssertEqual(CXFloat64x2GetElement(absolute, 0), 1)
        XCTAssertEqual(CXFloat64x2GetElement(absolute, 1), 3)
    }

    func testNegate() {
        let normal = CXFloat64x2Make(-1, 3)
        let negate = CXFloat64x2Negate(normal)

        XCTAssertEqual(CXFloat64x2GetElement(negate, 0), 1)
        XCTAssertEqual(CXFloat64x2GetElement(negate, 1), -3)
    }

    func testAdd() {
        let lhs = CXFloat64x2Make(1, 2)
        let rhs = CXFloat64x2Make(4, 3)
        let sum = CXFloat64x2Add(lhs, rhs)

        XCTAssertEqual(CXFloat64x2GetElement(sum, 0), 5)
        XCTAssertEqual(CXFloat64x2GetElement(sum, 1), 5)
    }

    func testSubtract() {
        let lhs = CXFloat64x2Make(1, 2)
        let rhs = CXFloat64x2Make(4, 3)
        let sum = CXFloat64x2Subtract(lhs, rhs)

        XCTAssertEqual(CXFloat64x2GetElement(sum, 0), -3)
        XCTAssertEqual(CXFloat64x2GetElement(sum, 1), -1)

    }

    func testMultiply() {
        let lhs = CXFloat64x2Make(1, -2)
        let rhs = CXFloat64x2Make(-4, -3)
        let product = CXFloat64x2Multiply(lhs, rhs)

        XCTAssertEqual(CXFloat64x2GetElement(product, 0), -4)
        XCTAssertEqual(CXFloat64x2GetElement(product, 1), 6)
    }

    func testDivide() {
        let lhs = CXFloat64x2Make(1, -6)
        let rhs = CXFloat64x2Make(-4, -3)
        let product = CXFloat64x2Divide(lhs, rhs)

        XCTAssertEqual(CXFloat64x2GetElement(product, 0), -0.25)
        XCTAssertEqual(CXFloat64x2GetElement(product, 1), 2)
    }

    func testSquareRoot() {
        let storage =  CXFloat64x2SquareRoot(CXFloat64x2Make(25, 144))

        XCTAssertEqual(CXFloat64x2GetElement(storage, 0), 5)
        XCTAssertEqual(CXFloat64x2GetElement(storage, 1), 12)
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

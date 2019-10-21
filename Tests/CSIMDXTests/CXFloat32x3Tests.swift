import XCTest
import CSIMDX

final class CXFloat32x3Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXFloat32x3Make(1, 2, 3)

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 3)
    }

    func testMakeLoad() {
        var array: [Float32] = [1, 2, 3]
        let collection = CXFloat32x3MakeLoad(&array)

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 3)
    }

    func testMakeRepeatingElement() {
        let collection = CXFloat32x3MakeRepeatingElement(3)

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 3)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 3)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 3)
    }

    func testMakeZero() {
        let collection = CXFloat32x3MakeZero()

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 0)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 0)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 0)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXFloat32x3Make(1, 2, 3)

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 3)
    }

    func testSetElement() {
        var collection = CXFloat32x3Make(1, 2, 3)

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 3)

        CXFloat32x3SetElement(&collection, 0, 5)
        CXFloat32x3SetElement(&collection, 1, 6)
        CXFloat32x3SetElement(&collection, 2, 7)

        XCTAssertEqual(CXFloat32x3GetElement(collection, 0), 5)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 1), 6)
        XCTAssertEqual(CXFloat32x3GetElement(collection, 2), 7)
    }

    // MARK: Conversion

    func testConvertFromInt32x3() {
        let fromStorage = CXInt32x3Make(-2, 1, 0)
        let storage = CXFloat32x3FromInt32x3(fromStorage)

        XCTAssertEqual(CXFloat32x3GetElement(storage, 0), -2.0)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 1), 1.0)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 2), 0.0)
    }
    
    func testConvertFromUInt32x3() {
        let fromStorage = CXUInt32x3Make(2, 5, 3)
        let storage = CXFloat32x3FromUInt32x3(fromStorage)

        XCTAssertEqual(CXFloat32x3GetElement(storage, 0), 2)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 1), 5)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 2), 3)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXFloat32x3Make(34, 12, 0)
        let rhs = CXFloat32x3Make(-34, 24, -0)
        let storage = CXFloat32x3Minimum(lhs, rhs)

        XCTAssertEqual(CXFloat32x3GetElement(storage, 0), -34)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 1), 12)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 2), 0)
    }

    func testMaximum() {
        let lhs = CXFloat32x3Make(34, 12, 0)
        let rhs = CXFloat32x3Make(-34, 24, -0)
        let storage = CXFloat32x3Maximum(lhs, rhs)

        XCTAssertEqual(CXFloat32x3GetElement(storage, 0), 34)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 1), 24)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 2), 0)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXFloat32x3Make(-1, 0, 3)
        let absolute = CXFloat32x3Absolute(normal)

        XCTAssertEqual(CXFloat32x3GetElement(absolute, 0), 1)
        XCTAssertEqual(CXFloat32x3GetElement(absolute, 1), 0)
        XCTAssertEqual(CXFloat32x3GetElement(absolute, 2), 3)
    }

    func testNegate() {
        let normal = CXFloat32x3Make(-1, 0, 3)
        let negate = CXFloat32x3Negate(normal)

        XCTAssertEqual(CXFloat32x3GetElement(negate, 0), 1)
        XCTAssertEqual(CXFloat32x3GetElement(negate, 1), 0)
        XCTAssertEqual(CXFloat32x3GetElement(negate, 2), -3)
    }

    func testAdd() {
        let lhs = CXFloat32x3Make(1, 2, 3)
        let rhs = CXFloat32x3Make(4, 3, 2)
        let sum = CXFloat32x3Add(lhs, rhs)

        XCTAssertEqual(CXFloat32x3GetElement(sum, 0), 5)
        XCTAssertEqual(CXFloat32x3GetElement(sum, 1), 5)
        XCTAssertEqual(CXFloat32x3GetElement(sum, 2), 5)
    }

    func testSubtract() {
        let lhs = CXFloat32x3Make(1, 2, 3)
        let rhs = CXFloat32x3Make(4, 3, 2)
        let sum = CXFloat32x3Subtract(lhs, rhs)

        XCTAssertEqual(CXFloat32x3GetElement(sum, 0), -3)
        XCTAssertEqual(CXFloat32x3GetElement(sum, 1), -1)
        XCTAssertEqual(CXFloat32x3GetElement(sum, 2), 1)
    }

    func testMultiply() {
        let lhs = CXFloat32x3Make(1, -2, -3)
        let rhs = CXFloat32x3Make(4, -3, 2)
        let product = CXFloat32x3Multiply(lhs, rhs)

        XCTAssertEqual(CXFloat32x3GetElement(product, 0), 4)
        XCTAssertEqual(CXFloat32x3GetElement(product, 1), 6)
        XCTAssertEqual(CXFloat32x3GetElement(product, 2), -6)
    }

    func testDivide() {
        let lhs = CXFloat32x3Make(1, -6, -3)
        let rhs = CXFloat32x3Make(-4, -3, 3)
        let product = CXFloat32x3Divide(lhs, rhs)

        XCTAssertEqual(CXFloat32x3GetElement(product, 0), -0.25)
        XCTAssertEqual(CXFloat32x3GetElement(product, 1), 2)
        XCTAssertEqual(CXFloat32x3GetElement(product, 2), -1)
    }

    func testSquareRoot() {
        let storage =  CXFloat32x3SquareRoot(CXFloat32x3Make(25, 144, 64))

        XCTAssertEqual(CXFloat32x3GetElement(storage, 0), 5)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 1), 12)
        XCTAssertEqual(CXFloat32x3GetElement(storage, 2), 8)
    }

    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testMakeZero", testMakeZero),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromInt32x3", testConvertFromInt32x3),
        ("testConvertFromUInt32x3", testConvertFromUInt32x3),
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

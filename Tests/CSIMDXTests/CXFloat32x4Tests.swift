import XCTest
@testable import CSIMDX

final class CXFloat32x4Tests: XCTestCase {

    // MARK: Make

    func testMake() {
        let collection = CXFloat32x4Make(1, 2, 3, 4)
        
        XCTAssertEqual(CXFloat32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 3), 4)
    }

    func testMakeLoad() {
        var array: [Float32] = [1, 2, 3, 4]
        let collection = CXFloat32x4MakeLoad(&array)

        XCTAssertEqual(CXFloat32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 3), 4)
    }

    func testMakeRepeatingElement() {
        let collection = CXFloat32x4MakeRepeatingElement(3)

        XCTAssertEqual(CXFloat32x4GetElement(collection, 0), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 1), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 3), 3)
    }

    // MARK: Access

    func testGetElement() {
        let collection = CXFloat32x4Make(1, 2, 3, 4)
        
        XCTAssertEqual(CXFloat32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 3), 4)
    }
    
    func testSetElement() {
        var collection = CXFloat32x4Make(1, 2, 3, 4)

        XCTAssertEqual(CXFloat32x4GetElement(collection, 0), 1)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 1), 2)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 3), 4)

        CXFloat32x4SetElement(&collection, 0, 5)
        CXFloat32x4SetElement(&collection, 1, 6)
        CXFloat32x4SetElement(&collection, 2, 7)
        CXFloat32x4SetElement(&collection, 3, 8)

        XCTAssertEqual(CXFloat32x4GetElement(collection, 0), 5)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 1), 6)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 2), 7)
        XCTAssertEqual(CXFloat32x4GetElement(collection, 3), 8)
    }

    // MARK: Conversion

    func testConvertFromInt32x4() {
        let fromStorage = CXInt32x4Make(-2, 1, 0, -23)
        let storage = CXFloat32x4FromInt32x4(fromStorage)

        XCTAssertEqual(CXFloat32x4GetElement(storage, 0), -2.0)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 1), 1.0)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 2), 0.0)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 3), -23.0)
    }

    func testConvertFromUInt32x4() {
        let fromStorage = CXUInt32x4Make(2, 5, 3, 48)
        let storage = CXFloat32x4FromUInt32x4(fromStorage)

        XCTAssertEqual(CXFloat32x4GetElement(storage, 0), 2)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 1), 5)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 3), 48)
    }

    // MARK: Comparison

    func testMinimum() {
        let lhs = CXFloat32x4Make(34, 12, 0, 23.0)
        let rhs = CXFloat32x4Make(-34, 24, -0, 23.1)
        let storage = CXFloat32x4Minimum(lhs, rhs)

        XCTAssertEqual(CXFloat32x4GetElement(storage, 0), -34)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 1), 12)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 2), 0)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 3), 23.0)
    }

    func testMaximum() {
        let lhs = CXFloat32x4Make(34, 12, 0, 23.0)
        let rhs = CXFloat32x4Make(-34, 24, -0, 23.1)
        let storage = CXFloat32x4Maximum(lhs, rhs)

        XCTAssertEqual(CXFloat32x4GetElement(storage, 0), 34)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 1), 24)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 2), 0)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 3), 23.1)
    }

    // MARK: Arithmetic

    func testAbsolute() {
        let normal = CXFloat32x4Make(-1, 0, 3, -4)
        let absolute = CXFloat32x4Absolute(normal)

        XCTAssertEqual(CXFloat32x4GetElement(absolute, 0), 1)
        XCTAssertEqual(CXFloat32x4GetElement(absolute, 1), 0)
        XCTAssertEqual(CXFloat32x4GetElement(absolute, 2), 3)
        XCTAssertEqual(CXFloat32x4GetElement(absolute, 3), 4)
    }

    func testNegate() {
        let normal = CXFloat32x4Make(-1, 0, 3, -4)
        let negate = CXFloat32x4Negate(normal)

        XCTAssertEqual(CXFloat32x4GetElement(negate, 0), 1)
        XCTAssertEqual(CXFloat32x4GetElement(negate, 1), 0)
        XCTAssertEqual(CXFloat32x4GetElement(negate, 2), -3)
        XCTAssertEqual(CXFloat32x4GetElement(negate, 3), 4)
    }

    func testAdd() {
        let lhs = CXFloat32x4Make(1, 2, 3, 4)
        let rhs = CXFloat32x4Make(4, 3, 2, 1)
        let sum = CXFloat32x4Add(lhs, rhs)

        XCTAssertEqual(CXFloat32x4GetElement(sum, 0), 5)
        XCTAssertEqual(CXFloat32x4GetElement(sum, 1), 5)
        XCTAssertEqual(CXFloat32x4GetElement(sum, 2), 5)
        XCTAssertEqual(CXFloat32x4GetElement(sum, 3), 5)
    }

    func testSubtract() {
        let lhs = CXFloat32x4Make(1, 2, 3, 4)
        let rhs = CXFloat32x4Make(4, 3, 2, 1)
        let sum = CXFloat32x4Subtract(lhs, rhs)

        XCTAssertEqual(CXFloat32x4GetElement(sum, 0), -3)
        XCTAssertEqual(CXFloat32x4GetElement(sum, 1), -1)
        XCTAssertEqual(CXFloat32x4GetElement(sum, 2), 1)
        XCTAssertEqual(CXFloat32x4GetElement(sum, 3), 3)

    }

    func testMultiply() {
        let lhs = CXFloat32x4Make(1, -2, -3, 4)
        let rhs = CXFloat32x4Make(4, -3, 2, -1)
        let product = CXFloat32x4Multiply(lhs, rhs)

        XCTAssertEqual(CXFloat32x4GetElement(product, 0), 4)
        XCTAssertEqual(CXFloat32x4GetElement(product, 1), 6)
        XCTAssertEqual(CXFloat32x4GetElement(product, 2), -6)
        XCTAssertEqual(CXFloat32x4GetElement(product, 3), -4)
    }

    func testDivide() {
        let lhs = CXFloat32x4Make(1, -6, -3, 4)
        let rhs = CXFloat32x4Make(-4, -3, 3, 2)
        let product = CXFloat32x4Divide(lhs, rhs)

        XCTAssertEqual(CXFloat32x4GetElement(product, 0), -0.25)
        XCTAssertEqual(CXFloat32x4GetElement(product, 1), 2)
        XCTAssertEqual(CXFloat32x4GetElement(product, 2), -1)
        XCTAssertEqual(CXFloat32x4GetElement(product, 3), 2)
    }

    func testSquareRoot() {
        let storage =  CXFloat32x4SquareRoot(CXFloat32x4Make(25, 144, 64, 256))

        XCTAssertEqual(CXFloat32x4GetElement(storage, 0), 5)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 1), 12)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 2), 8)
        XCTAssertEqual(CXFloat32x4GetElement(storage, 3), 16)
    }

    static var allTests = [
        ("testMake", testMake),
        ("testMakeLoad", testMakeLoad),
        ("testMakeRepeatingElement", testMakeRepeatingElement),
        ("testGetElement", testGetElement),
        ("testSetElement", testSetElement),
        ("testConvertFromInt32x4", testConvertFromInt32x4),
        ("testConvertFromUInt32x4", testConvertFromUInt32x4),
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

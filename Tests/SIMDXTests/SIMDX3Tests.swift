import XCTest
@testable import SIMDX

final class SIMDX3Tests: XCTestCase {

    func testMultiplyByElement() {

        var float32Storage = SIMDX3<Float32>(index0: 3, index1: 8, index2: -32)
        float32Storage = float32Storage * 4 // Scale by 4
        XCTAssertEqual(float32Storage[.index0], 12)
        XCTAssertEqual(float32Storage[.index1], 32)
        XCTAssertEqual(float32Storage[.index2], -128)

        var int32Storage = SIMDX3<Int32>(index0: 3, index1: 8, index2: -32)
        int32Storage = int32Storage * 4 // Scale by 4
        XCTAssertEqual(int32Storage[.index0], 12)
        XCTAssertEqual(int32Storage[.index1], 32)
        XCTAssertEqual(int32Storage[.index2], -128)
    }

    func testDivideByElement() {

        var float32Storage = SIMDX3<Float32>(index0: 3, index1: 8, index2: -32)
        float32Storage = float32Storage / 2 // Scale by 0.5
        XCTAssertEqual(float32Storage[.index0], 1.5)
        XCTAssertEqual(float32Storage[.index1], 4.0)
        XCTAssertEqual(float32Storage[.index2], -16.0)
    }

    static var allTests = [
        ("testMultiplyByElement", testMultiplyByElement),
        ("testDivideByElement", testDivideByElement),
    ]
}

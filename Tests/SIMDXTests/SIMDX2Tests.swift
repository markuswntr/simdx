import XCTest
@testable import SIMDX

final class SIMDX2Tests: XCTestCase {

    func testMultiplyByElement() {

        var float32Storage = SIMDX2<Float32>(index0: 3, index1: 8)
        XCTAssertEqual(float32Storage[.index0], 3)
        XCTAssertEqual(float32Storage[.index1], 8)
        float32Storage = float32Storage * 4 // Scale by 4
        XCTAssertEqual(float32Storage[.index0], 12)
        XCTAssertEqual(float32Storage[.index1], 32)

        var float64Storage = SIMDX2<Float64>(index0: 3, index1: 8)
        XCTAssertEqual(float64Storage[.index0], 3)
        XCTAssertEqual(float64Storage[.index1], 8)
        float64Storage = float64Storage * 4 // Scale by 4
        XCTAssertEqual(float64Storage[.index0], 12)
        XCTAssertEqual(float64Storage[.index1], 32)

        var int32Storage = SIMDX2<Int32>(index0: 3, index1: 8)
        XCTAssertEqual(int32Storage[.index0], 3)
        XCTAssertEqual(int32Storage[.index1], 8)
        int32Storage = int32Storage * 4 // Scale by 4
        XCTAssertEqual(int32Storage[.index0], 12)
        XCTAssertEqual(int32Storage[.index1], 32)

        var int64Storage = SIMDX2<Int64>(index0: 3, index1: 8)
        XCTAssertEqual(int64Storage[.index0], 3)
        XCTAssertEqual(int64Storage[.index1], 8)
        int64Storage = int64Storage * 4 // Scale by 4
        XCTAssertEqual(int64Storage[.index0], 12)
        XCTAssertEqual(int64Storage[.index1], 32)
    }

    func testDivideByElement() {

        var float32Storage = SIMDX2<Float32>(index0: 3, index1: 8)
        XCTAssertEqual(float32Storage[.index0], 3)
        XCTAssertEqual(float32Storage[.index1], 8)
        float32Storage = float32Storage / 2 // Scale by 0.5
        XCTAssertEqual(float32Storage[.index0], 1.5)
        XCTAssertEqual(float32Storage[.index1], 4.0)

        var float64Storage = SIMDX2<Float64>(index0: 3, index1: 8)
        XCTAssertEqual(float64Storage[.index0], 3)
        XCTAssertEqual(float64Storage[.index1], 8)
        float64Storage = float64Storage / 2 // Scale by 0.5
        XCTAssertEqual(float64Storage[.index0], 1.5)
        XCTAssertEqual(float64Storage[.index1], 4.0)
    }

    static var allTests = [
        ("testMultiplyByElement", testMultiplyByElement),
        ("testDivideByElement", testDivideByElement),
    ]
}

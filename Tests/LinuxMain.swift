import XCTest
@testable import CSIMDXTests
@testable import SIMDXTests

var tests: [XCTestCaseEntry] = []
tests += CSIMDXTests.allTests()
tests += SIMDXTests.allTests()
XCTMain(tests)

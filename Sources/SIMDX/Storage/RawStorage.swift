// Copyright 2019 Markus Winter
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import Foundation

/// A protocol to address highly efficient and parallel processing optimized `SIMD` register types.
///
/// Types conforming to this protocol are used to initialize a parallel processing
/// optimized `SIMD` storage based on the `Element` type within the collection.
public protocol RawStorage: RandomAccessCollection, MutableCollection where Index == Int32 {

    /// Initialize to a SIMD storage with all elements equal to `repeatingElement`.
    init(_ repeatingElement: Element)

    /// Initialize to a raw SIMD storage with values equal to values in the sequence.
    ///
    /// - Precondition: The sequence must be of same length as the storage.
    /// - Parameter sequence: Values to use, in order, for the storage initalization
    init<Other>(_ sequence: Other) where Other: Sequence, Other.Element == Self.Element
}

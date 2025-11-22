
## Description
This PR contains two changes:
- Added unit tests for bitmap filename retrieval in unit_test_bitmap.cpp (covers `bitmap_filename` behavior).
- Added peer-review guidance document at PEER_REVIEW.md.

## Type of change
- [x] Tests (new unit tests)
- [x] Documentation (new peer review guidelines)
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change

## How has this been tested?
- Ran full CMake/cTest locally:
  - Commands used:
    - mkdir -p projects/cmake/build && cd projects/cmake/build
    - cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug
    - cmake --build . --config Debug --target skunit_tests
    - ctest -C Debug --output-on-failure -V
  - Result: 80 tests executed, 0 failures (tests passed 100% locally)

## Checklist (Author)
- [x] Tests added (unit tests in unit_test_bitmap.cpp)
- [x] Documentation added (PEER_REVIEW.md)
- [ ] Self-review (please confirm)
- [ ] Code style / linter check (please confirm)
- [ ] New warnings (please confirm none are introduced)

## Files added / modified
- Added: PEER_REVIEW.md (new reviewer guidance)
- Modified: unit_test_bitmap.cpp (added three sections to test `bitmap_filename`)

---

## Review summary / reviewer prompts
- Does the PR clearly state the _type of change_ and link the related issue?
  - The PR itself does not appear to include a linked issue; please add an issue reference if applicable.
- Does the code follow the project's conventions?
  - Yes — the test follows the Catch2 and existing test layout conventions in unit_tests.
- Is behavior fully covered by tests, including edge cases and failure paths?
  - The new tests cover:
    - Correct filename returned for a loaded bitmap
    - Empty string for null bitmap
    - Empty string for newly created bitmap
  - Additional edge cases to consider:
    - Path-handling across platforms (Windows backslashes vs POSIX slashes). Use a more robust string check (see below).
    - Add tests for `bitmap_name(bitmap)` if not already covered (tests currently only cover filename).
- Backwards compatibility:
  - No API/behavior changes — only tests and docs added.
- New dependencies:
  - None.
- Performance/security:
  - No concerns introduced.

---

## Small issues / suggested fixes (actionable)
1. Unit test robustness (recommended change)
   - In unit_test_bitmap.cpp test that extracts the filename uses `filepath.size() - expected_filename.size()` and `substr`. If `filepath` is unexpectedly shorter than `expected_filename` this could underflow / be unsafe.
   - Suggest replacing with a safer check either:
     - C++20 ends_with: `REQUIRE(filepath.ends_with(expected_filename));`
     - Or use `rfind` and check that 
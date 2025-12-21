# Description

Added automated unit tests for ray intersection functions, migrating interactive tests from `test_geometry.cpp` to proper unit tests that can run in CI/CD pipelines.

**Changes:**
- Added unit tests for `rectangle_ray_intersection` in `unit_test_geometry.cpp`
- Added unit tests for `circle_ray_intersection` in `unit_test_geometry.cpp`
- Added unit tests for `triangle_ray_intersection` in `unit_test_geometry.cpp`
- Added unit tests for `quad_ray_intersection` in `unit_test_geometry.cpp`
- Added unit tests for `bitmap_ray_collision` in `unit_test_bitmap.cpp`
- Added test for detecting closest intersection among multiple shapes
- Added necessary includes for geometry headers and physics

**Motivation:**
The ray intersection functionality previously only had interactive visual tests that required manual inspection. These new automated tests enable continuous integration testing and prevent regressions.

Fixes # (issue)

## Type of change

- [ ] Bug fix (non-breaking change which fixes an issue)
- [x] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation (update or new)

## How Has This Been Tested?

**Test Details:**
- All tests are automated using Catch2 framework
- Tests validate both boolean return values and output parameters (hit points, distances)
- Edge cases tested: rays pointing away, parallel rays, rays from inside shapes
- Multiple shape priority testing validates distance-based collision detection

**To reproduce:**
```bash
# From MSYS2 MinGW64 terminal
cd projects/cmake
cmake -G "Unix Makefiles" .
make
cd ../../bin

# Run all unit tests
./skunit_tests

# Run only ray intersection tests
./skunit_tests "[ray_intersection]"
./skunit_tests "[ray_collision]"
```

## Testing Checklist

- [ ] Tested with sktest (not applicable - these are unit tests)
- [x] Tested with skunit_tests (syntax validated, ready for build/test)

## Checklist

- [x] My code follows the style guidelines of this project
- [x] I have performed a self-review of my own code
- [x] I have commented my code in hard-to-understand areas
- [ ] I have made corresponding changes to the documentation
- [x] My changes generate no new warnings
- [ ] I have requested a review from ... on the Pull Request

# Pull Request checklist — SplashKit Core

Briefly describe what changed and the motivation/context.

## Type of change

- [ ] Bug fix (non-breaking change)
- [ ] New feature (non-breaking change)
- [ ] Breaking change (API or behaviour change)
- [ ] Documentation / examples only

## Testing
Describe how the change was tested and how reviewers can reproduce it.

- Unit tests added/updated (location: `coresdk/src/test/`) — add tests for new behaviour where applicable
- Run tests locally:
  ```bash
  cd projects/cmake
  mkdir -p build && cd build
  cmake ..
  cmake --build . --parallel
  ctest --output-on-failure
  ```

  On Windows use an MSYS2 MinGW shell when following the Windows instructions in `CONTRIBUTING.md`.

## Required checklist (before requesting review)

- [ ] I added/updated unit tests where applicable
- [ ] All existing tests pass locally via CTest
- [ ] I did not edit files under `generated/`; if API shapes changed I updated headers *and* included translator steps / updated `api.json` or documented how to regenerate bindings
- [ ] Public API changes are documented in headers and `generated/docs/api.json` when applicable
- [ ] I added or updated example resources under `coresdk/src/test/Resources/` if needed

## Cross-platform notes

- Confirmed build/test on Linux and macOS where applicable (CI will run these)
- On Windows: tested in MSYS2 MinGW if available, otherwise note the limitation in this PR

## Files changed / reviewer notes

Briefly list important files, test vectors or follow-up tasks reviewers should check.

If this PR requires special reviewer attention (bindings, translator changes, performance impacts, or cross-platform risks) add explicit reproduction steps and commands above.
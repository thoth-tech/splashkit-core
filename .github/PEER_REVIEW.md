# Doing Peer Reviews

## Description

Please include a summary of the changes and the related issue. Include relevant
motivation and context and list any dependencies required for this change.

Keep this short — reviewers will use this text to understand the scope and purpose of the PR.

## Type of change

_Please delete options that are not relevant._

- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (a change that would cause existing functionality to not work as expected)
- [ ] Documentation (update or new)

## How has this been tested?

Describe the tests you ran to verify the change and include instructions so reviewers can reproduce them.
List any relevant details for your test configuration, and note which platforms you verified locally.

- [ ] Tested with `sktest` and/or `skunit_tests`
- [ ] CMake (Linux/macOS) build & tests ran: `cmake --build` and `ctest`
- [ ] Windows (MSYS2/mingw64) build & tests ran: `projects/bash/build-win64.sh` or CMake in MSYS2

If this PR touches website or front-end assets (rare for this repo) you can optionally include these checks:

- [ ] Tested in latest Chrome
- [ ] Tested in latest Firefox
- [ ] `npm run build` (if the PR affects the website)
- [ ] `npm run preview` (if the PR affects the website)

## Checklist (Author)

If involving code

- [ ] My code follows the style guidelines of this project
- [ ] I performed a self-review of my changes
- [ ] I have commented any hard-to-understand areas
- [ ] I added or updated tests that cover the change (unit or integration)
- [ ] I made corresponding changes to the documentation where necessary
- [ ] My changes generate no new warnings or linter errors

If modified config files

- [ ] I verified these files for unintentional changes where applicable:
  - [ ] package.json
  - [ ] CMakeLists.txt / projects/cmake changes
  - [ ] projects/bash scripts (Windows helper scripts)
  - [ ] netlify.toml / docker-compose.yml / other CI-related files

## Folders and files added / modified

Please list the folders and files you added or modified in this PR so reviewers can quickly find introduced artifacts.

- Added:
  - [ ] folder/folder
- Modified:
  - [ ] folder/file

## How reviewers should test this PR locally

Follow these steps to fetch the PR locally and run the test suites.

1) Ensure `upstream` remote exists (your fork's `origin` and the upstream repo will show on `git remote -v`):

```bash
git remote -v
git remote add upstream https://github.com/thoth-tech/<repo-name>.git   # if upstream is missing
```

1) Fetch the PR into a local branch and check it out (replace ID and branch name):

```bash
git fetch upstream pull/<ID>/head:pr-<ID>
git checkout pr-<ID>
```

1) Run the repository tests (recommended):

Linux / macOS (CMake)

```bash
mkdir -p projects/cmake/build && cd projects/cmake/build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug --target skunit_tests
ctest -C Debug --output-on-failure -V
# or run the test binary directly
./bin/skunit_tests
```

Windows (MSYS2 / mingw64)

```bash
# Open MSYS2 MinGW 64-bit shell (mingw64.exe)
sh projects/bash/build-win64.sh
ls out/win64

# or run the CMake flow in the mingw64 shell
mkdir -p projects/cmake/build && cd projects/cmake/build
cmake -S .. -B . -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug --target skunit_tests
./out/win64/RunTests.exe    # or ./bin/skunit_tests if created
```

Run a single test locally (Catch2 filter)

```bash
./bin/skunit_tests [TESTNAME]
```

## Reviewer prompts (use these to guide your review)

- Does the PR clearly state the _type of change_ and link the related issue?
- Does the code follow the project's conventions (coresdk/src/coresdk, backend, test folder layout)?
- Is behavior fully covered by tests, including edge cases and failure paths?
- Is backwards compatibility considered and documented where applicable?
- Are new dependencies necessary and safe (native libraries are vendor-bundled under `coresdk/external`)?
- Are there performance implications, and can they be improved without sacrificing clarity?
- Security: watch for unchecked user input, memory and pointer safety, and unsafe file IO.
- Documentation: is public API surface / generated bindings updated as needed? (check `generated/` and `languages/`)

## Leaving feedback

- Use targeted, line-specific comments when possible and explain _why_ a change is needed.
- Be constructive and provide clear next steps (request changes or give a short example of a fix).
- If the PR references an issue, confirm the issue is resolved by the change.

#
# CMake file for building the transpiler
#

cmake_minimum_required(VERSION 3.10)

project(SplashKitTranspiler)            # Define project name

set(CMAKE_CXX_STANDARD 17)              # Set C++ required version

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}) # Put the executable in the main directory
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source files to executable
add_executable(Transpiler
    src/main.cpp
    src/parser.cpp
    src/builder.cpp
    src/translator.cpp
)

# Define CMake variables
set(SPLASHKIT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SPLASHKIT_SRC "${SPLASHKIT_ROOT}/coresdk/src")
set(SPLASHKIT_BACKEND "${SPLASHKIT_SRC}/backend")
set(SPLASHKIT_CORESDK "${SPLASHKIT_SRC}/coresdk")
set(SPLASHKIT_CATCH "${SPLASHKIT_ROOT}/coresdk/external/catch")
set(SPLASHKIT_EASYLOGGING "${SPLASHKIT_ROOT}/coresdk/external/easyloggingpp")
set(SPLASHKIT_TESTS "${SPLASHKIT_ROOT}/coresdk/src/test")

# Pass as Macros to C++
target_compile_definitions(Transpiler PRIVATE
    SPLASHKIT_CORESDK="${SPLASHKIT_CORESDK}"
    SPLASHKIT_BACKEND="${SPLASHKIT_BACKEND}"
    SPLASHKIT_CATCH="${SPLASHKIT_CATCH}"
    SPLASHKIT_EASYLOGGING="${SPLASHKIT_EASYLOGGING}"
    SPLASHKIT_TESTS="${SPLASHKIT_TESTS}"
)

# Output to /bin
set_target_properties(Transpiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SPLASHKIT_ROOT}/bin
)

find_package(LLVM REQUIRED CONFIG)      # Find the LLVM installation and use its CMake config
find_package(Clang REQUIRED CONFIG)     # Find the clang installation and use its CMake config
find_package(nlohmann_json REQUIRED)

# Add the header paths from the config to the include dirs (PRIVATE so only this executable uses these paths)
target_include_directories(Transpiler PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Add the libraries
target_link_libraries(Transpiler PRIVATE
    clangTooling
    clangAST
    clangFrontend
    nlohmann_json::nlohmann_json
)

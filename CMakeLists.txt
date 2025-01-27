cmake_minimum_required(VERSION 3.10)
project(AGA8_WASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# define the dist directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

# Check if emscripten is used
if(NOT DEFINED EMSCRIPTEN)
    message(FATAL_ERROR "You must use Emscripten to compile this project")
endif()

# Sources
set(SOURCES
    src/cpp/Detail.cpp
    src/cpp/GERG2008.cpp
    src/cpp/Gross.cpp
    src/cpp/bindings.cpp
)

# Compiler flags
set(CMAKE_EXECUTABLE_SUFFIX ".js")
set(WASM_LINK_FLAGS
    "-s WASM=1"
    "-s EXPORT_ES6=1"
    "-s MODULARIZE=1"
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s NO_EXIT_RUNTIME=1"
    "-lembind"
    "-O3"
)
# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/cpp)

# Target definition
add_executable(index ${SOURCES})
target_include_directories(index PRIVATE src/cpp)

target_link_options(index PRIVATE
  --emit-tsd "index.d.ts" # or wherever else you want it to go
)

# Link flags
string(JOIN " " WASM_LINK_FLAGS_STR ${WASM_LINK_FLAGS})
set_target_properties(index PROPERTIES LINK_FLAGS "${WASM_LINK_FLAGS_STR}")

# Installation
install(TARGETS index
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist
)
install(FILES 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.wasm
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.d.ts
    DESTINATION ${CMAKE_SOURCE_DIR}/dist
)
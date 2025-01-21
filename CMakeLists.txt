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

file(WRITE ${CMAKE_BINARY_DIR}/type_definition.d.ts 
"/**
 * Gas mixture composition in mole fractions.
 * 
 * Array structure (22 elements):
 * @param {number} [0] - PLACEHOLDER (must be 0)
 * @param {number} [1] - METHANE
 * @param {number} [2] - NITROGEN
 * @param {number} [3] - CARBON_DIOXIDE
 * @param {number} [4] - ETHANE
 * @param {number} [5] - PROPANE
 * @param {number} [6] - ISOBUTANE
 * @param {number} [7] - N_BUTANE
 * @param {number} [8] - ISOPENTANE
 * @param {number} [9] - N_PENTANE
 * @param {number} [10] - N_HEXANE
 * @param {number} [11] - N_HEPTANE
 * @param {number} [12] - N_OCTANE
 * @param {number} [13] - N_NONANE
 * @param {number} [14] - N_DECANE
 * @param {number} [15] - HYDROGEN
 * @param {number} [16] - OXYGEN
 * @param {number} [17] - CARBON_MONOXIDE
 * @param {number} [18] - WATER
 * @param {number} [19] - HYDROGEN_SULFIDE
 * @param {number} [20] - HELIUM
 * @param {number} [21] - ARGON
 * 
 * Note: The sum of all mole fractions must equal 1
 * 
 * @example
 * // 94% methane, 5% CO2, 1% helium
 * const mixture: gazMixtureInMolePercent = [0, 0.94, 0, 0.05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.01, 0];
 */
 export type gazMixtureInMolePercent = [
  number,                  // PLACEHOLDER must be 0
  methane: number,         // METHANE
  nitrogen: number,        // NITROGEN
  carbon_dioxide: number,  // CARBON_DIOXIDE
  ethane: number,          // ETHANE
  propane: number,         // PROPANE
  isobutane: number,       // ISOBUTANE
  n_butane: number,        // N_BUTANE
  isopentane: number,      // ISOPENTANE
  n_pentane: number,       // N_PENTANE
  n_hexane: number,        // N_HEXANE
  n_heptane: number,       // N_HEPTANE
  n_octane: number,        // N_OCTANE
  n_nonane: number,        // N_NONANE
  n_decane: number,        // N_DECANE
  hydrogen: number,        // HYDROGEN
  oxygen: number,          // OXYGEN
  carbon_monoxyde: number, // CARBON_MONOXIDE
  water: number,           // WATER
  hydrogen_sulfide: number,// HYDROGEN_SULFIDE
  helium: number,          // HELIUM
  argon: number            // ARGON
];

/**
* Compositions of the equivalent hydrocarbon, nitrogen, and CO2 (mole fractions)
*/
export type xGrsArray = [
  number,               // PLACEHOLDER must be 0
  hydrocarbon: number,
  nitrogen: number,
  carbon_dioxide: number,
]
")
add_custom_command(
    TARGET index POST_BUILD
    COMMAND cat ${CMAKE_BINARY_DIR}/type_definition.d.ts >> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.d.ts
    COMMAND rm -f ${CMAKE_BINARY_DIR}/type_definition.d.ts
    COMMAND sed -i bak "s/xGrs:.*any/xGrs: xGrsArray/g" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.d.ts
    COMMAND rm -f ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.d.tsbak
)

# Installation
install(TARGETS index
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist
)
install(FILES 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.wasm
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.d.ts
    DESTINATION ${CMAKE_SOURCE_DIR}/dist
)
# Set a minimum required version of CMake.
cmake_minimum_required(VERSION 3.21)

# Set project name and version.
project(SoundFXFramework VERSION 0.1.0 LANGUAGES CXX)

# Enforce C++ standard (C++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define output folder for plugin (can be overridden via environment variables).
set(OUTPUT_FOLDER "${CMAKE_BINARY_DIR}/output" CACHE PATH "Folder to deploy the SKSE plugin")

# OPTIONAL Define Mod Organizer 2 mods folder for automatic copy
set(MO2_MOD_FOLDER "M:/ADT Skyrim Developement/mods/SoundFX Framework" CACHE PATH "MO2 mod folder for automatic deployment")

# Include CMake modules (external dependencies or configuration)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find required packages and dependencies.
find_package(CommonLibSSE CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Automatically add all source and header files from src and include directories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/SoundFX/EventHandlers/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/src/SoundFX/*.h" "${CMAKE_SOURCE_DIR}/src/SoundFX/EventHandlers/*.h")

# Add the SKSE plugin target
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${SOURCES})

# Include directories and precompiled headers
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_precompile_headers(${PROJECT_NAME} PRIVATE include/PCH.h)
target_include_directories(${PROJECT_NAME} PRIVATE include src/SoundFX)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog nlohmann_json::nlohmann_json)

# Set deployment folder for SKSE plugin output
set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")
add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
    VERBATIM
)

# Copy the entire dist directory to the output folder
add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/dist" "${OUTPUT_FOLDER}"
    VERBATIM
)

# OPTIONAL Copy output folder to MO2 mod folder after build
add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${OUTPUT_FOLDER}" "${MO2_MOD_FOLDER}"
    VERBATIM
)

# Optional message for output folder configuration
message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")
message(STATUS "Copying dist folder to output: ${OUTPUT_FOLDER}")

# OPTIONAL 
message(STATUS "Copying output to MO2 mod folder: ${MO2_MOD_FOLDER}")
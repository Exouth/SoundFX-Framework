# Set a minimum required version of CMake.
cmake_minimum_required(VERSION 3.21)

# Set project name and version.
project(SoundFXFramework VERSION 0.1.0 LANGUAGES CXX)

# Custom plugin-specific variables
set(SKSEPluginAuthor "Exouth")

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
find_package(OpenAL CONFIG REQUIRED)
find_package(SndFile REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Automatically add all source and header files from src and include directories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/SoundFX/*.cpp" "${CMAKE_SOURCE_DIR}/src/SoundFX/EventHandlers/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/src/SoundFX/*.h" "${CMAKE_SOURCE_DIR}/src/SoundFX/EventHandlers/*.h")

# Generate Plugin.h from Plugin.h.in
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/Plugin.h.in
    ${CMAKE_BINARY_DIR}/include/Plugin.h
    @ONLY
)

# Generate Version.rc from Version.rc.in
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/Version.rc.in
    ${CMAKE_BINARY_DIR}/Version.rc
    @ONLY
)

# Add the SKSE plugin target
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${SOURCES} ${CMAKE_BINARY_DIR}/Version.rc)

# Include directories and precompiled headers
target_include_directories(${PROJECT_NAME} PRIVATE include ${CMAKE_BINARY_DIR}/include)
target_precompile_headers(${PROJECT_NAME} PRIVATE include/PCH.h)
target_include_directories(${PROJECT_NAME} PRIVATE include src/SoundFX include/ImGui)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog nlohmann_json::nlohmann_json OpenAL::OpenAL SndFile::sndfile imgui::imgui)

# Add definitions for SSE/AE compatibility
target_compile_definitions(${PROJECT_NAME} PRIVATE SKSE_SUPPORT_XBYAK ENABLE_SSE ENABLE_AE)

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
if (EXISTS "${MO2_MOD_FOLDER}")
    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${OUTPUT_FOLDER}" "${MO2_MOD_FOLDER}"
        VERBATIM
    )
else()
    message(WARNING "MO2 folder not found, skipping deployment.")
endif()

# Optional message for output folder configuration
message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")
message(STATUS "Copying dist folder to output: ${OUTPUT_FOLDER}")

# OPTIONAL
if (EXISTS "${MO2_MOD_FOLDER}")
    message(STATUS "Copying output to MO2 mod folder: ${MO2_MOD_FOLDER}")
else()
    message(WARNING "MO2 folder not found, skipping message.")
endif()

# Launch Mod Organizer 2 with SKSE immediately after successful compilation
add_custom_target(RunGame
    COMMAND cmd /C start "" "M:/ADT Skyrim Developement/ModOrganizer.exe" -p "ADT15 - Without ENB" "moshortcut://Portable:SKSE 1.5"
    VERBATIM
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running Skyrim Special Edition with SKSE via Mod Organizer"
    COMMAND IF %ERRORLEVEL% NEQ 0 (echo "Game launch failed!" && exit 1)
)
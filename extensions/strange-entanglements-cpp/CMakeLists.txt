# ------------------------------------------------------------------------------
# strange-entanglements-cpp/CMakeLists.txt
# * 23-Feb-2025: That One Game Dev (2023), Godot 4: GDExtension for C++ using CMake,
#   https://thatonegamedev.com/cpp/cmake/godot-4-gdextension-for-c-using-cmake/
# * 01-Mar-2025: The Great Refactoring (2024), Setting up your GDExtension with CMake and Qt Creator,
#   https://vilelasagna.ddns.net/coding/setting-up-your-gdextension-with-cmake-and-qt-creator/
# ------------------------------------------------------------------------------
project(strange-entanglements-cpp)

# ------------------------------------------------------------------------------
# Automatically pick up files added to source
# ------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# ------------------------------------------------------------------------------
# Add a dynamic library called ${PROJECT_NAME}, to build ${PROJECT_NAME}.dll
# ------------------------------------------------------------------------------
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# ------------------------------------------------------------------------------
# Set the library filename to match .gdextension file
# ------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME ${PROJECT_NAME}.$<LOWER_CASE:$<PLATFORM_ID>>.template_release.x86_64)

else()

	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME ${PROJECT_NAME}.$<LOWER_CASE:$<PLATFORM_ID>>.template_debug.x86_64)

endif()




# ------------------------------------------------------------------------------
# * 11-Oct-2025: This seems to have linked our static inkcpp libraries?
#   * FIXME: Find a way to use inkcpp library names, ...
# ------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
	godot::cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

# ------------------------------------------------------------------------------
# Copy library to Godot project and executable post-build
# ------------------------------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/game/bin/
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/exe/)
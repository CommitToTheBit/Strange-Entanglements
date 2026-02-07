# -----------------------------------------------------------------------------
# extensions/CMakeLists.txt
# * 23-Feb-2025: That One Game Dev (2023), Godot 4: GDExtension for C++ using CMake,
#   https://thatonegamedev.com/cpp/cmake/godot-4-gdextension-for-c-using-cmake/
# * 01-Mar-2025: Key difference with the above tutorial is I've checked out the
#   godot-cpp repository as a submodule, and built it by calling scons, before
#   using CMake to build extensions
# -----------------------------------------------------------------------------
project(extensions)

add_subdirectory(godot-cpp)
add_subdirectory(strange-entanglements-cpp)
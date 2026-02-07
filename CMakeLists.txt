# -----------------------------------------------------------------------------
# CMakeLists.txt
# * 23-Feb-2025: That One Game Dev (2023), Godot 4: GDExtension for C++ using CMake,
#   https://thatonegamedev.com/cpp/cmake/godot-4-gdextension-for-c-using-cmake
# * 01-Mar-2025: Key difference with the above tutorial is I've checked out the
#   godot-cpp repository as a submodule, and built it by calling scons, before
#   using CMake to build extensions
# * 08-Jun-2025: asmaloney (2023), GDExtensionTemplate, https://github.com/asmaloney/GDExtensionTemplate
# * 08-Jun-2025: runvs (2023), Link Time Optimization in C++, https://runvs.io/News/929
# * 09-Jun-2025: GDExtensions are shared libs, hence CMAKE_SHARED_LINKER_FLAGS
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.19)
project(strange-entanglements)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ------------------------------------------------------------------------------
# * 25-Oct-2025: Reddit (2023), -fcs-profile-generate doesn't generate profiles. Any ideas?
#   https://www.reddit.com/r/LLVM/comments/182psjg/fcsprofilegenerate_doesnt_generate_profiles_any/
# ------------------------------------------------------------------------------
if (NOT CMAKE_CXX_PGO STREQUAL "csir-pgo-instr")

	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -Wno-everything")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -finline-functions -fvisibility=hidden")
	
	# --------------------------------------------------------------------------
	# Append if and only if debugging Release builds, to see debug symbols.
	# * 31-Dec-2025: Stack Overflow (2014), Clang and LLVM - Release vs Debug builds,
	#   https://stackoverflow.com/questions/21643917/clang-and-llvm-release-vs-debug-builds
	# * 31-Dec-2025: Stack Overflow (2011), Debug vs Release in CMake [closed],
	#   https://stackoverflow.com/questions/7724569/debug-vs-release-in-cmake
	# --------------------------------------------------------------------------
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
	
	# --------------------------------------------------------------------------
	# Profile-Guided Optimisation
	# * 25-Oct-2025: After eyeballing frontend PGO, intermediate representation
	#   (IR) PGO, and context-sensitive intermediate representation PGO (CSIR),
	#   all appear to offer the expected ~10%-20% performance boost, with
	#   negligible differences between. Will revisit after setting up a robust
	#   benchmarking pipeline.
	# * UNIMPLEMENTED: Integrate post-link profile-guided optimisations (PLO)
	#   with BOLT?
	# --------------------------------------------------------------------------
	# * 25-Oct-2025: Amir Ayupov (2023), The many faces of LLVM PGO and FDO,
	#   https://aaupov.github.io/blog/2023/07/09/pgo
	# * 25-Oct-2025: IBM, Frequently asked questions (FAQs) on PGO
	#   https://www.ibm.com/docs/en/openxl-c-and-cpp-aix/17.1.3?topic=compatibility-frequently-asked-questions-faqs-pgo
	# --------------------------------------------------------------------------
	if (CMAKE_CXX_PGO STREQUAL "pgo-instr")

		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-instr-generate=${CMAKE_BINARY_DIR}/pgo/pgo-instr.profraw")

	elseif (CMAKE_CXX_PGO STREQUAL "pgo")
	
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-instr-use=${CMAKE_BINARY_DIR}/pgo/pgo.profdata")

	elseif (CMAKE_CXX_PGO STREQUAL "ir-pgo-instr")

		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-generate=${CMAKE_BINARY_DIR}/pgo/ir-pgo-instr.profraw")

	elseif (CMAKE_CXX_PGO STREQUAL "ir-pgo")
	
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-use=${CMAKE_BINARY_DIR}/pgo/ir-pgo.profdata")

	elseif (CMAKE_CXX_PGO STREQUAL "csir-pgo-instr")

		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-use=${CMAKE_BINARY_DIR}/pgo/ir-pgo.profdata -fcs-profile-generate=${CMAKE_BINARY_DIR}/pgo/csir-pgo-instr.profraw")

	elseif (CMAKE_CXX_PGO STREQUAL "csir-pgo")
	
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-use=${CMAKE_BINARY_DIR}/pgo/csir-pgo.profdata")

	endif()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Ob3 -GL")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-LTCG")

endif()

add_subdirectory(extensions)
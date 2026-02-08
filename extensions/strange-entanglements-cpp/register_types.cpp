// -----------------------------------------------------------------------------
// register_types.cpp
// * 23-Feb-2025: That One Game Dev (2023), Godot 4: GDExtension for C++ using CMake,
//   https://thatonegamedev.com/cpp/cmake/godot-4-gdextension-for-c-using-cmake/
// -----------------------------------------------------------------------------
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "StrangeQuantumState.h"

// ----------------------------------------------------------------------------
// InitialiseModule: Register all classes here
// ----------------------------------------------------------------------------
void InitialiseModule(godot::ModuleInitializationLevel p_level)
{
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}

	// ------------------------------------------------------------------------
	// Classes
	// ------------------------------------------------------------------------
	godot::ClassDB::register_class<StrangeQuantumState>();
}

// ----------------------------------------------------------------------------
// UninitialiseModule: Do nothing.
// ----------------------------------------------------------------------------
void UninitialiseModule(godot::ModuleInitializationLevel p_level)
{
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
}

extern "C"
{
	GDExtensionBool GDE_EXPORT strange_entanglements_init(const GDExtensionInterfaceGetProcAddress p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_object(p_interface, p_library, r_initialization);

		init_object.register_initializer(InitialiseModule);
		init_object.register_terminator(UninitialiseModule);
		init_object.set_minimum_library_initialization_level(godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_object.init();
	}
}
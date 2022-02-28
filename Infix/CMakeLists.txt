cmake_minimum_required (VERSION 3.8)

set(COMPILE_SHADERS ON)

add_executable (Infix "Infix.cpp" "Includes/stb_image.h" "Infix_Engine/Infix_Core/Display/Graphics/Graphics.h" "Infix_Engine/Infix_Core/Display/Display.h"  "Infix_Engine/Infix_Core/Display/Graphics/Context/Swap_Chain.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Device_Context.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Command_Buffer.h" "Infix_Engine/Infix_Core/Display/Graphics/Debug/Debug.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Application_Context.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Sync_Object.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Memory_Context/Circular_Buffer.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Memory_Context/Texture_Context.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Memory_Context/Buffer.h" "Infix_Engine/Infix_Core/Display/Graphics/Context/Memory_Context/Unifrorm_Buffer.h")
# download and install vcpkg for windows development
# also CMakeSettings.json needs manual path to vcpkg
# change "C:\\Users\\johnw\\OneDrive\\Documents\\Vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
# to the corrent file path to vcpkg

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)


target_link_libraries(Infix PRIVATE glfw glm Vulkan::Vulkan)

if(WIN32 AND COMPILE_SHADERS)
	# copy shaders file to output file:
	file(GLOB shader_file_paths ${CMAKE_CURRENT_SOURCE_DIR}/Infix_Engine/Infix_Core/Display/Graphics/GraphicsCode/Shaders/*)
	FOREACH(shdaer_file_path ${shader_file_paths})
		get_filename_component(file_name_com ${shdaer_file_path} NAME)
		configure_file(${shdaer_file_path} ${CMAKE_CURRENT_BINARY_DIR}/Shaders/${file_name_com} COPYONLY)
	ENDFOREACH()
	# now compile the files
	ADD_CUSTOM_TARGET(
    shaders_to_compile
    COMMAND cmd /c ${CMAKE_CURRENT_BINARY_DIR}/Shaders/compile.bat)
	ADD_DEPENDENCIES(Infix shaders_to_compile)
endif()

# copy all resources to output file
file(GLOB resources_file_paths ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*)
FOREACH(resources_file_path ${resources_file_paths})
	get_filename_component(file_name_com ${resources_file_path} NAME)
	configure_file(${resources_file_path} ${CMAKE_CURRENT_BINARY_DIR}/Resources/${file_name_com} COPYONLY)
ENDFOREACH()
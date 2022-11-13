cmake_minimum_required(VERSION 3.5)

project (BasicRT3 VERSION 1.0.0 LANGUAGES CXX )

#=== CREATING COMPILING DATABASE ===#
# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== FINDING PACKAGES ===#

# # Locate TinyXml2 package (library)
# # find_package(TinyXml2 REQUIRED)
# # include_directories(${TinyXML2_INCLUDE_DIRS})

# TinyXML2 is not a module package, therefore cannot be found with find_package() command.
# Set those manually, for now.
set( TinyXML2_INCLUDE_DIRS "tinyxml2" )
set( TinyXML2_LIB_DIRS "tinyxml2" )
set( TinyXML2_LIBRARIES "tinyxml2" )

# Set "manually" paths that need to be considered while compiling/linking
include_directories( cameras
                     integrators
                     core
                     ext
                     ${TinyXML2_INCLUDE_DIRS} )
link_directories( ${TinyXML2_LIB_DIRS} )

#=== SETTING VARIABLES ===#
# Compiling flags
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic -fopenmp" )
set( RT3_SOURCE_DIR "src" )

#=== main  target ===
add_executable(basic_rt3 ${RT3_SOURCE_DIR}/core/api.cpp
                         ${RT3_SOURCE_DIR}/core/Background/background.cpp
			 ${RT3_SOURCE_DIR}/core/Background/BackgroundColor.cpp
			 ${RT3_SOURCE_DIR}/core/Background/BackgroundTexture.cpp
                         ${RT3_SOURCE_DIR}/core/material.cpp
                         ${RT3_SOURCE_DIR}/core/camera.cpp
                         ${RT3_SOURCE_DIR}/core/error.cpp
                         ${RT3_SOURCE_DIR}/core/film.cpp
                         ${RT3_SOURCE_DIR}/core/image_io.cpp
                         ${RT3_SOURCE_DIR}/core/parser.cpp
                         ${RT3_SOURCE_DIR}/core/camera.cpp
                         ${RT3_SOURCE_DIR}/core/Integrator/integrator.cpp
                         ${RT3_SOURCE_DIR}/core/Shape/Sphere.cpp
			 ${RT3_SOURCE_DIR}/core/Shape/Triangle.cpp
                         ${RT3_SOURCE_DIR}/core/Light/light.cpp
			 ${RT3_SOURCE_DIR}/core/Light/AmbientLight.cpp
			 ${RT3_SOURCE_DIR}/core/Light/DirectionalLight.cpp
			 ${RT3_SOURCE_DIR}/core/Light/PointLight.cpp
			 ${RT3_SOURCE_DIR}/core/Light/SpotLight.cpp
                         ${RT3_SOURCE_DIR}/core/tinyxml2.cpp
                         ${RT3_SOURCE_DIR}/ext/lodepng.cpp
                         ${RT3_SOURCE_DIR}/main/rt3.cpp
                        )

#target_link_libraries(basic_rt3 PRIVATE ${TinyXML2_LIBRARIES})

#define C++17 as the standard.
set_property(TARGET basic_rt3 PROPERTY CXX_STANDARD 17)

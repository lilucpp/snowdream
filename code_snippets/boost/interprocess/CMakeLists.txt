cmake_minimum_required(VERSION 3.10)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 11)              # Compile as C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Require C++11 support

project(BlockingQueue
        VERSION 0.0.1
        DESCRIPTION "The BlockingQueue is C++ MPMC library (lilu) @ https://github.com/lilucpp/base"
        LANGUAGES CXX
        )

# People keep running CMake in the wrong folder, completely nuking their project or creating weird bugs.
# This checks if you're running CMake from a folder that already has CMakeLists.txt.
# Importantly, this catches the common case of running it from the root directory.
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT)
if (EXISTS "${PATH_TO_CMAKELISTS_TXT}")
    message(FATAL_ERROR "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake ..\" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif ()

if(MSVC)
	add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
	)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

if(UNIX) 
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE CACHE STRING "Release")
    endif()
elseif(MSVC)
    # cmake --build . --config Release
endif()

find_package(Boost )

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()

if(MSVC)
  #add_compile_options(/W4 /WX)
else()
  #add_compile_options(-Wall -Wextra -pedantic -Werror -Wconversion -Wno-unused-parameter -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -march=native)
endif()

add_executable(producer producer.cpp)
if(UNIX)
    target_link_libraries(producer  pthread)
endif()

add_executable(consumer consumer.cpp)
if(UNIX)
    target_link_libraries(consumer  pthread)
endif()

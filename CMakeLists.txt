cmake_minimum_required(VERSION 3.27)

# Set project without CUDA language by default
project(Proj LANGUAGES CXX)

# Set optional RequireCUDA flag (defaults to OFF)
option(RequireCUDA "Require CUDA support" ON)

if (RequireCUDA)
    find_package(CUDAToolkit)
    if (CUDAToolkit_FOUND)
        message(STATUS "CUDA found! Enabling CUDA support.")
        enable_language(CUDA)
        
        set(CUDA_VISIBILITY_PRESET hidden)
        set(CMAKE_CUDA_STANDARD 20)
        set(CMAKE_CUDA_STANDARD_REQUIRED ON)
        set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
        set(CMAKE_CUDA_VISIBILITY_PRESET hidden)
        set(CMAKE_CUDA_ARCHITECTURES native)

        # Add general CMake module paths
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
        list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

        # Include CUDA-specific files or flags
        include(compiler_flags)
        include(find_cuda)
    else()
        message(STATUS "CUDA not found. Continuing without CUDA support.")
    endif()
else()
    message(STATUS "CUDA support not required.")
    # Add general CMake module paths
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
endif()

# Configure the build

include(pr_build_options)
find_package(lapackpp REQUIRED)
add_subdirectory(proj)

add_executable(proj_main main.cc)
set_source_files_properties(main.cc PROPERTIES LANGUAGE CUDA)
set_property(TARGET proj_main PROPERTY CUDA_ARCHITECTURES native)
target_link_libraries(proj_main PUBLIC blaspp lapackpp)

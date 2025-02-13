cmake_minimum_required(VERSION 3.27)

# Set project without HIP language by default
project(Proj LANGUAGES CXX)


find_package(HIPToolkit)
enable_language(HIP)
if (HIPToolkit_FOUND)
    message(STATUS "HIP found! Enabling HIP support.")
    enable_language(HIP)
    
    set(CUDA_VISIBILITY_PRESET hidden)
    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
    set(CMAKE_CUDA_VISIBILITY_PRESET hidden)
    set(CMAKE_CUDA_ARCHITECTURES native)

    # Add general CMake module paths
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

    # Include HIP-specific files or flags
    include(compiler_flags)
    include(find_cuda)
else()
    message(STATUS "HIP not found. Continuing without HIP support.")
endif()

# Configure the build

include(/home/users/gcapodag/repos/gpu-portability/CMake/pr_build_options.cmake)
find_package(lapackpp REQUIRED)
add_subdirectory(proj)

add_executable(proj_main main.cc)
set_source_files_properties(main.cc PROPERTIES LANGUAGE HIP)
set_property(TARGET proj_main PROPERTY HIP_ARCHITECTURES native)
target_link_libraries(proj_main PUBLIC blaspp lapackpp)

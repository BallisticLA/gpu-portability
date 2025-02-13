cmake_minimum_required(VERSION 3.27)

# Set project without HIP language by default
project(Proj LANGUAGES CXX)

if (NOT CMAKE_GPU_RUNTIME)
   set(GPU_RUNTIME "ROCM" CACHE STRING "Switches between ROCM and CUDA")
else (NOT CMAKE_GPU_RUNTIME)
   set(GPU_RUNTIME "${CMAKE_GPU_RUNTIME}" CACHE STRING "Switches between ROCM and CUDA")
endif (NOT CMAKE_GPU_RUNTIME)
# Really should only be ROCM or CUDA, but allowing HIP because it is the currently built-in option
set(GPU_RUNTIMES "ROCM" "CUDA" "HIP")
if(NOT "${GPU_RUNTIME}" IN_LIST GPU_RUNTIMES)
    set(ERROR_MESSAGE "GPU_RUNTIME is set to \"${GPU_RUNTIME}\".\nGPU_RUNTIME must be either HIP, ROCM, or CUDA.")
    message(FATAL_ERROR ${ERROR_MESSAGE})
endif()
# GPU_RUNTIME for AMD GPUs should really be ROCM, if selecting AMD GPUs
# so manually resetting to HIP if ROCM is selected
if (${GPU_RUNTIME} MATCHES "ROCM")
   set(GPU_RUNTIME "HIP")
endif (${GPU_RUNTIME} MATCHES "ROCM")
set_property(CACHE GPU_RUNTIME PROPERTY STRINGS ${GPU_RUNTIMES})

enable_language(${GPU_RUNTIME})
set(CMAKE_${GPU_RUNTIME}_EXTENSIONS OFF)
set(CMAKE_${GPU_RUNTIME}_STANDARD_REQUIRED ON)

if (DEFINED ENV{ROCM_PATH})
	set(ROCM_PATH $ENV{ROCM_PATH})
else (DEFINED ENV{ROCM_PATH})
	execute_process(COMMAND hipconfig --path OUTPUT_VARIABLE ROCM_PATH ERROR_QUIET)
endif (DEFINED ENV{ROCM_PATH})

set(ROCMCC_FLAGS "${ROCMCC_FLAGS} -munsafe-fp-atomics")
set(CUDACC_FLAGS "${CUDACC_FLAGS} ")

if (${GPU_RUNTIME} MATCHES "HIP")
   set(HIPCC_FLAGS "${ROCMCC_FLAGS}")
else (${GPU_RUNTIME} MATCHES "HIP")
   set(HIPCC_FLAGS "${CUDACC_FLAGS} -I/${ROCM_PATH}/include")
endif (${GPU_RUNTIME} MATCHES "HIP")

    # Add general CMake module paths
    #    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
    #list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

    # Include HIP-specific files or flags
    #include(compiler_flags)
    #include(find_cuda)

# Configure the build

include(/home/users/gcapodag/repos/gpu-portability/CMake/pr_build_options.cmake)
find_package(lapackpp REQUIRED)
add_subdirectory(proj)

add_executable(proj_main main.cc)
set_source_files_properties(main.cc PROPERTIES LANGUAGE ${GPU_RUNTIME})
#set_property(TARGET proj_main PROPERTY HIP_ARCHITECTURES native)
target_link_libraries(proj_main PUBLIC blaspp lapackpp)

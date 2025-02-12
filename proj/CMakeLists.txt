
set(Proj_cxx_sources
    pr_cusolver.hh
    pr_cuda_kernels.cuh
    pr_cuda_macros.hh
    pr_algorithm.hh
)

add_library(Proj INTERFACE)

target_link_libraries(Proj INTERFACE 
    lapackpp
    blaspp
)

if (Proj_HAS_OpenMP)
    target_link_libraries(Proj INTERFACE OpenMP::OpenMP_CXX)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pr_config.hh.in pr_config.hh)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pr_config.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Proj)


set(Proj_cxx_opts -Wall -Wextra)
if (SANITIZE_ADDRESS)
    list(APPEND Proj_cxx_opts -fsanitize=address)
    target_link_options(Proj INTERFACE -fsanitize=address)
endif()
target_compile_options(Proj INTERFACE ${Proj_cxx_opts})

#set_source_files_properties(${Proj_cxx_sources}
#    PROPERTIES COMPILE_OPTIONS "${Proj_cxx_opts}"
#)

target_include_directories(
    Proj INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Proj>
)

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DESTINATION include FILES_MATCHING PATTERN "*.hh"
)

install(
    TARGETS Proj
    EXPORT Proj
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Proj
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT Proj
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
)

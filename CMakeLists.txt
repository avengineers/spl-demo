# cmake project definition
cmake_minimum_required(VERSION 3.24.0)
cmake_policy(VERSION 3.24)

# configure the current variant to be build
set(BUILD_KIT prod CACHE STRING "Target Group to build.")

include(${CMAKE_SOURCE_DIR}/variants/${VARIANT}/config.cmake)

if(BUILD_KIT STREQUAL prod)
    project(${VARIANT} C ASM)
    set(VARIANT_ADDITIONAL_COMPILE_C_FLAGS -Dstatic_scope_file=static)
else()
    # In case of test the project is a C++ project due to GTest usage
    project(${VARIANT} C ASM CXX)

    # define static to nothing on file scope to allow access to
    # static variables and function in unit test cases.
    # Furthermore store temporary files during compilation to
    # get preprocessed files.
    set(VARIANT_ADDITIONAL_COMPILE_C_FLAGS -Dstatic_scope_file= -save-temps)
endif()

# Fetch all external dependencies into modules directory
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/build/modules CACHE INTERNAL "")
set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)

# Fetch spl-core
FetchContent_Declare(
    spl-core
    GIT_REPOSITORY https://github.com/avengineers/spl-core.git
    GIT_TAG v4.5.0
)
FetchContent_MakeAvailable(spl-core)

# Include spl-core
include(${spl-core_SOURCE_DIR}/cmake/spl.cmake)

include(${CMAKE_SOURCE_DIR}/variants/${VARIANT}/parts.cmake)

# TODO: refactor/remove; we want to explicitly add includes where required
foreach(component_name ${COMPONENT_NAMES})
    target_include_directories(${component_name} PUBLIC ${target_include_directories__INCLUDES})
endforeach()

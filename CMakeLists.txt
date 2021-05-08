cmake_minimum_required(VERSION 3.20)
project(PhoenixKiller C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Add module path in case this is project root
if (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
endif ()

if(NVML_RETRIEVE_HEADER_ONLINE)
    file(DOWNLOAD "https://raw.githubusercontent.com/NVIDIA/nvidia-settings/master/src/nvml.h" ${PROJECT_BINARY_DIR}/include/nvml.h)
endif()

include_directories(include)

add_subdirectory(3rd-party/argparse)
add_subdirectory(3rd-party/termcolor)
add_subdirectory(src)
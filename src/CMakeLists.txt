find_package(NVML REQUIRED)
find_package(argparse REQUIRED)
find_package(termcolor REQUIRED)
find_package(crypto REQUIRED)

include_directories(${NVML_INCLUDE_DIRS})

add_library(GPUInfo STATIC extract_gpu_info.c get_process_info_linux.c)

add_executable(PhoenixKiller main.cpp util.cpp gpupid/gpu_pid.cpp)
add_executable(gpupid gpupid/main.cpp)

target_link_libraries(GPUInfo ${NVML_LIBRARIES})
target_link_libraries(PhoenixKiller crypto argparse termcolor GPUInfo)
target_link_libraries(gpupid GPUInfo)

install(TARGETS PhoenixKiller gpupid
        RUNTIME DESTINATION bin)
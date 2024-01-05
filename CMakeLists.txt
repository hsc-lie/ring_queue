cmake_minimum_required(VERSION 3.28.0)

project(ring_queue_test C)

add_executable(${PROJECT_NAME})

#设置生成目标的目录
set(EXECUTABLE_OUTPUT_PATH out)

# 彩色日志输出；
set(CMAKE_COLOR_DIAGNOSTICS TRUE)

target_sources(${PROJECT_NAME}
    PRIVATE
    main.c
)

add_subdirectory(ring_queue)
add_subdirectory(test)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    -Wall -O0 -g
)

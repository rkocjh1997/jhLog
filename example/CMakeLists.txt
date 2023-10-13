
cmake_minimum_required(VERSION 3.11.0)
project(jhLog_examples CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

set(JHLOG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

file (GLOB JHLOG_LIST
    ${JHLOG_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} 
    ${JHLOG_LIST}
    examples.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${JHLOG_SOURCE_DIR}
    ${JHLOG_SOURCE_DIR}/fmt
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pthread
)
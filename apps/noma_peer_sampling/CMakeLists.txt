cmake_minimum_required(VERSION 3.9)

project(noma_peer_sampling)

find_package(Boost REQUIRED) 

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "./*.h" 
)

include_directories(${Boost_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})

add_executable(noma_peer_sampling ${${PROJECT_NAME}_SRC_FILES})

cmake_minimum_required(VERSION 3.9)

project(noma_node_random_add)

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "./*.h" 
)

include_directories(${NOMA_CORE_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("../libs/actor-framework/libcaf_core")

message(${NOMA_CORE_LIBRARY_DIRS})
link_directories(${NOMA_CORE_LIBRARY_DIRS})

add_executable(noma_node_random_add "main.cpp")

target_link_libraries(noma_node_random_add noma_tcp noma_remoting noma_clustering noma_core)

if(WIN32)
  target_link_libraries(noma_node_random_add libcaf_core_static)
else(WIN32)
  target_link_libraries(noma_node_random_add libcaf_core_shared pthread boost_system)
endif(WIN32)

ADD_DEPENDENCIES(noma_node_random_add noma_core)

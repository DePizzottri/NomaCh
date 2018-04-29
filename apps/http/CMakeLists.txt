cmake_minimum_required(VERSION 3.9)

project(noma_http)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "./*.h" 
)

include_directories(${NOMA_CORE_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("../libs/actor-framework/libcaf_core")

link_directories(${NOMA_CORE_LIBRARY_DIRS})

add_executable(noma_http "http.cpp" "request_parser.cpp")

target_link_libraries(noma_http noma_tcp noma_remoting noma_clustering noma_core restbed-static)

if(WIN32)
  target_link_libraries(noma_http libcaf_core_static)
else(WIN32)
  target_link_libraries(noma_http libcaf_core_shared pthread boost_system)
endif(WIN32)

ADD_DEPENDENCIES(noma_http noma_core)


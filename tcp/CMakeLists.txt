cmake_minimum_required(VERSION 3.9)

project(noma_tcp)

find_package(Boost COMPONENTS filesystem system REQUIRED) 

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "src/*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "include/*.h" 
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("../libs/actor-framework/libcaf_core")

link_directories(${Boost_LIBRARY_DIRS})

add_library(noma_tcp STATIC ${${PROJECT_NAME}_SRC_FILES})

if(WIN32)
  ADD_DEPENDENCIES(noma_tcp libcaf_core_static)
else(WIN32)
  ADD_DEPENDENCIES(noma_tcp libcaf_core_shared)
endif(WIN32)


set(NOMA_TCP_INCLUDE_DIRS 
    ${${PROJECT_NAME}_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIRS} 
    ${PROJECT_SOURCE_DIR}/../libs/actor-framework/libcaf_core
CACHE INTERNAL "Noma TCP include" FORCE)

set(NOMA_TCP_LIBRARY_DIRS 
    ${Boost_LIBRARY_DIRS} 
CACHE INTERNAL "Noma TCP libs" FORCE)

#message(${NOMA_TCP_LIBRARY_DIRS})

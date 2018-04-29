cmake_minimum_required(VERSION 3.9)

project(noma_remoting)

find_package(Boost REQUIRED) 

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "src/*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "include/*.h" 
)

include_directories(${NOMA_TCP_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIRS})

add_library(noma_remoting STATIC ${${PROJECT_NAME}_SRC_FILES})

ADD_DEPENDENCIES(noma_remoting noma_tcp)

set(NOMA_REMOTING_INCLUDE_DIRS ${NOMA_TCP_INCLUDE_DIRS} ${${PROJECT_NAME}_INCLUDE_DIR}  CACHE INTERNAL "Noma Remoting include" FORCE)

set(NOMA_REMOTING_LIBRARY_DIRS 
    ${NOMA_TCP_LIBRARY_DIRS} 
CACHE INTERNAL "Noma Remoting libs" FORCE)

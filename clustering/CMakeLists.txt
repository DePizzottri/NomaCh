cmake_minimum_required(VERSION 3.9)

project(noma_clustering)

find_package(Boost REQUIRED) 

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "src/*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "include/*.h" 
)

include_directories(${NOMA_REMOTING_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("../libs/delta-enabled-crdts")
include_directories("../crdt/include")
include_directories("../utils/include")

link_directories(${Boost_LIBRARY_DIRS})

add_library(noma_clustering STATIC ${${PROJECT_NAME}_SRC_FILES})

ADD_DEPENDENCIES(noma_clustering noma_remoting)

set(NOMA_CLUSTERING_INCLUDE_DIRS 
    ${${PROJECT_NAME}_INCLUDE_DIR} 
    ${NOMA_REMOTING_INCLUDE_DIRS} 
    ${PROJECT_SOURCE_DIR}/../libs/delta-enabled-crdts
    ${PROJECT_SOURCE_DIR}/../crdt/include
    ${PROJECT_SOURCE_DIR}/../utils/include
CACHE INTERNAL "Noma Clustering include" FORCE)

set(NOMA_CLUSTERING_LIBRARY_DIRS 
    ${NOMA_REMOTING_LIBRARY_DIRS} 
CACHE INTERNAL "Noma Clustering libs" FORCE)

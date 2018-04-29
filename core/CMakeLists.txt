cmake_minimum_required(VERSION 3.9)

project(noma_core)

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "src/*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "include/*.h" 
)

include_directories(${NOMA_CLUSTERING_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})

add_library(noma_core STATIC ${${PROJECT_NAME}_SRC_FILES})

ADD_DEPENDENCIES(noma_core noma_clustering)

set(NOMA_CORE_INCLUDE_DIRS 
    ${${PROJECT_NAME}_INCLUDE_DIR} 
    ${NOMA_CLUSTERING_INCLUDE_DIRS}
CACHE INTERNAL "Noma Core include" FORCE)

set(NOMA_CORE_LIBRARY_DIRS 
    ${NOMA_CLUSTERING_LIBRARY_DIRS} 
CACHE INTERNAL "Noma Core libs" FORCE)

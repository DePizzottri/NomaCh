cmake_minimum_required(VERSION 3.9)

project(noma_rest)

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "src/*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "include/*.h" 
)

include_directories(${NOMA_CORE_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("../../libs/restbed/source")

add_library(noma_rest STATIC ${${PROJECT_NAME}_SRC_FILES})

ADD_DEPENDENCIES(noma_rest noma_core)
ADD_DEPENDENCIES(noma_rest restbed-static)

set(NOMA_REST_INCLUDE_DIRS 
    ${${PROJECT_NAME}_INCLUDE_DIR} 
    ${NOMA_CORE_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}../../libs/restbed/source
CACHE INTERNAL "Noma REST include" FORCE)

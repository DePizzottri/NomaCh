cmake_minimum_required(VERSION 3.9)

project(noma_tests)

file(GLOB ${PROJECT_NAME}_SRC_FILES
    "src/*.cpp"
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_NAME}_INCLUDE_FILES
    "./*.h" 
)

include_directories(${NOMA_CORE_INCLUDE_DIRS})

include_directories(${${PROJECT_NAME}_INCLUDE_DIR})

link_directories(${NOMA_CORE_LIBRARY_DIRS})

add_executable(noma_tests ${${PROJECT_NAME}_SRC_FILES})

target_link_libraries(noma_tests noma_tcp noma_remoting noma_clustering noma_core)

if(WIN32)
  target_link_libraries(noma_tests libcaf_core_static)
else(WIN32)
  ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK) 
  target_link_libraries(noma_tests libcaf_core_shared pthread boost_system boost_unit_test_framework)
endif(WIN32)

ADD_DEPENDENCIES(noma_tests noma_core)

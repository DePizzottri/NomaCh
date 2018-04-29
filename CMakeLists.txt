cmake_minimum_required(VERSION 3.9)

project(NomaCh)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(CAF_NO_EXAMPLES YES)
set(CAF_NO_IO YES)
set(CAF_NO_UNIT_TESTS YES)
add_subdirectory(libs/actor-framework)
add_subdirectory(tcp)
add_subdirectory(remoting)
add_subdirectory(clustering)
add_subdirectory(core)

option(BUILD_SSL OFF)
add_subdirectory(libs/restbed)
add_subdirectory(interface/rest)

add_subdirectory(apps/node_random_add)
add_subdirectory(apps/http)
add_subdirectory(apps/nomadb)

add_subdirectory(apps/noma_peer_sampling)

add_subdirectory(tests)

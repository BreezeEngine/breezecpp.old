cmake_minimum_required(VERSION 3.0)
project(breeze)
include(ExternalProject)
SET(BREEZE_DEPS_DOWNLOAD_DIR ${breeze_SOURCE_DIR}/Downloads)
SET(CMAKE_MODULE_PATH ${breeze_SOURCE_DIR}/CMake)

INCLUDE(ExternalGLFW)
INCLUDE(ExternalGlad)
#INCLUDE(ExternalBoostLog)
add_subdirectory(dependencies/asyncplusplus)
add_subdirectory(dependencies/enet)
add_subdirectory(dependencies/serialization/CMake)
#add_subdirectory(dependencies/SDL-mirror)


#SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-g -std=c++1z -fcolor-diagnostics")
include_directories(
  dependencies/glad/GL/include/
  dependencies/glm/
  dependencies/SDL-mirror/include
  dependencies/range-v3/include
  dependencies/hana/include
  dependencies/ctti/include
  dependencies/asyncplusplus/include
  dependencies/asio/asio/include
  dependencies/enet/include
  dependencies/serialization/include
  dependencies/msgpack-c/include
  )
include_directories(SYSTEM dependencies/stb/)
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

include_directories(src)
include_directories(src/util)
include_directories(src/window)
include_directories(src/network)
file(GLOB VENDORS_SOURCES dependencies/glad/GL/src/glad.c)
file(GLOB_RECURSE PROJECT_HEADERS src/*.hpp)
#file(GLOB PROJECT_SOURCES Glitter/Sources/*.cpp)
#file(GLOB PROJECT_SHADERS Glitter/Shaders/*.comp
#                          Glitter/Shaders/*.frag
#                          Glitter/Shaders/*.geom
#                          Glitter/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
  Readme.md
  .gitattributes
  .gitignore
  .gitmodules)
#add_custom_target(headers SOURCES ${PROJECT_HEADERS})
source_group("Headers" FILES ${PROJECT_HEADERS})
#source_group("Shaders" FILES ${PROJECT_SHADERS})
#source_group("Sources" FILES ${PROJECT_SOURCES})
#source_group("Vendors" FILES ${VENDORS_SOURCES})

include_directories(src/)
add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(sdltest ${VENDORS_SOURCES} examples/sdl2/main.cpp )

target_link_libraries(sdltest SDL2-static ${SDL2_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(sdltest PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(ecs ${VENDORS_SOURCES} examples/ecs/main.cpp )
target_link_libraries(ecs glfw
  ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
  )
set_target_properties(ecs PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(graphics examples/graphics/main.cpp )
target_link_libraries(graphics glad glfw Async++
  ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
  )
set_target_properties(graphics PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(asioclient ${VENDORS_SOURCES} examples/asio/client.cpp )
target_link_libraries(asioclient serialization Async++
  ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
  )
set_target_properties(asioclient PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})


add_executable(asioserver ${VENDORS_SOURCES} examples/asio/server.cpp )
target_link_libraries(asioserver Async++
  ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
  )
set_target_properties(asioserver PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

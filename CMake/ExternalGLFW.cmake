option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF) 
option(GLFW_BUILD_TESTS OFF) 
include_directories(${PROJECT_SOURCE_DIR}/dependencies/glfw/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/glfw)
add_definitions(-DGLFW_INCLUDE_NONE)


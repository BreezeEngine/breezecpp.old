# Install script for directory: /home/maik/projects/breeze/dependencies/asyncplusplus

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/maik/projects/breeze/build/dependencies/asyncplusplus/libasync++.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/Async++.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/Async++.cmake"
         "/home/maik/projects/breeze/build/dependencies/asyncplusplus/CMakeFiles/Export/cmake/Async++.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/Async++-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/Async++.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "/home/maik/projects/breeze/build/dependencies/asyncplusplus/CMakeFiles/Export/cmake/Async++.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "/home/maik/projects/breeze/build/dependencies/asyncplusplus/CMakeFiles/Export/cmake/Async++-noconfig.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/async++" TYPE FILE FILES
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/aligned_alloc.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/cancel.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/continuation_vector.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/parallel_for.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/parallel_invoke.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/parallel_reduce.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/partitioner.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/range.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/ref_count.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/scheduler.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/scheduler_fwd.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/task.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/task_base.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/traits.h"
    "/home/maik/projects/breeze/dependencies/asyncplusplus/include/async++/when_all_any.h"
    )
endif()


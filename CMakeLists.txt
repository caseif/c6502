cmake_minimum_required(VERSION 3.10)

project(c6502 VERSION 0.1.0)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
endif()

set(SRC_DIR src)
set(INC_DIR include)

file(GLOB_RECURSE C_FILES ${SRC_DIR}/*.c)
file(GLOB_RECURSE H_FILES ${INC_DIR}/*.h)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  if(MSVC_VERSION LESS 1911)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG:FULL /Od")
  else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG:FASTLINK /Od")
  endif()
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized \
                       -Wmissing-declarations -Winit-self -pedantic -pedantic-errors -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

add_library(${PROJECT_NAME} STATIC ${C_FILES} ${H_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR}) 

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)

if(UNIX)
  install(TARGETS ${PROJECT_NAME}
          ARCHIVE
          DESTINATION "/usr/lib")
  install(DIRECTORY "${INC_DIR}/"
          DESTINATION "/usr/include"
          PATTERN "${INC_DIR}/*")
endif()

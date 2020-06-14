cmake_minimum_required(VERSION 3.10)

project(c6502 VERSION 0.1.0)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)

option(C6502_BUILD_TEST "Build target for test executable" ON)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
endif()

set(TARGET_LIB ${PROJECT_NAME})

set(LIB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE LIB_C_FILES ${LIB_SRC_DIR}/*.c)
file(GLOB_RECURSE LIB_H_FILES ${LIB_INC_DIR}/*.h)

if(C6502_BUILD_TEST)
  set(TARGET_TEST ${PROJECT_NAME}_test)

  set(TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/src")
  set(TEST_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/include")
  file(GLOB_RECURSE TEST_C_FILES ${TEST_SRC_DIR}/*.c)
  file(GLOB_RECURSE TEST_H_FILES ${TEST_INC_DIR}/*.h)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

add_library(${PROJECT_NAME} STATIC ${LIB_C_FILES} ${LIB_H_FILES})

target_include_directories(${TARGET_LIB} PUBLIC ${LIB_INC_DIR}) 

set_target_properties(${TARGET_LIB} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${TARGET_LIB} PROPERTIES LINKER_LANGUAGE C)
set_target_properties(${TARGET_LIB} PROPERTIES C_STANDARD 11)

if(C6502_BUILD_TEST)
  add_executable(${TARGET_TEST} ${TEST_C_FILES} ${TEST_H_FILES})

  target_include_directories(${TARGET_TEST} PRIVATE "${TEST_INC_DIR};${LIB_INC_DIR}")

  target_link_libraries(${TARGET_TEST} ${TARGET_LIB})

  set_target_properties(${TARGET_TEST} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  set_target_properties(${TARGET_TEST} PROPERTIES LINKER_LANGUAGE C)
  set_target_properties(${TARGET_TEST} PROPERTIES C_STANDARD 11)
endif()

if(UNIX)
  install(TARGETS ${TARGET_LIB}
          ARCHIVE
          DESTINATION "/usr/lib")
  install(DIRECTORY "${LIB_INC_DIR}/"
          DESTINATION "/usr/include"
          PATTERN "${LIB_INC_DIR}/*")
endif()

if(C6502_BUILD_TEST)
  enable_testing()
  add_test(NAME "cputest" COMMAND $<TARGET_FILE:${TARGET_TEST}> "${CMAKE_CURRENT_SOURCE_DIR}/test/res")
endif()

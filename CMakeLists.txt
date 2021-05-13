cmake_minimum_required(VERSION 3.0)
project(bokket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++17 -Wall -Wno-deprecated -Werror -Wno-reorder -Wno-unused-function -Wno-unused-variable")

include_directories(./bokket/include)
link_directories(./bokket/lib)

set(LIB_SRC ./bokket/Log/Log.cpp
            ./bokket/thread/util.cpp)

add_library(bokket SHARED ${LIB_SRC})

add_executable(test test/test_log.cpp)
add_dependencies(test bokket)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


#add_executable(../test/logtest test/LogStream_test.cpp Log/LogStream.cpp )
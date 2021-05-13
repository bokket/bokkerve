cmake_minimum_required(VERSION 3.0)
project(bokket)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS
		-O2
		-g
		-Wall
		-Wextra
		-Werror
		-std=c++17
		-pthread
		)
#set(CMAKE_CXX_FLAGS
#	-rdynamic
#	-O0
#	-ggdb
#	-std=c++17
#	-Wall
#	-Werror
#	-Wno-reorder
#	-Wno-deprecated
#		-Wno-sign-compare
#	-Wno-unused-function
#	-Wno-unused-variable -pthread)

#include_directories(./bokket/include)
#link_directories(./bokket/lib)

set(LIB_SRC ./bokket/Log/Log.cpp
            ./bokket/thread/util.cpp
	    ./bokket/Log/FileWriterType.cpp
		./bokket/Log/LogStream.cpp
		./bokket/Log/FixedBuffer.h)

add_library(bokket SHARED ${LIB_SRC})

add_executable(test_Log ./test/test_log.cpp)
add_dependencies(test_Log bokket)
#target_link_libraries(test bokket)

add_executable(test_LogStream ./test/test_LogStream.cpp)
add_dependencies(test_LogStream bokket)
#target_link_libraries(test_LogStream bokket)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


#add_executable(../test/logtest test/LogStream_test.cpp Log/LogStream.cpp )

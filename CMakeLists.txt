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

include_directories(.)

find_library(YAMLCPP yaml-cpp)
message("****", ${YAMLCPP})
#link_libraries(-lyaml-cpp)

set(LIB_SRC
		./bokket/Log/Log.cpp
		./bokket/Log/FileWriterType.cpp
		./bokket/Log/LogStream.cpp
		./bokket/Config/config.cpp
		./bokket/thread/util.cpp
		#./bokket/thread/thread.cpp
		#./bokket/thread/CurrentThread.cpp
		)

set(LIBS
		bokket
		pthread
		yaml-cpp)


add_library(bokket SHARED ${LIB_SRC})

add_executable(test_Log ./test/test_log.cpp)
add_dependencies(test_Log bokket)
target_link_libraries(test_Log ${LIBS})

add_executable(test_LogStream ./test/test_LogStream.cpp)
add_dependencies(test_LogStream bokket)
target_link_libraries(test_LogStream ${LIBS})

add_executable(test_Yaml ./test/test_yaml.cpp)
add_dependencies(test_Yaml bokket)
target_link_libraries(test_Yaml bokket ${LIBS})

add_executable(test_Config ./test/test_config.cpp)
add_dependencies(test_Config bokket)
target_link_libraries(test_Config bokket ${LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


#add_executable(../test/logtest test/LogStream_test.cpp Log/LogStream.cpp )

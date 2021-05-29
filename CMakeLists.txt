cmake_minimum_required(VERSION 3.0)
project(bokket)


set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS
		-O2
		-g
		-ggdb
		-rdynamic
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

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_library(YAMLCPP yaml-cpp)
#message("****", ${YAMLCPP})

find_library(PTHREAD pthread)

find_library(GTEST gtest)
#message("****", ${PTHREAD})
#link_libraries(-lyaml-cpp)


set(LIBS
		bokket
		pthread
		yaml-cpp
		gtest)

message("***", ${LIBS})


set(LIB_SRC
		bokket/Log/Log.cpp
		bokket/Log/Log.h
		bokket/Log/LogFile.cpp
		#bokket/Log/AsyncFileLogAppender.h
		#bokket/Log/AsyncFileLogAppender.cpp
		#bokket/Log/LogStream.cpp
		bokket/thread/util.h
		bokket/thread/util.cpp
		bokket/thread/thread.h
		bokket/thread/thread.cpp
		bokket/thread/Assert.h
		bokket/Log/FixedBuffer.h
		bokket/Config/config.h
		bokket/Config/config.cpp

		)

add_library(bokket SHARED ${LIB_SRC})


add_executable(test_Log test/test_log.cpp)
add_dependencies(test_Log bokket)
#force_redefine_file_macro_for_sources(test_log)
target_link_libraries(test_Log ${LIBS})

#add_executable(test_LogStream ./test/test_LogStream.cpp)
#add_dependencies(test_LogStream bokket)
#force_redefine_file_macro_for_sources(test_LogStream)
#target_link_libraries(test_LogStream ${LIBS})

#add_executable(test_Yaml ./test/test_yaml.cpp)
#add_dependencies(test_Yaml bokket)
#force_redefine_file_macro_for_sources(test_Yaml)
#target_link_libraries(test_Yaml ${LIBS})

add_executable(test_Config ./test/test_config.cpp)
add_dependencies(test_Config bokket)
target_link_libraries(test_Config ${LIBS})

add_executable(test_thread ./test/test_thread.cpp)
add_dependencies(test_thread bokket)
#force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread ${LIBS})


add_executable(test_Util ./test/test_util.cpp)
add_dependencies(test_Util bokket)
target_link_libraries(test_Util ${LIBS})


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


#add_executable(../test/logtest test/LogStream_test.cpp Log/LogStream.cpp )

cmake_minimum_required(VERSION 3.0)
project(bokket)

#include(cmake/utils.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS
		-O2
		-g2
		-ggdb
		-rdynamic
		-Wall
		-Wextra
		-Werror
		-std=c++17
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



set(LIB_SRC
		bokket/Log/Log.cpp
		#bokket/Log/Log.h
		bokket/Log/LogLevel.h
		bokket/Log/FixedBuffer.h
		bokket/Log/LogFile.cpp
		#bokket/Log/AsyncFileLogAppender.h
		bokket/Log/AsyncFileLogAppender.cpp
		#bokket/Log/LogStream.cpp
		#bokket/thread/util.h
		bokket/thread/util.cpp
		#bokket/thread/thread.h
		bokket/thread/thread.cpp
		bokket/thread/Assert.h
		#bokket/Config/config.h
		bokket/Config/config.cpp


		#bokket/address/address.h
		bokket/address/address.cpp
		bokket/address/endian.h

		#bokket/Fiber/fiber.h
		bokket/Fiber/fiber.cpp

		bokket/time/timer.cpp

		bokket/Scheduler/scheduler.cpp
		#bokket/Scheduler/scheduler.h
		bokket/iomanager/iocontext.cpp
		bokket/iomanager/iomanager.cpp

		)

add_library(bokket SHARED ${LIB_SRC})


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
		dl
		gtest)

message("***", ${LIBS})

#if(BUILD_TEST)
#	bokket_add_executable(test_Log "test/test_log.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_AsyncLog "test/test_AsyncFileLog.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_Config "test/test_config.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_Thread "test/test_thread.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_Util "test/test_util.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_Address "test/test_Address.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_Fiber "test/test_Fiber.cpp" bokket "${LIBS}")
#	bokket_add_executable(test_Scheduler "test/test_Scheduler.cpp" bokket "${LIBS}")
#endif()



add_executable(test_Log ./test/test_log.cpp)
add_dependencies(test_Log bokket)
target_link_libraries(test_Log ${LIBS})

add_executable(test_AsyncLog ./test/test_AsyncFileLog.cpp)
add_dependencies(test_AsyncLog bokket)
target_link_libraries(test_AsyncLog ${LIBS})

#add_executable(test_LogStream /test/test_LogStream.cpp)
#add_dependencies(test_LogStream bokket)
#target_link_libraries(test_LogStream ${LIBS})

#add_executable(test_Yaml /test/test_yaml.cpp)
#add_dependencies(test_Yaml bokket)
#target_link_libraries(test_Yaml ${LIBS})

add_executable(test_Config ./test/test_config.cpp)
add_dependencies(test_Config bokket)
target_link_libraries(test_Config ${LIBS})

add_executable(test_thread ./test/test_thread.cpp)
add_dependencies(test_thread bokket)
target_link_libraries(test_thread ${LIBS})


add_executable(test_Util ./test/test_util.cpp)
add_dependencies(test_Util bokket)
target_link_libraries(test_Util ${LIBS})

add_executable(test_Address ./test/test_Address.cpp)
add_dependencies(test_Address bokket)
target_link_libraries(test_Address ${LIBS})


add_executable(test_Fiber ./test/test_Fiber.cpp)
add_dependencies(test_Fiber bokket)
target_link_libraries(test_Fiber ${LIBS})


add_executable(test_Scheduler ./test/test_Scheduler.cpp)
add_dependencies(test_Scheduler bokket)
target_link_libraries(test_Scheduler ${LIBS})


add_executable(test_Timer ./test/test_Timer.cpp)
add_dependencies(test_Timer bokket)
target_link_libraries(test_Timer ${LIBS})

#add_executable(test_Ucontext ./test/test_ucontext.cpp)
#add_dependencies(test_Ucontext bokket)
#target_link_libraries(test_Ucontext ${LIBS})

add_executable(test_IOManager ./test/test_iomanager.cpp)
add_dependencies(test_IOManager bokket)
target_link_libraries(test_IOManager ${LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)



#add_executable(../test/logtest test/LogStream_test.cpp Log/LogStream.cpp )

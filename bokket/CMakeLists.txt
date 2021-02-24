cmake_minimum_required(VERSION 2.8)
project(Logtest)


include_directories(Log)

add_executable(../test/logtest test/LogStream_test.cpp Log/LogStream.cpp )
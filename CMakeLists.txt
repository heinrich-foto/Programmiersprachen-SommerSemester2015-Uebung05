cmake_minimum_required (VERSION 2.8) 
project(Aufgabenblatt5) 

set(CMAKE_CXX_FLAGS "-std=c++0x")

add_executable(testList
				testList.cpp
				catch.hpp
)

add_custom_command(
	OUTPUT test.txt
	COMMAND testList
	VERBATIM
)

add_custom_target(
  run_test ALL
  DEPENDS test.txt
)
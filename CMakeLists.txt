cmake_minimum_required(VERSION 3.0.0)
project(portal32playermod_plugin VERSION 0.1.0)

FILE(GLOB cppSrc *.cpp)
add_library(32pmod SHARED ${cppSrc})
target_include_directories(32pmod PRIVATE .)
add_custom_command(TARGET 32pmod POST_BUILD
	COMMAND strip $<TARGET_FILE:32pmod>
	COMMENT "Stripped $<TARGET_FILE:32pmod>"
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:32pmod> ${CMAKE_SOURCE_DIR}/32pmod.so
	COMMENT "Copied binary to ${CMAKE_SOURCE_DIR}/32pmod.so"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC -Wall -Werror -O2")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -m32 -fPIC -Wall -Werror -O2")
SET(CMAKE_SHARED_LINKER_FLAGS "-m32")


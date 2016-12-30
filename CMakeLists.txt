cmake_minimum_required(VERSION 3.0)
project(poll-windows)

if(NOT WIN32)
    message(FATAL_ERROR "This library is intended to be compiled only on Windows, sorry")
endif()


set(POLL_FILES src/poll.cpp
               src/poll.h)
add_library(poll STATIC ${POLL_FILES})
target_link_libraries(poll ntdll)
target_compile_definitions(poll PUBLIC "-D_WIN32_WINNT=0x600")

add_executable(connect-fail examples/connect-fail.cpp)
target_link_libraries(connect-fail poll ws2_32)
target_include_directories(connect-fail PRIVATE src)

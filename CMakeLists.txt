cmake_minimum_required(VERSION 3.6)
project(csci4430_asg1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES_SERVER server.c)
add_executable(csci4430_asg1_server ${SOURCE_FILES_SERVER})
set(SOURCE_FILES_CLIENT client.c client.c)
add_executable(csci4430_asg1_client ${SOURCE_FILES_CLIENT})

set(SOURCE_FILES_TCP_SERVER tcp_sample/server.c)
set(SOURCE_FILES_TCP_CLIENT tcp_sample/client.c)

add_executable(tcp_sample_server ${SOURCE_FILES_TCP_SERVER})
add_executable(tcp_sample_client ${SOURCE_FILES_TCP_CLIENT})
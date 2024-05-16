cmake_minimum_required(VERSION 3.21)
project(ProjetoRC C)

set(CMAKE_C_STANDARD 11)

add_executable(ProjetoRC udp_server.c header.h header.c)

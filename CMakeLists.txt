cmake_minimum_required(VERSION 3.22)

project(
	HTTP-Server
	VERSION "0.0.1"
	DESCRIPTION "simple http server written in C++"
	LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)


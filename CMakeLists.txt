cmake_minimum_required(VERSION 3.19)
project(c_asserts C)
add_library(c_asserts_obj OBJECT c_asserts.c c_asserts.h)
target_include_directories(c_asserts_obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
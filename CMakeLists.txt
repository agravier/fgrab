cmake_minimum_required(VERSION 2.8)
project(fgrab)

find_package(qibuild)

# Library source files
set(_srcs fgrab.cpp fgrab.h main.cpp)

# Build the libfgrab library in lib/naoqi
qi_create_lib(fgrab SHARED ${_srcs} SUBFOLDER naoqi)
qi_use_lib(fgrab ALCOMMON ALVISION BOOST)
qi_stage_lib(fgrab)

# Create a executable named call_fgrab using call_fgrab.cpp
qi_create_bin(call_fgrab call_fgrab.cpp)
qi_use_lib(call_fgrab ALCOMMON)

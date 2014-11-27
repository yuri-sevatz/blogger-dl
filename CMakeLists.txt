cmake_minimum_required(VERSION 2.8.8)

project(prj)

include_directories(./)

OPTION(SUBMODULES "Build Submodules?" OFF)
IF(SUBMODULES)
	add_subdirectory(libtwirl)
ENDIF(SUBMODULES)

add_subdirectory(libblogger)
add_subdirectory(blogger-dl)

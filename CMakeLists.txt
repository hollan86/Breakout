cmake_minimum_required(VERSION 3.5)
project(Hech)
set (CMAKE_CXX_STANDARD 14)


##LINKING SHARED LIBRARIES
#glfw library
set (GLFW_LINK_LIBS libglfw.3.2.dylib)

set(SOIL libSOIL.a)

set(SOIL_LIB libSOIL.1.07.dylib)


link_directories(
        /usr/local/Cellar/glfw/3.2.1/lib/
		/usr/local/lib/
)

#ADDING INCLUDE DIRECTORIES
include_directories(
        include
		vendor
		vendor/soil
		usr/local/Cellar/glfw/3.2.1/include	
)

file(GLOB SOURCES "src/*" )

add_executable(Breakout ${SOURCES})

target_link_libraries(Breakout ${GLFW_LINK_LIBS} ${CMAKE_DL_LIBS} ${SOIL_LIB})

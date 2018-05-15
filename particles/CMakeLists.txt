cmake_minimum_required(VERSION 3.5.1)
project(opengl_2)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


### GLFW ###

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_subdirectory (external/glfw-3.2.1)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(
		external/glfw-3.1.2/include/GLFW/
		external/glew-1.13.0/include/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(OPENGL_LIBRARY
			${OPENGL_LIBRARY}
			-lGL -lGLU -lXrandr -lXext -lX11 -lrt
			${CMAKE_DL_LIBS}
			${GLFW_LIBRARIES}
			)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(OPENGL_LIBRARY
			${OPENGL_LIBRARY}
			${CMAKE_DL_LIBS}
			${GLFW_LIBRARIES}
			)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
###

### GLEW ###

set(GLEW_SOURCE
		external/glew-1.13.0/src/glew.c
		)

add_library( GLEW_1130 STATIC
		${GLEW_SOURCE}
		${GLEW_INCLUDE}
		)

target_link_libraries(GLEW_1130
		${OPENGL_LIBRARY}
		${EXTRA_LIBS}
		)
###

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(
	external/glfw-3.2.1/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
)
set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLEW_1130
	glfw
)


set(SMOKE_HEADERS
	common/controls.hpp
	common/shader.hpp
        common/texture.hpp)

set(SMOKE_SOURCES
        smoke/src/main_particles.cpp
	common/controls.cpp
	common/shader.cpp
        common/texture.cpp)


add_executable(smoke
		${SMOKE_HEADERS}
		${SMOKE_SOURCES})

target_link_libraries(smoke
		${ALL_LIBS}
		)

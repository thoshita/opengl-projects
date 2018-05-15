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

set(TRIANGLE_HEADERS
        commons/shader.h
        )

set(TRIANGLE_SOURCES
        triangle/src/main.cpp
        commons/shader.cpp)

set(CAR_HEADERS
        commons/shader.h
        commons/drawer.h)

set(CAR_SOURCES
        car/src/main.cpp
        commons/shader.cpp
        commons/drawer.cpp)

set(KIJANG_HEADERS
		commons/shader.h
		commons/drawer.h)

set(KIJANG_SOURCES
		kijang/src/main.cpp
		commons/shader.cpp
		commons/drawer.cpp)

set(TEXTURE_HEADERS
		commons/shader.h
		commons/drawer.h
        commons/texture.h)

set(TEXTURE_SOURCES
        texture/src/_main.cpp
		commons/shader.cpp
		commons/drawer.cpp
        commons/texture.cpp)

set(UAS_HEADERS
		commons/shader.h
		commons/drawer.h
        commons/texture.h
		commons/objloader.h)

set(UAS_SOURCES
        uas/src/main_uas.cpp
		commons/shader.cpp
		commons/drawer.cpp
        commons/texture.cpp
		commons/objloader.cpp)

add_executable(triangle
        ${TRIANGLE_HEADERS}
        ${TRIANGLE_SOURCES}
        )

add_executable(car
        ${CAR_HEADERS}
        ${CAR_SOURCES})

add_executable(kijang
		${KIJANG_HEADERS}
		${KIJANG_SOURCES})

add_executable(texture
		${TEXTURE_HEADERS}
		${TEXTURE_SOURCES})

add_executable(uas
		${UAS_HEADERS}
		${UAS_SOURCES})

target_link_libraries(triangle
        ${ALL_LIBS}
        )

target_link_libraries(car
        ${ALL_LIBS}
        )

target_link_libraries(kijang
		${ALL_LIBS}
		)

target_link_libraries(texture
		${ALL_LIBS}
		)

target_link_libraries(uas
		${ALL_LIBS}
		)

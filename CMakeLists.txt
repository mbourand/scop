cmake_minimum_required(VERSION 3.18.2)

project(scop)

set(CMAKE_CXX_STANDARD 17)

# Sources and headers

add_executable(${PROJECT_NAME} src/EZGL/Math/mathutils.cpp src/EZGL/Math/Matrix.cpp src/EZGL/FlyController.cpp
	src/EZGL/KeyListener.cpp src/EZGL/Light.cpp src/EZGL/Material.cpp src/EZGL/Mesh.cpp src/EZGL/MouseListener.cpp
	src/EZGL/PerspectiveCamera.cpp src/EZGL/Shader.cpp src/EZGL/VertexBuffer.cpp src/EZGL/Window.cpp
	src/ObjParser/ObjParser.cpp src/ObjParser/Face.cpp src/BMPParser.cpp src/Logger.cpp src/main.cpp
	src/ModelController.cpp src/utils.cpp)

# Libs

add_subdirectory(external/GLFW)
add_subdirectory(external/GLEW)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW/include/ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/GLEW/include/ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLM)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} libglew_static opengl32 User32 Shell32 Gdi32)

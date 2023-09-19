cmake_minimum_required( VERSION 3.9 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project( Softgen )

find_package( OpenGL REQUIRED )
add_subdirectory( glfw )
add_subdirectory( assimp )

set( GLFW-CMAKE-STARTER-SRC
        main.cpp
        core/Render.cpp
        core/Render.h
        shader/ShaderParser.cpp
        shader/ShaderParser.h
        core/object/Object.h
        core/object/types/SceneObject.cpp
        core/object/types/SceneObject.h
        core/object/mesh/Mesh.h
        core/object/utility/Transform.cpp
        core/object/utility/Transform.h
        core/object/utility/stb_image.h
        core/object/utility/stb_image.cpp
        core/camera/SceneCamera.cpp
        core/camera/SceneCamera.h
        core/scene/Scene.cpp
        core/scene/Scene.h
)

add_executable( Softgen ${GLFW-CMAKE-STARTER-SRC} )
target_link_libraries(Softgen PRIVATE ${OPENGL_LIBRARIES} glfw)
target_link_libraries(Softgen PRIVATE assimp::assimp)

target_include_directories( Softgen PRIVATE ${OPENGL_INCLUDE_DIRS} )
target_include_directories( Softgen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glm)


add_definitions(-DGL_SILENCE_DEPRECATION)
cmake_minimum_required(VERSION 3.27)

add_subdirectory(raylib)
add_subdirectory(yaml-cpp)
add_subdirectory(msgpack-cpp)
add_subdirectory(csv-parser)
add_subdirectory(glm)

set_target_properties(
    raylib
    yaml-cpp
    glfw
    msgpack-cxx-headers
    csv
    glm
    PROPERTIES FOLDER "ThirdParty"
)
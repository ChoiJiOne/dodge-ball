cmake_minimum_required(VERSION 3.27)

add_subdirectory(DataPack)
add_subdirectory(DataPackGenerator)

set_target_properties(
    DataPack
    DataPackGenerator
    PROPERTIES FOLDER "DataPack"
)
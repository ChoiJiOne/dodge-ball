cmake_minimum_required(VERSION 3.27)

add_subdirectory(DataPack)
add_subdirectory(DataChunkGenerator)

set_target_properties(
    DataPack
    DataChunkGenerator
    PROPERTIES FOLDER "DataPack"
)
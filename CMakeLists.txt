cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME "DodgeBall")
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC) # Visual Studio에서 표시되는 Code Page 949 경고 비활성화
    add_compile_options(/utf-8 /wd4819 /wd4828)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(ThirdParty)
add_subdirectory(DataPack)
add_subdirectory(Framework)
add_subdirectory(Application)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT App)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "v0.0.0")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}")

install(TARGETS App DESTINATION .)

include(CPack)
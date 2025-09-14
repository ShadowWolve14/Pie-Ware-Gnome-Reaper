cmake_minimum_required(VERSION 3.11)

include(../cmake/raylib.cmake)
include(../cmake/raylib_cpp.cmake)
include(../cmake/nlohmann_json.cmake)

project(game VERSION 0.1.0 LANGUAGES CXX)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

include(../cmake/doxygen.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(core)
add_subdirectory(scenes)

add_executable(${PROJECT_NAME} main.cpp
        Vectors.h
        game/PlayerBaseClass.cpp
        game/HUD.h
        game/HUD.cpp
)

# Add config.h.in file
configure_file(config.h.in config.h)

target_include_directories(${PROJECT_NAME} PUBLIC
"${PROJECT_BINARY_DIR}"
)

# Link to libraries
target_link_libraries(${PROJECT_NAME} core scenes nlohmann_json::nlohmann_json)
set(FINAL_FOLDER_NAME "${CMAKE_BINARY_DIR}/Gnome_Reaper_Spiel")

add_custom_command(
        TARGET game POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E make_directory ${FINAL_FOLDER_NAME}

        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game> ${FINAL_FOLDER_NAME}

        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets ${FINAL_FOLDER_NAME}/assets

        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/PieWare ${FINAL_FOLDER_NAME}/PieWare

        COMMENT "Erstelle finalen Spiel-Ordner unter ${FINAL_FOLDER_NAME}"
)
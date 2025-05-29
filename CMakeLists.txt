cmake_minimum_required(VERSION 3.30)
project(Team_project_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(Team_project_cpp
        main.cpp
        playlist_functions.cpp
        playlist.h)

if(WIN32)
    add_custom_command(TARGET Team_project_cpp POST_BUILD
            COMMAND start cmd /c "${CMAKE_BINARY_DIR}/Team_project_cpp.exe"
    )
elseif(UNIX AND NOT APPLE)
    add_custom_command(TARGET Team_project_cpp POST_BUILD
            COMMAND gnome-terminal -- .${CMAKE_BINARY_DIR}/Team_project_cpp
    )
elseif(APPLE)
    add_custom_command(TARGET Team_project_cpp POST_BUILD
            COMMAND open -a Terminal ${CMAKE_BINARY_DIR}/Team_project_cpp
    )
endif()
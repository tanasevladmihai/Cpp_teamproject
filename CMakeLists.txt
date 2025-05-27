cmake_minimum_required(VERSION 3.30)
project(Final_project_program C)

set(CMAKE_C_STANDARD 11)

add_executable(Final_project_program main.c
        playlist_functions.c
        playlist.h)

if(WIN32)
    add_custom_command(TARGET Final_project_program POST_BUILD
            COMMAND start cmd /c "${CMAKE_BINARY_DIR}/Final_project_program.exe"
    )
elseif(UNIX AND NOT APPLE)
    add_custom_command(TARGET Final_project_program POST_BUILD
            COMMAND gnome-terminal -- .${CMAKE_BINARY_DIR}/Final_project_program
    )
elseif(APPLE)
    add_custom_command(TARGET Final_project_program POST_BUILD
            COMMAND open -a Terminal ${CMAKE_BINARY_DIR}/Final_project_program
    )
endif()
cmake_minimum_required(VERSION 3.11)

project(sdl-sprite)

add_executable(${PROJECT_NAME} main.c)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
)

if(PSP)
    # Create an EBOOT.PBP file
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
        VERSION 01.00
    )
endif()
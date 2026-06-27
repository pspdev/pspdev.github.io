cmake_minimum_required(VERSION 3.14)

project(
  jokr
  VERSION 0.1.0
  DESCRIPTION "PSP cURL Jokes app"
  LANGUAGES CXX
)

add_executable(${PROJECT_NAME} main.cpp network.hpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (PSP)

  set(DEFAULT_STUBS
    pspdebug 
    pspdisplay 
    pspge 
    pspctrl 
    pspnet 
    pspnet_apctl
  )

  set(LIBS
    stdc++
    SDL2main
    SDL2
    SDL2_ttf
    SDL2_gfx
    GL
    freetype
    pspvram
    pspvfpu
    pspgu
    pspgum
    psphprm
    pspaudio
    bz2
    psphttp
    curl
    mbedtls
    mbedcrypto
    mbedx509
    png
    z
    harfbuzz
  )

else()

  set(LIBS
    stdc++
    curl
    SDL2
    SDL2_ttf
  )

endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS} ${DEFAULT_STUBS})

if (PSP)

  set(BUILD_PRX 1)
  create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_DESCRIPTION}
  )

  add_custom_target( 
    RUN_PSP
    DEPENDS ${PROJECT_NAME}
    COMMAND PPSSPPSDL ./EBOOT.PBP
  )
else()
  message(STATUS "Building native version")

  find_package(CURL REQUIRED)
  find_package(SDL2 REQUIRED)
  find_package(SDL_ttf REQUIRED)
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E create_symlink
  ${CMAKE_CURRENT_SOURCE_DIR}/res
  ${CMAKE_CURRENT_BINARY_DIR}/res
)

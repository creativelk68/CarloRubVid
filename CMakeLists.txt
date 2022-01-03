cmake_minimum_required(VERSION 3.0)

project("CarloRubVid")


set(CARLORUBVID_SRCS
    "${CMAKE_SOURCE_DIR}/carlorubvid.c++"
    "${CMAKE_SOURCE_DIR}/src/init.c++"
    "${CMAKE_SOURCE_DIR}/src/loop.c++"
    "${CMAKE_SOURCE_DIR}/src/optional_loop.c++"
    "${CMAKE_SOURCE_DIR}/src/loader.c++"
    "${CMAKE_SOURCE_DIR}/src/tabbar.c++"
    "${CMAKE_SOURCE_DIR}/src/editing.c++"
    "${CMAKE_SOURCE_DIR}/src/button.c++"
)

add_executable(carlorubvid ${CARLORUBVID_SRCS} ${WernerGamesLibary})

target_include_directories(carlorubvid PUBLIC include)

set_target_properties(carlorubvid PROPERTIES COMPIPLE_FLAGS "-Wno-psabi")

target_link_libraries(carlorubvid sfml-system sfml-window sfml-graphics sfml-audio)
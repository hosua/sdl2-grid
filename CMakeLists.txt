# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project information
project(grid-game LANGUAGES CXX)

# Set C++ standard to C++17 (or other version if desired)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
set(SOURCES
	src/app.cc
	src/color.cc
	src/font.cc
    src/main.cc
	src/scene.cc
	src/scene_manager.cc
	src/scenes/game.cc
	src/scenes/main_menu.cc
	src/scenes/test.cc
	src/pathfinder/dfs.cc
	src/pathfinder/bfs.cc
	src/pathfinder/astar.cc
	src/pathfinder/world.cc
	src/ui/widget_manager.cc
	src/ui/widget.cc
	src/ui/text.cc
	src/ui/button.cc
	src/ui/spinner.cc
)

# Define include directories
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scenes
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pathfinder
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# Add the executable
add_executable(grid-game ${SOURCES})

# Include directories
target_include_directories(grid-game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link required SDL2 libraries
target_link_libraries(grid-game PRIVATE
    SDL2
    SDL2_ttf
    SDL2_mixer
    SDL2_image
)

# Optionally, set some warning flags (these can be adjusted as needed)
target_compile_options(grid-game PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Add debugging flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set (symlink_src "${CMAKE_SOURCE_DIR}/assets")
set (symlink_dest "${CMAKE_CURRENT_BINARY_DIR}/assets")
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${symlink_src} ${symlink_dest}
	DEPENDS ${symlink_dest}
	COMMENT "Creates a symbolic link to the assets folder"
)


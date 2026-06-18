cmake_minimum_required(VERSION 3.10)
project(SE_TicTacToe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set source files
set(SOURCES
    src/main.cpp
    src/Model/Board.cpp
    src/Model/Player.cpp
    src/Model/GameState.cpp
    src/View/ConsoleView.cpp
    src/Controller/GameController.cpp
    src/Controller/AIPlayer.cpp
)

set(HEADERS
    include/Model/Board.h
    include/Model/Player.h
    include/Model/GameState.h
    include/View/ConsoleView.h
    include/Controller/GameController.h
    include/Controller/AIPlayer.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set output binary name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "TicTacToe"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# MSVC-specific settings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)
endif()

# Enable warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

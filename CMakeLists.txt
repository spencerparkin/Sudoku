# CMakeLists.txt for Sudoku app

cmake_minimum_required(VERSION 3.27)

project(Sudoku)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(wxBUILD_SHARED OFF)

add_subdirectory(App)
add_subdirectory(ThirdParty/FontSystem)
add_subdirectory(ThirdParty/HappyMath)
add_subdirectory(ThirdParty/UltraUtilities)
add_subdirectory(ThirdParty/wxWidgets)
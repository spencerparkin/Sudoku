# CMakeLists.txt for Sudoku wxWidgets-based app

set(SUDOKU_APP_SOURCES
	Source/App.cpp
	Source/App.h
	Source/Frame.cpp
	Source/Frame.h
	Source/Canvas.cpp
	Source/Canvas.h
	Source/SudokuSquare.cpp
	Source/SudokuSquare.h
	Source/SudokuSolver.cpp
	Source/SudokuSolver.h
	Source/Random.cpp
	Source/Random.h
)

add_executable(Sudoku WIN32
	${SUDOKU_APP_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/wx_dpi_aware_pmv2.manifest
)

target_compile_definitions(Sudoku PUBLIC
	__WXMSW__
	_UNICODE
)

target_compile_definitions(Sudoku PRIVATE
	_USE_MATH_DEFINES
)

target_link_libraries(Sudoku
	HappyMath
	UltraUtilities
	FontSystem
	wx::core
	wx::base
	wx::gl
)
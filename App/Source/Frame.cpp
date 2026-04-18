#include "Frame.h"

SudokuFrame::SudokuFrame(const wxPoint& position, const wxSize& size) : wxFrame(nullptr, wxID_ANY, wxT("Sudoku"), position, size)
{
}

/*virtual*/ SudokuFrame::~SudokuFrame()
{
}
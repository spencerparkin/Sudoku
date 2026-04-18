#include "Frame.h"
#include "Canvas.h"
#include <wx/sizer.h>
#include <wx/menu.h>

SudokuFrame::SudokuFrame(const wxPoint& position, const wxSize& size) : wxFrame(nullptr, wxID_ANY, wxT("Sudoku"), position, size)
{
	wxMenu* gameMenu = new wxMenu();
	gameMenu->Append(new wxMenuItem(gameMenu, ID_NewPuzzle, wxT("New Puzzle"), wxT("Generate a new puzzle at the desired difficulty level.")));
	gameMenu->AppendSeparator();
	gameMenu->Append(new wxMenuItem(gameMenu, ID_Exit, wxT("Exit"), wxT("Go do something else with your life.")));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(new wxMenuItem(helpMenu, ID_About, wxT("About"), wxT("Show the about box.")));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(gameMenu, wxT("Game"));
	menuBar->Append(helpMenu, wxT("Help"));
	this->SetMenuBar(menuBar);

	wxStatusBar* statusBar = new wxStatusBar(this);
	this->SetStatusBar(statusBar);

	this->canvas = new SudokuCanvas(this);

	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(this->canvas, 1, wxALL | wxGROW, 0);
	this->SetSizer(boxSizer);

	this->Bind(wxEVT_MENU, &SudokuFrame::OnNewPuzzle, this, ID_NewPuzzle);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnAbout, this, ID_About);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnExit, this, ID_Exit);
}

/*virtual*/ SudokuFrame::~SudokuFrame()
{
}

void SudokuFrame::OnNewPuzzle(wxCommandEvent& event)
{
}

void SudokuFrame::OnAbout(wxCommandEvent& event)
{
}

void SudokuFrame::OnExit(wxCommandEvent& event)
{
	this->Close(true);
}
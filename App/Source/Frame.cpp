#include "Frame.h"
#include "Canvas.h"
#include "App.h"
#include "SudokuSquare.h"
#include "SudokuSolver.h"
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/choicdlg.h>
#include <wx/aboutdlg.h>
#include <memory>

SudokuFrame::SudokuFrame(const wxPoint& position, const wxSize& size) : wxFrame(nullptr, wxID_ANY, wxT("Sudoku"), position, size), timer(this, ID_Timer)
{
	wxMenu* gameMenu = new wxMenu();
	gameMenu->Append(new wxMenuItem(gameMenu, ID_NewPuzzle, wxT("New Puzzle"), wxT("Generate a new puzzle at the desired difficulty level.")));
	gameMenu->Append(new wxMenuItem(gameMenu, ID_SolvePuzzle, wxT("Solve Puzzle"), wxT("Let the computer try to solve the puzzle.")));
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
	this->Bind(wxEVT_MENU, &SudokuFrame::OnSolvePuzzle, this, ID_SolvePuzzle);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnAbout, this, ID_About);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnExit, this, ID_Exit);
	this->Bind(wxEVT_TIMER, &SudokuFrame::OnTimer, this, ID_Timer);

	this->timer.Start(16);
}

/*virtual*/ SudokuFrame::~SudokuFrame()
{
}

void SudokuFrame::OnNewPuzzle(wxCommandEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return;

	wxArrayString choiceArray;
	choiceArray.Add("Easy");
	choiceArray.Add("Medium");
	choiceArray.Add("Hard");

	wxSingleChoiceDialog choiceDialog(this, wxT("Please select difficulty level."), wxT("Difficulty Level"), choiceArray);
	if (choiceDialog.ShowModal() != wxID_OK)
		return;

	std::unique_ptr<SudokuSolver> solver;
	int puzzleSizeLowerBound = 0;

	int choice = choiceDialog.GetSelection();
	
	switch (choice)
	{
	case 0:
		solver.reset(new SimpleSudokuSolver());
		puzzleSizeLowerBound = 45;
		break;
	case 1:
		solver.reset(new SimpleSudokuSolver());
		puzzleSizeLowerBound = 0;
		break;
	case 2:
		solver.reset(new AdvancedSudokuSolver());
		puzzleSizeLowerBound = 0;
		break;
	}

	square->MakePuzzle(*wxGetApp().GetRandom(), solver.get(), puzzleSizeLowerBound);

	wxGetApp().SetOriginalSquare();
}

void SudokuFrame::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Sudoku"));
	info.SetVersion(wxT("1.0.0"));
	info.SetDescription(wxT("This is a basic Sudoku puzzle application."));
	info.SetCopyright("(C) 2026 Spencer T. Parkin <spencer.parkin@proton.me>");

	wxAboutBox(info);
}

void SudokuFrame::SetStatusText(const wxString& text)
{
	this->GetStatusBar()->SetLabelText(text);
}

void SudokuFrame::OnExit(wxCommandEvent& event)
{
	this->Close(true);
}

void SudokuFrame::OnTimer(wxTimerEvent& event)
{
	this->canvas->Refresh();
}

void SudokuFrame::OnSolvePuzzle(wxCommandEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return;

	square->Copy(wxGetApp().GetOriginalSquare());

	AdvancedSudokuSolver solver;
	bool solved = solver.Solve(square);
	if (!solved)
	{
		wxMessageBox(wxT("The computer failed to solve the puzzle!  Hmmm...something went wrong."), wxT("Error!"), wxOK | wxICON_ERROR, this);
	}
}
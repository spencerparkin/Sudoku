#include "Frame.h"
#include "Canvas.h"
#include "App.h"
#include "SudokuSquare.h"
#include "SudokuSolver.h"
#include "SudokuValueRemover.h"
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/choicdlg.h>
#include <wx/aboutdlg.h>
#include <memory>
#include <math.h>

SudokuFrame::SudokuFrame(const wxPoint& position, const wxSize& size) : wxFrame(nullptr, wxID_ANY, wxT("Sudoku"), position, size), timer(this, ID_Timer)
{
	wxMenu* puzzleMenu = new wxMenu();
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_NewPuzzle, wxT("New Puzzle"), wxT("Generate a new puzzle at the desired difficulty level.")));
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_SolvePuzzle, wxT("Solve Puzzle"), wxT("Let the computer try to solve the puzzle.")));
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_ResetPuzzle, wxT("Reset Puzzle"), wxT("Remove all values from the square except for the original values defining the puzzle.")));
	puzzleMenu->AppendSeparator();
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_Exit, wxT("Exit"), wxT("Go do something else with your life.")));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(new wxMenuItem(helpMenu, ID_GiveHint, wxT("Give Hint"), wxT("Have the computer see if it can find a move.")));
	helpMenu->Append(new wxMenuItem(helpMenu, ID_ToggleShowPossibilities, wxT("Show Possibilities"), wxT("Show what possible values can go into the hover square in the status bar."), wxITEM_CHECK));
	helpMenu->AppendSeparator();
	helpMenu->Append(new wxMenuItem(helpMenu, ID_About, wxT("About"), wxT("Show the about box.")));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(puzzleMenu, wxT("Puzzle"));
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
	this->Bind(wxEVT_MENU, &SudokuFrame::OnResetPuzzle, this, ID_ResetPuzzle);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnAbout, this, ID_About);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnGiveHint, this, ID_GiveHint);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnExit, this, ID_Exit);
	this->Bind(wxEVT_MENU, &SudokuFrame::OnToggleShowPossibilities, this, ID_ToggleShowPossibilities);
	this->Bind(wxEVT_UPDATE_UI, &SudokuFrame::OnUpdateUI, this, ID_SolvePuzzle);
	this->Bind(wxEVT_UPDATE_UI, &SudokuFrame::OnUpdateUI, this, ID_ResetPuzzle);
	this->Bind(wxEVT_UPDATE_UI, &SudokuFrame::OnUpdateUI, this, ID_ToggleShowPossibilities);
	this->Bind(wxEVT_TIMER, &SudokuFrame::OnTimer, this, ID_Timer);

	this->timer.Start(16);
}

/*virtual*/ SudokuFrame::~SudokuFrame()
{
}

void SudokuFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();

	switch (event.GetId())
	{
		case ID_SolvePuzzle:
		{
			event.Enable(square && square->GetNumSetValues() != square->GetNumValues());
			break;
		}
		case ID_ResetPuzzle:
		{
			event.Enable(square && square->GetNumSetValues() != wxGetApp().GetOriginalSquare()->GetNumSetValues());
			break;
		}
		case ID_ToggleShowPossibilities:
		{
			event.Check(this->canvas->GetShowPossibilities());
			break;
		}
	}
}

void SudokuFrame::OnNewPuzzle(wxCommandEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return;

	wxArrayString difficultyChoiceArray;
	difficultyChoiceArray.Add("Easy");
	difficultyChoiceArray.Add("Medium");
	difficultyChoiceArray.Add("Hard");

	wxSingleChoiceDialog difficultyChoiceDialog(this, wxT("Please select difficulty level."), wxT("Difficulty Level"), difficultyChoiceArray);
	if (difficultyChoiceDialog.ShowModal() != wxID_OK)
		return;

	wxArrayString patternChoiceArray;
	patternChoiceArray.Add("Completely Random");
	patternChoiceArray.Add("Random, but 180-degree Symmetric");
	patternChoiceArray.Add("Random, but 90-degree Symmetric");

	wxSingleChoiceDialog patternChoiceDialog(this, wxT("Please select pattern."), wxT("Pattern"), patternChoiceArray);
	if (patternChoiceDialog.ShowModal() != wxID_OK)
		return;

	std::unique_ptr<SudokuSolver> solver;
	std::unique_ptr<SudokuValueRemover> remover;
	int puzzleSizeLowerBound = 0;

	int difficultyChoice = difficultyChoiceDialog.GetSelection();
	switch (difficultyChoice)
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

	int patternChoice = patternChoiceDialog.GetSelection();
	switch (patternChoice)
	{
	case 0:
		remover.reset(new RandomSingleValueRemover());
		break;
	case 1:
		remover.reset(new Random180DegreeSymmetryValueRemover());
		break;
	case 2:
		remover.reset(new Random90DegreeSymmetryValueRemover());
		break;
	}

	square->MakePuzzle(*wxGetApp().GetRandom(), solver.get(), remover.get(), puzzleSizeLowerBound);

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

void SudokuFrame::OnToggleShowPossibilities(wxCommandEvent& event)
{
	this->canvas->SetShowPossibilities(!this->canvas->GetShowPossibilities());
}

void SudokuFrame::OnGiveHint(wxCommandEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return;

	std::unique_ptr<SudokuSquare> squareCopy((SudokuSquare*)square->Clone());

	AdvancedSudokuSolver solver;
	int resolvedRow = -1;
	int resolvedCol = -1;
	if (!solver.ResolveAnyValue(squareCopy.get(), &resolvedRow, &resolvedCol))
		wxMessageBox(wxT("Failed to find move!"), wxT("Oops!"), wxOK | wxICON_ERROR, this);
	else
	{
		int resolvedValue = -1;
		squareCopy->GetValue(resolvedRow, resolvedCol, resolvedValue);
		wxString message = wxString::Format(wxT("Consider placing %d at row %d, column %d."), resolvedValue + 1, resolvedRow + 1, resolvedCol + 1);
		wxMessageBox(message, wxT("Hint!"), wxOK | wxICON_INFORMATION, this);
	}
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

	if (square->GetNumSetValues() == 0)
		return;

	AdvancedSudokuSolver solver;
	bool solved = solver.Solve(square);
	if (!solved)
	{
		wxMessageBox(wxT("The computer failed to solve the puzzle!  Hmmm...something went wrong."), wxT("Error!"), wxOK | wxICON_ERROR, this);
	}
}

void SudokuFrame::OnResetPuzzle(wxCommandEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return;

	square->Copy(wxGetApp().GetOriginalSquare());
}
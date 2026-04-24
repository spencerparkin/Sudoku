#pragma once

#include <wx/frame.h>
#include <wx/timer.h>

class SudokuCanvas;

/**
 * 
 */
class SudokuFrame : public wxFrame
{
public:
	SudokuFrame(const wxPoint& position, const wxSize& size);
	virtual ~SudokuFrame();

	enum
	{
		ID_NewPuzzle = wxID_HIGHEST,
		ID_SolvePuzzle,
		ID_ResetPuzzle,
		ID_Exit,
		ID_About,
		ID_GiveHint,
		ID_ToggleShowPossibilities,
		ID_Timer
	};

	void SetStatusText(const wxString& text);

private:
	void OnNewPuzzle(wxCommandEvent& event);
	void OnSolvePuzzle(wxCommandEvent& event);
	void OnResetPuzzle(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnGiveHint(wxCommandEvent& event);
	void OnToggleShowPossibilities(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);

	SudokuCanvas* canvas;
	wxTimer timer;
};
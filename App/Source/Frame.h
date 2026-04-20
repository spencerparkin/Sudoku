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
		ID_Exit,
		ID_About,
		ID_Timer
	};

	void SetStatusText(const wxString& text);

private:
	void OnNewPuzzle(wxCommandEvent& event);
	void OnSolvePuzzle(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);

	SudokuCanvas* canvas;
	wxTimer timer;
};
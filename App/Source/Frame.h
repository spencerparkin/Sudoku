#pragma once

#include <wx/frame.h>

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
		ID_Exit,
		ID_About
	};

private:
	void OnNewPuzzle(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	SudokuCanvas* canvas;
};
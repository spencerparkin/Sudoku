#pragma once

#include <wx/app.h>

class SudokuFrame;

/**
 * 
 */
class SudokuApp : public wxApp
{
public:
	SudokuApp();
	virtual ~SudokuApp();

	virtual bool OnInit() override;
	virtual int OnExit() override;

private:
	SudokuFrame* frame;
};

wxDECLARE_APP(SudokuApp);
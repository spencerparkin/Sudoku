#pragma once

#include <wx/app.h>
#include "SudokuSquare.h"
#include "Random.h"

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

	SudokuSquare* GetSquare();
	Random* GetRandom();

private:
	SudokuFrame* frame;
	SudokuSquare square;
	Random random;
};

wxDECLARE_APP(SudokuApp);
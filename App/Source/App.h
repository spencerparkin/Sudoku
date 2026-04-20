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

	SudokuFrame* GetFrame();
	SudokuSquare* GetSquare();
	const SudokuSquare* GetOriginalSquare() const;
	Random* GetRandom();
	void SetOriginalSquare();

private:
	SudokuFrame* frame;
	SudokuSquare square;
	SudokuSquare originalSquare;
	Random random;
};

wxDECLARE_APP(SudokuApp);
#include "App.h"
#include "Frame.h"

wxIMPLEMENT_APP(SudokuApp);

SudokuApp::SudokuApp()
{
	this->frame = nullptr;
}

/*virtual*/ SudokuApp::~SudokuApp()
{
}

/*virtual*/ bool SudokuApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	this->square.RandomlyGenerate(this->random);

	this->frame = new SudokuFrame(wxDefaultPosition, wxSize(1600, 1200));
	this->frame->Show();

	return true;
}

/*virtual*/ int SudokuApp::OnExit()
{
	return 0;
}

SudokuSquare* SudokuApp::GetSquare()
{
	return &this->square;
}

Random* SudokuApp::GetRandom()
{
	return &this->random;
}
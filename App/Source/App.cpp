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

	std::random_device randomDevice;
	this->random.SetSeed(randomDevice());

	this->square.Clear();

	this->frame = new SudokuFrame(wxDefaultPosition, wxSize(1600, 1200));
	this->frame->Show();

	return true;
}

/*virtual*/ int SudokuApp::OnExit()
{
	return 0;
}

SudokuFrame* SudokuApp::GetFrame()
{
	return this->frame;
}

const SudokuSquare* SudokuApp::GetOriginalSquare() const
{
	return &this->originalSquare;
}

SudokuSquare* SudokuApp::GetSquare()
{
	return &this->square;
}

Random* SudokuApp::GetRandom()
{
	return &this->random;
}

void SudokuApp::SetOriginalSquare()
{
	this->originalSquare.Copy(&this->square);
}
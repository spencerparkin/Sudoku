#include "App.h"
#include "Frame.h"
#include <wx/filename.h>
#include <wx/wfstream.h>

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
	this->originalSquare.Clear();

	wxString puzzleFile = wxFileName::GetTempDir() + wxT("/SudokuPuzzle.bin");
	if (wxFileExists(puzzleFile))
	{
		wxFile file(puzzleFile, wxFile::read);
		if (file.IsOpened())
		{
			wxFileInputStream fileStream(file);
			if (!this->square.LoadFromStream(fileStream) || !this->originalSquare.LoadFromStream(fileStream))
			{
				this->square.Clear();
				this->originalSquare.Clear();
			}

			file.Close();
		}
	}

	this->frame = new SudokuFrame(wxDefaultPosition, wxSize(1600, 1200));
	this->frame->Show();

	return true;
}

/*virtual*/ int SudokuApp::OnExit()
{
	wxString puzzleFile = wxFileName::GetTempDir() + wxT("/SudokuPuzzle.bin");
	wxFile file(puzzleFile, wxFile::write);
	if (file.IsOpened())
	{
		wxFileOutputStream fileStream(file);
		this->square.SaveToStream(fileStream);
		this->originalSquare.SaveToStream(fileStream);
		file.Close();
	}

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
#include "Canvas.h"
#include "App.h"
#include "SudokuSquare.h"
#include <gl/GLU.h>

int SudokuCanvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

SudokuCanvas::SudokuCanvas(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, attributeList)
{
	this->context = new wxGLContext(this);

	this->Bind(wxEVT_PAINT, &SudokuCanvas::OnPaint, this);
	this->Bind(wxEVT_SIZE, &SudokuCanvas::OnResize, this);

	this->worldRect.minCorner = HappyMath::Vector2(0.0, 0.0);
	this->worldRect.maxCorner = HappyMath::Vector2(100.0, 100.0);

	bool fontSysInitialized = this->fontSystem.Initialize();
	wxASSERT(fontSysInitialized);

	this->fontSystem.SetFontBaseDir("D:\\git_repos\\Sudoku\\App");
	this->fontSystem.SetFont("OpenSans-Regular.ttf");
}

/*virtual*/ SudokuCanvas::~SudokuCanvas()
{
	delete this->context;

	this->fontSystem.Finalize();
}

void SudokuCanvas::OnPaint(wxPaintEvent& event)
{
	this->SetCurrent(*this->context);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		this->adjustedWorldRect.minCorner.x,
		this->adjustedWorldRect.maxCorner.x,
		this->adjustedWorldRect.minCorner.y,
		this->adjustedWorldRect.maxCorner.y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SudokuSquare* square = wxGetApp().GetSquare();
	if (square)
	{
		HappyMath::Rectangle renderRect = this->worldRect;
		renderRect.ScaleVertically(0.9);
		renderRect.ScaleHorizontally(0.9);

		square->Render(renderRect, &this->fontSystem);
	}

	glFlush();

	this->SwapBuffers();
}

void SudokuCanvas::OnResize(wxSizeEvent& event)
{
	this->SetCurrent(*this->context);

	wxSize size = event.GetSize();
	glViewport(0, 0, size.x, size.y);

	double aspectRatio = (size.y != 0) ? (double(size.x) / double(size.y)) : 1.0;

	this->adjustedWorldRect = this->worldRect;
	this->adjustedWorldRect.ExpandToMatchAspectRatio(aspectRatio);
}
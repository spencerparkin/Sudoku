#include "Canvas.h"
#include "App.h"
#include "Frame.h"
#include "SudokuSquare.h"
#include <gl/GLU.h>

int SudokuCanvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

SudokuCanvas::SudokuCanvas(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, attributeList)
{
	this->hoverRow = -1;
	this->hoverCol = -1;

	this->context = new wxGLContext(this);

	this->Bind(wxEVT_PAINT, &SudokuCanvas::OnPaint, this);
	this->Bind(wxEVT_SIZE, &SudokuCanvas::OnResize, this);
	this->Bind(wxEVT_MOTION, &SudokuCanvas::OnMouseMotion, this);

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

void SudokuCanvas::OnMouseMotion(wxMouseEvent& event)
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return;

	HappyMath::Vector2 worldPos = this->MousePosToWorldPos(event.GetPosition());
	if (this->CalcMatrixLocationFromWorldPos(worldPos, this->hoverRow, this->hoverCol))
	{
		wxString text;

		int value = -1;
		square->GetValue(this->hoverRow, this->hoverCol, value);
		if (value == -1)
		{
			UU::DArray<int> possibleValuesArray;
			square->GetAllPossibleValuesForLocation(this->hoverRow, this->hoverCol, possibleValuesArray);

			text = wxT("Possible values: ");
			for (int i = 0; i < (int)possibleValuesArray.GetSize(); i++)
			{
				text += wxString::Format("%d", possibleValuesArray[i] + 1);
				if (i + 1 < (int)possibleValuesArray.GetSize())
					text += wxT(", ");
			}
		}

		wxGetApp().GetFrame()->SetStatusText(text);
	}
}

bool SudokuCanvas::CalcMatrixLocationFromWorldPos(const HappyMath::Vector2& worldPos, int& row, int& col) const
{
	SudokuSquare* square = wxGetApp().GetSquare();
	if (!square)
		return false;

	for (row = 0; row < square->GetSize(); row++)
	{
		for (col = 0; col < square->GetSize(); col++)
		{
			HappyMath::Rectangle valueRect;
			square->CalcValueSquare(this->renderRect, row, col, valueRect, 1.0);
			if (valueRect.ContainsPoint(worldPos))
				return true;
		}
	}

	row = -1;
	col = -1;
	return false;
}

HappyMath::Vector2 SudokuCanvas::MousePosToWorldPos(const wxPoint& mousePos) const
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	HappyMath::Rectangle viewportRect;
	viewportRect.minCorner.x = double(viewport[0]);
	viewportRect.maxCorner.x = double(viewport[0]) + double(viewport[2]);
	viewportRect.minCorner.y = double(viewport[1]);
	viewportRect.maxCorner.y = double(viewport[1]) + double(viewport[3]);

	HappyMath::Vector2 point(mousePos.x, viewport[3] - mousePos.y);
	HappyMath::Vector2 uvs = viewportRect.PointToUVs(point);
	point = this->adjustedWorldRect.PointFromUVs(uvs);

	return point;
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
		square->Render(this->renderRect, &this->fontSystem);

		if (this->hoverRow != -1 && this->hoverCol != -1)
		{
			HappyMath::Rectangle valueRect;
			square->CalcValueSquare(this->renderRect, this->hoverRow, this->hoverCol, valueRect, 0.95);

			glLineWidth(5.0f);
			glBegin(GL_LINE_LOOP);
			glColor3f(1.0f, 0.0f, 0.0f);

			glVertex2d(valueRect.minCorner.x, valueRect.minCorner.y);
			glVertex2d(valueRect.maxCorner.x, valueRect.minCorner.y);
			glVertex2d(valueRect.maxCorner.x, valueRect.maxCorner.y);
			glVertex2d(valueRect.minCorner.x, valueRect.maxCorner.y);

			glEnd();
		}
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

	this->renderRect = this->worldRect;
	this->renderRect.ScaleVertically(0.9);
	this->renderRect.ScaleHorizontally(0.9);
}
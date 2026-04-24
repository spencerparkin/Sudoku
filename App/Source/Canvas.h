#pragma once

#include <wx/glcanvas.h>
#include "HappyMath/Rectangle.h"
#include "FontSystem.h"

/**
 * 
 */
class SudokuCanvas : public wxGLCanvas
{
public:
	SudokuCanvas(wxWindow* parent);
	virtual ~SudokuCanvas();

	void SetShowPossibilities(bool showPossibilities);
	bool GetShowPossibilities() const;

private:
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnKeyEvent(wxKeyEvent& event);

	HappyMath::Vector2 MousePosToWorldPos(const wxPoint& mousePos) const;
	bool CalcMatrixLocationFromWorldPos(const HappyMath::Vector2& worldPos, int& row, int& col) const;

	wxGLContext* context;
	static int attributeList[];
	HappyMath::Rectangle worldRect;
	HappyMath::Rectangle adjustedWorldRect;
	HappyMath::Rectangle renderRect;
	FontSys::System fontSystem;
	int hoverRow, hoverCol;
	bool showPossibilities;
};
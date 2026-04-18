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

	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);

private:
	wxGLContext* context;
	static int attributeList[];
	HappyMath::Rectangle worldRect;
	HappyMath::Rectangle adjustedWorldRect;
	FontSys::System fontSystem;
};
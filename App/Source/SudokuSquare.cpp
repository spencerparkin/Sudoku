#include "SudokuSquare.h"
#include "UltraUtilities/Random.h"
#include <wx/wxcrtvararg.h>
#include <wx/glcanvas.h>
#include <format>

SudokuSquare::SudokuSquare() : LatinSquare(9)
{
}

/*virtual*/ SudokuSquare::~SudokuSquare()
{
}

void SudokuSquare::Render(const HappyMath::Rectangle& renderRect, FontSys::System* fontSystem) const
{
	glDisable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex2d(renderRect.minCorner.x, renderRect.minCorner.y);
	glVertex2d(renderRect.maxCorner.x, renderRect.minCorner.y);
	glVertex2d(renderRect.maxCorner.x, renderRect.maxCorner.y);
	glVertex2d(renderRect.minCorner.x, renderRect.maxCorner.y);

	glEnd();

	double y = renderRect.minCorner.y;
	double dy = renderRect.GetHeight() / 9.0;
	double x = renderRect.minCorner.x;
	double dx = renderRect.GetWidth() / 9.0;

	for (int i = 0; i < this->GetSize() - 1; i++)
	{
		y += dy;
		x += dx;

		if ((i - 2) % 3 == 0)
			glLineWidth(6.0f);
		else
			glLineWidth(3.0f);

		glBegin(GL_LINES);

		if ((i - 2) % 3 == 0)
			glColor3f(0.0f, 0.0f, 0.0f);
		else
			glColor3f(0.5f, 0.5f, 0.5f);

		glVertex2d(renderRect.minCorner.x, y);
		glVertex2d(renderRect.maxCorner.x, y);

		glVertex2d(x, renderRect.minCorner.y);
		glVertex2d(x, renderRect.maxCorner.y);

		glEnd();
	}

	glColor3f(0.0f, 0.0f, 0.0f);

	for (int row = 0; row < this->size; row++)
	{
		for (int col = 0; col < this->size; col++)
		{
			int value = this->matrix[row][col];
			if (value < 0 || value > 9)
				continue;

			std::string text = std::format("{}", value + 1);

			HappyMath::Rectangle valueRect;
			this->CalcValueSquare(renderRect, row, col, valueRect, 0.7);

			fontSystem->SetLineHeight(valueRect.GetHeight());

			GLfloat textLength = 0.0f;
			fontSystem->CalcTextLength(text, textLength);

			HappyMath::Vector2 textLocation = valueRect.minCorner;
			textLocation.x += (valueRect.GetWidth() - textLength) / 2.0;
			textLocation.y += (valueRect.GetHeight() - fontSystem->GetLineHeight()) / 2.0;

			fontSystem->RenderText(textLocation.x, textLocation.y, text, true);
		}
	}
}

void SudokuSquare::CalcValueSquare(const HappyMath::Rectangle& renderRect, int row, int col, HappyMath::Rectangle& valueRect, double scale) const
{
	double width = renderRect.GetWidth() / 9.0;
	double height = renderRect.GetHeight() / 9.0;

	valueRect.minCorner.x = renderRect.minCorner.x + width * double(col);
	valueRect.minCorner.y = renderRect.minCorner.y + height * double(this->size - 1 - row);

	valueRect.maxCorner.x = valueRect.minCorner.x + width;
	valueRect.maxCorner.y = valueRect.minCorner.y + height;

	valueRect.ScaleHorizontally(scale);
	valueRect.ScaleVertically(scale);
}

void SudokuSquare::SaveToStream(wxOutputStream& outputStream) const
{
	for (int row = 0; row < this->GetSize(); row++)
	{
		for (int col = 0; col < this->GetSize(); col++)
		{
			int value = this->matrix[row][col];
			outputStream.Write(&value, sizeof(int));
		}
	}
}

bool SudokuSquare::LoadFromStream(wxInputStream& inputStream)
{
	for (int row = 0; row < this->GetSize(); row++)
	{
		for (int col = 0; col < this->GetSize(); col++)
		{
			if (!inputStream.CanRead())
				return false;

			int value = -1;
			inputStream.Read(&value, sizeof(int));

			this->matrix[row][col] = value;
		}
	}

	return true;
}

void SudokuSquare::Print() const
{
	for (int row = 0; row < this->GetSize(); row++)
	{
		for (int col = 0; col < this->GetSize(); col++)
		{
			int value = -1;
			this->GetValue(row, col, value);
			if (value < 0 || value >= this->GetSize())
				wxPrintf("*");
			else
				wxPrintf("%d", value);
			if (col < this->GetSize() - 1)
				wxPrintf(", ");
		}

		wxPrintf("\n");
	}

	wxPrintf("\n");
}

/*virtual*/ bool SudokuSquare::CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value)
{
	if (!LatinSquare::CanPlaceValueAtTargetLocation(targetRow, targetCol, value))
		return false;

	int subSquareRow = 3 * (targetRow / 3);
	int subSquareCol = 3 * (targetCol / 3);

	for (int row = subSquareRow; row <= targetRow; row++)
	{
		int colStop = (row == targetRow) ? targetCol : (subSquareCol + 3);
		for (int col = subSquareCol; col < colStop; col++)
			if (this->matrix[row][col] == value)
				return false;
	}

	return true;
}

/*virtual*/ bool SudokuSquare::IsValid() const
{
	if (!LatinSquare::IsValid())
		return false;

	UU::DArray<int> valuesCountArray;
	valuesCountArray.SetSize(9);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 9; k++)
				valuesCountArray[k] = 0;

			for (int row = i * 3; row < (i + 1) * 3; row++)
			{
				for (int col = j * 3; col < (j + 1) * 3; col++)
				{
					int value = this->matrix[row][col];

					if (valuesCountArray[value] != 0)
						return false;

					valuesCountArray[value]++;
				}
			}
		}
	}

	return true;
}

/*virtual*/ void SudokuSquare::BumpIllegalValuesForLocation(int targetRow, int targetCol, UU::DArray<int>& countArray)
{
	LatinSquare::BumpIllegalValuesForLocation(targetRow, targetCol, countArray);

	int subSquareRow = 3 * (targetRow / 3);
	int subSquareCol = 3 * (targetCol / 3);

	for (int row = subSquareRow; row < subSquareRow + 3; row++)
	{
		for (int col = subSquareCol; col < subSquareCol + 3; col++)
		{
			int value = this->matrix[row][col];
			if (value != -1)
				countArray[value]++;
		}
	}
}

// There is a stratagy for solving these puzzles algorithmically that
// I have not yet tried.  It is as follows.  First, calculate the possibilities
// for each empty square just as I currently am.  We then make passes
// over all the empty squares trying to reduces those possibilities.
// For example, for an empty square, it has a set of empty square
// influences in its row.  If none of those influential squares can
// take on a value that it itself holds as a possibility, then that
// possibility must be the only one for the empty square.  We do this also, of course, for
// all influences the empty square has in its column and its 3x3 sub-square.
// Our algorithm here should make passes over the entire sudoku puzzle
// until a single pass of the entire puzzle does not yield any reductions.
// Are there any other reasonings we can make in this fasion?

void SudokuSquare::MakePuzzle(UU::Random& random)
{
	this->RandomlyGenerate(random);

	struct Location
	{
		int row, col;
		int value;
	};

	UU::DArray<Location> locationArray;
	for (int row = 0; row < this->size; row++)
		for (int col = 0; col < this->size; col++)
			locationArray.Push({ row, col, this->matrix[row][col] });

	random.Shuffle(locationArray.GetBuffer(), locationArray.GetSize());

	int i = 0;
	while (true)
	{
		SudokuSquare* sudokuSquare = (SudokuSquare*)this->Clone();
		bool solved = sudokuSquare->CompleteSquare(true);		// STPTODO: Replace with smart or dumb solver to set difficulty level.
		delete sudokuSquare;

		if (solved)
		{
			const Location& location = locationArray[i++];
			this->matrix[location.row][location.col] = -1;
		}
		else
		{
			const Location& location = locationArray[--i];
			this->matrix[location.row][location.col] = location.value;
			break;
		}
	}
}

/*virtual*/ UU::LatinSquare* SudokuSquare::Clone() const
{
	auto sudokuSquare = new SudokuSquare();
	sudokuSquare->Copy(this);
	return sudokuSquare;
}
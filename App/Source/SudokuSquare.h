#pragma once

#include "UltraUtilities/Math/LatinSquare.h"
#include "HappyMath/Rectangle.h"
#include "FontSystem.h"
#include <wx/stream.h>

/**
 * This is a special case of 9x9 latin square where
 * each 3x3 sub-square also contains the numberse 0-9.
 */
class SudokuSquare : public UU::LatinSquare
{
public:
	SudokuSquare();
	virtual ~SudokuSquare();

	virtual bool IsValid() const override;
	virtual LatinSquare* Clone() const override;

	void MakePuzzle(UU::Random& random, int difficultyLevel);
	void Print() const;
	void SaveToStream(wxOutputStream& outputStream) const;
	bool LoadFromStream(wxInputStream& inputStream);
	void Render(const HappyMath::Rectangle& renderRect, FontSys::System* fontSystem) const;
	void CalcValueSquare(const HappyMath::Rectangle& renderRect, int row, int col, HappyMath::Rectangle& valueRect) const;

protected:
	virtual bool CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value) override;
	virtual void BumpIllegalValuesForLocation(int targetRow, int targetCol, UU::DArray<int>& countArray) override;
};
#include "SudokuValueRemover.h"
#include "SudokuSquare.h"

//----------------------------------- SudokuValueRemover -----------------------------------

SudokuValueRemover::SudokuValueRemover()
{
}

/*virtual*/ SudokuValueRemover::~SudokuValueRemover()
{
}

//----------------------------------- RandomSingleValueRemover -----------------------------------

RandomSingleValueRemover::RandomSingleValueRemover()
{
}

/*virtual*/ RandomSingleValueRemover::~RandomSingleValueRemover()
{
}

/*virtual*/ int RandomSingleValueRemover::GetNumValuesRemovedPerStep()
{
	return 1;
}

/*virtual*/ int RandomSingleValueRemover::RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random)
{
	this->possibleLocationArray.SetSize(0);

	for (int row = 0; row < square->GetSize(); row++)
	{
		for (int col = 0; col < square->GetSize(); col++)
		{
			int value = -1;
			square->GetValue(row, col, value);
			if (value == -1)
				continue;

			Location location;
			location.row = row;
			location.col = col;
			location.value = value;
			this->possibleLocationArray.Push(location);
		}
	}

	random.Shuffle(this->possibleLocationArray.GetBuffer(), this->possibleLocationArray.GetSize());

	return (int)this->possibleLocationArray.GetSize();
}

/*virtual*/ void RandomSingleValueRemover::DoRemovalStep(int i, SudokuSquare* square)
{
	const Location& location = this->possibleLocationArray[i];

	square->SetValue(location.row, location.col, -1);
}

/*virtual*/ void RandomSingleValueRemover::UndoRemovalStep(int i, SudokuSquare* square)
{
	const Location& location = this->possibleLocationArray[i];

	square->SetValue(location.row, location.col, location.value);
}

//----------------------------------- Random180DegreeSymmetryValueRemover -----------------------------------

Random180DegreeSymmetryValueRemover::Random180DegreeSymmetryValueRemover()
{
}

/*virtual*/ Random180DegreeSymmetryValueRemover::~Random180DegreeSymmetryValueRemover()
{
}

/*virtual*/ int Random180DegreeSymmetryValueRemover::GetNumValuesRemovedPerStep()
{
	return 2;
}

/*virtual*/ int Random180DegreeSymmetryValueRemover::RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random)
{
	this->possibleLocationPairArray.SetSize(0);

	for (int row = 0; row < square->GetSize(); row++)
	{
		int oppositeRow = square->GetSize() - 1 - row;

		for (int col = 0; col <= row; col++)
		{
			int oppositeCol = square->GetSize() - 1 - col;

			int value = -1;
			square->GetValue(row, col, value);
			if (value == -1)
				continue;

			int oppositeValue = -1;
			square->GetValue(oppositeRow, oppositeCol, oppositeValue);
			if (oppositeValue == -1)
				continue;	// This shouldn't happen.

			LocationPair pair;
			pair.location[0].row = row;
			pair.location[0].col = col;
			pair.location[0].value = value;
			pair.location[1].row = oppositeRow;
			pair.location[1].col = oppositeCol;
			pair.location[1].value = oppositeValue;
			this->possibleLocationPairArray.Push(pair);
		}
	}

	random.Shuffle(this->possibleLocationPairArray.GetBuffer(), this->possibleLocationPairArray.GetSize());

	return (int)this->possibleLocationPairArray.GetSize();
}

/*virtual*/ void Random180DegreeSymmetryValueRemover::DoRemovalStep(int i, SudokuSquare* square)
{
	const LocationPair& pair = this->possibleLocationPairArray[i];

	square->SetValue(pair.location[0].row, pair.location[0].col, -1);
	square->SetValue(pair.location[1].row, pair.location[1].col, -1);
}

/*virtual*/ void Random180DegreeSymmetryValueRemover::UndoRemovalStep(int i, SudokuSquare* square)
{
	const LocationPair& pair = this->possibleLocationPairArray[i];

	square->SetValue(pair.location[0].row, pair.location[0].col, pair.location[0].value);
	square->SetValue(pair.location[1].row, pair.location[1].col, pair.location[1].value);
}

//----------------------------------- Random90DegreeSymmetryValueRemover -----------------------------------

Random90DegreeSymmetryValueRemover::Random90DegreeSymmetryValueRemover()
{
}

/*virtual*/ Random90DegreeSymmetryValueRemover::~Random90DegreeSymmetryValueRemover()
{
}

/*virtual*/ int Random90DegreeSymmetryValueRemover::GetNumValuesRemovedPerStep()
{
	return 4;
}

/*virtual*/ int Random90DegreeSymmetryValueRemover::RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random)
{
	this->possibleLocationQuadArray.SetSize(0);

	for (int row = 0; row < square->GetSize() / 2 + 1; row++)
	{
		int oppositeRow = square->GetSize() - 1 - row;

		for (int col = 0; col < square->GetSize() / 2 + 1; col++)
		{
			int oppositeCol = square->GetSize() - 1 - col;

			int value0 = -1, value1 = -1, value2 = -1, value3 = -1;

			square->GetValue(row, col, value0);
			square->GetValue(oppositeRow, col, value1);
			square->GetValue(row, oppositeCol, value2);
			square->GetValue(oppositeRow, oppositeCol, value3);

			if (value0 == -1 || value1 == -1 || value2 == -2 || value3 == -1)
				continue;

			LocationQuad quad;
			quad.location[0].row = row;
			quad.location[0].col = col;
			quad.location[0].value = value0;
			quad.location[1].row = oppositeRow;
			quad.location[1].col = col;
			quad.location[1].value = value1;
			quad.location[2].row = row;
			quad.location[2].col = oppositeCol;
			quad.location[2].value = value2;
			quad.location[3].row = oppositeRow;
			quad.location[3].col = oppositeCol;
			quad.location[3].value = value3;

			this->possibleLocationQuadArray.Push(quad);
		}
	}

	random.Shuffle(this->possibleLocationQuadArray.GetBuffer(), this->possibleLocationQuadArray.GetSize());

	return (int)this->possibleLocationQuadArray.GetSize();
}

/*virtual*/ void Random90DegreeSymmetryValueRemover::DoRemovalStep(int i, SudokuSquare* square)
{
	const LocationQuad& quad = this->possibleLocationQuadArray[i];

	for (int i = 0; i < 4; i++)
		square->SetValue(quad.location[i].row, quad.location[i].col, -1);
}

/*virtual*/ void Random90DegreeSymmetryValueRemover::UndoRemovalStep(int i, SudokuSquare* square)
{
	const LocationQuad& quad = this->possibleLocationQuadArray[i];

	for (int i = 0; i < 4; i++)
		square->SetValue(quad.location[i].row, quad.location[i].col, quad.location[i].value);
}
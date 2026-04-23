#pragma once

#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Random.h"

class SudokuSquare;

/**
 * Derivatives of this class are used in the puzzle generation process.
 */
class SudokuValueRemover
{
public:
	SudokuValueRemover();
	virtual ~SudokuValueRemover();

	virtual int GetNumValuesRemovedPerStep() = 0;
	virtual int RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random) = 0;
	virtual void DoRemovalStep(int i, SudokuSquare* square) = 0;
	virtual void UndoRemovalStep(int i, SudokuSquare* square) = 0;

	struct Location
	{
		int row, col;
		int value;
	};
};

/**
 * This class helps us generate a typically random Sudoku puzzle.
 */
class RandomSingleValueRemover : public SudokuValueRemover
{
public:
	RandomSingleValueRemover();
	virtual ~RandomSingleValueRemover();

	virtual int GetNumValuesRemovedPerStep() override;
	virtual int RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random) override;
	virtual void DoRemovalStep(int i, SudokuSquare* square) override;
	virtual void UndoRemovalStep(int i, SudokuSquare* square) override;

protected:
	UU::DArray<Location> possibleLocationArray;
};

/**
 * This class helps us generate a Sudoku puzzle with 180-degree symmetry.
 */
class Random180DegreeSymmetryValueRemover : public SudokuValueRemover
{
public:
	Random180DegreeSymmetryValueRemover();
	virtual ~Random180DegreeSymmetryValueRemover();

	virtual int GetNumValuesRemovedPerStep() override;
	virtual int RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random) override;
	virtual void DoRemovalStep(int i, SudokuSquare* square) override;
	virtual void UndoRemovalStep(int i, SudokuSquare* square) override;

protected:
	struct LocationPair
	{
		Location location[2];
	};

	UU::DArray<LocationPair> possibleLocationPairArray;
};

/**
 * This class helps us generate a Sudoku puzzle with 90-degree symmetry.
 */
class Random90DegreeSymmetryValueRemover : public SudokuValueRemover
{
public:
	Random90DegreeSymmetryValueRemover();
	virtual ~Random90DegreeSymmetryValueRemover();

	virtual int GetNumValuesRemovedPerStep() override;
	virtual int RegeneratePossibleRemovalSteps(SudokuSquare* square, UU::Random& random) override;
	virtual void DoRemovalStep(int i, SudokuSquare* square) override;
	virtual void UndoRemovalStep(int i, SudokuSquare* square) override;

protected:
	struct LocationQuad
	{
		Location location[4];
	};

	UU::DArray<LocationQuad> possibleLocationQuadArray;
};
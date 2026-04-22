#pragma once

class SudokuSquare;

/**
 * 
 */
class SudokuSolver
{
public:
	SudokuSolver();
	virtual ~SudokuSolver();

	virtual bool Solve(SudokuSquare* square) = 0;
};

/**
 * 
 */
class SimpleSudokuSolver : public SudokuSolver
{
public:
	SimpleSudokuSolver();
	virtual ~SimpleSudokuSolver();

	virtual bool Solve(SudokuSquare* square) override;
	virtual bool ResolveAnyValue(SudokuSquare* square, int* resolvedRow = nullptr, int* resolvedCol = nullptr);
};

/**
 *
 */
class AdvancedSudokuSolver : public SimpleSudokuSolver
{
public:
	AdvancedSudokuSolver();
	virtual ~AdvancedSudokuSolver();
	virtual bool ResolveAnyValue(SudokuSquare* square, int* resolvedRow = nullptr, int* resolvedCol = nullptr);

protected:

	bool CanAnyEmptyRowNeighborTakeOnValue(SudokuSquare* square, int targetRow, int targetCol, int possibleValue);
	bool CanAnyEmptyColumnNeighborTakeOnValue(SudokuSquare* square, int targetRow, int targetCol, int possibleValue);
	bool CanAnyEmpty3x3SubSquareNeighborTakeOnValue(SudokuSquare* square, int targetRow, int targetCol, int possibleValue);
};
#include "SudokuSolver.h"
#include "SudokuSquare.h"

//-------------------------------- SudokuSolver --------------------------------

SudokuSolver::SudokuSolver()
{
}

/*virtual*/ SudokuSolver::~SudokuSolver()
{
}

//-------------------------------- SimpleSudokuSolver --------------------------------

SimpleSudokuSolver::SimpleSudokuSolver()
{
}

/*virtual*/ SimpleSudokuSolver::~SimpleSudokuSolver()
{
}

/*virtual*/ bool SimpleSudokuSolver::Solve(SudokuSquare* square)
{
	while (square->GetNumSetValues() != square->GetNumValues())
	{
		if (this->ResolveAnyValue(square))
			continue;
			
		return false;
	}
	
	return true;
}

/*virtual*/ bool SimpleSudokuSolver::ResolveAnyValue(SudokuSquare* square, int* resolvedRow /*= nullptr*/, int* resolvedCol /*= nullptr*/)
{
	for (int row = 0; row < square->GetSize(); row++)
	{
		for (int col = 0; col < square->GetSize(); col++)
		{
			int value = -1;
			square->GetValue(row, col, value);
			if (value != -1)
				continue;

			UU::DArray<int> possibleValuesArray;
			square->GetAllPossibleValuesForLocation(row, col, possibleValuesArray);
			if (possibleValuesArray.GetSize() == 1)
			{
				square->SetValue(row, col, possibleValuesArray[0]);

				if (resolvedRow && resolvedCol)
				{
					*resolvedRow = row;
					*resolvedCol = col;
				}

				return true;
			}
		}
	}

	return false;
}

//-------------------------------- AdvancedSudokuSolver --------------------------------

AdvancedSudokuSolver::AdvancedSudokuSolver()
{
}

/*virtual*/ AdvancedSudokuSolver::~AdvancedSudokuSolver()
{
}

/*virtual*/ bool AdvancedSudokuSolver::ResolveAnyValue(SudokuSquare* square, int* resolvedRow /*= nullptr*/, int* resolvedCol /*= nullptr*/)
{
	if (SimpleSudokuSolver::ResolveAnyValue(square, resolvedRow, resolvedCol))
		return true;

	for (int row = 0; row < square->GetSize(); row++)
	{
		for (int col = 0; col < square->GetSize(); col++)
		{
			int value = -1;
			square->GetValue(row, col, value);
			if (value != -1)
				continue;

			UU::DArray<int> possibleValuesArray;
			square->GetAllPossibleValuesForLocation(row, col, possibleValuesArray);
			for (int i = 0; i < (int)possibleValuesArray.GetSize(); i++)
			{
				int possibleValue = possibleValuesArray[i];

				if (!this->CanAnyEmptyRowNeighborTakeOnValue(square, row, col, possibleValue) ||
					!this->CanAnyEmptyColumnNeighborTakeOnValue(square, row, col, possibleValue) ||
					!this->CanAnyEmpty3x3SubSquareNeighborTakeOnValue(square, row, col, possibleValue))
				{
					square->SetValue(row, col, possibleValue);

					if (resolvedRow && resolvedCol)
					{
						*resolvedRow = row;
						*resolvedCol = col;
					}

					return true;
				}
			}
		}
	}

	return false;
}

bool AdvancedSudokuSolver::CanAnyEmptyRowNeighborTakeOnValue(SudokuSquare* square, int targetRow, int targetCol, int possibleValue)
{
	for (int col = 0; col < square->GetSize(); col++)
	{
		if (col == targetCol)
			continue;

		int value = -1;
		square->GetValue(targetRow, col, value);
		if (value != -1)
			continue;

		if (square->IsPossibleValueForLocation(targetRow, col, possibleValue))
			return true;
	}

	return false;
}

bool AdvancedSudokuSolver::CanAnyEmptyColumnNeighborTakeOnValue(SudokuSquare* square, int targetRow, int targetCol, int possibleValue)
{
	for (int row = 0; row < square->GetSize(); row++)
	{
		if (row == targetRow)
			continue;

		int value = -1;
		square->GetValue(row, targetCol, value);
		if (value != -1)
			continue;

		if (square->IsPossibleValueForLocation(row, targetCol, possibleValue))
			return true;
	}

	return false;
}

bool AdvancedSudokuSolver::CanAnyEmpty3x3SubSquareNeighborTakeOnValue(SudokuSquare* square, int targetRow, int targetCol, int possibleValue)
{
	int subSquareRow = 3 * (targetRow / 3);
	int subSquareCol = 3 * (targetCol / 3);

	for (int row = subSquareRow; row < subSquareRow + 3; row++)
	{
		for (int col = subSquareCol; col < subSquareCol + 3; col++)
		{
			if (row == targetRow && col == targetCol)
				continue;

			int value = -1;
			square->GetValue(row, targetCol, value);
			if (value != -1)
				continue;

			if (square->IsPossibleValueForLocation(row, col, possibleValue))
				return true;
		}
	}

	return false;
}
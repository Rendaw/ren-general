#include "range.h"

#include "vector.h"
#include "rotation.h"

#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <iostream>

unsigned int Mod(int Input, int Size)
{
	// Note: Size must be int, not unsigned int (or else % fails hard)
	int const Intermediate = Input % Size;
	return Intermediate < 0 ? (Intermediate + Size) : Intermediate;
}

void OrderD(int &Min, int &Max)
{
	if (Min <= Max) return;
	int Temp = Min;
	Min = Max;
	Max = Temp;
}

void OrderF(float &Min, float &Max)
{
	if (Min <= Max) return;
	float Temp = Min;
	Min = Max;
	Max = Temp;
}

bool RanderB(void)
	{ return rand() % 2 == 0; }

float RanderF(const RangeF &RandomRange, const unsigned int Possibilities)
{
	if (Possibilities == 0) return RandomRange.Min;
	return RandomRange.Min + float(rand() % Possibilities) / float(Possibilities) * RandomRange.Length();
}

float RanderF(const float &Min, const float &Max, const unsigned int Possibilities)
{
	if (Possibilities == 0) return Min;
	return Min + float(rand() % Possibilities) / float(Possibilities) * (Max - Min);
}

Vector RandomSphericalVector(void)
{
	Vector NewVector;
	RangeF SpaceRange(-1, 1);
	do { NewVector = Vector(RanderF(SpaceRange, 1000), RanderF(SpaceRange, 1000), RanderF(SpaceRange, 1000)); }
		while (NewVector.Length() > 1.0f);
	return NewVector;
}

FlatVector RandomCircularVector(void)
{
	return Angle(RanderF(RangeF(0, 360), 1000)).Rotate(FlatVector(1, 0));
}

FlatVector RanderR(const Region &Selection, const unsigned int PossibilitiesSquareRoot)
{
	return Selection.Start +
		FlatVector(
			RanderF(0, Selection.Size[0], PossibilitiesSquareRoot),
			RanderF(0, Selection.Size[1], PossibilitiesSquareRoot));
}

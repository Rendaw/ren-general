#ifndef range_h
#define range_h

/*
	Requires:
	vector.h (for random values)
	rotation.h (for random values as well)
*/

#include "vector.h"
#include "region.h"

#include <algorithm>
#include <iostream>

// Range class
// Does not include Max, range is Min >= x > Max
template <typename Type> class Range
{
	public:
		Range(const Type &NewMin, const Type &NewMax) :
			Min((std::min)(NewMin, NewMax)),
			Max((std::max)(NewMin, NewMax))
			{}

		Range(const Range<Type> &Coperand) : Min(Coperand.Min), Max(Coperand.Max)
			{}

		Range(void) : Min(0), Max(0)
			{}

		Range<Type> &SpanSet(const Type &Start, const Type &Width)
			{ Min = Start; Max = Start + fabs(Width); return *this; }

		Range<Type> &Include(const Type &Value)
			{ Min = (std::min)(Min, Value); Max = (std::max)(Max, Value); return *this; }

		Range<Type> Including(const Type &Value) const
			{ return Range<Type>((std::min)(Min, Value), (std::max)(Max, Value)); }

		Range<Type> Expand(const Type &Amount) const
			{ return Range<Type>((std::min)(Min, Min + Amount), (std::max)(Max, Max + Amount)); }

		Range<Type> &Contract(const Type &Amount)
		{
			Min = (std::min)((std::max)(Min, Min + Amount), Max);
			Max = (std::max)((std::min)(Max, Max + Amount), Min);
			return *this;
		}

		Type Length(void) const
			{ return Max - Min; }

		Type Center(void) const
			{ return (Max + Min) / 2; }

		float Percent(const Type &Marker) const
			{ return (float)(Marker - Min) / (float)(Max - Min); }

		Type AtPercent(float Percent) const
			{ return Percent * (Max - Min) + Min; }

		bool Contains(const Type &Operand) const
			{ return (Operand >= Min) && (Operand <= Max); }

		bool Contains(const Range<Type> &Operand) const
			{ return (Operand.Min >= Min) && (Operand.Max <= Max); }

		bool Intersects(const Range<Type> &Operand) const
			{ return (Min < Operand.Max) && (Operand.Min <= Max); }

		Type Shared(const Range<Type> &Operand) const // Negative if not intersecting
			{ return (std::min)(Max, Operand.Max) - (std::max)(Min, Operand.Min); }

		Type SpaceDifference(const Range<Type> &Operand) const // Negative if right of operand - bad if intersecting
		{
			if (Min > Operand.Max) return Operand.Max - Min;
			else return Operand.Min - Max;
		}

		// Amount of this lower than Operand
		Type LowerUnshared(const Range<Type> &Operand) const
		{
			/*if (Max < Operand.Min) return Length();
			else*/ return Operand.Min - Min;
		}

		// Amount of this above operand
		Type UpperUnshared(const Range<Type> &Operand) const
		{
			/*if (Min > Operand.Max) return Length();
			else*/ return Max - Operand.Max;
		}

		Type Constrain(const Type &Operand) const
			{ return (std::max)((std::min)(Operand, Max), Min); }

		Type Wrap(const Type &Operand) const
		{
			Type Out = Operand;
			if (Out < Min) Out += Max - Min;
			if (Out > Max) Out -= Max - Min;
			return Out;
		}

		String AsString(void) const
		{
			StringStream Out;
			Out << "[" << Min << ", " << Max << "]";
			return Out.str();
		}

		Type Min, Max;
};

typedef Range<int> RangeD;
typedef Range<long int> RangeL;
typedef Range<float> RangeF;

// Mixes values
template <typename Type> inline Type Mix(const Type &Start, const Type &End, const float Percent)
{
	return (End - Start) * (std::min)((std::max)(0.0f, Percent), 1.0f) + Start;
}

// Orders two values
void OrderD(int &Min, int &Max);
void OrderF(float &Min, float &Max);

// Random booleans
bool RanderB(void);

// Random value from range
template <typename Type> inline Type RanderD(const Range<Type> &RandomRange)
{
	if (RandomRange.Length() == 0) return RandomRange.Min;
	return RandomRange.Min + rand() % (RandomRange.Min - RandomRange.Max);
}

template <typename Type> inline Type RanderD(const Type Min, const Type Max)
{
	if (Max - Min == 0) return Min;
	return Min + rand() % (Min - Max);
}

// Random values
float RanderF(const RangeF &RandomRange, const unsigned int Possibilities);
float RanderF(const float &Min, const float &Max, const unsigned int Possibilities);
Vector RandomSphericalVector(void);
FlatVector RandomCircularVector(void);
FlatVector RanderR(const Region &Selection, const unsigned int PossibilitiesSquareRoot);

#endif

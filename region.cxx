#include "region.h"

#include "inputoutput.h"

Region::Region(void) :
	Start(), Size(1, 1)
	{}

Region::Region(const FlatVector &Start, const FlatVector &Size) :
	Start(Start), Size(Size)
	{}

String Region::AsString(void) const
{
	MemoryStream Out;
	Out << "(" << Start[0] << ", " << Start[1] << " to " <<
		(Start[0] + Size[0]) << ", " << (Start[1] + Size[1]) << " size " <<
		Size[0] << "x" << Size[1] << ")";
	return Out;
}

bool Region::Valid(void) const
{
	return (Size[0] >= 0.0f) && (Size[1] >= 0.0f);
}

bool Region::Contains(const FlatVector &Point) const
{
	FlatVector RelativePoint = Point - Start;
	return (RelativePoint[0] >= 0) && (RelativePoint[0] < Size[0]) &&
		(RelativePoint[1] >= 0) && (RelativePoint[1] < Size[1]);
}

FlatVector Region::Transform(const FlatVector &Point, const Region &ToRegion) const
{
	return (Point - Start) * (ToRegion.Size / Size) + ToRegion.Start;
}

FlatVector Region::Scale(const FlatVector &Area, const Region &ToRegion) const
{
	return Area / Size * ToRegion.Size;
}

Region Region::Transform(const Region &Subregion, const Region &ToRegion) const
{
	return Region(Transform(Subregion.Start, ToRegion), Scale(Subregion.Size, ToRegion));
}

FlatVector Region::Constrain(const FlatVector &Point) const
{
	return FlatVector(std::min(std::max(Point[0], Start[0]), Start[0] + Size[0]),
		std::min(std::max(Point[1], Start[1]), Start[1] + Size[1]));
}

Region Region::Translate(const FlatVector &Offset) const
{
	return Region(Start + Offset, Size);
}

Region Region::operator *(const Region &RelativeSubregion) const
{
	return Region(Start + Size * RelativeSubregion.Start, Size * RelativeSubregion.Size);
}

Region Region::operator *(const float &Scale) const
{
	return Region(Start * Scale, Size * Scale);
}

Region Region::operator +(const Region &Operand) const
{
	FlatVector NewStart(std::min(Start[0], Operand.Start[0]), std::min(Start[1], Operand.Start[1]));
	return Region(NewStart,
		FlatVector(std::max(Start[0] + Size[0], Operand.Start[0] + Operand.Size[0]),
			std::max(Start[1] + Size[1], Operand.Start[1] + Operand.Size[1])) - NewStart);
}

Region Region::operator +(const FlatVector &Operand) const
{
	return Region(
		FlatVector(std::min(Start[0], Operand[0]), std::min(Start[1], Operand[1])),
		FlatVector(std::max(Start[0] + Size[0], Operand[0]), std::max(Start[1] + Size[1], Operand[1])));
}

Region Region::operator +=(const Region &Operand)
{
	Size[0] = std::max(Start[0] + Size[0], Operand.Start[0] + Operand.Size[0]);
	Size[1] = std::max(Start[1] + Size[1], Operand.Start[1] + Operand.Size[1]);
	Start[0] = std::min(Start[0], Operand.Start[0]);
	Start[1] = std::min(Start[1], Operand.Start[1]);
	Size -= Start;
	return *this;
}

Region Region::operator +=(const FlatVector &Operand)
{
	return *this = Region(
		FlatVector(std::min(Start[0], Operand[0]), std::min(Start[1], Operand[1])),
		FlatVector(std::max(Start[0] + Size[0], Operand[0]), std::max(Start[1] + Size[1], Operand[1])));
}

Region Region::Intersect(const Region &Operand) const
{
	Region Out(
		FlatVector(
			std::max(Start[0], Operand.Start[0]),
			std::max(Start[1], Operand.Start[1])),
		FlatVector());
	Out.Size =
		FlatVector(
			std::min(Start[0] + Size[0], Operand.Start[0] + Operand.Size[0]),
			std::min(Start[1] + Size[1], Operand.Start[1] + Operand.Size[1])) -
		Out.Start;

	return Out;
}

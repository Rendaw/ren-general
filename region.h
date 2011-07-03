#ifndef region_h
#define region_h

#include "vector.h"

struct Region // For transformations between portals and
{
	Region(void);
	Region(const FlatVector &Start, const FlatVector &Size);

	String AsString(void) const;

	bool Valid(void) const;

	FlatVector Start, Size;

	bool Contains(const FlatVector &Point) const;
	FlatVector Transform(const FlatVector &Point, const Region &ToRegion) const;
	FlatVector Scale(const FlatVector &Area, const Region &ToRegion) const;
	Region Transform(const Region &Subregion, const Region &ToRegion) const;
	FlatVector Constrain(const FlatVector &Point) const;
	
	Region Translate(const FlatVector &Offset) const;

	// Subregion operator -- takes a relative subregion (0-1, 0-1) and determines its
	// non-relative coordinates.
	Region operator *(const Region &RelativeSubregion) const;
	Region operator *(const float &Scale) const;
	Region operator +(const Region &Operand) const; // Including region operator
	Region operator +(const FlatVector &Operand) const;
	Region operator +=(const Region &Operand);
	Region operator +=(const FlatVector &Operand);

	Region Intersect(const Region &Operand) const;
};

#endif

#ifndef exception_h
#define exception_h

#include "string.h"

namespace Error
{

class Construction
{
	/// Indicates that construction of an object could not complete for some reason.  This will only be thrown from constructors.  It is the only exception thrown from constructors.
	public:
		Construction(String const &Explanation);
		String Explanation;
};

class Validation
{
	public:
		Validation(String const &Explanation);
		String Explanation;
};

class System
{
	/// Indicates that some system operation failed or the system provided bad input (non user-provided input).
	public:
		System(String const &Explanation);
		String Explanation;
};

}

#endif

#ifndef exception_h
#define exception_h

#include "string.h"

class Failure
{
	public:
		Failure(String const &Explanation);
	
		String Explanation;
};

#endif

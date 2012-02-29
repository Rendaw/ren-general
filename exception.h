#ifndef exception_h
#define exception_h

#include "string.h"

namespace Error
{

class Construction
{
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

}

#endif

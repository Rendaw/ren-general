#ifndef exception_h
#define exception_h

#include "string.h"

namespace Error
{

class Construction
{
	/// Used if construction fails because of preventable or expected conditions.  This should always be caught and handled.
	public:
		Construction(String const &Explanation);
		String Explanation;
};

class Input
{
	/// Indicates that external input was incorrect in some way.  This should be cleanly reported but otherwise doesn't need to be worked around.
	public:
		Input(String const &Explanation);
		String Explanation;
};

class System
{
	/// Indicates that some system operation failed or the system provided bad input (non user-provided input).  This should be cleanly reported, but otherwise doesn't need to be worked around.
	public:
		System(String const &Explanation);
		String Explanation;
};

}

#endif

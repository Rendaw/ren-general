#include "exception.h"

namespace Error
{

Construction::Construction(String const &Explanation) :
	Explanation(Explanation)
	{}

Input::Input(String const &Explanation) :
	Explanation(Explanation)
	{}

System::System(String const &Explanation) :
	Explanation(Explanation)
	{}

}

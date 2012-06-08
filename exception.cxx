#include "exception.h"

namespace Error
{

Construction::Construction(String const &Explanation) :
	Explanation(Explanation)
	{}

Validation::Validation(String const &Explanation) :
	Explanation(Explanation)
	{}

System::System(String const &Explanation) :
	Explanation(Explanation)
	{}

}

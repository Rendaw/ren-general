#ifndef entanglement_h
#define entanglement_h

#include <stdlib.h>

// For parent/child (1-1) two-way referencing.  Can't be used for three way
// relationships.  Cannot be used for three way relationships.

// One entanglement acts as the original, and a second can be split from it
// using the other constructor.  If either side disappears, the other side's
// pointer will be invalidated.

template <class Linkee> class Entanglement
{
	public:
		Entanglement(Linkee *Base) :
			Base(Base), Other(NULL)
			{}

		Entanglement(Linkee *Base, Entanglement<Linkee> &Coperand) :
			Base(Base), Other(&Coperand)
		{
			if (Coperand.Other != NULL)
				Coperand.Other.Other = NULL;
			Coperand.Other = this;
			Other = Coperand;
		}

		~Entanglement(void)
		{
			if (Other != NULL) Other->Other = NULL;
		}

		// Status checking
		bool IsValid(void)
			{ return Other != NULL; }

		bool operator ! (void)
			{ return Other == NULL; }

		// Accessors of the other side
		Linkee &Get(void)
		{
			assert(Other != NULL);
			return *Other->Base;
		}

		Linkee *operator *(void)
		{
			return Other->Base;
		}

		Linkee *operator ->(void)
		{
			assert(Other != NULL);
			return Other->Base;
		}

	private:
		Linkee *Base;
		Entanglement<Linkee> *Other;
};

#endif

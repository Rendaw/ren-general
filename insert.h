#ifndef insert_h
#define insert_h

#include <stdlib.h>
#include <cassert>

template <typename Insertee> class Slot;

template <typename Insertee> class Insert
{
	public:
		Insert(Insertee *Base) : Base(Base), Active(NULL) {}
		~Insert(void) { if (Active != NULL) *Active = false; }

	protected:
		friend class Slot<Insertee>;
		Insertee *GetBase(void) { return Base; }
		void Link(bool *ActiveReference) { Active = ActiveReference; }
	private:
		Insertee *Base;
		bool *Active;
};

template <typename Insertee> class Slot
{
	public:
		Slot(void) : Filled(false) {}

		void Insert(Insert<Insertee> &Insertment)
		{
			assert(Filled == false);
			Filled = true;
			Attachment = Insertment.GetBase();
			Insertment.Link(&Filled);
		}

		bool IsValid(void) { return Filled; }
		bool operator !(void) { return !Filled; }

		Insertee *operator *(void) { return Attachment; }
		Insertee *operator ->(void) { return Attachment; }

	private:
		Insertee *Attachment;
		bool Filled;
};

#endif

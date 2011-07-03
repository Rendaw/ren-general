#ifndef publicgood_h
#define publicgood_h

#include <map>
#include <cassert>
#include <iostream>

/*
Pools manage the lifespans of widely shared objects.
*/

// POOLs manage GOODs, and provide users with ACCESSes
template <typename ReferenceType, class ItemType> class Pool;

template <typename ReferenceType, class ItemType> class PoolItem
{
	public:
		PoolItem(void) : Bindings(0) {}
		void Reserve(void) { Bindings++; }
		void Release(void) { assert(Bindings > 0); Bindings--; }
	friend class Pool<ReferenceType, ItemType>;
	protected:
		~PoolItem(void) {}
		bool ShouldBeDeleted(void) { return Bindings <= 0; }
	private:
		int Bindings;
};

template <class ItemType> class Access
{
	public:
		Access(ItemType *Accessee) : Accessee(Accessee) { Accessee->Reserve();}
		Access(const Access<ItemType> &Coperand) : Accessee(Coperand.Accessee) { Accessee->Reserve(); }
		~Access(void) { Accessee->Release(); }
		ItemType *operator ->(void) { return Accessee; }
	private:
		ItemType *Accessee;
};

template <typename ReferenceType, class ItemType> class Pool
{
	public:
		~Pool(void)
		{
			for (typename std::map<ReferenceType, ItemType *>::iterator
				CurrentItem = Items.begin(); CurrentItem != Items.end(); CurrentItem++)
			{
				assert(CurrentItem->second->ShouldBeDeleted());
				delete CurrentItem->second;
			}
		}

		Access<ItemType> *Get(const ReferenceType &Reference)
		{
			if (Items.find(Reference) == Items.end())
			{
				ItemType *Creation = new ItemType(Reference);
				Items[Reference] = Creation;

				return new Access<ItemType>(Creation);
			}
			else return new Access<ItemType>(Items[Reference]);
		}

		void Add(const ReferenceType &Reference, ItemType *Addee)
		{
			Items[Reference] = Addee;
		}

		void Prune(void)
		{
			typename std::map<ReferenceType, ItemType *>::iterator ReleasableItem;

			for (typename std::map<ReferenceType, ItemType *>::iterator
				CurrentItem = Items.begin(); CurrentItem != Items.end(); )
			{
				if (CurrentItem->second->ShouldBeDeleted())
				{
					ReleasableItem = CurrentItem;
					CurrentItem++;
					delete ReleasableItem->second;
					Items.erase(ReleasableItem);
				}
				else CurrentItem++;
			}
		}
	protected:
		std::map<ReferenceType, ItemType *> Items;
};

#endif

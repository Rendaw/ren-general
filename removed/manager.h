#ifndef manager_h
#define manager_h

#include <list>
#include <map>
#include <iostream>

// Managed objects should
// 1. Have an "Update" function
// 2. Have a "IsFinished" function, which can be used to check for "ready to 
// delete status" after an update.

template <class Managed> class ManagerBase
{
	public:
		ManagerBase(void) :
			Manageds(), Count(0), Deleted(false)
		{
		}
	
		void Clear(void)
		{
			for (typename std::list<Managed *>::iterator CurrentManaged = Manageds.begin();
				CurrentManaged != Manageds.end(); CurrentManaged++)
			{
				delete *CurrentManaged;
			}
			Manageds.clear();
			
			Count = 0;
		}
	
		virtual ~ManagerBase(void)
		{
			Clear();
		}
		
		virtual Managed *Add(Managed *New)
		{
			Count++;
			Manageds.push_back(New);
			return Manageds.back();
		}
		
		virtual void Update(void)
		{
			Deleted = false;
			
			//std::cout << "\nUpdating starting." << std::endl;
			
			for (typename std::list<Managed *>::iterator CurrentManaged = Manageds.begin();
				CurrentManaged != Manageds.end(); )
			{
				//std::cout << "Updating " << *CurrentManaged << std::endl;
			
				(*CurrentManaged)->Update();
				if ((*CurrentManaged)->IsFinished())
				{
					//std::cout << "Deleting " << *CurrentManaged << std::endl;
					typename std::list<Managed *>::iterator Delete = CurrentManaged;
					CurrentManaged++;
					delete *Delete;
					Manageds.erase(Delete);
					
					Deleted = true;
					Count--;		
				}
				else CurrentManaged++;
			}
		}
		
		bool Empty(void)
			{ return Manageds.empty(); }
			
		unsigned int Size(void)
			{ return Count; }
		
		bool DeletedItems(void) 
			{ return Deleted; }
		
		typename std::list<Managed *>::iterator Begin(void)
			{ return Manageds.begin(); }
		
		typename std::list<Managed *>::iterator End(void)
			{ return Manageds.end(); }
		
	protected:
		std::list<Managed *> Manageds;
		unsigned int Count;
		
		bool Deleted;
};

// Objects maintained by this manager cannot have explicitly defined
// lifespans.
// Requirements:
// Must have single argument constructor which takes the reference type
// (e.g. string)

template <class Managed> struct HandleDataStore
{
	Managed *Data;
	int References;
	
	Managed *operator -> (void) { return Data; }
};

template <class Managed> class Handle
{
	public:
		Handle(HandleDataStore<Managed> *NewTarget)
		{
			Target = NewTarget;
			Target->References++;
		}
		
		~Handle(void)
		{
			Target->References--;
		}
		
		Handle(const Handle<Managed> &Coperand)
		{
			Target = Coperand.Target;
			Target->References++;
		}
		
		Handle<Managed> &operator = (const Handle<Managed> &Coperand)
		{
			Target = Coperand.Target;
			Target->References++;
		}
		
		Managed *operator ->(void)
		{
			return Target->Data;
		}
		
		Managed &operator *(void)
		{
			return *Target->Data;
		}
		
	protected:
		HandleDataStore<Managed> *Target;
};

template <class Managed, class MapTo> class ResourceManagerBase
{
	public:	
		~ResourceManagerBase(void)
		{
			for (typename std::map<MapTo, HandleDataStore<Managed> >::iterator CurrentItem = Data.begin();
				CurrentItem != Data.end(); CurrentItem++)
			{
				delete CurrentItem->second.Data;
			}
		}
	
		HandleDataStore<Managed> *Request(const MapTo &Description)
		{
			if (Data.find(Description) == Data.end())
			{
				HandleDataStore<Managed> Store;
				Store.Data = new Managed(Description);
				Store.References = 0; 
				Data[Description] = Store;
			}
			return &Data[Description];
		}
		
		void Update(void) // Handles cleaning
		{
			for (typename std::map<MapTo, HandleDataStore<Managed> >::iterator CurrentItem = Data.begin();
				CurrentItem != Data.end();)
			{
				if (CurrentItem->second.References <= 0)
				{
					typename std::map<MapTo, HandleDataStore<Managed> >::iterator Delete = CurrentItem;
					CurrentItem++;
					delete Delete->second;
					Data.erase(Delete);
				}
				else CurrentItem++;
			}
		}
		
	private:
		std::map<MapTo, HandleDataStore<Managed> > Data;
};

#endif

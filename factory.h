#ifndef factorybase_h
#define factorybase_h

#include <list>
#include <cassert>
#include <iostream>

/*
The scope of the managed objects is controlled by the factory.

The factory and product are a combo class.  When products are created, hand them to the factory, which 
destroys them when the objects have determined they are ready to be destroyed.

Because the objects may not be destroyed immediately, the objects should disable themselves and do
the majority of their destruction in their Destroy function.
*/

template <class Type> class Factory
{
	public:
		Factory(bool ShouldCheckOnDestruct) : CheckOnDestruct(ShouldCheckOnDestruct)
			{}

		~Factory(void)
		{
			for (typename std::list<Type *>::iterator CurrentItem = Items.begin();
				CurrentItem != Items.end(); CurrentItem++)
			{
				if (CheckOnDestruct) assert((*CurrentItem)->ShouldBeDeleted());
				delete *CurrentItem;
			}
		}

		void AddItem(Type *ToBeManaged)
			{ Items.push_front(ToBeManaged); }

		/*void Prune(void)
		{
			typename std::list<Type *>::reverse_iterator DeleteItem;

			for (typename std::list<Type *>::reverse_iterator CurrentItem = Items.rbegin();
				CurrentItem != Items.rend();)
			{
				if ((*CurrentItem)->ShouldBeDeleted())
				{
					delete *CurrentItem;
					DeleteItem = CurrentItem;
					CurrentItem++;
					Items.erase(DeleteItem.base() - 1);
				}
				else CurrentItem++;
			}
		}*/
		
		void Clean(void)
		{
			typename std::list<Type *>::iterator DeleteItem;
			for (typename std::list<Type *>::iterator CurrentItem = Factory<Type>::Items.begin();
				CurrentItem != Factory<Type>::Items.end();)
			{
				if (!(*CurrentItem)->ShouldBeDeleted()) CurrentItem++;
				else
				{
					delete *CurrentItem;
					DeleteItem = CurrentItem;
					CurrentItem++;
					Factory<Type>::Items.erase(DeleteItem);
				}
			}
		}

		typename std::list<Type *>::iterator GetStart(void)
			{ return Items.begin(); }

		typename std::list<Type *>::iterator GetEnd(void)
			{ return Items.end(); }
	protected:
		bool CheckOnDestruct;
		typename std::list<Type *> Items;
};

// Derive from this <YourType> to have a base which you can easily delete
template <class Type> class Product
{
	protected:
		Product(void) : Deletable(false)
			{}

	private:
		bool Deletable;

	friend class Factory<Type>;
	protected:
		virtual ~Product(void)
			{}
			
		virtual void Destroy(void) {}

	public:
		void Delete(void)
			{ Deletable = true; Destroy(); }
			
		bool ShouldBeDeleted(void)
			{ return Deletable; }
};

// ========================================================================
// Some implementations of things

// The activefactory provides a update method which can perform an operation
// on all contained products, while cleaning.
template <class Type> class ActiveFactory : public Factory<Type>
{
	public:
		ActiveFactory(bool ShouldCheckOnDestruct) : Factory<Type>(ShouldCheckOnDestruct)
			{}
		
		void Update(void)
		{
			/*typename std::list<Type *>::reverse_iterator DeleteItem;

			for (typename std::list<Type *>::reverse_iterator CurrentItem = Factory<Type>::Items.rbegin();
				CurrentItem != Factory<Type>::Items.rend();)
			{
				if (!(*CurrentItem)->ShouldBeDeleted())
				{
					UpdateProduct(*CurrentItem);
					
					CurrentItem++;
				}
				else
				{
					delete *CurrentItem;
					DeleteItem = CurrentItem;
					CurrentItem++;
					Factory<Type>::Items.erase(DeleteItem.base() - 1);
				}
			}*/
			
			typename std::list<Type *>::iterator DeleteItem;
			for (typename std::list<Type *>::iterator CurrentItem = Factory<Type>::Items.begin();
				CurrentItem != Factory<Type>::Items.end();)
			{
				if (!(*CurrentItem)->ShouldBeDeleted())
				{
					UpdateProduct(*CurrentItem);
					
					CurrentItem++;
				}
				else
				{
					delete *CurrentItem;
					DeleteItem = CurrentItem;
					CurrentItem++;
					Factory<Type>::Items.erase(DeleteItem);
				}
			}
		}
		
	protected:
		virtual void UpdateProduct(Type *Updatee) = 0;
};

#endif

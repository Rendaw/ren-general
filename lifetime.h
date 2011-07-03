#ifndef lifetime_h
#define lifetime_h

#include <stdlib.h>
#include <cassert>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <deque>
#include <algorithm>

#include <iostream>

template <class Type> class Anchor
{
	private:
		Type *Data;
	public:
		Anchor(Type *Data) : Data(Data) {}
		~Anchor(void) { delete Data; }
		operator Type*(void) { return Data; }
		Type *operator *(void) { return Data; }
		Type *operator ->(void) { assert(Data != NULL); return Data; }
		bool operator ==(void *Other) { return Data == Other; }
		bool operator !=(void *Other) { return Data != Other; }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////// Deleter - deletes members
template <class Type, template<class Retype, class = std::allocator<Retype> > class Container> class DeleterBase : public Container<Type *>
{
	public:
		~DeleterBase(void)
		{
			for (typename Container<Type *>::iterator CurrentElement = Container<Type *>::begin();
				CurrentElement != Container<Type *>::end(); CurrentElement++)
				delete *CurrentElement;
		}

		void clear(void)
		{
			for (typename Container<Type *>::iterator CurrentElement = Container<Type *>::begin();
				CurrentElement != Container<Type *>::end(); CurrentElement++)
				delete *CurrentElement;
			Container<Type *>::clear();
		}

		void flush(void)
		{
			Container<Type *>::clear();
		}

		typename Container<Type *>::iterator erase(typename Container<Type *>::iterator Value)
		{
			delete *Value;
			return Container<Type *>::erase(Value);
		}

		//bool erase(Type *Value)
		bool erase(Type *Value)
		{
			typename Container<Type *>::iterator Found =
				find(Container<Type *>::begin(), Container<Type *>::end(), Value);
			if (Found != Container<Type *>::end())
			{
				delete *Found;
				Container<Type *>::erase(Found);
				return true;
			}
			else return false;
		}

		/*typename Container<Type *>::iterator remove(typename Container<Type *>::iterator Value)
		{
			return Container<Type *>::erase(Value);
		}*/

		/*bool remove(Type *Value)
		{
			typename Container<Type *>::iterator Found =
				find(Container<Type *>::begin(), Container<Type *>::end(), Value);
			if (Found != Container<Type *>::end())
			{
				Container<Type *>::erase(Found);
				return true;
			}
			else return false;
		}*/
};

template <class Type> class DeleterList : public DeleterBase<Type, std::list> {};
template <class Type> class DeleterVector : public DeleterBase<Type, std::vector> {};

template <class Type> class DeleterSet : public std::set<Type *, std::less<Type *>, std::allocator<Type> >
{
	public:
		typedef std::set<Type *, std::less<Type *>, std::allocator<Type> > Container;

		~DeleterSet(void)
		{
			for (typename Container::iterator CurrentElement = Container::begin();
				CurrentElement != Container::end(); CurrentElement++)
				delete *CurrentElement;
		}

		void clear(void)
		{
			for (typename Container::iterator CurrentElement = Container::begin();
				CurrentElement != Container::end(); CurrentElement++)
				delete *CurrentElement;
			Container::clear();
		}

		typename Container::iterator erase(typename Container::iterator Value)
		{
			delete *Value;
			return Container::erase(Value);
		}

		bool erase(Type *Value)
		{
			typename Container::iterator Found = Container::find(Value);
			if (Found != Container::end())
			{
				delete *Found;
				Container::erase(Found);
				return true;
			}
			else return false;
		}

		typename Container::iterator remove(typename Container::iterator Value)
		{
			return Container::erase(Value);
		}

		bool remove(Type *Value)
		{
			typename Container::iterator Found = Container::find(Value);
			if (Found != Container::end())
			{
				Container::erase(Found);
				return true;
			}
			else return false;
		}
};

template <class Type, class Base = std::deque<Type *> > class DeleterQueue : public std::queue<Type *, Base>
{
	public:
		typedef std::queue<Type *, Base> Container;

		~DeleterQueue(void)
		{
			while (!Container::empty())
				{ delete Container::front(); Container::pop(); }
		}

		void clear(void)
		{
			while (!Container::empty())
				{ delete Container::front(); Container::pop(); }
		}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////// Cleaner - deletes members ready to die
template <class BaseType> class Cleaner : private std::vector<BaseType *>
{
	public:
		class Item // Easy base for members
		{
			public:
				Item(void) : Destroyed(false) {}
				void Destroy(void) { Destroyed = true; }
			protected:
				friend class Cleaner<BaseType>;
				bool HasExpired(void) { return Destroyed; }
			private:
				bool Destroyed;
		};

	public:
		using std::vector<BaseType *>::begin;
		using std::vector<BaseType *>::end;
		using std::vector<BaseType *>::front;
		using std::vector<BaseType *>::back;
		using std::vector<BaseType *>::push_back;
		using std::vector<BaseType *>::insert;
		using std::vector<BaseType *>::size;
		using std::vector<BaseType *>::empty;

		using std::vector<BaseType *>::iterator;

		Cleaner(bool Warn = false) : Warn(Warn) {}

		virtual ~Cleaner(void)
		{
			for (typename std::vector<BaseType *>::iterator Current = begin(); Current != end(); Current++)
			{
				assert(!(Warn && (*Current)->HasExpired()));
				delete *Current;
			}
		}

		void Purge(void)
		{
			for (typename std::vector<BaseType *>::iterator Current = begin(); Current != end(); )
			{
				if ((*Current)->HasExpired())
				{
					delete *Current;
					Current = erase(Current);
				}
				else Current++;
			}
		}

		void Update(void)
		{
			for (typename std::vector<BaseType *>::iterator Current = begin(); Current != end(); )
			{
				if ((*Current)->HasExpired())
				{
					delete *Current;
					Current = erase(Current);
				}
				else
				{
					UpdateItem(*Current);
					Current++;
				}
			}
		}

	protected: virtual void UpdateItem(BaseType *Item) {}
	private: bool Warn;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////// Sink - removes items that were deleted elsewhere automatically
// "and the kitchen sink" + "synchronizes"
namespace Sink
{
	class Wrapper;
	class Item
	{
		public:
			~Item(void)
			{
				for (std::vector<bool *>::iterator CurrentSignal = ExpirationSignals.begin();
					CurrentSignal != ExpirationSignals.end(); CurrentSignal++)
					**CurrentSignal = true;
			}

		protected:
			friend class Wrapper;

			void AddExpirationSignal(bool *Expired)
				{ ExpirationSignals.push_back(Expired); }

			void RemoveExpirationSignal(bool *Expired)
			{
				std::vector<bool *>::iterator Found =
					std::find(ExpirationSignals.begin(), ExpirationSignals.end(), Expired);
				assert(Found != ExpirationSignals.end());
				ExpirationSignals.erase(Found);
			}
		private:
			std::vector<bool *> ExpirationSignals;
	};

	class Wrapper
	{
		public:
			Wrapper(Item *Target) : Target(Target), Expired(false)
				{ Target->AddExpirationSignal(&Expired); }

			~Wrapper(void)
				{ if (!Expired) Target->RemoveExpirationSignal(&Expired); }

		protected:
			Item *Target;
			bool Expired;
	};

	template <typename BaseType> class Manager;
	template <typename BaseType> class TypedWrapper : public Wrapper
	{
		public:
			TypedWrapper(BaseType *Data) : Wrapper(Data),
				Data(Data)
				{}

			BaseType *operator ->(void) { return Data; }
			BaseType *operator *(void) { return Data; }
		protected:
			friend class Manager<BaseType>;
			bool HasExpired(void) const { return Expired; }

		private:
			BaseType *Data;
	};

	template <typename BaseType> class Manager : private std::vector<TypedWrapper<BaseType> *>
	{
		public:
			using std::vector<TypedWrapper<BaseType> *>::begin;
			using std::vector<TypedWrapper<BaseType> *>::end;
			using std::vector<TypedWrapper<BaseType> *>::front;
			using std::vector<TypedWrapper<BaseType> *>::back;
			using std::vector<TypedWrapper<BaseType> *>::size;
			using std::vector<TypedWrapper<BaseType> *>::empty;

			using std::vector<TypedWrapper<BaseType> *>::iterator;

			void push_back(BaseType *Managee)
				{ std::vector<TypedWrapper<BaseType> *>::push_back(new TypedWrapper<BaseType>(Managee)); }

			void Purge(void)
			{
				for (typename std::vector<TypedWrapper<BaseType> *>::iterator CurrentItem = begin();
					CurrentItem != end(); )
					if ((*CurrentItem)->HasExpired())
					{
						delete *CurrentItem;
						CurrentItem = erase(CurrentItem);
					}
					else CurrentItem++;
			}

			~Manager(void)
			{
				for (typename std::vector<TypedWrapper<BaseType> *>::iterator CurrentItem = begin();
					CurrentItem != end(); CurrentItem++)
					delete *CurrentItem;
			}
		protected:
			struct Wrapper : public Sink::Wrapper { BaseType *Data; };
	};
}

#endif

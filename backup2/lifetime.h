#ifndef lifetime_h
#define lifetime_h

#include <stdlib.h>
#include <cassert>
#include <vector>
#include <list>
#include <algorithm>

#include <iostream>

// Deletes members when deleted
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

		typename Container<Type *>::iterator erase(typename Container<Type *>::iterator Value)
		{
			delete *Value;
			return Container<Type *>::erase(Value);
		}
};

template <class Type> struct Deleter
{
	typedef DeleterBase<Type, std::list> List;
	typedef DeleterBase<Type, std::vector> Vector;
};


class SinkItem
{
	public:
};

class SinkSingleItem : public SinkItem
{
};

class SinkMultiItem : public SinkItem
{
};

class SinkWrapperBase
{
	public:
		SinkWrapperBase(SinkItemBase *Target) : Target(Target)
			{}

		~SinkWrapperBase(void)
			{ Target->RemoveDeathSignal(&Dead); }

	private:
		SinkItemBase *Target;
		bool Dead;
};

// Lol - it is a collection (put it in the sink), and it synchronizes items
namespace Sink
{
	class Wrapper;
	class Item
	{
		protected:
			friend class Wrapper;
			void AddExpirationSignal(bool *Expired);
			void RemoveExpirationSignal(bool *Expired);

		private:
			std::vector<bool *> ExpirationSignals;
	};

	class Wrapper
	{
		public:
			SinkWrapperBase(Item *Target);
			~SinkWrapperBase(void);

		protected:
			Item *Target;
			bool Expired;
	};

	template <typename BaseType> class Manager;
	template <typename BaseType> class TypedWrapper : public Wrapper
	{
		public: BaseType *Data;
		protected:
			friend class Manager<BaseType>;
			bool IsDead(void) const { return Expired; }
	}

	template <typename BaseType> class Manager : private std::vector<TypedWrapper<BaseType> *>
	{
		public:
			using std::vector<TypedWrapper<BaseType> *>::begin();
			using std::vector<TypedWrapper<BaseType> *>::end();
			using std::vector<TypedWrapper<BaseType> *>::size();

			void Add(Base *Managee)
				{ push_back(new WrapperType(Managee)); }

			void Purge(void)
			{
				for (iterator CurrentItem = begin(); CurrentItem != end(); CurrentItem++)
					if (CurrentItem->IsDead())
					{
						delete *CurrentItem;
						CurrentItem = erase(CurrentItem);
					}
			}

			~Manager(void)
			{
				for (iterator CurrentItem = begin(); CurrentItem != end(); CurrentItem++)
				delete *CurrentItem;
				CurrentItem = erase(CurrentItem);
			}
		protected:
			struct Wrapper : public Sink::Wrapper { BaseType *Data; };
	};
}

/* A ufference of regliness
Lifetime::Manager
Lifetime::External::Manager, Lifetime::External::Base
Lifetime::External::DefaultManager, Lifetime::External::Base
*/

/* The lifetime management classes are lists (vectors, actually) that handle cleanup
of objects whose lifetimes may or may not be controlled internally. Duh.

The lifetime management classes extend some stl base classs so they are easy
enough to use. They provide a custom iterators, the Manerators, to hide the wrapper
junk that goes on to contain the managed objects.

All lifetime management classes have a member called Update.  Update will call
a virtual member called UpdateMember on each member.  The virtual member
function UpdateMember can do whatever you want it to do.  It is virtual.
*/

namespace Lifetime
{
	/* The lifetime manager will delete (and, optionally warn about) any objects on its death,
	and delete objects that are tagged, otherwise.

	Managed objects must not be deleted manually.

	Managed objects must have a member called IsDead that the manager can access, to determine
	if the objects should be deleted or not.

	When manually traversing the manager, if any dead elements are found, the data they
	reference must be deleted, and the reference must be removed manually.
	*/

	template <typename Base> class Manager;

	template <typename BaseType> class Base
	{
		public:
			Base(void) : Destroyed(false) {}
			void Destroy(void) { Destroyed = true; }
		protected:
			friend class Manager<BaseType>;
			bool IsDead(void) { return Destroyed; }
		private:
			bool Destroyed;
	};

	template <typename Base> class Manager : public std::vector<Base *>
	{
		public:
			class Manerator : public Manager::iterator
			{
				public:
					Base *operator->(void)
						{ return Manager::iterator::operator*(); }
					Base &operator*(void)
						{ return *Manager::iterator::operator*(); }
			};

			Manager(bool Warn) : Warn(Warn) {}

			virtual ~Manager(void)
			{
				assert(!Warn || this->empty());
				for (typename Manager::iterator CurrentElement = this->begin(); CurrentElement != this->end(); CurrentElement++)
					delete *CurrentElement;
			}

			void Purge(void)
			{
				for (typename Manager::iterator CurrentElement = this->begin(); CurrentElement != this->end(); )
					if ((*CurrentElement)->IsDead())
					{
						delete *CurrentElement;
						CurrentElement = this->erase(CurrentElement);
					}
					else CurrentElement++;
			}

			void Update(void)
			{
				for (typename Manager::iterator CurrentElement = this->begin(); CurrentElement != this->end(); )
					if ((*CurrentElement)->IsDead)
					{
						delete *CurrentElement;
						CurrentElement = this->erase(CurrentElement);
					}
					else
					{
						UpdateMember(*CurrentElement);
						CurrentElement++;
					}
			}

		protected:
			virtual void UpdateMember(Base *Target) {}

		private:
			bool Warn;
	};


	namespace External
	{
		/* The external lifetime manager does not delete any of the objects it manages.

		If an object is deleted elsewhere, the list will automatically cull it if
		it is "purged" with holy fire.  Otherwise, one should use the Manerator's
		IsDead check first, and delete it manually (remove the iterator) if it is dead.

		All managed objects must extend External::Base or External::Multibase or
		something.
		*/

		template <typename Base> class Wrapper;

		template <typename Derived> class Base
		{
			public:
				Base(void) : DeathSignal(NULL) {}
				~Base(void) { *DeathSignal = false; }

			protected:
				friend class Wrapper<Derived>;
					void AddDeathSignal(bool *NewDeathSignal)
						{ assert(DeathSignal == NULL); DeathSignal = NewDeathSignal; }

					void RemoveDeathSignal(void)
						{ assert(DeathSignal != NULL); DeathSignal = NULL; }

			private:
				bool *DeathSignal;

		};

		template <typename Derived> class Multibase
		{
			public:
				~Multibase(void)
				{
					for (std::vector<bool *>::iterator CurrentSignal = DeathSignals.begin();
						CurrentSignal != DeathSignals.end(); CurrentSignal++)
						**CurrentSignal = true;
				}

			protected:
				friend class Wrapper<Derived>;
					void AddDeathSignal(bool *NewDeathSignal)
					{
						DeathSignals.push_back(NewDeathSignal);
						std::cout << "Adding NDS " << (void *)NewDeathSignal << std::endl;
						for (std::vector<bool *>::iterator Foundling = DeathSignals.begin();
							Foundling != DeathSignals.end(); Foundling++)
							std::cout << "HOS " << (void *)*Foundling << std::endl;
					}

					void RemoveDeathSignal(bool *OldDeathSignal)
					{
						std::vector<bool *>::iterator Found =
							std::find(DeathSignals.begin(), DeathSignals.end(), OldDeathSignal);

						std::cout << "Has " << DeathSignals.size() << " signals of deathington." << std::endl;
						for (std::vector<bool *>::iterator Foundling = DeathSignals.begin();
							Foundling != DeathSignals.end(); Foundling++)
							std::cout << "HOS " << (void *)*Foundling << std::endl;
						std::cout << "ODS is " << (void *)OldDeathSignal << std::endl;
						assert(Found != DeathSignals.end());
						std::cout << "Now, has " << DeathSignals.size() << " signals of deathington." << std::endl;
						DeathSignals.erase(Found);
					}

			private:
				std::vector<bool *> DeathSignals;
		};

		template <typename Base, typename WrapperType> class Manager;

		template <typename Base> class Wrapper
		{
			public:
				Base *Data;

				bool IsDead(void)
					{ return Dead; }

			protected:
				friend class Manager<Base, Wrapper<Base> >;
				friend class Lifetime::Manager<Wrapper<Base> >;

				Wrapper(Base *Data) : Data(Data), Dead(false)
					{ Data->AddDeathSignal(&Dead); }

				~Wrapper(void)
					{ if (!Dead) Data->RemoveDeathSignal(&Dead); }

			private:
				bool Dead;
		};

		template <typename Base, typename WrapperType> class Manager : public Lifetime::Manager<WrapperType>
		{
			public:
				// Types
				class Manerator : public Lifetime::Manager<WrapperType>::iterator
				{
					public:
						Base *operator->(void)
							{ return (Manager::iterator::operator*())->Data; }
						Base &operator*(void)
							{ return *(Manager::iterator::operator*())->Data; }

						bool IsDead(void) const
							{ return (Manager::iterator::operator*())->Dead; }
				};

				Manager(void) : Lifetime::Manager<WrapperType>(false)
					{}

				void Add(Base *Managee)
					{ push_back(new WrapperType(Managee)); }
		};

		// The conclusion of a god-awful mess.
		template <typename Base> struct DefaultManager : public Manager<Base, Wrapper<Base> > {};
	}
}

#endif

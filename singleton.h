#ifndef singleton_h
#define singleton_h

#include <cstdlib>
#include <cassert>

/* USAGE

class Classname : public Singleton<Classname>
{
	friend class Singleton<Classname>;
	protected: Classname(void);
	public: ...
	private: ...
};

*/

template <class Derived> class Singleton
{
	public:
		~Singleton() {}

		static void Start(void)
		{
			if (Instance != NULL) return;
			Instance = new Derived;
		}

		static void Stop(void)
		{
			if (Instance != NULL) delete Instance;
			Instance = NULL;
		}

		static Derived &Get(void)
		{
			assert(Instance != NULL);
			return *Instance;
		}
	protected:
		Singleton() {}
		
	private:
		static Derived *Instance;
};

template <class Derived> Derived *Singleton<Derived>::Instance = NULL;

#endif

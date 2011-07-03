#ifndef statemachine_h
#define statemachine_h

namespace State
{

class Base { public: virtual ~Base(void) {}; virtual void Update(void) = 0; };

class Machine
{
	public:
		Machine(void);
		~Machine(void);
		
		void Update(void);
		bool IsDone(void);

		void End(void);
		void SetState(Base *NewState);
	private:
		Base *CurrentState;
		Base *NextState;
};

}

#endif

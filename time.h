#ifndef time_h
#define time_h

#include "singleton.h"

class Clock : public Singleton<Clock>
{
	// This class controls the flow of time, and allows others to query it
	// through its Step function and Singleton interface.
	friend class Singleton<Clock>;
	protected: 
		Clock(void);
		~Clock(void);
	public:
		void UpdateDifference(const unsigned int MillisecondStep);
		void UpdateAbsolute(const unsigned int Milliseconds);

		float Step(void) const;
		unsigned int Milliseconds(void);
		float UnpausedSeconds(void);

		void Pause(void);
		void Unpause(void);

	private:
		float TimeStep;
		unsigned int LastTime, CurrentTime;
		unsigned int Unpaused;

		int Paused;
};


class Timer // A beautifully streamlined timer class
{
	// This class is an encapsulation of all the junk that's required
	// to have, say, a physics update frequency, or a graphics update frequency,
	// or something like that.

	// Example:
	// Timer Nobuk;
	// Nobuk.SetPauseClock(0.25f);
	// -or-
	// Nobuk.SetUpdateFrequency(4);
	// ...
	// SomeOtherObject Booj;
	// ...
	// float ElapsedTime;
	// if (Nobuk.Update(ElapsedTime)) Booj.Update(ElapsedTime);

	public:
		Timer(void);
		~Timer(void);

		void SetPauseTime(float NewPauseTime);
		void SetUpdateFrequency(float Frequency);
		bool Update(void);
		float Percent(void) const; // Percent of time until next update passed
		float Elapsed(void) const; // Time elapsed...
		float Remaining(void) const; // Time remaining

	private:
		float PauseTime;
		float ElapsedTime;
};

#endif

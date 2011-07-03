#include "time.h"

#include <iostream>

// Base time system
Clock::Clock(void) : TimeStep(0), LastTime(0), CurrentTime(0), 
		Unpaused(0), Paused(0)
	{}

Clock::~Clock(void)
	{}

void Clock::UpdateDifference(const unsigned int MillisecondStep)
{
	if (Paused) return;

	LastTime = CurrentTime;
	CurrentTime += MillisecondStep;
	TimeStep = float(CurrentTime - LastTime) / 1000.0f;
	
	Unpaused += CurrentTime - LastTime;
}

void Clock::UpdateAbsolute(const unsigned int Milliseconds)
{
	if (Paused) return;

	LastTime = CurrentTime;
	CurrentTime = Milliseconds;
	TimeStep = float(CurrentTime - LastTime) / 1000.0f;
	
	Unpaused += CurrentTime - LastTime;
}

float Clock::Step(void) const
	{ return (!Paused) ? TimeStep : 0.0f; }

unsigned int Clock::Milliseconds(void) 
	{ return CurrentTime; }

float Clock::UnpausedSeconds(void)
{
	return Unpaused * 0.001f;
}

void Clock::Pause(void)
{
	++Paused;
}

void Clock::Unpause(void)
{
	--Paused;
	Paused = std::max(Paused, 0);
}

// Timer wrapper thing
Timer::Timer(void)
{
	PauseTime = 0.0f;
	ElapsedTime = 0.0f;
}

Timer::~Timer(void)
{
}

void Timer::SetPauseTime(float NewPauseTime)
{
	PauseTime = NewPauseTime;
	ElapsedTime = PauseTime + 0.01f;
}

void Timer::SetUpdateFrequency(float Frequency)
{
	PauseTime = 1.0f / Frequency;
	ElapsedTime = PauseTime + 0.01f;
}

bool Timer::Update(void)
{
	ElapsedTime += Clock::Get().Step();

	if (ElapsedTime < PauseTime) return false;

	ElapsedTime = 0.0f;
	return true;
}

float Timer::Percent(void) const
{
	return ElapsedTime / PauseTime;
}

float Timer::Elapsed(void) const
{
	return ElapsedTime;
}

float Timer::Remaining(void) const
{
	return PauseTime - ElapsedTime;
}

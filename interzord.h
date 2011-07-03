#ifndef interzord_h
#define interzord_h

#include "vector.h"
#include "singleton.h"

#include <assert.h>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <iostream>

// Interzord technology
// Reclaim the world with floating point-type wrappers which interpolate between two values.
// It's like having two floats point types in one!

// Use them just like a float/other type, but be sure to call update every once and a while!

// Interpolater base for the interzord
template <typename Type> class Interpolater
{ 
	public:
		virtual ~Interpolater(void) {}; 
		virtual void Interpolate(const Type &Start, Type &Current, const Type &End, const float &Percent, float &TotalPercent) const = 0; 
		virtual Interpolater<Type> *Copy(void) const = 0;
};

namespace Interpolaters
{
	template <typename Type> class Linear : public Interpolater<Type>
	{ 
		public: 
			void Interpolate(const Type &Start, Type &Current, const Type &End, const float &Percent, float &TotalPercent) const
			{
				if (TotalPercent < 1.0f) Current += (End - Start) * Percent;
				else Current = End;
				TotalPercent = std::min(TotalPercent + Percent, 1.0f);
			}
			Interpolater<Type> *Copy(void) const { return new Linear<Type>; }
	};
	
	template <typename Type> class Exponential : public Interpolater<Type>
	{
		public:
			void Interpolate(const Type &Start, Type &Current, const Type &End, const float &Percent, float &TotalPercent) const
			{
				Current += (End - Current) * Percent;
			}
			Interpolater<Type> *Copy(void) const { return new Exponential<Type>; }
	};
	
	template <typename Type> class Step : public Interpolater<Type>
	{
		public:
			void Interpolate(const Type &Start, Type &Current, const Type &End, const float &Percent, float &TotalPercent) const
			{
				if (TotalPercent < 0.5f) Current = Start;
				else Current = End;
				TotalPercent = std::min(TotalPercent + Percent, 1.0f);
			}
			Interpolater<Type> *Copy(void) const { return new Step<Type>; }
	};
	
	template <typename Type> class Parabola : public Interpolater<Type>
	{
		public:
			void Interpolate(const Type &Start, Type &Current, const Type &End, const float &Percent, float &TotalPercent) const
			{
				if (TotalPercent < 1.0f) Current = Start + (End - Start) * (-pow(TotalPercent * 2 - 1, 2) + 1);
				else Current = Start;
				TotalPercent = std::min(TotalPercent + Percent, 1.0f);
			}
			Interpolater<Type> *Copy(void) const { return new Parabola<Type>; }
	};
	
	template <typename Type> class SineSpeedDown : public Interpolater<Type>
	{
		public:
			void Interpolate(const Type &Start, Type &Current, const Type &End, const float &Percent, float &TotalPercent) const
			{
				TotalPercent = std::min(TotalPercent + Percent, 1.0f);
				if (TotalPercent < 1.0f) Current = Start + (End - Start) * sin(TotalPercent * 3.14159f * 0.5f);
				else Current = End;
			}
			Interpolater<Type> *Copy(void) const { return new SineSpeedDown<Type>; }
	};
}

// The actual interzord type!
template <typename Type> class Interzord
{
	public:
		Interzord(void) : // Should be explicitly initialized before usage
			Start(Type()), Current(Type()), End(Type()), TotalPercent(0), Zord(NULL)
		{}
	
		Interzord(const Type &StartValue, const Interpolater<Type> *MyZord) :
			Start(StartValue), Current(StartValue), End(StartValue), 
			TotalPercent(0.0f), Zord(MyZord)
		{ assert(MyZord != NULL); }
		
		Interzord(const Interzord<Type> &Coperand) :
			Start(Coperand.Start), Current(Coperand.Current), End(Coperand.End), 
			TotalPercent(Coperand.TotalPercent), Zord(Coperand.Zord)
		{}
		
		~Interzord(void)
		{ 
			if (Zord != NULL) delete Zord;
		}
	
		Interzord<Type> &operator = (const Interzord<Type> &Coperand)
		{
			if (Zord != NULL) delete Zord;
			Start = Coperand.Start;
			Current = Coperand.Current;
			End = Coperand.End;
			TotalPercent = Coperand.TotalPercent;
			Zord = Coperand.Zord->Copy();
			return *this;
		}
		
		Type operator = (const Type &NewValue)
		{
			Start = Current;
			End = NewValue;
			TotalPercent = 0.0f;
			return Current;
		}
		
		//operator Type (void) const { return Current; }
		operator const Type &(void) const { return Current; }
		Type operator * (void) const { return Current; }

		void Update(const float &Percent) { assert(Zord != NULL); Zord->Interpolate(Start, Current, End, Percent, TotalPercent); }
		
		Type Target(void) const { return End; }
		
		void Jump(void) { TotalPercent = 1; Current = End; }
	
	private:
		Type Start, Current, End;
		float TotalPercent;
		const Interpolater<Type> *Zord;
};

// Creator functions
inline Interzord<float> Linezord(const float &StartValue = 0) { return Interzord<float>(StartValue, new Interpolaters::Linear<float>()); }
inline Interzord<Vector> Linezord(const Vector &StartValue = Vector()) { return Interzord<Vector>(StartValue, new Interpolaters::Linear<Vector>()); }
inline Interzord<FlatVector> Linezord(const FlatVector &StartValue = FlatVector()) { return Interzord<FlatVector>(StartValue, new Interpolaters::Linear<FlatVector>()); }
inline Interzord<float> Expozord(const float &StartValue = 0) { return Interzord<float>(StartValue, new Interpolaters::Exponential<float>()); }
inline Interzord<Vector> Expozord(const Vector &StartValue = Vector()) { return Interzord<Vector>(StartValue, new Interpolaters::Exponential<Vector>()); }
inline Interzord<FlatVector> Expozord(const FlatVector &StartValue = FlatVector()) { return Interzord<FlatVector>(StartValue, new Interpolaters::Exponential<FlatVector>()); }
inline Interzord<float> Stepozord(const float &StartValue = 0) { return Interzord<float>(StartValue, new Interpolaters::Step<float>()); }
inline Interzord<Vector> Stepozord(const Vector &StartValue = Vector()) { return Interzord<Vector>(StartValue, new Interpolaters::Step<Vector>()); }
inline Interzord<FlatVector> Stepozord(const FlatVector &StartValue = FlatVector()) { return Interzord<FlatVector>(StartValue, new Interpolaters::Step<FlatVector>()); }
inline Interzord<float> Parabolord(const float &StartValue = 0) { return Interzord<float>(StartValue, new Interpolaters::Parabola<float>()); }
inline Interzord<Vector> Parabolord(const Vector &StartValue = Vector()) { return Interzord<Vector>(StartValue, new Interpolaters::Parabola<Vector>()); }
inline Interzord<FlatVector> Parabolord(const FlatVector &StartValue = FlatVector()) { return Interzord<FlatVector>(StartValue, new Interpolaters::Parabola<FlatVector>()); }
inline Interzord<float> SprintingSinozord(const float &StartValue = 0) { return Interzord<float>(StartValue, new Interpolaters::SineSpeedDown<float>()); }
inline Interzord<Vector> SprintingSinozord(const Vector &StartValue = Vector()) { return Interzord<Vector>(StartValue, new Interpolaters::SineSpeedDown<Vector>()); }
inline Interzord<FlatVector> SprintingSinozord(const FlatVector &StartValue = FlatVector()) { return Interzord<FlatVector>(StartValue, new Interpolaters::SineSpeedDown<FlatVector>()); }

#endif

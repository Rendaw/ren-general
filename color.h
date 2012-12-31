#ifndef color_h
#define color_h

#include <map>

#include "string.h"
#include "range.h"

class Color
{
	public:
		constexpr Color(void) : Red(1), Green(1), Blue(1), Alpha(1) {}
		constexpr Color(const Color &Coperand) :
			Red(Coperand.Red), Green(Coperand.Green), Blue(Coperand.Blue), Alpha(Coperand.Alpha) {}
		constexpr Color(float NewRed, float NewGreen, float NewBlue, float NewAlpha = 1.0f) :
			Red(NewRed), Green(NewGreen), Blue(NewBlue), Alpha(NewAlpha) {}
		constexpr Color(const Color &Coperand, float NewAlpha) :
			Red(Coperand.Red), Green(Coperand.Green), Blue(Coperand.Blue), Alpha(NewAlpha) {}
		constexpr Color(const Color &MixA, const Color &MixB, float Percent = 0.5f) :
			Red(Mix(MixA.Red, MixB.Red, Percent)),
			Green(Mix(MixA.Green, MixB.Green, Percent)),
			Blue(Mix(MixA.Blue, MixB.Blue, Percent)),
			Alpha(Mix(MixA.Alpha, MixB.Alpha, Percent)) {}
		
		String AsString(void) const;

		//void Set(void) const;

		Color operator + (const Color &Operand) const;
		Color &operator += (const Color &Operand);
		Color operator - (const Color &Operand) const;
		Color &operator -= (const Color &Operand);
		Color operator * (const float &Operand) const;
		Color operator * (const Color &Operand) const;
		Color &operator *= (const float &Operand);
		Color &operator *= (const Color &Operand);
		Color operator / (const float &Operand) const;
		Color operator / (const Color &Operand) const;
		Color &operator /= (const float &Operand);
		Color &operator /= (const Color &Operand);

		float Red, Green, Blue, Alpha;
};
Color operator * (const float &Operand, const Color &Other);

class ColorChain // Top is position 1, bottom is 0
{
	public:
		ColorChain(const Color &NewTop, const Color &NewBottom);

		void Add(float Position, const Color &NewNode);
		void AddMirrored(float Position, const Color &NewNode);
		void Mirror(void);

		Color GetColor(float Position);
	private:
		std::map<float, Color> Nodes;
};

#endif

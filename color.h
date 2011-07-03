#ifndef color_h
#define color_h

#include <map>
#include "string.h"

class Color
{
	public:
		Color(void);
		Color(const Color &Coperand);
		Color(float Red, float Green, float Blue, float Alpha = 1.0f);
		Color(const Color &Coperand, float Alpha);
		Color(const Color &MixA, const Color &MixB, float Percent = 0.5f);

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

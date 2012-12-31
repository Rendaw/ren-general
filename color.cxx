#include "color.h"

#include "inputoutput.h"

// Color //////////////////////////////////////////////////////////////////////
String Color::AsString(void) const
	{ MemoryStream Out; Out << "(C " << Red << ", " << Green << ", " << Blue << ", " << Alpha << ")"; return Out; }

Color Color::operator + (const Color &Operand) const
	{ return Color(Red + Operand.Red, Green + Operand.Green, Blue + Operand.Blue, Alpha); }
Color &Color::operator += (const Color &Operand)
	{ Red += Operand.Red; Green += Operand.Green; Blue += Operand.Blue; return *this; }
Color Color::operator - (const Color &Operand) const
	{ return Color(Red - Operand.Red, Green - Operand.Green, Blue - Operand.Blue, Alpha); }
Color &Color::operator -= (const Color &Operand)
	{ Red -= Operand.Red; Green -= Operand.Green; Blue -= Operand.Blue; return *this; }
Color Color::operator * (const float &Operand) const
	{ return Color(Red * Operand, Green * Operand, Blue * Operand, Alpha); }
Color Color::operator * (const Color &Operand) const
	{ return Color(Red * Operand.Red, Green * Operand.Green, Blue * Operand.Blue, Alpha); }
Color &Color::operator *= (const float &Operand)
	{ Red *= Operand; Green *= Operand; Blue *= Operand; return *this; }
Color &Color::operator *= (const Color &Operand)
	{ Red *= Operand.Red; Green *= Operand.Green; Blue *= Operand.Blue; return *this; }
Color Color::operator / (const float &Operand) const
	{ return Color(Red / Operand, Green / Operand, Blue / Operand, Alpha); }
Color Color::operator / (const Color &Operand) const
	{ return Color(Red / Operand.Red, Green / Operand.Green, Blue / Operand.Blue, Alpha); }
Color &Color::operator /= (const float &Operand)
	{ Red /= Operand; Green /= Operand; Blue /= Operand; return *this; }
Color &Color::operator /= (const Color &Operand)
	{ Red /= Operand.Red; Green /= Operand.Green; Blue /= Operand.Blue; return *this; }

//void Color::Set(void) const { glColor4f(Red, Green, Blue, Alpha); }

Color operator * (const float &Operand, const Color &Other)
	{ return Color(Other.Red * Operand, Other.Green * Operand, Other.Blue * Operand, Other.Alpha); }

// Color Chain type ///////////////////////////////////////////////////////////
ColorChain::ColorChain(const Color &NewTop, const Color &NewBottom)
{
	Nodes[1] = NewTop;
	Nodes[0] = NewBottom;
}

void ColorChain::Add(float Position, const Color &NewNode)
{
	Nodes[RangeF(0, 1).Constrain(Position)] = NewNode;
}

void ColorChain::AddMirrored(float Position, const Color &NewNode)
{
	Position = RangeF(0, 1).Constrain(Position);
	Nodes[Position] = NewNode;
	Nodes[1.0f - Position] = NewNode;
}

void ColorChain::Mirror(void)
{
	std::map<float, Color> NewNodes;
	for (std::map<float, Color>::iterator CurrentNode = Nodes.begin(); CurrentNode != Nodes.end(); CurrentNode++)
	{
		NewNodes[CurrentNode->first * 0.5f] = CurrentNode->second;
		NewNodes[1.0f - CurrentNode->first * 0.5f] = CurrentNode->second;
	}
	Nodes = NewNodes;
}

Color ColorChain::GetColor(float Position)
{
	if (Position <= Nodes.begin()->first) return Nodes.begin()->second;

	std::map<float, Color>::iterator LastNode = Nodes.begin();
	for (std::map<float, Color>::iterator CurrentNode = Nodes.begin(); CurrentNode != Nodes.end();
			LastNode = CurrentNode, CurrentNode++)
		if (Position <= CurrentNode->first)
			return Color(LastNode->second, CurrentNode->second,
				(Position - LastNode->first) / (CurrentNode->first - LastNode->first));

	return Nodes.rbegin()->second;
}

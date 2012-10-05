#include "vector.h"

#include <math.h>
#include <cassert>

#include "inputoutput.h"

// Vector 3D =======================================================================================
// Operators - base operations
// MEMBER CONSTRUCTORS
Vector::Vector(void)
{
	Data[0] = 0;
	Data[1] = 0;
	Data[2] = 0;
}

Vector::Vector(const Vector &Operand)
{
	*this = Operand;
}

Vector::Vector(const float x, const float y, const float z)
{
	assert(!isnan(x) && !isinf(x));
	assert(!isnan(y) && !isinf(y));
	assert(!isnan(z) && !isinf(z));
	Data[0] = x;
	Data[1] = y;
	Data[2] = z;
}

Vector::Vector(const FlatVector &Operand, const float z)
{
	assert(!isnan(z) && !isinf(z));
	Data[0] = Operand[0];
	Data[1] = Operand[1];
	Data[2] = z;
}

// TO STRINGGG
String Vector::AsString(void) const
{
	MemoryStream Out;
	Out << "(" << Data[0] << ", " << Data[1] << ", " << Data[2] << ")";
	return Out;
}

// MEMBER OPERATORS
Vector Vector::operator + (const Vector &Operand) const
{
	return Vector(Data[0] + Operand[0], Data[1] + Operand[1], Data[2] + Operand[2]);
}

Vector Vector::operator += (const Vector &Operand)
{
	return *this = *this + Operand;
}

Vector Vector::operator - (const Vector &Operand) const
{
	return Vector(Data[0] - Operand[0], Data[1] - Operand[1], Data[2] - Operand[2]);
}

Vector Vector::operator -= (const Vector &Operand)
{
	return *this = *this - Operand;
}

Vector Vector::operator * (const Vector &Operand) const
{
	return Vector(Data[0] * Operand[0], Data[1] * Operand[1], Data[2] * Operand[2]);
}

Vector Vector::operator * (const float &Scalar) const
{
	return Vector(Data[0] * Scalar, Data[1] * Scalar, Data[2] * Scalar);
}

Vector Vector::operator *= (const Vector &Operand)
{
	return *this = *this * Operand;
}

Vector Vector::operator *= (const float &Scalar)
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return *this = *this * Scalar;
}

Vector Vector::operator / (const Vector &Operand) const
{
	return Vector(Data[0] / Operand[0], Data[1] / Operand[1], Data[2] / Operand[2]);
}

Vector Vector::operator / (const float &Scalar) const
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return Vector(Data[0] / Scalar, Data[1] / Scalar, Data[2] / Scalar);
}

Vector Vector::operator /= (const Vector &Operand)
{
	return *this = *this / Operand;
}

Vector Vector::operator /= (const float &Scalar)
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return *this = *this / Scalar;
}

bool Vector::operator == (const Vector &Operand) const
{
	if ((Data[0] == Operand[0]) && (Data[1] == Operand[1]) && (Data[2] == Operand[2]))
		return true;
	else return false;
}

bool Vector::operator != (const Vector &Operand) const
{
	if ((Data[0] != Operand[0]) || (Data[1] != Operand[1]) || (Data[2] != Operand[2]))
		return true;
	else return false;
}

float &Vector::operator [] (int Element)
{
	assert(Element < 3);
	assert(Element >= 0);
	return Data[Element];
}

const float &Vector::operator [] (int Element) const
{
	assert(Element < 3);
	assert(Element >= 0);
	return Data[Element];
}

const float *Vector::operator * (void) const
{
	return Data;
}

float Vector::Length(void) const
{
	return sqrt(Data[0] * Data[0] + Data[1] * Data[1] + Data[2] * Data[2]);
}

float Vector::SquaredLength(void) const
{
	return Data[0] * Data[0] + Data[1] * Data[1] + Data[2] * Data[2];
}

Vector Vector::Normal(void) const
{
	return *this / Length();
}

void Vector::Normalize(void)
{
	*this = *this / Length();
}

// GLOBAL OPERATORS
Vector operator * (const float &Scalar, const Vector &Operand)
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return Vector(Operand[0] * Scalar, Operand[1] * Scalar, Operand[2] * Scalar);
}

/*Vector operator / (const float &Scalar, const Vector &Operand)
{
	return Vector(Scalar / Operand[0], Scalar / Operand[1], Scalar / Operand[2]);
}*/

// GLOBAL
float DotProduct(const Vector &Operand1, const Vector &Operand2)
{
	return Operand1[0] * Operand2[0] + Operand1[1] * Operand2[1] + Operand1[2] * Operand2[2];
}

Vector CrossProduct(const Vector &Operand1, const Vector &Operand2)
{
	return Vector(Operand1[1] * Operand2[2] - Operand1[2] * Operand2[1],
		Operand1[2] * Operand2[0] - Operand1[0] * Operand2[2],
		Operand1[0] * Operand2[1] - Operand1[1] * Operand2[0]);
}

// 2D Vector ==================================================================
// ============================================================================
FlatVector::FlatVector(void)
{
	Data[0] = 0.0f;
	Data[1] = 0.0f;
}

FlatVector::FlatVector(const FlatVector &Operand)
{
	Data[0] = Operand[0];
	Data[1] = Operand[1];
}

FlatVector::FlatVector(const float x, const float y)
{
	assert(!isnan(x) && !isinf(x));
	assert(!isnan(y) && !isinf(y));
	Data[0] = x;
	Data[1] = y;
}

FlatVector::FlatVector(const Vector &Operand)
{
	Data[0] = Operand[0];
	Data[1] = Operand[1];
}

String FlatVector::AsString(void) const
{
	MemoryStream Out;
	Out << "(" << Data[0] << ", " << Data[1] << ")";
	return Out;
}

FlatVector FlatVector::operator + (const FlatVector &Operand) const
{
	return FlatVector(Data[0] + Operand[0], Data[1] + Operand[1]);
}

FlatVector FlatVector::operator += (const FlatVector &Operand)
{
	return *this = *this + Operand;
}

FlatVector FlatVector::operator - (const FlatVector &Operand) const
{
	return FlatVector(Data[0] - Operand[0], Data[1] - Operand[1]);
}

FlatVector FlatVector::operator -= (const FlatVector &Operand)
{
	return *this = *this - Operand;
}

FlatVector FlatVector::operator * (const FlatVector &Operand) const
{
	return FlatVector(Data[0] * Operand[0], Data[1] * Operand[1]);
}

FlatVector FlatVector::operator * (const float &Scalar) const
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return FlatVector(Data[0] * Scalar, Data[1] * Scalar);
}

FlatVector FlatVector::operator *= (const FlatVector &Operand)
{
	return *this = *this * Operand;
}

FlatVector FlatVector::operator *= (const float &Scalar)
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return *this = *this * Scalar;
}

FlatVector FlatVector::operator / (const FlatVector &Operand) const
{
	return FlatVector(Data[0] / Operand[0], Data[1] / Operand[1]);
}

FlatVector FlatVector::operator / (const float &Scalar) const
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return FlatVector(Data[0] / Scalar, Data[1] / Scalar);
}

FlatVector FlatVector::operator /= (const FlatVector &Operand)
{
	return *this = *this / Operand;
}

FlatVector FlatVector::operator /= (const float &Scalar)
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return *this = *this / Scalar;
}

FlatVector FlatVector::operator - (void) const
{
	return *this * -1.0f;
}

bool FlatVector::operator == (const FlatVector &Operand) const
{
	return ((Data[0] == Operand[0]) && (Data[1] == Operand[1]) && (Data[2] == Operand[1]));
}

bool FlatVector::operator != (const FlatVector &Operand) const
{
	return ((Data[0] != Operand[0]) || (Data[1] != Operand[1]) || (Data[2] != Operand[1]));
}

float &FlatVector::operator [] (int Element)
{
	assert(Element < 2);
	assert(Element >= 0);
	return Data[Element];
}

const float &FlatVector::operator [] (int Element) const
{
	assert(Element < 2);
	assert(Element >= 0);
	return Data[Element];
}

const float *FlatVector::operator * (void) const
{
	return Data;
}

float FlatVector::Length(void) const
{
	return sqrt(Data[0] * Data[0] + Data[1] * Data[1]);
}

float FlatVector::SquaredLength(void) const
{
	return Data[0] * Data[0] + Data[1] * Data[1];
}

FlatVector FlatVector::Normal(void) const
{
	return *this / Length();
}

void FlatVector::Normalize(void)
{
	*this = *this / Length();
}

FlatVector FlatVector::QuarterRight(void)
{
	return FlatVector(Data[1], -Data[0]);
}

FlatVector FlatVector::QuarterLeft(void)
{
	return FlatVector(-Data[1], Data[0]);
}

FlatVector FlatVector::Integer(void)
{
	return FlatVector(int(Data[0]), int(Data[1]));
}

// GLOBAL OPERATORS
FlatVector operator * (const float &Scalar, const FlatVector &Operand)
{
	assert(!isnan(Scalar) && !isinf(Scalar));
	return FlatVector(Operand[0] * Scalar, Operand[1] * Scalar);
}

// GLOBAL
float DotProduct(const FlatVector &Operand1, const FlatVector &Operand2)
{
	return Operand1[0] * Operand2[0] + Operand1[1] * Operand2[1];
}

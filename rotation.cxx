#include "rotation.h"

#include <math.h>

#include "vector.h"

static inline void FixAngle(float &Value)
{
	while (Value > 360)
		Value -= 360;

	while (Value < 0)
		Value += 360;
}

////////// Angle
Angle::Angle(void)
{
	Data = 0;
}

Angle::Angle(const float &NewData)
{
	*this = NewData;
}

Angle::Angle(const Angle &NewData)
{
	*this = NewData;
}

Angle::Angle(const FlatVector &Convertee)
{
	*this = ToDegrees * atan2(Convertee[1], Convertee[0]);
}

float Angle::operator - (void) const
{
	float Output = Data + 180.0f;
	FixAngle(Output);
	return Output;
}

float Angle::operator + (const float &Operand) const
{
	float Output = Data + Operand;
	FixAngle(Output);
	return Output;
}

float Angle::operator += (const float &Operand)
{
	Data = Data + Operand;
	FixAngle(Data);
	return Data;
}

float Angle::operator - (const float &Operand) const
{
	float Output = Data - Operand;
	FixAngle(Output);
	return Output;
}

float Angle::operator -= (const float &Operand)
{
	Data = Data - Operand;
	FixAngle(Data);
	return Data;
}

float Angle::operator * (const float &Operand) const
{
	float Output = Data * Operand;
	FixAngle(Output);
	return Output;
}

float Angle::operator *= (const float &Operand)
{
	Data = Data * Operand;
	FixAngle(Data);
	return Data;
}

float Angle::operator / (const float &Operand) const
{
	float Output = Data / Operand;
	FixAngle(Output);
	return Output;
}

float Angle::operator /= (const float &Operand)
{
	Data = Data / Operand;
	FixAngle(Data);
	return Data;
}

float Angle::operator = (const float &Operand)
{
	Data = Operand;
	FixAngle(Data);
	return Data;
}

Angle::operator float() const
{
	return Data;
}

bool Angle::InRange(Angle Min, Angle Max) const
{
	if ((Min >= Max) && ((Data >= Min) || (Data <= Max))) return true;
	else if ((Data >= Min) && (Data <= Max)) return true;
	else return false;
}

float Angle::ShortestChange(Angle Goal) const
{
	float Change1 = (float)Goal - Data,
		Change2 = (float)Goal - 360.0f - Data,
		Change3 = (float)Goal - Data + 360.0f;

	if ((fabs(Change1) < fabs(Change2)) && (fabs(Change1) < fabs(Change3))) return Change1;
	else if (fabs(Change2) < fabs(Change3)) return Change2;
	else return Change3;
}

FlatVector Angle::Rotate(const FlatVector &Operand) const
{
	return FlatVector(Operand[0] * cos(Data * ToRadians) - Operand[1] * sin(Data * ToRadians),
		Operand[0] * sin(Data * ToRadians) + Operand[1] * cos(Data * ToRadians));
}

///////////////////////// QUATERNION
//// MEMBER CONSTRUCTORS
Quaternion::Quaternion(void)
{
	Data[0] = Data[1] = Data[2] = 0;
	Data[3] = 1;
}

Quaternion::Quaternion(const Quaternion &Operand)
{
	Data[0] = Operand[0];
	Data[1] = Operand[1];
	Data[2] = Operand[2];
	Data[3] = Operand[3];
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	Data[0] = x;
	Data[1] = y;
	Data[2] = z;
	Data[3] = w;
}

Quaternion::Quaternion(const Vector &Operand, const Angle &Rotation)
{
	float AngleSin = sin((Rotation * ToRadians) / 2.0f);
	Data[0] = Operand[0] * AngleSin;
	Data[1] = Operand[1] * AngleSin;
	Data[2] = Operand[2] * AngleSin;
	Data[3] = cos((Rotation * ToRadians) / 2.0f);
}

Quaternion::Quaternion(const Vector &Operand)
{
	Data[0] = Operand[0];
	Data[1] = Operand[1];
	Data[2] = Operand[2];
	Data[3] = 0;
}

//// MEMBER METHODS
Quaternion Quaternion::operator * (const Quaternion &Operand) const
{
	Vector VectorThis, VectorOperand;
	VectorThis = Vector(Data[0], Data[1], Data[2]);
	VectorOperand = Vector(Operand[0], Operand[1], Operand[2]);

	Quaternion Output(CrossProduct(VectorThis, VectorOperand) + Data[3] * VectorOperand + Operand[3] * VectorThis);
	Output[3] = Data[3] * Operand[3] - DotProduct(VectorThis, VectorOperand);
	return Output;
}

Quaternion Quaternion::operator *= (const Quaternion &Operand)
{
	return *this = *this * Operand;
}

Quaternion Quaternion::operator + (const Quaternion &Operand) const
{
	return Quaternion(Data[0] + Operand[0], Data[1] + Operand[1], Data[2] + Operand[2], Data[3] + Operand[3]);
}

Quaternion Quaternion::operator - (const Quaternion &Operand) const
{
	return Quaternion(Data[0] - Operand[0], Data[1] - Operand[1], Data[2] - Operand[2], Data[3] - Operand[3]);
}

Quaternion Quaternion::operator / (const float &Operand) const
{
	return Quaternion(Data[0] / Operand, Data[1] / Operand, Data[2] / Operand, Data[3] / Operand);
}

Quaternion Quaternion::operator * (const float &Operand) const
{
	return Quaternion(Data[0] * Operand, Data[1] * Operand, Data[2] * Operand, Data[3] * Operand);
}

/*bool Quaternion::operator == (const Quaternion &Operand) const
{
}

bool Quaternion::operator != (const Quaternion &Operand) const
{
}*/

Quaternion Quaternion::Conjugate(void) const
{
	return Quaternion(-Data[0], -Data[1], -Data[2], Data[3]);
}

Quaternion Quaternion::Normal(void) const
{
	float Total = GetLength(*this);
	//return Quaternion(Data[0] / Total, Data[1] / Total, Data[2] / Total, Data[3] / Total);
	return Quaternion(Data[0], Data[1], Data[2], Data[3]) / Total;
}

float &Quaternion::operator [] (int Element)
{
	if ((Element > 0) && (Element < 4))
		return Data[Element];
	else return Data[0];
}

const float &Quaternion::operator [] (int Element) const
{
	if ((Element > 0) && (Element < 4))
		return Data[Element];
	else return Data[0];
}

float *Quaternion::operator * (void)
{
	return Data;
}

Quaternion::operator Vector() const
{
	return Vector(Data[0], Data[1], Data[2]);
}

//// RELATED FUNCTIONS
float GetLength(const Quaternion &Operand)
{
	return sqrt(Operand[0] * Operand[0] + Operand[1] * Operand[1] + Operand[2] * Operand[2] + Operand[3] * Operand[3]);
}

float DotProduct(const Quaternion &Operand1, const Quaternion &Operand2)
{
	return Operand1[0] * Operand2[0] + Operand1[1] * Operand2[1] + Operand1[2] * Operand2[2] + Operand1[3] * Operand2[3];
}

Quaternion Interpolate(float Percent, const Quaternion &Operand1, const Quaternion &Operand2)
{
	float AngleDifference, OperandDot;
	OperandDot = DotProduct(Operand1, Operand2);

	if (fabs(OperandDot) < 0.99f)
	{
		Quaternion FixedOperand2;

		// Spherical
		if (OperandDot < 0)
			FixedOperand2 = Operand2 * -1;

		AngleDifference = acos(DotProduct(Operand1, Operand2));
		return (Operand1 * sin(AngleDifference * (1.0f - Percent)) +
			Operand2 * sin(AngleDifference * Percent)) / sin(AngleDifference);
	}
	else
	{
		Quaternion Output;

		// Linear interpolation - like with vectors
		Output = (Operand2 - Operand1) * Percent + Operand1;
		return Output.Normal();
	}
}

////////////////////////////// MATRIX
Matrix::Matrix(void)
{
	Identiate();
}

Matrix::Matrix(float E1, float E2, float E3, float E4, float E5, float E6, float E7, float E8, float E9)
{
	Elements[0] =  E1;
	Elements[1] =  E2;
	Elements[2] =  E3;
	Elements[3] =  E4;
	Elements[4] =  E5;
	Elements[5] =  E6;
	Elements[6] =  E7;
	Elements[7] =  E8;
	Elements[8] =  E9;
}

Matrix::Matrix(const Quaternion &Parent)
{
	// Top row
	Elements[0] = 1 - 2 * Parent[1] * Parent[1] - 2 * Parent[2] * Parent[2];
	Elements[3] = 2 * Parent[0] * Parent[1] - 2 * Parent[2] * Parent[3];
	Elements[6] = 2 * Parent[0] * Parent[2] + 2 * Parent[1] * Parent[3];

	// Middle row
	Elements[1] = 2 * Parent[0] * Parent[1] + 2 * Parent[2] * Parent[3];
	Elements[4] = 1 - 2 * Parent[0] * Parent[0] - 2 * Parent[2] * Parent[2];
	Elements[7] = 2 * Parent[1] * Parent[2] - 2 * Parent[0] * Parent[3];

	// Bottom row
	Elements[2] = 2 * Parent[0] * Parent[2] - 2 * Parent[1] * Parent[3];
	Elements[5] = 2 * Parent[1] * Parent[2] + 2 * Parent[0] * Parent[3];
	Elements[8] = 1 - 2 * Parent[0] * Parent[0] - 2 * Parent[1] * Parent[1];
}

void Matrix::Identiate(void)
{
	Elements[0] = 1; Elements[3] = 0; Elements[6] = 0;
	Elements[1] = 0; Elements[4] = 1; Elements[7] = 0;
	Elements[2] = 0; Elements[5] = 0; Elements[8] = 1;
}

Matrix Matrix::Transpose(void)
{
	return Matrix(
		Elements[0], Elements[3], Elements[6],
		Elements[1], Elements[4], Elements[7],
		Elements[2], Elements[5], Elements[8]);
}

void Matrix::Scale(const Vector &ScalingFactors)
{
	Elements[0] *= ScalingFactors[0];
	Elements[4] *= ScalingFactors[1];
	Elements[8] *= ScalingFactors[2];
}

Vector Matrix::operator * (const Vector &Operand) const
{
	return Vector(
		Elements[0] * Operand[0] + Elements[3] * Operand[1] + Elements[6] * Operand[2],
		Elements[1] * Operand[0] + Elements[4] * Operand[1] + Elements[7] * Operand[2],
		Elements[2] * Operand[0] + Elements[5] * Operand[1] + Elements[8] * Operand[2]);
}

Matrix Matrix::operator * (const Matrix &Operand) const
{
	return Matrix(
		// Row n by column 1
		Elements[0] * Operand[0] + Elements[3] * Operand[1] + Elements[6] * Operand[2],
		Elements[1] * Operand[0] + Elements[4] * Operand[1] + Elements[7] * Operand[2],
		Elements[2] * Operand[0] + Elements[5] * Operand[1] + Elements[8] * Operand[2],
		// Row n by column 2
		Elements[0] * Operand[3] + Elements[3] * Operand[4] + Elements[6] * Operand[5],
		Elements[1] * Operand[3] + Elements[4] * Operand[4] + Elements[7] * Operand[5],
		Elements[2] * Operand[3] + Elements[5] * Operand[4] + Elements[8] * Operand[5],
		// Row n by column 3
		Elements[0] * Operand[6] + Elements[3] * Operand[7] + Elements[6] * Operand[8],
		Elements[1] * Operand[6] + Elements[4] * Operand[7] + Elements[7] * Operand[8],
		Elements[2] * Operand[6] + Elements[5] * Operand[7] + Elements[8] * Operand[8]
	);
}

const float &Matrix::operator [] (int Element) const
{
	return Elements[Element];
}

const float *Matrix::operator * (void) const
{
	return Elements;
}

float *Matrix::GetGLMatrix() const
{
	float *OutMatrix = new float[16];
	OutMatrix[0] = Elements[0];
	OutMatrix[1] = Elements[1];
	OutMatrix[2] = Elements[2];
	OutMatrix[3] = 0;
	OutMatrix[4] = Elements[3];
	OutMatrix[5] = Elements[4];
	OutMatrix[6] = Elements[5];
	OutMatrix[7] = 0;
	OutMatrix[8] = Elements[6];
	OutMatrix[9] = Elements[7];
	OutMatrix[10] = Elements[8];
	OutMatrix[11] = 0;
	OutMatrix[12] = 0;
	OutMatrix[13] = 0;
	OutMatrix[14] = 0;
	OutMatrix[15] = 1;
	return OutMatrix;
}

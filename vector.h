#ifndef vector_h
#define vector_h

#include "string.h"

// 3D Vector class ------------------------------------------------------------
class FlatVector;

class Vector
{
	public:
		Vector(void);
		Vector(const Vector &Operand);
		Vector(const float x, const float y, const float z);
		Vector(const FlatVector &Operand, const float z = 0.0f);

		String AsString(void) const;

		Vector operator + (const Vector &Operand) const;
		Vector operator += (const Vector &Operand);
		Vector operator - (const Vector &Operand) const;
		Vector operator -= (const Vector &Operand);
		Vector operator * (const Vector &Operand) const;
		Vector operator * (const float &Scalar) const;
		Vector operator *= (const Vector &Operand);
		Vector operator *= (const float &Scalar);
		Vector operator / (const Vector &Operand) const;
		Vector operator / (const float &Scalar) const;
		Vector operator /= (const Vector &Operand);
		Vector operator /= (const float &Scalar);

		bool operator == (const Vector &Operand) const;
		bool operator != (const Vector &Operand) const;

		float &operator [] (int Element);
		const float &operator [] (int Element) const;
		const float *operator * (void) const;
		//const float *operator *(void) const;

		float Length(void) const;
		float SquaredLength(void) const;
		Vector Normal(void) const;
		void Normalize(void);

	private:
		float Data[3];
};

Vector operator * (const float &Scalar, const Vector &Operand);
//Vector operator / (const float &Scalar, const Vector &Operand);
float DotProduct(const Vector &Operand1, const Vector &Operand2);
Vector CrossProduct(const Vector &Operand1, const Vector &Operand2);


// 2D vector class ------------------------------------------------------------
class LFlatVector;

class FlatVector
{
	public:
		FlatVector(void);
		FlatVector(const FlatVector &Operand);
		FlatVector(const LFlatVector &Operand);
		//FlatVector(const float Operand[2]);
		//FlatVector(const float Operand);
		FlatVector(const float x, const float y);
		FlatVector(const Vector &Operand);

		String AsString(void) const;

		FlatVector operator + (const FlatVector &Operand) const;
		FlatVector operator += (const FlatVector &Operand);
		FlatVector operator - (const FlatVector &Operand) const;
		FlatVector operator -= (const FlatVector &Operand);
		FlatVector operator * (const FlatVector &Operand) const;
		FlatVector operator * (const float &Scalar) const;
		FlatVector operator *= (const FlatVector &Operand);
		FlatVector operator *= (const float &Scalar);
		FlatVector operator / (const FlatVector &Operand) const;
		FlatVector operator / (const float &Scalar) const;
		FlatVector operator /= (const FlatVector &Operand);
		FlatVector operator /= (const float &Scalar);

		FlatVector operator - (void) const;

		bool operator == (const FlatVector &Operand) const;
		bool operator != (const FlatVector &Operand) const;

		float &operator [] (int Element);
		const float &operator [] (int Element) const;
		const float *operator * (void) const;

		float Length(void) const;
		float SquaredLength(void) const;
		FlatVector Normal(void) const;
		void Normalize(void);

		FlatVector QuarterRight(void);
		FlatVector QuarterLeft(void);
		
		FlatVector Integer(void);

	private:
		float Data[2];
};

FlatVector operator * (const float &Scalar, const FlatVector &Operand);
float DotProduct(const FlatVector &Operand1, const FlatVector &Operand2);

#endif

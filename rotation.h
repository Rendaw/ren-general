#ifndef rotation_h
#define rotation_h

#include "vector.h"

const float Pi = 3.141592653589793238;
const float ToRadians = Pi / 180.0;
const float ToDegrees = 180.0 / Pi;

// Angle with wrapping
struct Angle
{
	Angle(void);
	Angle(const float &NewData);
	Angle(const Angle &NewData);
	Angle(const FlatVector &Convertee);

	float operator - (void) const;
	
	float operator + (const float &Operand) const;
	float operator += (const float &Operand);
	float operator - (const float &Operand) const;
	float operator -= (const float &Operand);
	float operator * (const float &Operand) const;
	float operator *= (const float &Operand);
	float operator / (const float &Operand) const;
	float operator /= (const float &Operand);
	float operator = (const float &Operand);
	operator float() const;

	bool InRange(Angle Min, Angle Max) const;
	float ShortestChange(Angle Goal) const;

	FlatVector Rotate(const FlatVector &Operand) const;

	private:
		float Data;
};

// Storage and individual rotations
class Quaternion
{
	public:
		Quaternion(void);
		Quaternion(const Quaternion &Operand);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Vector &Operand, const Angle &Rotation);
		Quaternion(const Vector &Operand);

		Quaternion operator * (const Quaternion &Operand) const;
		Quaternion operator *= (const Quaternion &Operand);

		Quaternion operator + (const Quaternion &Operand) const;
		Quaternion operator - (const Quaternion &Operand) const;
		Quaternion operator / (const float &Operand) const;
		Quaternion operator * (const float &Operand) const;

		/*bool operator == (const Quaternion &Operand) const;
		bool operator != (const Quaternion &Operand) const;*/
		Quaternion Conjugate(void) const;
		Quaternion Normal(void) const;

		float &operator [] (int Element);
		const float &operator [] (int Element) const;
		float *operator * (void);
		operator Vector() const;

	private:
		float Data[4];
};

float GetLength(const Quaternion &Operand);
float DotProduct(const Quaternion &Operand1, const Quaternion &Operand2);

Quaternion Interpolate(float Percent, const Quaternion &Operand1, const Quaternion &Operand2);

// For inertia, quick rotates with one quaternion, etc.
class Matrix
{
	public:
		Matrix(void);
		Matrix(float E1, float E2, float E3, float E4, float E5, float E6, float E7, float E8, float E9);
		Matrix(const Quaternion &Parent);
		void Identiate();
		Matrix Transpose(void);
		void Scale(const Vector &ScalingFactors);
		Vector operator * (const Vector &Operand) const;
		Matrix operator * (const Matrix &Operand) const;
		const float &operator [] (int Element) const;
		const float *operator * (void) const;

		float *GetGLMatrix() const; // Remember to delete allocated memory; ex: delete [] Pounter;
	private:
		float Elements[9];
};

#endif

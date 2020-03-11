#pragma once
#include <math.h>
#include <map>
#include <list>

using namespace std;

double global_sqrt_of_neg_one = sqrtf(-1);

//////////////////////////////// COMPLEX NUMBER FUNCTIONS ////////////////////////////////////
/*Returns the electrical value based on what is passed to the function
RESISTANCE			= Voltage / Amparage
AMPARAGE			= Voltage / Resistance
VOLTAGE				= Resistance * Amparage
POWER				= Voltage * Amparage
POWER RESISTANCE	= Amparage^2 * Resistence 
*/
double calculate_electrical_value(double a, double b, bool divide);

/*Returns Resistance in Ohms based on resistor values passed to function*/
double calculate_parallel_resistance(double r1, double r2, list<double>* resistors = nullptr);

/*Eulers Formula by. Leonhard Euler(d. 1748) used for phasor analysis*/
double eulers_formula(double angle, bool getConjugate);

/*C = |Z| = sqrt(a^2 + b^2)*/
double calculate_magnitude(double a, double b);

/*
returns the angle of a/b 
Sin0	= imaginary / magZ	= b / c
Cos0	=	real / magZ		= a / c
Tan0	= imaginary / real	= b / a
b-----------c
|          /|
|         / |
|        /  |
|       /   |
|      /    |
|     /     |
|    /      |
|   /       |
|  /        |
| /         |
|/_angle____|
0           a
*/
double calculate_phase_angle(double& a, double& b);

class Phasor
{
	///////////////////////////////////////////////////// Base Parts To Formulas /////////////////////////////////////////////////////
	/*a = real	
	Z = a + jb*/
	double mRealPart;
	/*b = imaginary
	Z = a + jb*/
	double mImaginaryPart;
	/*n = powerToRaiseTo
	Z^n*/
	double mPowerToRaiseComplexTo;

	///////////////////////////////////////////////////// Complex and Conjugate /////////////////////////////////////////////////////
	//Complex = Z = real + j * imaginary
	double mZRectForm;
	//Complex = Z = ce^(j0)
	double mZPolarForm;
	//Z^n
	double mZRectFormToN;
	//Conjugate of Complex = real - j * imaginary
	double mConjZRectForm;
	//Conjugate of Complex = ce^-(j0)
	double mConjZPolarForm;
	//C = |Z| = sqrt(real^2 + imaginary^2)
	double mMagnitudeZ;

	///////////////////////////////////////////////////// Angles /////////////////////////////////////////////////////
	//Sin0 = imaginary / MagnitudeZ
	double mSinAngle;
	//Cos0 = real / MagnitudeZ
	double mCosAngle;
	//Angle = atan(imaginary / real)
	double mAngle;
	
	
	Phasor(double real, double imaginary, double PowToRaiseComplexTo = 1.0);

	~Phasor();

	///////////////////////////////////////////////////// Phasor Calculations ///////////////////////////////////////

	/*Calculate rectangular form of complex number given real part, and imaginary part*/
	double calculate_rectangular_form(bool getConjugate = false);

	/*Calculate Polar form of complex number given realPart, imaginaryPart, angle, and power*/
	double calculate_polar_form_raised(bool getConjugate = false);

	///////////////////////////////////////////////////// Print Function ////////////////////////////////////////////

	void output_phasor();

	void output_complex();

	void output_conjugate();

	///////////////////////////////////////////////////// Operator Overloads ////////////////////////////////////////

	/*z1 - z2 = (real1 - real2) + j(imaginary1 - imaginary2)*/
	double operator - (Phasor& other);

	/*z1 + z2 = (real1 + real2) + j(imaginary1 + imaginary2)*/
	double operator + (Phasor& other);

	/*z1 * z2 = c1 * c2 * e^j(01+02)*/
	double operator * (Phasor& other);

	/*z1 * z2 = (c1 / c2) * e^j(01-02)*/
	double operator / (Phasor& other);
};
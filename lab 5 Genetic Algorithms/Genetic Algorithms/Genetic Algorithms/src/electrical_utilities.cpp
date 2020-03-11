#include <electrical_utilities.h>
#include <iostream>
#include <sstream>

double calculate_electrical_value(double a, double b, bool divide)
{
	if (divide)
	{
		if (b == 0)
			throw new exception("Divide by Zero ERROR!! Pass a number greate or less than Zero if dividing");
		return a / b;
	}
	return a * b;
}

double calculate_parallel_resistance(double r1, double r2, list<double>* resistors)
{
	if (resistors != nullptr)
	{
		double denominator = (1 / r1) + (1 / r2);
		for (double d : *resistors)
			denominator += 1 / d;
		return 1 / denominator;
	}
	else
		return (r1 * r2) / (r1 + r2);
}

double eulers_formula(double angle, bool getConjugate)
{
	double j = getConjugate ? -1 * global_sqrt_of_neg_one : global_sqrt_of_neg_one;
	return cos(angle) + j * sin(angle);
}

double calculate_magnitude(double a, double b)
{
	return sqrt(pow(a, 2) + pow(b, 2));
}

double calculate_phase_angle(double& a, double& b)
{
	if(b == 0)
		throw new exception("Divizion by Zero Error!! Need to pass a valid b value that is not zero");
	return a / b;
}

///////////////////////////////////////////////////// Phasor CLASS //////////////////////////////////////////////

Phasor::Phasor(double real, double imaginary, double PowToRaiseComplexTo)
{
	// Set Base Components
	mRealPart = real;
	mImaginaryPart = imaginary;
	mPowerToRaiseComplexTo = PowToRaiseComplexTo;

	// Calculate Complex and Conjugate
	mZRectForm = calculate_rectangular_form();
	mZPolarForm = calculate_polar_form_raised();
	mConjZRectForm = calculate_rectangular_form(true);
	mConjZPolarForm = calculate_polar_form_raised(false);

	// Calculate Magnitude of Z
	mMagnitudeZ = calculate_magnitude(mRealPart, mImaginaryPart);

	// Calculate Angles
	mSinAngle	= calculate_phase_angle(mImaginaryPart, mMagnitudeZ);
	mCosAngle	= calculate_phase_angle(mRealPart, mMagnitudeZ);
	mAngle		= atan(calculate_phase_angle(mImaginaryPart, mRealPart));

	output_phasor();
	output_complex();
}

///////////////////////////////////////////////////// Phasor Calculations ///////////////////////////////////////

double Phasor::calculate_rectangular_form(bool getConjugate)
{
	double rightSide = global_sqrt_of_neg_one * mImaginaryPart;
	return getConjugate ? mRealPart - rightSide : mRealPart + rightSide;
}

double Phasor::calculate_polar_form_raised(bool getConjugate)
{
	return pow(mMagnitudeZ * eulers_formula(mAngle, getConjugate), mPowerToRaiseComplexTo);
}

///////////////////////////////////////////////////// Print Function ////////////////////////////////////////////

void Phasor::output_phasor()
{
	stringstream ss;
	ss << "Z = " << mMagnitudeZ << "/_" << mAngle << "degrees";
	cout << ss.str() << endl;
}

void Phasor::output_complex()
{
	stringstream ss;
	ss << "Z = " << mMagnitudeZ << "/_" << mAngle << "degrees";
	ss << "\nRect: " << mZRectForm << " = " << mRealPart << " + j" << mImaginaryPart;
	ss << "\nPolar: " << mZPolarForm << " = " << mMagnitudeZ << "^" << mPowerToRaiseComplexTo << " * e^j * " << mAngle << " * " << mPowerToRaiseComplexTo;
	cout << ss.str() << endl;
}

void Phasor::output_conjugate()
{
	stringstream ss;
	ss << "\nConjRect: " << mConjZRectForm << " = " << mRealPart << " - j" << mImaginaryPart;
	ss << "\nConjPolar: " << mConjZPolarForm << " = " << mMagnitudeZ << "^" << mPowerToRaiseComplexTo << " * e^-j * " << mAngle << " * " << mPowerToRaiseComplexTo;
	cout << ss.str() << endl;
}

///////////////////////////////////////////////////// Operator Overloads ////////////////////////////////////////

double Phasor::operator-(Phasor& other)
{
	return (mRealPart - other.mRealPart) + global_sqrt_of_neg_one * (mImaginaryPart - other.mImaginaryPart);
}

double Phasor::operator+(Phasor& other)
{
	return (mRealPart + other.mRealPart) + global_sqrt_of_neg_one * (mImaginaryPart + other.mImaginaryPart);
}

double Phasor::operator*(Phasor& other)
{
	return (mMagnitudeZ * other.mMagnitudeZ) * eulers_formula(mAngle + other.mAngle, false);
}

double Phasor::operator/(Phasor& other)
{
	return (mMagnitudeZ / other.mMagnitudeZ) * eulers_formula(mAngle - other.mAngle, false);
}

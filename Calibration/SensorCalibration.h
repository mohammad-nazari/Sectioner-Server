#pragma once

#ifndef _SENSORCALIBRATION_H_
#define _SENSORCALIBRATION_H_
#include "Header.h"

class __declspec(dllexport)SensorCalibration
{
public:
	std::string Name() const;
	void Name(std::string val);
	double Zero() const;
	void Zero(double val);
	double Span() const;
	void Span(double val);
	double Min() const;
	void Min(double val);
	double Max() const;
	void Max(double val);
private:
	std::string _name;
	double _zero;
	double _span;
	double _min;
	double _max;
};
#endif // DEBUG

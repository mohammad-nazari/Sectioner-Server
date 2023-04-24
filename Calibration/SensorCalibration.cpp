#include "SensorCalibration.h"

void SensorCalibration::Name(std::string val)
{
	_name = val;
}

std::string SensorCalibration::Name() const
{
	return _name;
}

void SensorCalibration::Zero(double val)
{
	_zero = val;
}

double SensorCalibration::Zero() const
{
	return _zero;
}

void SensorCalibration::Span(double val)
{
	_span = val;
}

double SensorCalibration::Span() const
{
	return _span;
}

void SensorCalibration::Min(double val)
{
	_min = val;
}

double SensorCalibration::Min() const
{
	return _min;
}

void SensorCalibration::Max(double val)
{
	_max = val;
}

double SensorCalibration::Max() const
{
	return _max;
}

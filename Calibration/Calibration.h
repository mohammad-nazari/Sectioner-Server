#pragma once

#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "Header.h"
#include "SensorCalibration.h"
#include "json/json.h" //json-cpp, tested with version 0.5

class __declspec(dllexport)Calibration
{
public:
	Calibration(void);
	~Calibration(void);

	int ReadFromFile(std::string FileName);
	int ReadFromString(std::string JsonData);
	int WriteToFile(std::string FileName);
	int WriteToString(std::string& JsonData);
	void SetDefault(void);

	std::vector<SensorCalibration> SensorsCalibration() const;
	void SensorsCalibration(std::vector<SensorCalibration> val);
private:
	std::vector<SensorCalibration> _sensorsCalibration;
	int Read(std::string JsonData);
	int Write(std::string& JsonData);
};

#endif

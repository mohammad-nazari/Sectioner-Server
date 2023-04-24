#pragma once

#ifndef _SCONNECTION_H_
#define _SCONNECTION_H_

#include "soapsectionnerService.h"
#include "DeviceMap.h"

#include "Header.h"
#include "Tools.h"
#include "SyntaxChecker.h"
#include "Calibration.h"
#include "..\DataBase\DBConnection.h"
#include <map>

class SConnection
{
public:
	SConnection();
	~SConnection();
	static void LogData(ns__Device DeviceObject, std::string Data);

	int CreateSocket();
	int CloseSocket() const;

	int ThreadStartListen();

	int GetDeviceStatus(ns__Device& DeviceObject) const;
	int SetDeviceStatus(ns__Device& DeviceObject, std::vector<ns__ErrorCode>& ErrorCode) const;
	int GetDevicePicture(ns__Device& DeviceObject) const;
	int GetDevicePicturePart(ns__Device& DeviceObject) const;
	int GetReport(ns__Device& DeviceObject, std::string ReportData) const;
	void InitiateDeviceStatus(ns__Device& DeviceObject) const;
	int SaveDevieStatus(ns__Device& DeviceObject, std::string PureData) const;
	static int GetDeviceSamplingTime(ns__Device& deviceInfo);
	static int UpdateDeviceRow(ns__Device& deviceInfo);

	int ThreadHandleDevice2(DeviceMap deviceMap);
private:
	int StartListen2();
	int ThreadHandleDevice(DeviceMap& DeviceObject);
	int StartListen();
	int HandleDevice(DeviceMap& DeviceMapObject) const;
	int HandleDevice2(DeviceMap & DeviceMapObject) const;
	SOCKET _listenSocket;
	int IsDevice(DeviceMap& DeviceMapObject) const;
	int ParsDeviceData(std::string strMessageReceived, SyntaxChecker parserObject, DeviceMap & DeviceMapObject, int & tryCounter) const;
	static int ReadData(SOCKET Socket, std::string& strMessage);
	static int WriteData(SOCKET Socket, const std::string strMessage);
	static int AddUpdateDeletSearchDeviceInList(DeviceMap& DeviceMapObject, Device_AddRemoveUpdateSearch CommandType);
	int CreateDeviceRow(ns__Device& deviceInfo) const;
	int ExistDeviceRow(ns__Device& deviceInfo) const;
	std::string ApplyCommand(DeviceMap& DeviceMapObject) const;
	bool GetCNFResult(ns__Device& DeviceObject, std::string CNFResult) const;
	static std::map<std::string, std::string> GetSensorsName(std::string Input);
	static bool GenerateCalibrationData(ns__Device DeviceObject, ns__CalibrationList& calibrationList);
	static double CalculateCalibratedValue(double & DeviceObject, double Minimum, double Maximum, double Zero, double Span);
	int SetDateTime(ns__Device& DeviceObject) const;
	int SetRelay(ns__Device& DeviceObject) const;
	int SetReset(ns__Device& DeviceObject) const;
	int SetStt(ns__Device& DeviceObject) const;
	void RemoveUnusedChars(std::string & Input) const;
	static int SetSampling(ns__Device& DeviceObject);
	bool CheckSensors(ns__Device & DeviceObject, std::string Input, ns__SensorName SensorName, std::string SensorNikeName) const;
	static bool AddToSensorsList(ns__Device & DeviceObject, ns__Sensor Sensor, ns__SensorName SensorName);
	Tools _toolsObject;
	DeviceMap _deviceMapObject;
public:
	int CreateSocket2();
	int ThreadStartListen2();
};
#endif // _SCONNECTION_H_

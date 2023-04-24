#pragma once
#ifndef _DBCONNECTION_H_
#define _DBCONNECTION_H_

#include "soapH.h"
#include "Header.h"
#include "Tools.h"
#include "Sectionner.h"

class __declspec(dllexport)DBConnection
{
public:
	DBConnection();
	~DBConnection();

	int GetUsersList(std::vector<ns__User> &UserList) const;
	int AddUser(ns__User UserInfo) const;
	int UpdateUser(ns__User UserInfo) const;
	int DeleteUser(ns__User UserInfo) const;
	int AssignDeviceToUser(ns__UserDevice UserDeviceList) const;
	int DeleteDeviceFromUser(ns__UserDevice UserDeviceList) const;

	int UpdateDeviceRow(ns__Device DeviceInfo) const;
	int UpdateDeviceRowSetting(ns__Device DeviceInfo) const;
	int CreateDeviceRow(ns__Device DeviceInfo, std::string CalibrationData) const;
	int CheckSerialNumber(std::string Input) const;
	int CheckUser(ns__User& UserInfo) const;
	static int CheckDevice(ns__Device& DeviceInfo);
	int GetDeviceSetting(ns__Device& DeviceInfo) const;
	int RemoveDeviceSms(ns__Device& DeviceInfo) const;
	int GetDeviceSettingSms(ns__Device& DeviceInfo) const;
	int SetDeviceStatusSms(ns__Device& DeviceInfo) const;
	int SetDeviceSettingSms(ns__Device& DeviceInfo) const;
	void FillDeviceInformation(ns__Device & DeviceInfo, db::Device DeviceObj) const;
	static int CheckUserDevice(ns__User UserInfo, ns__Device DeviceInfo, int AllowType = 0);
	int GetUserDeviceList(ns__User UserInfo, std::vector<ns__Device> &DeviceList) const;
	int GetAllDeviceList();
	int SaveDeviceStatus(ns__Device DeviceObject, std::string DeviceStatusString, std::string PureData) const;
	int SaveDeviceLog(ns__Device DeviceObject, std::string Data) const;

	std::vector<ns__Device> getUDeviceList() const;
	void setUserDeviceList();

	int GetAllCityLoactionName(ns__AllCityLocatoins &AllCityLocationName) const;
	static int GetCitiesName(std::vector<std::string> &AllCityName);
	int GetLacationsName(std::string CityName, std::vector<std::string> &AllLocationName) const;

	int ChangeCityName(std::string OldCityName, std::string NewCityName) const;
	int ChangeLocationName(std::string oldCityName, std::string newCityName, std::string OldLocationName, std::string NewLocationName) const;
	int ChangeDeviceName(int DeviceSerialNumber, std::string newCityName, std::string newLocationName, std::string NewDeviceName) const;

	int GetDeviceStatusLog(ns__DeviceStatus StatusLogInfo, ns__DeviceStatusLogList &StatusLogList) const;

	static int GetDevicePicture(ns__Device& DeviceInfo);
	static int SetDevicePicture(ns__Device DeviceInfo);
	static int GetDevicePictureLog(ns__DeviceStatus StatusLogInfo, ns__DeviceStatusLogList &StatusLogList);
	static int SetDevicePictureLog(ns__DeviceStatus StatusLogInfo, std::string DevicePicture);

	int GetDeviceCalibration(ns__Device& DeviceInfo, std::string& CalibrationData) const;
	static int SetDeviceCalibration(ns__Device DeviceInfo, std::string CalibrationData);

	static int GetDeviceSamplingTime(ns__Device& DeviceInfo);
	static int SetDeviceSamplingTimme(ns__Device DeviceInfo);


private:
	Tools _toolsObject;
	Database *connection;
	StderrLog log;
	std::vector<ns__Device> userDeviceList;

	bool ConnectToDatabase();
	int GetDeviceInfo(int DeviceId, ns__Device& DeviceInformation) const;
	int GetUserInfo(ns__User& UserInformation) const;
};

#endif // _DBCONNECTION_H_

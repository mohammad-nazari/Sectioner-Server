#pragma once

#ifndef _PARSER_H_
#define _PARSER_H_

#include "DBConnection.h"
#include "Base64.h"

class ExecuteRequest
{
public:
	ExecuteRequest();
	~ExecuteRequest();

	int GetUsersList(std::vector<ns__User> &UserList) const;
	int AddUser(ns__User UserInfo) const;
	int UpdateUser(ns__User UserInfo) const;
	int DeleteUser(ns__User UserInfo) const;
	int AssignDeviceToUser(ns__UserDevice UserDeviceList) const;
	int DeleteDeviceFromUser(ns__UserDevice UserDeviceList) const;
	int GetUserDeviceList(ns__User UserInfo, std::vector<ns__Device> &DeviceListObject) const;
	int GetAllDeviceList(std::vector<ns__Device> &DeviceListObject);
	int AuthenticateUser(ns__User &UserInfo) const;
	int CheckDevice(ns__Device& DeviceInfo) const;
	int GetDeviceSetting(ns__Device& DeviceInfo) const;
	int RemoveDeviceSms(ns__Device& DeviceInfo) const;
	int SetDeviceSetting(ns__Device& DeviceInfo) const;
	int SetDeviceStatusSms(ns__Device& DeviceInfo) const;
	int SetDeviceSettingSms(ns__Device& DeviceInfo) const;
	int SetSampling(ns__Device& DeviceObject) const;
	static int SetCalibrationData(ns__Device& DeviceObject, ns__CalibrationList CalibrationObject);
	static int GetCalibrationData(ns__Device& DeviceObject, ns__CalibrationList& CalibrationObject);
	int IsDeviceForUser(ns__User UserInfo, ns__Device& DeviceInfo, int AllowType = 0) const;

	int DeviceInfoToBase64(ns__Device& DeviceObject);
	int DeviceInfoFromBase64(ns__Device& DeviceObject);

	int GetDeviceCityLocationList(ns__AllCityLocatoins &AllCityLocation) const;

	int ChangeCityName(std::string OldCityName, std::string NewCityName) const;
	int ChangeLocationName(std::string OldCityName, std::string NewCityName, std::string OldLocationName, std::string NewLocationName) const;
	int ChangeDeviceName(int DeviceSerialNumber, std::string NewCityName, std::string NewLocationName, std::string NewDeviceName) const;

	int GetDeviceStatusLog(ns__DeviceStatus StatusLogInfo, ns__DeviceStatusLogList &StatusLogList) const;

private:
	DBConnection _dbObject;
	Base64 _base64Object;
};
#endif


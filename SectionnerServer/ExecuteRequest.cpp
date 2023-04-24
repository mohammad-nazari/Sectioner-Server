#include "ExecuteRequest.h"


ExecuteRequest::ExecuteRequest()
{
}


ExecuteRequest::~ExecuteRequest()
{
}

int ExecuteRequest::AddUser(ns__User UserInfo) const
{
	return this->_dbObject.AddUser(UserInfo);
}

int ExecuteRequest::DeleteUser(ns__User UserInfo) const
{
	return this->_dbObject.DeleteUser(UserInfo);
}

//************************************
// Method:    AuthenticateUser
// FullName:  ExecuteRequest::AuthenticateUser
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: ns__User UserInfo
//
// Description: Generate a connection to database and check user information
//************************************
int ExecuteRequest::AuthenticateUser(ns__User &UserInfo) const
{
	return this->_dbObject.CheckUser(UserInfo);
}

int ExecuteRequest::AssignDeviceToUser(ns__UserDevice UserDeviceList) const
{
	return this->_dbObject.AssignDeviceToUser(UserDeviceList);
}

int ExecuteRequest::CheckDevice(ns__Device& DeviceInfo) const
{
	return this->_dbObject.CheckDevice(DeviceInfo);
}

int ExecuteRequest::IsDeviceForUser(ns__User UserInfo, ns__Device& DeviceInfo, int AllowType/*=0*/) const
{
	return this->_dbObject.CheckUserDevice(UserInfo, DeviceInfo, AllowType);
}

int ExecuteRequest::GetUserDeviceList(ns__User UserInfo, std::vector<ns__Device> &DeviceListObject) const
{
	if (this->_dbObject.GetUserDeviceList(UserInfo, DeviceListObject) > 0)
	{
	}
	else
	{
		// No device found
		return 0;
	}

	return 1;
}

int ExecuteRequest::GetDeviceSetting(ns__Device& DeviceInfo) const
{
	return this->_dbObject.GetDeviceSetting(DeviceInfo);
}

int ExecuteRequest::RemoveDeviceSms(ns__Device& DeviceInfo) const
{
	return this->_dbObject.RemoveDeviceSms(DeviceInfo);
}

int ExecuteRequest::SetDeviceSetting(ns__Device& DeviceInfo) const
{
	for each (ns__RequestType var in DeviceInfo.dRequests)
	{
		switch (var)
		{
		case SAMPLING:
		{
			return this->_dbObject.UpdateDeviceRowSetting(DeviceInfo);
			break;
		}
		default:
			break;
		}
	}
	return true;
}

int ExecuteRequest::SetDeviceStatusSms(ns__Device& DeviceInfo) const
{
	return this->_dbObject.SetDeviceStatusSms(DeviceInfo);
}

int ExecuteRequest::SetDeviceSettingSms(ns__Device& DeviceInfo) const
{
	return this->_dbObject.SetDeviceSettingSms(DeviceInfo);
}

inline int ExecuteRequest::SetSampling(ns__Device & DeviceObject) const
{
	return this->_dbObject.SetDeviceSamplingTimme(DeviceObject);
}

int ExecuteRequest::SetCalibrationData(ns__Device& DeviceObject, ns__CalibrationList CalibrationObject)
{
	Tools toolsObject;
	
	std::string strStr = toolsObject.GetCalibrationDataFromSOAP(CalibrationObject);
	DBConnection dbObject;
	return dbObject.SetDeviceCalibration(DeviceObject, strStr);
}

int ExecuteRequest::GetCalibrationData(ns__Device& DeviceObject, ns__CalibrationList& CalibrationObject)
{
	Tools toolsObject;
	std::string CalibrationData;
	DBConnection dbObject;
	dbObject.GetDeviceCalibration(DeviceObject, CalibrationData);

	CalibrationObject = toolsObject.GetCalibrationDataFromXML(CalibrationData);

	return 1;
}

int ExecuteRequest::GetAllDeviceList(std::vector<ns__Device>& DeviceListObject)
{
	if (this->_dbObject.GetAllDeviceList() > 0)
	{
		DeviceListObject = this->_dbObject.getUDeviceList();
	}
	else
	{
		// No device found
		return 0;
	}

	return 1;
}

int ExecuteRequest::DeviceInfoToBase64(ns__Device& DeviceObject)
{
	DeviceObject.dNikeName = this->_base64Object.Encode(DeviceObject.dNikeName);
	DeviceObject.dLocation = this->_base64Object.Encode(DeviceObject.dLocation);
	DeviceObject.dCity = this->_base64Object.Encode(DeviceObject.dCity);

	return 1;
}

int ExecuteRequest::DeviceInfoFromBase64(ns__Device& DeviceObject)
{
	DeviceObject.dNikeName = this->_base64Object.Decode(DeviceObject.dNikeName);
	DeviceObject.dLocation = this->_base64Object.Decode(DeviceObject.dLocation);
	DeviceObject.dCity = this->_base64Object.Decode(DeviceObject.dCity);

	return 1;
}

int ExecuteRequest::GetDeviceCityLocationList(ns__AllCityLocatoins& AllCityLocation) const
{
	return this->_dbObject.GetAllCityLoactionName(AllCityLocation);
}

int ExecuteRequest::ChangeCityName(std::string OldCityName, std::string NewCityName) const
{
	return this->_dbObject.ChangeCityName(OldCityName, NewCityName);
}

int ExecuteRequest::ChangeLocationName(std::string OldCityName, std::string NewCityName, std::string OldLocationName, std::string NewLocationName) const
{
	return this->_dbObject.ChangeLocationName(OldCityName, NewCityName, OldLocationName, NewLocationName);
}

int ExecuteRequest::ChangeDeviceName(int DeviceSerialNumber, std::string NewCityName, std::string NewLocationName, std::string NewDeviceName) const
{
	return this->_dbObject.ChangeDeviceName(DeviceSerialNumber, NewCityName, NewLocationName, NewDeviceName);
}

int ExecuteRequest::GetUsersList(std::vector<ns__User> &UserList) const
{
	return this->_dbObject.GetUsersList(UserList);
}

int ExecuteRequest::DeleteDeviceFromUser(ns__UserDevice UserDeviceList) const
{
	return this->_dbObject.DeleteDeviceFromUser(UserDeviceList);
}

int ExecuteRequest::UpdateUser(ns__User UserInfo) const
{
	return this->_dbObject.UpdateUser(UserInfo);
}

int ExecuteRequest::GetDeviceStatusLog(ns__DeviceStatus StatusLogInfo, ns__DeviceStatusLogList &StatusLogList) const
{
	return this->_dbObject.GetDeviceStatusLog(StatusLogInfo, StatusLogList);
}


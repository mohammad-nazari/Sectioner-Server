#pragma once
#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "soapH.h"

#include "Header.h"
#include "EnumString.h"

Begin_Enum_String(ns__SensorType)
{
	Enum_String(Mono);
	Enum_String(Multi);
}
End_Enum_String;

Begin_Enum_String(ns__UserType)
{
	Enum_String(Admin);
	Enum_String(Control);
	Enum_String(Monitor);
}
End_Enum_String;

Begin_Enum_String(ns__SensorName)
{
	Enum_String(TEMPERATURE);
	Enum_String(HUMIDITY);
	Enum_String(ACVOLTAGE);
	Enum_String(ACAMPERE);
	Enum_String(DCVOLTAGE);
	Enum_String(DCAMPERE);
	Enum_String(COSQ);
	Enum_String(DIGITALINPUT);
	Enum_String(DIGITALOUTPUT);
	Enum_String(DIGITALEXIST);
	Enum_String(RELAY);
}
End_Enum_String;

Begin_Enum_String(ns__RequestType)
{
	Enum_String(CALIBRATION);
	Enum_String(SAMPLING);
	Enum_String(OUTRELAY);
	Enum_String(PICTURE);
	Enum_String(RESET);
	Enum_String(PARTS);
	Enum_String(DATETIME);
	Enum_String(LIVE);
	Enum_String(OK);
	Enum_String(DATA);
	Enum_String(CUSTOM);
	Enum_String(STT);
	Enum_String(ALM);
}
End_Enum_String;

Begin_Enum_String(ns__DeviceModel)
{
	Enum_String(PROTECTOR);
	Enum_String(SECTIONNER);
	Enum_String(TRANSISTOR);
	Enum_String(SERVER);
	Enum_String(MANAGER);
	Enum_String(ROBER);
	Enum_String(ALARM);
}
End_Enum_String;

class __declspec(dllexport)Tools
{
public:
	Tools();
	~Tools();

	static std::string VectorStringToString(std::string const & v);

	static void replaceAll(std::string& str, const std::string& oldStr, const std::string& newStr);
	static size_t replaceOne(std::string& str, const std::string& oldStr, const std::string& newStr, size_t position);
	static size_t findOne(std::string str, const std::string& findStr, size_t position);

	static std::string getCurrentDateTime();
	struct tm StringToTMStruct(std::string Input) const;
	time_t StringToTime_t(std::string Input, std::string FormatDateTime = "%Y-%m-%d %H:%M:%S") const;
	static std::string Time_tToString(time_t Input, std::string Format = "%Y-%m-%d %H:%M:%S");
	static std::string TMStructToString(struct tm Input, std::string Format = "%Y-%m-%d %H:%M:%S");

	static std::string toUpper(std::string& str);
	static std::string toLower(std::string& str);

	bool is_number(const std::string& str) const;
	static bool is_number2(const std::string & str);
	static bool is_number3(const std::string & str);
	bool is_numberWithLen(const std::string& s, int lenght) const;
	bool is_double(std::string const& s) const;

	int toInteger(std::string const& Input) const;
	static std::string toString(int Input);
	static std::string toString(int Input, int Length);
	std::string toString(long long int Input) const;
	static std::string toString(long long int Input, int Length);
	static std::string toStringDouble(double Input, int PrecisionLength);
	static bool isInString(std::string Input, std::string ParentString);

	int getDigitLenth(long int InputNumber) const;
	long long int toLongLong(std::string const& Input) const;

	bool checkFormat(std::string Input, std::string DataFormat) const;

	/*std::string SimCardTypeToString(ns__SimcardType SimCardType);
	bool SimCardTypeFromString(ns__SimcardType &SimcardType, std::string SimCardTypeName);*/

	static std::string UserTypeToString(ns__UserType UserType);
	static bool UserTypeFromString(ns__UserType &UserType, std::string UserTypeName);

	static std::string SensorTypeToString(ns__SensorType SensorType);
	static bool SensorTypeFromString(ns__SensorType &SensorType, std::string SensorTypeName);

	static std::string SensorNameToString(ns__SensorName SensorType);
	static bool SensorNameFromString(ns__SensorName &SensorType, std::string SensorTypeName);

	static std::string RequestTypeToString(ns__RequestType RequestType);
	static bool RequestTypeFromString(ns__RequestType &RequestType, std::string RequestTypeName);

	static std::string DeviceModelToString(ns__DeviceModel DeviceModel);
	static bool DeviceModelFromString(ns__DeviceModel &DeviceModel, std::string DeviceModelName);

	std::string ltrim(std::string const &s) const;
	std::string rtrim(std::string const &s) const;
	std::string trim(std::string const &s) const;

	static std::vector<double> ConvertStringToBinaryList(unsigned int val);

	static void SetErrorData(ns__ErrorCode& ErrorCode, std::string ErrorMessage, int ErrorNumber, ns__SettingLevel ErrorType = ns__SettingLevel::Critical);

	ns__CalibrationList GetCalibrationDataFromXML(std::string CalibrationData) const;
	std::string GetCalibrationDataFromSOAP(ns__CalibrationList CalibrationList) const;

	ns__Device GetDeviceDataFromXML(std::string DeviceData) const;
	std::string GetDeviceDataFromSOAP(ns__Device DeviceObject) const;

private:

	static char * strptime(const char *buf, const char *fmt, struct tm *tm);

	static int conv_num(const char **buf, int *dest, int llim, int ulim);

	static int strncasecmp(char *s1, char *s2, size_t n);

	static void InitiateSoap(soap * pSoap);
	static void DestroySoap(soap * pSoap);
};

#endif //_TOOLS_H_
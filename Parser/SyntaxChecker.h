#pragma once
#ifndef _SYNTAX_CHECKER_H_
#define _SYNTAX_CHECKER_H_

#include "soapH.h"
#include "Header.h"

#include "../Split/Splitter.h"
#include "../Tools/Tools.h"
#include "../Tools/ConverterDecimal.h"
#include "CCRC32.H"
#include "BaseConverter.h"
#include "MD5.h"
#include <map>

class __declspec(dllexport)SyntaxChecker
{

public:
	SyntaxChecker();
	~SyntaxChecker();

	bool AnalyzeRecievedData(std::string& InputOrg, bool& IsSet);

	bool CheckSTTSyntax(std::string& InputOrg, int& IsCamera);

	bool CheckSetSyntax(std::string & InputOrg);

	bool CheckPartSyntax(std::string & InputOrg);

	bool CheckPICSyntax(std::string & InputOrg);

	std::string GenerateCNFCommand(ns__Device DeviceObject) const;

	bool CheckDateTime(std::string Input);

	bool CheckSensorsBit(std::string Input, ns__SensorName SensorName, int Length);

	bool CheckACAmpereFault(std::string Input, int Index, int Length);

	ns__Device& DeviceInfo()
	{
		return this->_deviceInfo;
	}
	void DeviceInfo(ns__Device& val)
	{
		this->_deviceInfo = val;
	}

	std::string& ErrorData()
	{
		return this->_errorData;
	}
	void ErrorData(std::string& val)
	{
		this->_errorData = val;
	}

	ns__Sensor SensorTemp() const
	{
		return this->_sensorTemp;
	}
	void SensorTemp(ns__Sensor val)
	{
		this->_sensorTemp = val;
	}
private:

	bool AddToSensorsList(ns__Sensor Sensor, ns__SensorName SensorName);

	bool CheckPhoneNumber(std::string Input) const;

	static bool CheckSimcardType(std::string Input);

	bool CheckDate(std::string Input, tm &dateTime) const;

	bool CheckTime(std::string Input, tm &dateTime) const;

	bool CheckIp(std::string Input, ns__IP& IPObject) const;

	bool CheckDeviceNumber(std::string Input);

	bool CheckDeviceType(std::string Input);

	bool CheckPictureSize(std::string Input);

	bool CheckPictrurePartSize(std::string Input);

	bool CheckPicturePartNumbers(std::string Input);

	bool CheckPartIndex(std::string Input);

	bool CheckPartData(std::string Input, std::string CRCValue);

	bool CheckPartCRC(std::string Input);

	bool CheckPictureCRC(std::string Input);

	bool CheckSampling(std::string Input) const;

	bool CheckCommandType(std::string Input) const;

	static bool CheckDeviceVersion(std::string Input);

	bool CheckSimcard(std::string Input) const;

	static bool CheckServer(std::string Input);

	static bool CheckGPRS(std::string Input);

	bool CheckSMSContact(std::string Input) const;

	bool CheckSMSConfig(std::string Input) const;

	static bool CheckSMSCommand(std::string Input);

	static bool CheckGSMCommand(std::string Input);

	static bool CheckKeyboardCommand(std::string Input);

	bool CheckSensorRelay(std::string Input, int index);

	std::string GenerateLable(std::string Prefix,int& Index, int Length) const;

	bool CheckSensors(std::string Input, ns__SensorName SensorName, std::string SensorNikeName = "");

	bool CheckRelay(std::string Input, int Length);

	std::map<std::string, ns__SensorName> GetSensorsName(std::string Input) const;
	std::map<std::string, ns__SensorName> dictionarySensorName;

	std::map<ns__SensorName, std::string> GetSensorsNikeName(std::string Input) const;
	void RemoveUnusedChars(std::string & Input) const;
	static bool CheckDeviceBuzzer(std::string Input);
	std::map<ns__SensorName, std::string> dictionarySensorNikeName;
	Tools _toolsObject;
	std::string _errorData;
	ns__Device _deviceInfo;
	ns__Sensor _sensorTemp;
	ns__SensorExtension sensorExtensionTemp;

	//[deviceNo],DAT,[sampling],[datetime],[digitalInput],[relay],[acv],[acv],[acv],[acv],[acv],[acv],[aca],[aca],[aca],[dcv],[dca],[tem],[hum],[cosQ],[cosQ],[cosQ]#
	// "DIGITALINPUT","RELAY"
	std::vector<ns__SensorName> sectionnerSensors{ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::DCVOLTAGE,ns__SensorName::DCAMPERE,ns__SensorName::TEMPERATURE,ns__SensorName::HUMIDITY,ns__SensorName::COSQ,ns__SensorName::COSQ,ns__SensorName::COSQ};

	// [deviceNo],DAT,[sampling],[datetime],[digitalInput],[digitalExist],[relay],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],[dcv],[dca],[tem],[hum],[cosQ],[cosQ],[cosQ]#
	// "DIGITALINPUT","DIGITALEXIST","RELAY"
	std::vector<ns__SensorName> managerSensors{ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACVOLTAGE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::ACAMPERE,ns__SensorName::DCVOLTAGE,ns__SensorName::DCAMPERE,ns__SensorName::TEMPERATURE,ns__SensorName::HUMIDITY,ns__SensorName::COSQ,ns__SensorName::COSQ,ns__SensorName::COSQ};
};

#endif //_SYNTAX_CHECKER_H_
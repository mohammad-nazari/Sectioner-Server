#include "SyntaxChecker.h"

SyntaxChecker::SyntaxChecker()
{
	// Generate dictionary of sensors names
	this->dictionarySensorName = GetSensorsName(SENSORSNAMEMULTI);
	std::map<std::string, ns__SensorName> mapTemp = GetSensorsName(SENSORSNAMEMONO);
	this->dictionarySensorName.insert(mapTemp.begin(), mapTemp.end());

	this->dictionarySensorNikeName = GetSensorsNikeName(SENSORSNAMEMULTI);
	std::map<ns__SensorName, std::string> mapNTemp = GetSensorsNikeName(SENSORSNAMEMONO);
	this->dictionarySensorNikeName.insert(mapNTemp.begin(), mapNTemp.end());

	this->_deviceInfo.dSensors.clear();
	/*this->_deviceInfo.deviceSMSConfig.clear();
	this->_deviceInfo.deviceSMSContact.clear();*/

	this->_deviceInfo.dErr.eNo = 0;
	this->_deviceInfo.dErr.eType = Normal;
	this->_deviceInfo.dErr.eMsg.clear();
}

SyntaxChecker::~SyntaxChecker()
{
}

bool SyntaxChecker::CheckDateTime(std::string Input)
{
	Splitter sp(Input, " ");
	if (sp.size() == 2)
	{
		tm dateTime;

		if (CheckDate(sp[0], dateTime))
		{
			if (CheckTime(sp[1], dateTime))
			{
				//this->_deviceInfo.dDateTime = mktime(&dateTime);
				this->_deviceInfo.dDateTime = this->_toolsObject.StringToTime_t(Input);
				//this->_deviceInfo.dDateTime -= 73800;
				//dateTime = *(localtime(&(this->_deviceInfo.dDateTime)));
				return true;
			}
		}
	}
	return false;
}

bool SyntaxChecker::CheckDate(std::string Input, tm &dateTime) const
{
	Splitter sp(Input, "-");
	if (sp.size() == 3)
	{
		if (this->_toolsObject.is_numberWithLen(sp[0], 2) || this->_toolsObject.is_numberWithLen(sp[0], 4))
		{
			if (this->_toolsObject.is_numberWithLen(sp[1], 1) || this->_toolsObject.is_numberWithLen(sp[1], 2))
			{
				if (this->_toolsObject.is_numberWithLen(sp[2], 1) || this->_toolsObject.is_numberWithLen(sp[2], 2))
				{
					dateTime.tm_year = this->_toolsObject.is_numberWithLen(sp[0], 2) ? this->_toolsObject.toInteger(sp[0]) + 100 : this->_toolsObject.toInteger(sp[0]) - 1900;
					dateTime.tm_mon = this->_toolsObject.toInteger(sp[1]);
					dateTime.tm_mday = this->_toolsObject.toInteger(sp[2]);
					return true;
				}
			}
		}
	}

	return false;
}

bool SyntaxChecker::CheckTime(std::string Input, tm &dateTime) const
{
	Splitter sp(Input, ":");
	if (sp.size() == 2 || sp.size() == 3)
	{
		if (this->_toolsObject.is_numberWithLen(sp[0], 1) || this->_toolsObject.is_numberWithLen(sp[0], 2))
		{
			if (this->_toolsObject.is_numberWithLen(sp[1], 1) || this->_toolsObject.is_numberWithLen(sp[1], 2))
			{
				dateTime.tm_hour = this->_toolsObject.toInteger(sp[0]);
				dateTime.tm_min = this->_toolsObject.toInteger(sp[1]);
				if (sp.size() == 3)
				{
					if (this->_toolsObject.is_numberWithLen(sp[2], 1) || this->_toolsObject.is_numberWithLen(sp[2], 2))
					{
						dateTime.tm_sec = this->_toolsObject.toInteger(sp[2]);
						return true;
					}
				}
				return true;
			}
		}
	}

	return false;
}

bool SyntaxChecker::CheckDeviceNumber(std::string Input)
{
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dSerialNumber = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckDeviceType(std::string Input)
{
	if (Input == "DATS")
	{
		this->_deviceInfo.dModel = ns__DeviceModel::SECTIONNER;
		return true;
	}
	else if (Input == "DATM")
	{
		this->_deviceInfo.dModel = ns__DeviceModel::MANAGER;
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckPictureSize(std::string Input)
{
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dPicture.pSize = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckPictrurePartSize(std::string Input)
{
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dPicture.pPartSize = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckPicturePartNumbers(std::string Input)
{
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dPicture.pPartNo = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckPictureCRC(std::string Input)
{
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dPicture.pCRC = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckPartIndex(std::string Input)
{
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dPicture.pParts[0].ppIndex = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckPartData(std::string Input, std::string CRCValue)
{
	if (this->CheckPartCRC(CRCValue))
	{
		/*CCRC32 crcObject;
		const BaseConverter& hex2dec = BaseConverter::HexToDecimalConverter();
		Input = hex2dec.Convert(Input);
		if(crcObject.FullCRC((const unsigned char *)Input.c_str(), Input.size()) == this->_deviceInfo.dPicture.pParts[0].partCRC)
		{*/
		this->_deviceInfo.dPicture.pParts[0].ppData = Input;
		return true;
		//}
	}
	return false;
}

bool SyntaxChecker::CheckPartCRC(std::string Input)
{
	const BaseConverter& hex2dec = BaseConverter::HexToDecimalConverter();
	Input = hex2dec.Convert(Input);
	if (this->_toolsObject.is_number(Input))
	{
		this->_deviceInfo.dPicture.pParts[0].ppCRC = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckSampling(std::string Input) const
{
	if (this->_toolsObject.is_number(Input))
	{
		//this->_deviceInfo.dSamplingTime = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckCommandType(std::string Input) const
{
	if (this->_toolsObject.toUpper(Input) == "STT")
	{
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckDeviceVersion(std::string Input)
{
	// split for version and device buzzer on or off
	Splitter sp(Input, ",");
	if (sp.size() == 2)
	{
		if (sp[0].size() > 0)
		{
			if (sp[0] == "-")
			{
				return true;
			}
			//this->_deviceInfo.deviceFirmWareVersion = Input;
			if (CheckDeviceBuzzer(sp[1]))
			{
				return true;
			}
		}
	}
	return false;
}

bool SyntaxChecker::CheckDeviceBuzzer(std::string Input)
{
	if (Input.size() > 0)
	{
		if (Input == "0" || Input == "1")
		{
			//this->_deviceInfo.deviceBuzzerOnOff = this->_toolsObject.toInteger(Input);
			return true;
		}
	}
	return false;
}

bool SyntaxChecker::CheckSimcard(std::string Input) const
{
	/*this->_deviceInfo.deviceMobile.mobileNumber = 0;
	this->_deviceInfo.deviceMobile.mobileSimCardType = MCI;
	this->_deviceInfo.deviceMobile.mobileSignalValue = 0;
	this->_deviceInfo.deviceMobile.mobileChargeValue = 0;*/

	if (Input == "-")
	{
		return true;
	}

	Splitter sp(Input, ",");
	if (sp.size() == 4)
	{
		if (CheckPhoneNumber(sp[0]))
		{
			if (CheckSimcardType(sp[1]))
			{
				if (this->_toolsObject.is_number(sp[2]))
				{
					if (this->_toolsObject.is_number(sp[3]))
					{
						/*this->_deviceInfo.deviceMobile.mobileNumber = this->_toolsObject.toLongLong(sp[0]);
						this->_toolsObject.SimCardTypeFromString(this->_deviceInfo.deviceMobile.mobileSimCardType, sp[1]);
						this->_deviceInfo.deviceMobile.mobileChargeValue = this->_toolsObject.toInteger(sp[2]);
						this->_deviceInfo.deviceMobile.mobileSignalValue = this->_toolsObject.toInteger(sp[3]);*/
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool SyntaxChecker::CheckPhoneNumber(std::string Input) const
{
	//this->_deviceInfo.deviceMobile.mobileNumber = 0;
	if (this->_toolsObject.is_numberWithLen(Input, 10))
	{
		//this->_deviceInfo.deviceMobile.mobileNumber = this->_toolsObject.toLongLong(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckSimcardType(std::string Input)
{
	/*if (this->_toolsObject.SimCardTypeFromString(this->_deviceInfo.deviceMobile.mobileSimCardType, Input))
	{
		return true;
	}*/
	return false;
}

bool SyntaxChecker::CheckServer(std::string Input)
{
	/*this->_deviceInfo.deviceServer.serverPort = 0;
	this->_deviceInfo.deviceServer.serverIP.ip1 = 0;
	this->_deviceInfo.deviceServer.serverIP.ip2 = 0;
	this->_deviceInfo.deviceServer.serverIP.ip3 = 0;
	this->_deviceInfo.deviceServer.serverIP.ip4 = 0;*/

	if (Input == "-")
	{
		return true;
	}

	Splitter sp(Input, ",");
	if (sp.size() == 2)
	{
		/*if (CheckIp(sp[0], this->_deviceInfo.deviceServer.serverIP))
		{
			if (this->_toolsObject.is_number(sp[1]))
			{
				this->_deviceInfo.deviceServer.serverPort = this->_toolsObject.toInteger(sp[1]);
				return true;
			}
		}*/
	}
	return false;
}

bool SyntaxChecker::CheckIp(std::string Input, ns__IP& IPObject) const
{
	Splitter sp(Input, ".");

	IPObject.ip1 = 0;
	IPObject.ip2 = 0;
	IPObject.ip3 = 0;
	IPObject.ip4 = 0;

	if (sp.size() == 4)
	{
		if (this->_toolsObject.is_number(sp[0]) && this->_toolsObject.toInteger(sp[0]) >= 0 && this->_toolsObject.toInteger(sp[0]) <= 255)
		{
			if (this->_toolsObject.is_number(sp[1]) && this->_toolsObject.toInteger(sp[1]) >= 0 && this->_toolsObject.toInteger(sp[1]) <= 255)
			{
				if (this->_toolsObject.is_number(sp[2]) && this->_toolsObject.toInteger(sp[2]) >= 0 && this->_toolsObject.toInteger(sp[2]) <= 255)
				{
					if (this->_toolsObject.is_number(sp[3]) && this->_toolsObject.toInteger(sp[3]) >= 0 && this->_toolsObject.toInteger(sp[3]) <= 255)
					{
						IPObject.ip1 = this->_toolsObject.toInteger(sp[0]);
						IPObject.ip2 = this->_toolsObject.toInteger(sp[1]);
						IPObject.ip3 = this->_toolsObject.toInteger(sp[2]);
						IPObject.ip4 = this->_toolsObject.toInteger(sp[3]);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool SyntaxChecker::CheckGPRS(std::string Input)
{
	/*this->_deviceInfo.deviceGprs.gprsPort = 0;
	this->_deviceInfo.deviceGprs.gprsIP.ip1 = 0;
	this->_deviceInfo.deviceGprs.gprsIP.ip2 = 0;
	this->_deviceInfo.deviceGprs.gprsIP.ip3 = 0;
	this->_deviceInfo.deviceGprs.gprsIP.ip4 = 0;*/

	if (Input == "-")
	{
		return true;
	}

	Splitter sp(Input, ",");
	if (sp.size() == 2)
	{
		/*if (CheckIp(sp[0], this->_deviceInfo.deviceGprs.gprsIP))
		{
			if (this->_toolsObject.is_number(sp[1]))
			{
				this->_deviceInfo.deviceGprs.gprsPort = this->_toolsObject.toInteger(sp[1]);
				return true;
			}
		}*/
	}
	return false;
}

bool SyntaxChecker::CheckSMSContact(std::string Input) const
{
	if (Input == "-")
	{
		return true;
	}

	Splitter sp(Input, ",");
	for (int i = 0; i < sp.size(); i++)
	{
		if (!this->_toolsObject.is_numberWithLen(sp[i], 10))
		{
			return false;
		}
		//this->_deviceInfo.deviceSMSContact.push_back(this->_toolsObject.toLongLong(sp[i]));
	}
	return true;
}

bool SyntaxChecker::CheckSMSConfig(std::string Input) const
{
	if (Input == "-")
	{
		return true;
	}

	Splitter sp(Input, ",");
	for (int i = 0; i < sp.size(); i++)
	{
		if (!this->_toolsObject.is_numberWithLen(sp[i], 10))
		{
			return false;
		}
		//this->_deviceInfo.deviceSMSConfig.push_back(this->_toolsObject.toLongLong(sp[i]));
	}
	return true;
}

bool SyntaxChecker::CheckSMSCommand(std::string Input)
{
	//this->_deviceInfo.deviceSMSCommand = false;
	if (Input == "-")
	{
		return true;
	}

	if (Input == "0" || Input == "1")
	{
		//this->_deviceInfo.deviceSMSCommand = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckGSMCommand(std::string Input)
{
	//this->_deviceInfo.deviceGSMCommand = false;
	if (Input == "-")
	{
		return true;
	}

	if (Input == "0" || Input == "1")
	{
		//this->_deviceInfo.deviceGSMCommand = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckKeyboardCommand(std::string Input)
{
	//this->_deviceInfo.deviceKeyBoardCommand = false;
	if (Input == "-")
	{
		return true;
	}

	if (Input == "0" || Input == "1")
	{
		//this->_deviceInfo.deviceKeyBoardCommand = this->_toolsObject.toInteger(Input);
		return true;
	}
	return false;
}

bool SyntaxChecker::CheckSensorRelay(std::string Input, int index)
{
	/*this->_sensorTemp.sensorRelay.relayIndex = 0;
	this->_sensorTemp.sensorRelay.relayOnOff = false;*/
	if (Input == "-")
	{
		return true;
	}

	Splitter sp(Input, ":");
	if (sp.size() == 2)
	{
		if (sp[0] == "" || this->_toolsObject.is_number(sp[0]))
		{
			/*this->_sensorTemp.sensorRelay.relayIndex = this->_toolsObject.toInteger(sp[0]);
			if (sp[1] == "0" || sp[1] == "1")
			{
				this->_sensorTemp.sensorRelay.relayOnOff = (this->_toolsObject.toInteger(sp[1]) != 0 ? true : false);
				return true;
			}*/
		}
		else
		{
			this->_errorData = "No number";
		}
	}
	else
	{
		this->_errorData = "No number";
	}
	return false;
}

std::string SyntaxChecker::GenerateLable(std::string Prefix, int & Index, int Length) const
{
	std::vector<std::string> labels = { "R","S","T","N" };
	std::string tempStr;
	if (Index < Length)
	{
		tempStr = Prefix + labels[Index];
	}
	else
	{
		tempStr = Prefix + labels[Index % Length] + this->_toolsObject.toString(static_cast<int>(Index / Length));
	}
	Index++;
	return tempStr;
}

bool SyntaxChecker::CheckSensors(std::string Input, ns__SensorName SensorName, std::string SensorNikeName /*= ""*/)
{
	if (Input == "-")
	{
		this->_sensorTemp.sNkName = (SensorNikeName == "" ? this->dictionarySensorNikeName[SensorName] : SensorNikeName);
		this->_sensorTemp.sVal = -100;
		this->_sensorTemp.sType = ns__SensorType::Multi;

		// Add sensor to list
		this->AddToSensorsList(this->_sensorTemp, SensorName);

		return true;
	}
	else
	{
		if (this->_toolsObject.is_double(Input))
		{
			this->_sensorTemp.sNkName = (SensorNikeName == "" ? this->dictionarySensorNikeName[SensorName] : SensorNikeName);
			this->_sensorTemp.sVal = std::stod(Input.c_str());
			this->_sensorTemp.sType = ns__SensorType::Multi;

			// Add sensor to list
			this->AddToSensorsList(this->_sensorTemp, SensorName);

			return true;
		}
		this->_errorData = "No number";
	}
	return false;
}

bool SyntaxChecker::CheckSensorsBit(std::string Input, ns__SensorName SensorName, int Length)
{
	if (this->_toolsObject.is_number(Input))
	{
		int number = this->_toolsObject.toInteger(Input);

		if (number >= 0 && number < std::pow(2, Length))
		{
			ConverterDecimal converter;
			std::vector<bool> bList = converter.ToBinaryArray(number, Length);
			for each (bool var in bList)
			{
				this->_sensorTemp.sNkName = this->dictionarySensorNikeName[SensorName];
				this->_sensorTemp.sVal = var;
				this->_sensorTemp.sType = ns__SensorType::Mono;

				// Add sensor to list
				this->AddToSensorsList(this->_sensorTemp, SensorName);
			}

			return true;
		}
		else
		{
			this->_errorData = "No number";
		}
	}
	else if (Input == "-")
	{
		for (int i = 0;i < Length;i++)
		{
			this->_sensorTemp.sNkName = this->dictionarySensorNikeName[SensorName];
			this->_sensorTemp.sVal = -100;
			this->_sensorTemp.sType = ns__SensorType::Mono;

			// Add sensor to list
			this->AddToSensorsList(this->_sensorTemp, SensorName);
		}

		return true;
	}
	else
	{
		this->_errorData = "No number";
	}
	return false;
}

bool SyntaxChecker::CheckACAmpereFault(std::string Input, int Index, int Length)
{
	if (this->_toolsObject.is_number(Input))
	{
		int number = this->_toolsObject.toInteger(Input);
		if (number == 0)
		{
			return true;
		}
		else if (number > 0 && number < 256)
		{
			ConverterDecimal converter;
			std::vector<bool> bList = converter.ToBinaryArray(number, Length);
			int index = 0;
			// Find ACAmpere sensors list in device sensor list
			for (;index < this->_deviceInfo.dSensors.size();index++)
			{
				if (this->_deviceInfo.dSensors[index].seName == ns__SensorName::ACAMPERE)
				{
					break;
				}
			}
			// Check if have fault
			// set sensor value to 9999 (a big number)
			if (this->_deviceInfo.dSensors.size() > index)
			{
				for (size_t i = Index; i < Index + Length; i++)
				{
					if (this->_deviceInfo.dSensors[index].seVal.size() > i)
					{
						if (bList[i % Length] == true)
						{
							this->_deviceInfo.dSensors[index].seVal[i] = 9999;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return false;
}

bool SyntaxChecker::CheckRelay(std::string Input, int Length)
{
	if (this->_toolsObject.is_number(Input))
	{
		int number = this->_toolsObject.toInteger(Input);

		if (number >= 0 && number < std::pow(2, Length))
		{
			ConverterDecimal converter;
			std::vector<bool> bList = converter.ToBinaryArray(number, Length);
			// ReSharper disable once CppEntityNeverUsed
			for each (bool var in bList)
			{
				this->_deviceInfo.dRelays.push_back(this->_toolsObject.toInteger(Input));
			}

			return true;
		}
		else
		{
			this->_errorData = "No number";
		}
	}
	else
	{
		this->_errorData = "No number";
	}
	return false;
}

bool SyntaxChecker::AnalyzeRecievedData(std::string & InputOrg, bool& IsSet)
{
	this->_deviceInfo.dErr.eNo = 0;
	this->_deviceInfo.dErr.eType = ns__SettingLevel::Normal;
	this->_deviceInfo.dErr.eMsg.clear();

	std::string Input = InputOrg;

	this->RemoveUnusedChars(Input);

	Splitter sp(Input, "#");
	Input = sp[0];

	//this->_deviceInfo.deviceServer.serverPort = 0;

	std::string a = "";

	sp.reset(Input, ",");

	IsSet = false;

	if (sp.size() > 1)
	{
		if (sp[1] == "SMP" || sp[1] == "TIM" || sp[1] == "DAT" || sp[1] == "OUT" || sp[1] == "PIC" || sp[1] == "PRT" || sp[1] == "ALM")
		{
			IsSet = true;
			return true;
		}
		if (sp[1] == "LIVE" || sp[1] == "CAM" || sp[1] == "DATS" || sp[1] == "DATM")
		{
			IsSet = false;
			return true;
		}
		IsSet = true;
		return true;
	}
	return true;
}

bool SyntaxChecker::CheckSTTSyntax(std::string& InputOrg, int& IsCamera)
{
	this->_deviceInfo.dErr.eNo = 0;
	this->_deviceInfo.dErr.eType = ns__SettingLevel::Normal;
	this->_deviceInfo.dErr.eMsg.clear();

	std::string Input = InputOrg;
	this->RemoveUnusedChars(Input);

	Splitter sp(Input, "#");
	Input = sp[0];

	std::string a = "";

	int index = 0;
	sp.reset(Input, ",");

	IsCamera = 0;
	/*
	* If size is 2
	* it is from device or camera
	* say I am alive
	*/
	// Device is alive
	if (sp.size() == 6 && this->_toolsObject.toUpper(sp[1]) == "LIVE")
	{
		if (CheckDeviceNumber(sp[index++]))
		{
			index++;
			if (this->CheckACAmpereFault(sp[index++], 0, 8))
			{
				if (this->CheckACAmpereFault(sp[index++], 8, 8))
				{
					if (this->CheckACAmpereFault(sp[index++], 16, 8))
					{
						if (this->CheckACAmpereFault(sp[index], 24, 8))
						{
							if (sp[2] != "0" || sp[3] != "0" || sp[4] != "0" || sp[5] != "0")
							{
								IsCamera = -1;
							}
							return true;
						}
						return false;
					}
					return false;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	if (sp.size() == 3 && this->_toolsObject.toUpper(sp[1]) == "CAM")
	{
		if (CheckDeviceNumber(sp[index]))
		{
			this->_deviceInfo.dModel = ns__DeviceModel::SECTIONNER;
			IsCamera = 1;
			return true;
		}
		return false;
	}
	if (sp.size() == 3 && this->_toolsObject.toUpper(sp[1]) == "ALM")
	{
		if (CheckDeviceNumber(sp[index]))
		{
			this->_deviceInfo.dModel = ns__DeviceModel::ALARM;
			IsCamera = 0;
			return true;
		}
		return false;
	}

	if (sp.size() == 3 && this->_toolsObject.toUpper(sp[1]) == "DAT")
	{
		IsCamera = 0;
		return true;
	}
	if (sp.size() == 6 && this->_toolsObject.toUpper(sp[1]) == "OUT")
	{
		IsCamera = -1;
		return true;
	}
	if (sp.size() == 2 && this->_toolsObject.toUpper(sp[1]) == "RST")
	{
		IsCamera = 0;
		return true;
	}
	if (sp.size() > 4)
	{
		std::string label;
		IsCamera = -1;
		// "[deviceNo],[DATS|DATM],[sampling],[datetime],1,[relayValue],[digitalInput],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[dcv],[[tem]|-],[[hum]|-],2,[relayValue],[digitalInput],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],3,[relayValue],[digitalInput],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],4,[relayValue],[digitalInput],[acv],[acv],[acv],[acv],[acv],[acv],[acv],[acv],5,[aca],[aca],[aca],[aca],[aca],[aca],[aca],[dca],6,[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],7,[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],8,[aca],[aca],[aca],[aca],[aca],[aca],[aca],[aca],9,[cosQ],[cosQ],[cosQ]#"
		this->_deviceInfo.dSensors.clear();
		/*this->_deviceInfo.deviceSMSConfig.clear();
		this->_deviceInfo.deviceSMSContact.clear();*/
		std::vector<std::string> labels = { "R","S","T","N" };
		std::vector<std::string> labelsVoltage = { "VR","VS","VT","VR1","VS1","VT1","VR2","VS2","VT2","VR3","VS3","VT3","VR4","VS4","VT4","VR5","VS5","VT5","VR6","VS6","VT6","VRL","VSL","VTL","VR11","VS11","VT11","VR22","VS22","VT22","VR33","VS33","VT33" };
		std::vector<std::string> labelsAmpereS = { "IR","IS","IT","IN","IR1","IS1","IT1","IN1","IR2","IS2","IT2","IN2","IR3","IS3","IT3","IN3","IR4","IS4","IT4","IN4","IR5","IS5","IT5","IN5","IR6","IS6","IT6","IN6","IRL","ISL","ITL","INL" };
		std::vector<std::string> labelsAmpereM = { "IR1","IS1","IT1","IN1","IR2","IS2","IT2","IN2","IR3","IS3","IT3","IN3","IR4","IS4","IT4","IN4","IR5","IS5","IT5","IN5","IR6","IS6","IT6","IN6","IRL","ISL","ITL","INL","IR","IS","IT","IN" };
		std::vector<std::string> labelsCosq = { "CosR","CosS","CosT" };
		int indexVoltage = 0;
		int indexAmpere = 0;
		int indexCosq = 0;
		std::string tempName = "";

		if (this->CheckDeviceNumber(sp[index++]))
		{
			if (this->CheckDeviceType(sp[index++]))
			{
				if (this->CheckSampling(sp[index++]))
				{
					if (this->CheckDateTime(sp[index++]))
					{
						this->_deviceInfo.dSensors.clear();
						const size_t dataLen = sp.size();
						while (index < dataLen)
						{
							std::string boardNumber = sp[index++];
							if (this->_toolsObject.is_number(boardNumber))
							{
								int boardNo = this->_toolsObject.toInteger(boardNumber);
								if (boardNo > 0 && boardNo < 10)
								{
									if (boardNo < 5)
									{
										try
										{
											if (this->CheckSensorsBit(sp[index++], ns__SensorName::RELAY, 8))
											{
												if (this->CheckSensorsBit(sp[index++], ns__SensorName::DIGITALINPUT, 8))
												{
													const int len = index + 7;
													for (;index < len; index++)
													{
														//tempName = this->GenerateLable("V", indexVoltage, 3);
														try
														{
															label = indexVoltage < labelsVoltage.size() ? labelsVoltage[indexVoltage] : "ACV" + this->_toolsObject.toString(indexVoltage);
															indexVoltage++;
															if (!this->CheckSensors(sp[index], ns__SensorName::ACVOLTAGE, label))
															{
																return false;
															}
														}
														catch (const std::exception&)
														{

														}
													}
													if (boardNo == 1)
													{
														//tempName = this->GenerateLable("V", indexVoltage, 3);
														try
														{
															label = indexVoltage < labelsVoltage.size() ? labelsVoltage[indexVoltage] : "ACV" + this->_toolsObject.toString(indexVoltage);
															indexVoltage++;
															if (!this->CheckSensors(sp[index++], ns__SensorName::ACVOLTAGE, label))
															{
																return false;
															}
														}
														catch (const std::exception&)
														{

														}
													}
													else
													{
														//tempName = this->GenerateLable("V", indexVoltage, 3);
														try
														{
															label = indexVoltage < labelsVoltage.size() ? labelsVoltage[indexVoltage] : "ACV" + this->_toolsObject.toString(indexVoltage);
															indexVoltage++;
															if (!this->CheckSensors(sp[index++], ns__SensorName::ACVOLTAGE, label))
															{
																return false;
															}
														}
														catch (const std::exception&)
														{

														}
													}
													if (this->CheckSensors(sp[index++], ns__SensorName::TEMPERATURE))
													{
														try
														{
															if (!this->CheckSensors(sp[index++], ns__SensorName::HUMIDITY))
															{
																return false;
															}
														}
														catch (const std::exception&)
														{

														}
													}
													else
													{
														return false;
													}
												}
												else
												{
													return false;
												}
											}
											else
											{
												return false;
											}
										}
										catch (const std::exception&)
										{

										}
									}
									else if (boardNo < 9)
									{
										const int len = index + 7;
										for (;index < len; index++)
										{
											//tempName = this->GenerateLable("I", indexAmpere, 4);
											try
											{
												label = (this->_deviceInfo.dModel == ns__DeviceModel::MANAGER ? (indexAmpere < labelsAmpereM.size() ? labelsAmpereM[indexAmpere] : "ACA") : (indexAmpere < labelsAmpereM.size() ? labelsAmpereM[indexAmpere] : "ACA")) + this->_toolsObject.toString(indexAmpere);
												indexAmpere++;
												if (!this->CheckSensors(sp[index], ns__SensorName::ACAMPERE, label))
												{
													return false;
												}
											}
											catch (const std::exception&)
											{

											}
										}
										if (boardNo == 5)
										{
											//tempName = this->GenerateLable("I", indexAmpere, 4);
											try
											{
												label = (this->_deviceInfo.dModel == ns__DeviceModel::MANAGER ? (indexAmpere < labelsAmpereM.size() ? labelsAmpereM[indexAmpere] : "ACA") : (indexAmpere < labelsAmpereM.size() ? labelsAmpereM[indexAmpere] : "ACA")) + this->_toolsObject.toString(indexAmpere);
												indexAmpere++;
												if (!this->CheckSensors(sp[index++], ns__SensorName::ACAMPERE, label))
												{
													return false;
												}
											}
											catch (const std::exception&)
											{

											}
										}
										else
										{
											//tempName = this->GenerateLable("I", indexAmpere, 4);
											try
											{
												label = (this->_deviceInfo.dModel == ns__DeviceModel::MANAGER ? (indexAmpere < labelsAmpereM.size() ? labelsAmpereM[indexAmpere] : "ACA") : (indexAmpere < labelsAmpereM.size() ? labelsAmpereM[indexAmpere] : "ACA")) + this->_toolsObject.toString(indexAmpere);
												indexAmpere++;
												if (!this->CheckSensors(sp[index++], ns__SensorName::ACAMPERE, label))
												{
													return false;
												}
											}
											catch (const std::exception&)
											{

											}
										}
									}
									else if (boardNo == 9)
									{
										const int len = index + 3;
										for (;index < len; index++)
										{
											try
											{
												label = indexCosq < labelsCosq.size() ? labelsCosq[indexCosq] : "COSq" + this->_toolsObject.toString(indexCosq);
												indexCosq++;
												if (!this->CheckSensors(sp[index], ns__SensorName::COSQ, label))
												{
													return false;
												}
											}
											catch (const std::exception&)
											{

											}
										}
									}
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		IsCamera = 0;
		this->_errorData = "Unrecognized data";
		return false;
	}

	return true;
}

bool SyntaxChecker::CheckSetSyntax(std::string& InputOrg)
{
	this->_deviceInfo.dErr.eNo = 0;
	this->_deviceInfo.dErr.eType = Normal;
	this->_deviceInfo.dErr.eMsg.clear();

	std::string Input = InputOrg;

	this->RemoveUnusedChars(Input);

	Splitter sp(Input, "#");
	Input = sp[0];

	std::string a = "";

	int index = 0;
	sp.reset(Input, ",");

	/*
	* If size is 2
	* it is from device or camera
	* say I am alive
	*/
	if (sp.size() == 3 && this->_toolsObject.toUpper(sp[1]) == "SET")
	{
		// [deviceNo],SET,[sampling]#
		if (this->CheckDeviceNumber(sp[index++]))
		{
			index++;
			if (this->_toolsObject.is_number(sp[index]))
			{
				this->_deviceInfo.dSamplingTime = this->_toolsObject.toInteger(sp[index]);
				return true;
			}
			else
			{
				this->_errorData = "Sampling number is incorrect";
			}
		}
		else
		{
			this->_errorData = "Device serial number is incorrect";
		}
	}
	else
	{
		this->_errorData = "Unrecognized data";
	}

	return false;
}

bool SyntaxChecker::CheckPartSyntax(std::string& InputOrg)
{
	this->_deviceInfo.dErr.eNo = 0;
	this->_deviceInfo.dErr.eType = Normal;
	this->_deviceInfo.dErr.eMsg.clear();

	ns__PicturePart picturePart;
	this->_deviceInfo.dPicture.pParts.push_back(picturePart);

	std::string Input = InputOrg;

	this->RemoveUnusedChars(Input);

	Splitter sp(Input, "#");
	Input = sp[0];

	std::string a = "";

	int index = 0;
	sp.reset(Input, ",");

	if (sp.size() == 5)
	{
		if (CheckDeviceNumber(sp[index++]))
		{
			if (this->_toolsObject.toUpper(sp[index++]) == "PRT")
			{
				if (this->CheckPartIndex(sp[index++])) // Picture size
				{
					std::string data = sp[index++];
					std::string crc = sp[index];
					if (this->CheckPartData(data, crc)) // Part size
					{
						return true;
					}
					this->_errorData = "Picture part data mismatch";
				}
				else
				{
					this->_errorData = "Part index error";
				}
			}
			else
			{
				this->_errorData = "Error";
			}
		}
		else
		{
			this->_errorData = "Device number mismatch";
		}
	}
	else
	{
		this->_errorData = "Size is not equal to 5";
	}

	return false;
}

bool SyntaxChecker::CheckPICSyntax(std::string& InputOrg)
{
	std::string Input = InputOrg;

	this->RemoveUnusedChars(Input);

	std::string a = "";
	std::string pictureMD5 = "";
	std::string pictureMD5Result = "";
	std::string pictureData = "";
	Splitter sp(Input, "#");
	Input = sp[0];

	int index = 0;
	sp.reset(Input, ",");

	if (sp.size() == 6)
	{
		if (CheckDeviceNumber(sp[index++]))
		{
			if (this->_toolsObject.toUpper(sp[index++]) == "PIC")
			{
				if (this->CheckPictureSize(sp[index++])) // Picture size
				{
					if (this->CheckPictrurePartSize(sp[index++])) // Part size
					{
						if (this->CheckPicturePartNumbers(sp[index++])) // Part numbers
						{
							if (this->CheckPictureCRC(sp[index])) // Picture CRC
							{
								return true;
							}
							else
							{
								this->_errorData = "Picture CRC";
							}
						}
						else
						{
							this->_errorData = "Picture part number";
						}
					}
					else
					{
						this->_errorData = "Picture part size";
					}
				}
				else
				{
					this->_errorData = "Picture size";
				}
			}
			else
			{
				this->_errorData = "Error";
			}
		}
		else
		{
			this->_errorData = "Device number";
		}
	}
	else
	{
		this->_errorData = "Size is not equal to 5";
	}

	return false;
}

bool SyntaxChecker::AddToSensorsList(ns__Sensor Sensor, ns__SensorName SensorName)
{
	for (int i = 0; i < this->_deviceInfo.dSensors.size();i++)
	{
		if (SensorName == this->_deviceInfo.dSensors[i].seName)
		{
			this->_deviceInfo.dSensors[i].seVal.push_back(Sensor.sVal);
			return true;
		}
	}
	this->sensorExtensionTemp.seVal.clear();
	this->sensorExtensionTemp.seName = SensorName;
	this->sensorExtensionTemp.seVal.push_back(this->_sensorTemp.sVal);
	this->_deviceInfo.dSensors.push_back(this->sensorExtensionTemp);

	return false;
}

//************************************
// Method:    GenerateCNFCommand
// FullName:  SConnection::GenerateCNFCommand
// Access:    private 
// Returns:   std::string
// Qualifier:
// Parameter: ns__Device DeviceObject
//
// Description: Generate CNF Command string sending to Device
//************************************
std::string SyntaxChecker::GenerateCNFCommand(ns__Device DeviceObject) const
{
	std::string strCommand = "";
	int replaceIndex = 0;
	for each (ns__RequestType request in DeviceObject.dRequests)
	{
		switch (request)
		{
		case ns__RequestType::SAMPLING:
		{
			strCommand = REQUEST_SET_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(DeviceObject.dSamplingTime), replaceIndex);
			break;
		}
		case ns__RequestType::DATETIME:
		{
			strCommand = REQUEST_TIME_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.Time_tToString(DeviceObject.dDateTime, "%Y-%m-%d %H:%M:%S"), replaceIndex);
			break;
		}
		case ns__RequestType::OUTRELAY:
		{
			ConverterDecimal converter;
			strCommand = REQUEST_OUT_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(converter.Binary_Decimal_Array(DeviceObject.dRelays)), replaceIndex);
			break;
		}
		case ns__RequestType::PARTS:
		{
			strCommand = REQUEST_PART_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(DeviceObject.dPicture.pParts[0].ppIndex), replaceIndex);
			break;
		}
		case ns__RequestType::PICTURE:
		{
			strCommand = REQUEST_PICTURE_STRING;
			break;
		}
		default:
		{
			break;
		}
		}
	}

	return strCommand;
}

std::map<std::string, ns__SensorName> SyntaxChecker::GetSensorsName(std::string Input) const
{
	// Initiate to empty map
	std::map<std::string, ns__SensorName> outPut = {};
	// Split input by ,
	Splitter sp(Input, ",");
	if (sp.size() > 0)
	{
		Splitter sp2("test", "test");
		for (int i = 0; i < sp.size(); i++)
		{
			if (sp2.size() == 2)
			{
				sp2.reset(sp[i], ">");
				this->_toolsObject.SensorNameFromString(outPut[this->_toolsObject.toUpper(sp2[0])], this->_toolsObject.toUpper(sp2[1]));
			}
		}
	}

	return outPut;
}

std::map<ns__SensorName, std::string> SyntaxChecker::GetSensorsNikeName(std::string Input) const
{
	// Initiate to empty map
	std::map<ns__SensorName, std::string> outPut = {};
	ns__SensorName sensorNameTemp;
	// Split input by ,
	Splitter sp(Input, ",");
	if (sp.size() > 0)
	{
		Splitter sp2("test", "test");
		for (int i = 0; i < sp.size(); i++)
		{
			if (sp2.size() == 2)
			{
				sp2.reset(sp[i], ">");
				this->_toolsObject.SensorNameFromString(sensorNameTemp, this->_toolsObject.toUpper(sp2[1]));
				outPut[sensorNameTemp] = this->_toolsObject.toUpper(sp2[0]);
			}
		}
	}

	return outPut;
}

void SyntaxChecker::RemoveUnusedChars(std::string& Input) const
{
	this->_toolsObject.replaceOne(Input, "\r\n", "", 0);
	this->_toolsObject.replaceOne(Input, "\n", "", 0);
	this->_toolsObject.replaceOne(Input, "\r", "", 0);
	Input.erase(std::remove(Input.begin(), Input.end(), '\r'), Input.end());
	Input.erase(std::remove(Input.begin(), Input.end(), '\n'), Input.end());
}

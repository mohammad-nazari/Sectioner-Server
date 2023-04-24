#include "Calibration.h"

Calibration::Calibration(void)
{
}

Calibration::~Calibration(void)
{
}

void Calibration::SetDefault(void)
{
	this->Read(DEFAULTJSON);
}

std::vector<SensorCalibration> Calibration::SensorsCalibration() const
{
	return _sensorsCalibration;
}

void Calibration::SensorsCalibration(std::vector<SensorCalibration> val)
{
	_sensorsCalibration = val;
}

int Calibration::Read(std::string JsonData)
{
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;

	try
	{
		bool parsingSuccessful = reader.parse(JsonData, root);
		if(!parsingSuccessful)
		{
			return 0;
		}
	}
	catch(...)
	{
		return 0;
	}
	if(root.type() != Json::objectValue)
	{
		return 0;
	}

	if(root["SensorCalibration"].type() != Json::arrayValue)
	{
		return 0;
	}
	bool isExist = false;
	for(int i = 0; i < root["SensorCalibration"].size(); i++)
	{
		isExist = false;
		for(int j = 0; j < this->_sensorsCalibration.size(); j++)
		{
			if(root["SensorCalibration"][i].get("Name", "test").asString() == this->_sensorsCalibration[j].Name())
			{
				isExist = true;
				break;
			}
		}

		if(isExist == false)
		{
			SensorCalibration sensorsCalibration;
			sensorsCalibration.Name(root["SensorCalibration"][i].get("Name", "TEMPERATURE").asString());
			sensorsCalibration.Zero(root["SensorCalibration"][i].get("Zero", -50.000000).asDouble());
			sensorsCalibration.Span(root["SensorCalibration"][i].get("Span", 150.000000).asDouble());
			sensorsCalibration.Min(root["SensorCalibration"][i].get("Min", -50.000000).asDouble());
			sensorsCalibration.Max(root["SensorCalibration"][i].get("Max", 150.000000).asDouble());

			this->_sensorsCalibration.push_back(sensorsCalibration);
		}
	}

	return 1;
}

int Calibration::ReadFromFile(std::string FileName)
{
	this->SetDefault();
	try
	{
		std::ifstream ifs(FileName.c_str());
		std::string strConfig((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();

		return this->Read(strConfig);
	}
	catch(...)
	{
		return 0;
	}
	return 0;
}

int Calibration::ReadFromString(std::string JsonData)
{
	this->SetDefault();
	return this->Read(JsonData);
}

int Calibration::Write(std::string& JsonData)
{
	Json::Value root;
	Json::StyledWriter writer;

	for(unsigned int i = 0; i < root["SensorCalibration"].size(); i++)
	{
		root["SensorCalibration"][i]["Name"] = this->_sensorsCalibration[i].Name();
		root["SensorCalibration"][i]["Zero"] = this->_sensorsCalibration[i].Zero();
		root["SensorCalibration"][i]["Span"] = this->_sensorsCalibration[i].Span();
		root["SensorCalibration"][i]["Min"] = this->_sensorsCalibration[i].Min();
		root["SensorCalibration"][i]["Max"] = this->_sensorsCalibration[i].Max();
	}

	JsonData = writer.write(root);

	return 1;
}

int Calibration::WriteToFile(std::string FileName)
{
	try
	{
		std::string outputConfig = "";
		this->Write(outputConfig);
		std::ofstream ofs(FileName.c_str());
		ofs << outputConfig;
		ofs.close();
	}
	catch(...)
	{
		return 0;
	}

	return 1;
}

int Calibration::WriteToString(std::string & JsonData)
{
	return this->Write(JsonData);
}

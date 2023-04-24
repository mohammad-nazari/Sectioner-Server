#include "DBConnection.h"
#include "Splitter.h"

DBConnection::DBConnection() : connection(nullptr)
{
}


DBConnection::~DBConnection()
{
}

int DBConnection::UpdateDeviceRow(ns__Device DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		/*db::Device dev(&conn);
		dev.SetDevsocketnumber(DeviceInfo.dSerialNumber);
		dev.SetDevsocketnumber(DeviceInfo.dSocket);
		dev.SetDevip(this->toolsObject.toString(DeviceInfo.dIP.ip1)+"."+this->toolsObject.toString(DeviceInfo.dIP.ip2)+"."+this->toolsObject.toString(DeviceInfo.dIP.ip3)+"."+this->toolsObject.toString(DeviceInfo.dIP.ip4));
		dev.SetDevport(DeviceInfo.dPort);
		dev.SetDevstatus('1');
		dev.SetDevsimcardnumber(DeviceInfo.deviceMobile.mobileNumber);
		dev.SetDevsimcardtype(this->toolsObject.SimCardTypeToString(DeviceInfo.deviceMobile.mobileSimCardType));

		// Update device information and return result
		dev.update();

		return 1;*/

		Query q(conn);

		std::string query = UPDATEDEVICE;

		size_t position = 0;

		position = this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dSocket), position); //DevSocket
		position = this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dIP.ip1) + "." + this->_toolsObject.toString(DeviceInfo.dIP.ip2) + "." + this->_toolsObject.toString(DeviceInfo.dIP.ip3) + "." + this->_toolsObject.toString(DeviceInfo.dIP.ip4), position); //DevIP
		position = this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dPort), position); //DevPort
		position = this->_toolsObject.replaceOne(query, "%s", "1", position); //DevStatus
		this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dSerialNumber), position); //DevSerialNumber

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (...)
	{
		return 0;
	}

}

int DBConnection::UpdateDeviceRowSetting(ns__Device DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Device dev(conn, DeviceInfo.dSerialNumber);

		dev.SetDevcity(DeviceInfo.dCity);
		dev.SetDevlocation(DeviceInfo.dLocation);
		dev.SetDevname(DeviceInfo.dNikeName);
		dev.SetDevsamplingtime(DeviceInfo.dSamplingTime);
		dev.SetDevgps(this->_toolsObject.toStringDouble(DeviceInfo.dGPS.gX, 16) + "," + this->_toolsObject.toStringDouble(DeviceInfo.dGPS.gY, 16));
		dev.SetDevtranspower(DeviceInfo.dTransPower);
		dev.SetDevtablecapacity(DeviceInfo.dTableCapacity);
		dev.SetDevkeytype(DeviceInfo.dKeyName);
		dev.SetDevsmsterm(DeviceInfo.dSmsTerm);

		// Update device information and return result
		dev.update();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::CreateDeviceRow(ns__Device DeviceInfo, std::string CalibrationData) const
{
	try
	{
		// save connection error
		StderrLog log;

		// Generate and connect
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Device dev(&conn);
		dev.SetDevserialnumber(DeviceInfo.dSerialNumber);
		dev.SetDevtype(this->_toolsObject.DeviceModelToString(DeviceInfo.dModel));
		dev.SetDevcity("NoCity");
		dev.SetDevlocation("NoLocation");
		dev.SetDevname("NoName" + this->_toolsObject.toString(DeviceInfo.dSerialNumber));
		dev.SetDevsocketnumber(DeviceInfo.dSocket);
		dev.SetDevip(this->_toolsObject.toString(DeviceInfo.dIP.ip1) + "." + this->_toolsObject.toString(DeviceInfo.dIP.ip2) + "." + this->_toolsObject.toString(DeviceInfo.dIP.ip3) + "." + this->_toolsObject.toString(DeviceInfo.dIP.ip4));
		dev.SetDevport(DeviceInfo.dPort);
		dev.SetDevsimcardtype("MCI");
		// ReSharper disable once CppMsExtBindingRValueToLvalueReference
		dev.SetDevcalibration(CalibrationData);
		dev.SetDevsamplingtime(60);

		// Add new device into database and return result
		return dev.insert();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::CheckSerialNumber(std::string Input) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = SELECTDEVICESERIAL;

		this->_toolsObject.replaceOne(query, "%s", Input, 0);

		q.get_result(query);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0; //Device Serial Number Does not exist
		}

		q.fetch_row();
		db::Device tbl(&conn, &q); // spawns an object from Query object

		q.free_result();

		return tbl.GetDevserialnumber();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

//************************************
// Method:    CheckUser
// FullName:  DBConnection::CheckUser
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: ns__User
//
// Description: Generate a connection to data base
//				check information of user
//************************************
int DBConnection::CheckUser(ns__User &UserInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Create object and set username and password to check
		db::Users userObj(conn, UserInfo.uName, UserInfo.uPassword);
		if (userObj.GetUserid() > 0)
		{
			UserInfo.uId = userObj.GetUserid();
			UserInfo.uFirstName = userObj.GetUserfirstname();
			UserInfo.uLastName = userObj.GetUserlastname();
			// Convert string to enum
			this->_toolsObject.UserTypeFromString(UserInfo.uType, userObj.GetUsertype());
		}
		// return result
		return userObj.GetUserid();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

//************************************
// Method:    ConnectToDatabase
// FullName:  DBConnection::ConnectToDatabase
// Access:    private 
// Returns:   bool
// Qualifier:
//
// Description: Set a Connection to database
//************************************
bool DBConnection::ConnectToDatabase()
{
	try
	{
		this->connection = new Database(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &(this->log));
		if (!this->connection->Connected())
		{
			return false; // DATABASE Connection Error
		}

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}

}

int DBConnection::CheckDevice(ns__Device& DeviceInfo)
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Create object and set username and password to check
		db::Device deviceObj(conn, DeviceInfo.dSerialNumber);

		// return result
		return deviceObj.GetDevserialnumber();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::CheckUserDevice(ns__User UserInfo, ns__Device DeviceInfo, int AllowType/*=0*/)
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Create object and set username and password to check
		db::Userdevice userDeviceObj(conn, UserInfo.uId, DeviceInfo.dSerialNumber);

		// Check for write to device allowing
		if (AllowType == 1)
		{
			if (userDeviceObj.GetId() != 0 && userDeviceObj.GetUpdateact() == 0)
			{
				// User Not allow to Write to device
				return -2;
			}
		}

		// return result
		return userDeviceObj.GetId();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetUserDeviceList(ns__User UserInfo, std::vector<ns__Device> &DeviceList) const
{
	try
	{
		ns__Device deviceTemp;

		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		if (this->GetUserInfo(UserInfo) > 0)
		{
			// String of query
			std::string strQuery = SELECTUSERDEVICELIST;

			// Replace Values
			this->_toolsObject.replaceOne(strQuery, "%s", this->_toolsObject.toString(UserInfo.uId), 0);

			q.get_result(strQuery);
			if (q.num_rows() < 1)
			{
				q.free_result();
				return 0; //Device Serial Number Does not exist
			}

			// Reset device list
			DeviceList.clear();

			// Get device info in loop
			while (q.fetch_row())
			{
				db::Userdevice tbl(&conn, &q); // spawns an object from Query object

				if (GetDeviceInfo(tbl.GetDeviceid(), deviceTemp) == 1)
				{
					DeviceList.push_back(deviceTemp);
				}
			}

			q.free_result();

			return 1;
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::GetDeviceInfo(int DeviceId, ns__Device& DeviceInformation) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // MYSQL Connection Error
		}

		Query q(conn);

		std::string query = SELECTDEVICEINFO;

		this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceId), 0);

		q.get_result(query);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0; // User name and or password is incorrect
		}

		q.fetch_row();
		db::Device tbl(&conn, &q); // spawns an object from Query object
		this->FillDeviceInformation(DeviceInformation, tbl);

		q.free_result();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

std::vector<ns__Device> DBConnection::getUDeviceList() const
{
	return this->userDeviceList;
}

void DBConnection::setUserDeviceList()
{
	this->userDeviceList = userDeviceList;
}

int DBConnection::GetDeviceSetting(ns__Device& DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Create object and set username and password to check
		db::Device deviceObj(conn, DeviceInfo.dSerialNumber);

		if (deviceObj.GetDevserialnumber() > 0)
		{
			this->FillDeviceInformation(DeviceInfo, deviceObj);
			return 1;
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::RemoveDeviceSms(ns__Device& DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = REMOVEDEVICESMS;

		size_t position = 0;
		this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dSerialNumber), position); //Old city name

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::GetDeviceSettingSms(ns__Device& DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Create object and set username and password to check
		db::Device deviceObj(conn, DeviceInfo.dSerialNumber);

		if (deviceObj.GetDevserialnumber() > 0)
		{
			this->FillDeviceInformation(DeviceInfo, deviceObj);
			return 1;
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::SetDeviceStatusSms(ns__Device& DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = SETDEVICESTATUSSMS;

		size_t position = 0;
		position = this->_toolsObject.replaceOne(query, "%s", "true", position); //set true
		this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dSerialNumber), position); //Device Serial number

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::SetDeviceSettingSms(ns__Device& DeviceInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = SETDEVICESETTINGSMS;

		size_t position = 0;
		position = this->_toolsObject.replaceOne(query, "%s", "true", position); //set true
		this->_toolsObject.replaceOne(query, "%s", DeviceInfo.dCustomCommand, position); //Device Serial number
		this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceInfo.dSerialNumber), position);//Device Serial number 

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

void DBConnection::FillDeviceInformation(ns__Device& DeviceInfo, db::Device DeviceObj) const
{
	// Fill Device info
	DeviceInfo.dSerialNumber = DeviceObj.GetDevserialnumber();
	Splitter sp(DeviceObj.GetDevip(), ".");
	if (sp.size() == 4)
	{
		DeviceInfo.dIP.ip1 = this->_toolsObject.toInteger(sp[0]);
		DeviceInfo.dIP.ip2 = this->_toolsObject.toInteger(sp[1]);
		DeviceInfo.dIP.ip3 = this->_toolsObject.toInteger(sp[2]);
		DeviceInfo.dIP.ip4 = this->_toolsObject.toInteger(sp[3]);
	}
	this->_toolsObject.DeviceModelFromString(DeviceInfo.dModel, DeviceObj.GetDevtype());
	DeviceInfo.dPort = DeviceObj.GetDevport();
	DeviceInfo.dCity = DeviceObj.GetDevcity();
	DeviceInfo.dLocation = DeviceObj.GetDevlocation();
	DeviceInfo.dNikeName = DeviceObj.GetDevname();
	DeviceInfo.dSocket = DeviceObj.GetDevsocketnumber();
	DeviceInfo.dSamplingTime = DeviceObj.GetDevsamplingtime();

	sp.reset(DeviceObj.GetDevgps(), ",");
	if (sp.size() == 2)
	{
		DeviceInfo.dGPS.gX = std::stod(sp[0]);
		DeviceInfo.dGPS.gY = std::stod(sp[1]);
	}
	DeviceInfo.dTransPower = DeviceObj.GetDevtranspower();
	DeviceInfo.dTableCapacity = DeviceObj.GetDevtablecapacity();
	DeviceInfo.dKeyName = DeviceObj.GetDevkeytype();

	DeviceInfo.dCustomCommand = DeviceObj.GetDevsmsdata();
	DeviceInfo.dSmsTerm = DeviceObj.GetDevsmsterm();
	Tools toolsObject;
	DeviceInfo.dSmsTime = toolsObject.StringToTime_t(DeviceObj.GetDevsmstime());
}

int DBConnection::GetAllDeviceList()
{
	try
	{
		ns__Device deviceTemp;

		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // MYSQL Connection Error
		}

		Query q(conn);

		std::string query = SELECTDEVICEINFOALL;

		q.get_result(query);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0; // User name and or password is incorrect
		}

		while (q.fetch_row())
		{
			db::Device tbl(&conn, &q); // spawns an object from Query object
			this->FillDeviceInformation(deviceTemp, tbl);

			this->userDeviceList.push_back(deviceTemp);
		}
		q.free_result();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::SaveDeviceStatus(ns__Device DeviceObject, std::string DeviceStatusString, std::string PureData) const
{
	try
	{
		// save connection error
		StderrLog log;

		// Generate and connect
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Logstatus logStatus(&conn);
		logStatus.SetDevserialnumber(DeviceObject.dSerialNumber);
		logStatus.SetLogdate(this->_toolsObject.Time_tToString(DeviceObject.dDDateTime));
		// ReSharper disable once CppMsExtBindingRValueToLvalueReference
		logStatus.SetStatusdata(DeviceStatusString);
		// ReSharper disable once CppMsExtBindingRValueToLvalueReference
		logStatus.SetPuredata(PureData);

		// Add new device into database and return result
		return logStatus.insert();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::SaveDeviceLog(ns__Device DeviceObject, std::string Data) const
{
	try
	{
		// save connection error
		StderrLog log;

		// Generate and connect
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Logactivity logActivity(&conn);
		logActivity.SetDevserialnumber(DeviceObject.dSerialNumber);
		logActivity.SetLogdate(this->_toolsObject.Time_tToString(DeviceObject.dDDateTime));
		// ReSharper disable once CppMsExtBindingRValueToLvalueReference
		logActivity.SetUserid(1);
		// ReSharper disable once CppMsExtBindingRValueToLvalueReference
		logActivity.SetDataactivity(Data);

		// Add new device into database and return result
		return logActivity.insert();
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

//************************************
// Method:    GetAllCityLoactionName
// FullName:  DBConnection::GetAllCityLoactionName
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: ns__AllCityLocatoins & AllCityLocationName
//
// Descriptions: Get all city with locations
//************************************
int DBConnection::GetAllCityLoactionName(ns__AllCityLocatoins &AllCityLocationName) const
{
	try
	{
		ns__CityLocation cityLocation;
		std::vector<std::string> cities;
		// First get all city name
		if (this->GetCitiesName(cities) > 0)
		{
			for each (std::string cityName in cities)
			{
				// Get all locations in this city
				cityLocation.clCity = cityName;

				if (this->GetLacationsName(cityName, cityLocation.clLocations) > 0)
				{
					AllCityLocationName.aclCityLocatoins.push_back(cityLocation);
				}
			}
			return 1;
		}
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::GetCitiesName(std::vector<std::string> &AllCityName)
{
	try
	{
		AllCityName.clear();

		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // MYSQL Connection Error
		}

		Query q(conn);

		std::string query = SELECTCITYNAME;

		q.get_result(query);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0;
		}

		while (q.fetch_row())
		{
			//db::Device tbl(&conn, &q); // spawns an object from Query object
			//AllCityName.push_back(tbl.GetDevcity());
			AllCityName.push_back(q.getstr(0));
		}
		q.free_result();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::GetLacationsName(std::string CityName, std::vector<std::string> &AllLocationName) const
{
	try
	{
		AllLocationName.clear();

		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // MYSQL Connection Error
		}

		Query q(conn);

		std::string query = SELECTLOCATIONNAME;

		this->_toolsObject.replaceAll(query, "%s", CityName);

		q.get_result(query);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0;
		}

		while (q.fetch_row())
		{
			//db::Device tbl(&conn, &q); // spawns an object from Query object
			//AllLocationName.push_back(tbl.GetDevlocation());
			AllLocationName.push_back(q.getstr(0));
		}
		q.free_result();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::ChangeCityName(std::string OldCityName, std::string NewCityName) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = UPDATECITYNAME;

		size_t position = 0;
		position = this->_toolsObject.replaceOne(query, "%s", NewCityName, position); //New city name
		this->_toolsObject.replaceOne(query, "%s", OldCityName, position); //Old city name

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::ChangeLocationName(std::string OldCityName, std::string NewCityName, std::string OldLocationName, std::string NewLocationName) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = UPDATELOCATIONNAME;

		size_t position = 0;
		position = this->_toolsObject.replaceOne(query, "%s", NewLocationName, position); //New location name
		position = this->_toolsObject.replaceOne(query, "%s", NewCityName, position); //New location name
		position = this->_toolsObject.replaceOne(query, "%s", OldCityName, position); //Location city name
		this->_toolsObject.replaceOne(query, "%s", OldLocationName, position); //Old location name

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::ChangeDeviceName(int DeviceSerialNumber, std::string NewCityName, std::string NewLocationName, std::string NewDeviceName) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		std::string query = UPDATEDEVICENAME;

		size_t position = 0;
		position = this->_toolsObject.replaceOne(query, "%s", NewDeviceName, position); //New device name
		position = this->_toolsObject.replaceOne(query, "%s", NewLocationName, position); //New device location name
		position = this->_toolsObject.replaceOne(query, "%s", NewCityName, position); //New device city name
		this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(DeviceSerialNumber), position); //Device serial number

		if (!(q.execute(query)))
		{
			return 0; // Error occurred
		}

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetUsersList(std::vector<ns__User> &UserList) const
{
	try
	{
		ns__Device deviceTemp;

		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		// String of query
		std::string strQuery = SELECTALLUSERS;

		q.get_result(strQuery);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0; //Device Serial Number Does not exist
		}

		// Reset User list
		UserList.clear();

		ns__User userTemp;
		// Get device info in loop
		while (q.fetch_row())
		{
			db::Users tbl(&conn, &q); // spawns an object from Query object
			if (tbl.GetUsername() != "admin")
			{
				userTemp.uId = tbl.GetUserid();
				userTemp.uName = tbl.GetUsername();
				userTemp.uFirstName = tbl.GetUserfirstname();
				userTemp.uLastName = tbl.GetUserlastname();
				//userTemp.userSubmit = std::asctime(tbl.GetRegisterdate());
				this->_toolsObject.UserTypeFromString(userTemp.uType, tbl.GetUsertype());

				UserList.push_back(userTemp);
			}
		}

		q.free_result();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::AddUser(ns__User UserInfo) const
{
	try
	{
		// save connection error
		StderrLog log;

		// Generate and connect
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Users usr(&conn);
		usr.SetUsername(UserInfo.uName);
		usr.SetPassword(UserInfo.uPassword);
		usr.SetUserfirstname(UserInfo.uFirstName);
		usr.SetUserlastname(UserInfo.uLastName);
		usr.SetUsertype(this->_toolsObject.UserTypeToString(UserInfo.uType));
		usr.SetRegisterdate(_toolsObject.getCurrentDateTime());
		usr.SetStatus("Logout");

		// Add new device into database and return result
		return usr.insert();
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::DeleteUser(ns__User UserInfo) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		if (this->GetUserInfo(UserInfo) > 0)
		{
			Query q(conn);

			std::string query = DELETEUSER;

			size_t position = 0;
			this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(UserInfo.uId), position); //User ID

			if (!(q.execute(query)))
			{
				return 0; // Error occurred
			}

			return 1;
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::AssignDeviceToUser(ns__UserDevice UserDeviceList) const
{
	try
	{
		// save connection error
		StderrLog log;

		// Generate and connect
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		if (this->GetUserInfo(UserDeviceList.udUser) > 0)
		{
			for each (ns__Device deviceRule in UserDeviceList.udDevs)
			{
				// Set device information
				db::Userdevice usrDev(&conn);
				usrDev.SetUserid(UserDeviceList.udUser.uId);
				usrDev.SetDeviceid(deviceRule.dSerialNumber);
				usrDev.SetAssigndate(_toolsObject.getCurrentDateTime());

				// Add new device into database and return result
				usrDev.insert();
			}
			return 1;
		}
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::DeleteDeviceFromUser(ns__UserDevice UserDeviceList) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		if (this->GetUserInfo(UserDeviceList.udUser) > 0)
		{
			Query q(conn);

			std::string query = "";

			size_t position = 0;

			for each (ns__Device deviceRule in UserDeviceList.udDevs)
			{
				query = DELETEUSERDEVICE;

				position = 0;

				position = this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(UserDeviceList.udUser.uId), position); //User ID
				position = this->_toolsObject.replaceOne(query, "%s", this->_toolsObject.toString(deviceRule.dSerialNumber), position); //Device ID
				try
				{
					q.execute(query);
				}
				catch (std::exception e)
				{
					log.error(conn, e.what());
					return 0;
				}
			}

			return 1;
		}
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::UpdateUser(ns__User UserInfo) const
{
	try
	{
		// save connection error
		StderrLog log;

		// Generate and connect
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}
		ns__User temp = UserInfo;

		if (this->GetUserInfo(temp) > 0)
		{
			UserInfo.uId = temp.uId;
			// Set device information
			db::Users usr(&conn);
			usr.SetUserid(UserInfo.uId);
			usr.SetUsername(UserInfo.uName);
			usr.SetPassword(UserInfo.uPassword);
			usr.SetUserfirstname(UserInfo.uFirstName);
			usr.SetUserlastname(UserInfo.uLastName);
			usr.SetUsertype(this->_toolsObject.UserTypeToString(UserInfo.uType));
			usr.SetStatus("Login");

			try
			{
				// Add new device into database and return result
				usr.update();

				return 1;
			}
			catch (std::exception e)
			{
				log.error(conn, e.what());
				return 0;
			}
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetUserInfo(ns__User& UserInformation) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		// String of query
		std::string strQuery = SELECTUSERWITHNAME;
		size_t position = 0;
		this->_toolsObject.replaceOne(strQuery, "%s", UserInformation.uName, position); //New device name

		q.get_result(strQuery);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0; //User Does not exist
		}

		if (q.fetch_row())
		{
			// Create object and set username and password to check
			db::Users userObj(&conn, &q);
			if (userObj.GetUserid() > 0)
			{
				UserInformation.uId = userObj.GetUserid();
				UserInformation.uFirstName = userObj.GetUserfirstname();
				UserInformation.uLastName = userObj.GetUserlastname();
				// Convert string to enum
				this->_toolsObject.UserTypeFromString(UserInformation.uType, userObj.GetUsertype());
			}

			q.free_result();

			// return result
			return userObj.GetUserid();
		}

		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetDeviceStatusLog(ns__DeviceStatus StatusLogInfo, ns__DeviceStatusLogList &StatusLogList) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		Query q(conn);

		// String of query
		std::string strQuery = SELECTDEVICESTATUSLOG;
		size_t position = 0;

		char strDateTime[30];
		std::strftime(strDateTime, 100, "%Y-%m-%d %H:%M:%S", std::localtime(&(StatusLogInfo.dsRange.rrStart)));
		position = this->_toolsObject.replaceOne(strQuery, "%s", strDateTime, position); //Start Date
		//position = this->_toolsObject.replaceOne(strQuery, "%s", this->_toolsObject.toString(StatusLogInfo.dsRange.rStart), position); //Start Date

		std::strftime(strDateTime, 100, "%Y-%m-%d %H:%M:%S", std::localtime(&(StatusLogInfo.dsRange.rrEnd)));
		position = this->_toolsObject.replaceOne(strQuery, "%s", strDateTime, position); //End Date
		//position = this->_toolsObject.replaceOne(strQuery, "%s", this->_toolsObject.toString(StatusLogInfo.dsRange.reportEndDate), position); //End Date

		std::string strTemp = "";
		size_t iSize = StatusLogInfo.dsDevice.size();
		for (int i = 0; i < iSize; i++)
		{
			strTemp += this->_toolsObject.toString(StatusLogInfo.dsDevice[i].dSerialNumber);
			if (i != iSize - 1)
			{
				strTemp += ",";
			}
		}
		this->_toolsObject.replaceOne(strQuery, "%s", strTemp, position); //New device name

		q.get_result(strQuery);
		if (q.num_rows() < 1)
		{
			q.free_result();
			return 0; //User Does not exist
		}

		int result = q.num_rows();

		while (q.fetch_row())
		{
			// Create object and set username and password to check
			db::Logstatus statusLogObj(&conn, &q);
			ns__DeviceStatusLog devStatusTemp;
			devStatusTemp.dslErr.eMsg = this->_toolsObject.VectorStringToString(statusLogObj.GetStatusdata());
			devStatusTemp.dslDateTime = this->_toolsObject.StringToTime_t(statusLogObj.GetLogdate());

			StatusLogList.dsllStatus.push_back(devStatusTemp);
		}

		q.free_result();

		return result;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetDevicePicture(ns__Device& DeviceInfo)
{
	try
	{
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::SetDevicePicture(ns__Device DeviceInfo)
{
	try
	{
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::GetDevicePictureLog(ns__DeviceStatus StatusLogInfo, ns__DeviceStatusLogList & StatusLogList)
{
	try
	{
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::SetDevicePictureLog(ns__DeviceStatus StatusLogInfo, std::string DevicePicture)
{
	try
	{
		return 0;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetDeviceCalibration(ns__Device& DeviceInfo, std::string& CalibrationData) const
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // MYSQL Connection Error
		}

		db::Device dev(conn, DeviceInfo.dSerialNumber); // spawns an object from Query object

		CalibrationData = this->_toolsObject.VectorStringToString(dev.GetDevcalibration());

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int DBConnection::SetDeviceCalibration(ns__Device DeviceInfo, std::string CalibrationData)
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Device dev(conn, DeviceInfo.dSerialNumber);
		// ReSharper disable once CppMsExtBindingRValueToLvalueReference
		dev.SetDevcalibration(CalibrationData);

		// Update device information and return result
		dev.update();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::GetDeviceSamplingTime(ns__Device & DeviceInfo)
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // MYSQL Connection Error
		}

		db::Device dev(conn, DeviceInfo.dSerialNumber); // spawns an object from Query object

		DeviceInfo.dSamplingTime = dev.GetDevsamplingtime();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

int DBConnection::SetDeviceSamplingTimme(ns__Device DeviceInfo)
{
	try
	{
		// save connection error
		StderrLog log;
		Database conn(DATABASESERVER, DATABASEUSER, DATABASEPASSWORD, DATABASENAME, &log);
		if (!conn.Connected())
		{
			return -1; // DATABASE Connection Error
		}

		// Set device information
		db::Device dev(conn, DeviceInfo.dSerialNumber);
		dev.SetDevsamplingtime(DeviceInfo.dSamplingTime);

		// Update device information and return result
		dev.update();

		return 1;
	}
	catch (const std::exception&)
	{
		return 0;
	}

}

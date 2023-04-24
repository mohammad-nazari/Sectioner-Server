//Content of file "Sectionner.h": 
//gsoap ns service name:            Sectionner Generate web service for sectionner system
//gsoap ns service namespace: Sectionner
//gsoap ns schema namespace: Sectionner
//gsoap ns service location: http://localhost:9090

#import "stlvector.h"

typedef std::string xsd__string;
typedef int xsd__int;
typedef bool xsd__boolean;
typedef double xsd__double;
typedef time_t xsd__dateTime;
typedef long long xsd__long;

enum ns__SettingLevel
{
	Normal,
	Disable,
	Warning,
	Minor,
	Major,
	Critical
};

enum ns__UserType
{
	Admin,
	Control,
	Monitor
};

enum ns__SensorType
{
	Multi,
	Mono
};

enum ns__SensorName
{
	TEMPERATURE,
	HUMIDITY,
	ACVOLTAGE,
	ACAMPERE,
	DCVOLTAGE,
	DCAMPERE,
	COSQ,
	DIGITALINPUT,
	DIGITALOUTPUT,
	DIGITALEXIST,
	RELAY,
	UNKNOWN
};

enum ns__DeviceModel
{
	PROTECTOR,
	SECTIONNER,
	TRANSISTOR,
	SERVER,
	MANAGER,
	ROBER,
	ALARM
};

enum ns__RequestType
{
	CALIBRATION,
	SAMPLING,
	OUTRELAY,
	DATETIME,
	PICTURE,
	PARTS,
	RESET,
	LIVE,
	OK,
	DATA,
	CUSTOM,
	STT,
	ALM
};

// Result of requests
class ns__ErrorCode
{
	xsd__int eNo = 0;
	xsd__string eMsg = "";
	enum ns__SettingLevel eType = Normal;
};

class ns__User
{
	xsd__int uId = 0;
	xsd__string uName = "";
	xsd__string uPassword = "";
	xsd__string uRePassword = "";
	xsd__string uFirstName = "";	// For new password
	xsd__string uLastName = "";
	enum ns__UserType uType = Monitor; // Admin, User, Monitor
	xsd__string uKey = ""; // Key for encrypt decrypt
	ns__ErrorCode uErr;
};

class ns__UsersList
{
	std::vector<ns__User> ulUsers;
	ns__ErrorCode ulErr;
};

class ns__IP
{
	xsd__int ip1 = 0;
	xsd__int ip2 = 0;
	xsd__int ip3 = 0;
	xsd__int ip4 = 0;
};

class ns__GPS
{
	xsd__double gX = 0;
	xsd__double gY = 0;
};

class ns__Calibration
{
	xsd__double cOffset = 0;
	xsd__double cZero = 0;
	xsd__double cSpan = 0;
	xsd__double cMin = 0;
	xsd__double cMax = 0;
};

class ns__CalibrationList
{
	std::vector<ns__Calibration> clVoltage;
	std::vector<ns__Calibration> clAmpere;
	std::vector<ns__Calibration> clCosq;
	ns__ErrorCode clErr;
};

class ns__Sensor
{
	xsd__string sNkName = "";
	xsd__double sVal = 0;
	xsd__int sMin = 0;
	xsd__int sMax = 0;
	enum ns__SensorType sType = Multi;	//Multi or Mono
	ns__ErrorCode sErr;
};

class ns__SensorExtension
{
	enum ns__SensorName seName = ACVOLTAGE;
	//std::vector<ns__Sensor> seSens;
	std::vector<xsd__double> seVal;
};

class ns__Camera
{
	xsd__int cSocket = 0;
	ns__IP cIP;
	xsd__int cPort = 0;
	ns__ErrorCode cErr;
};

class ns__PicturePart
{
	xsd__int ppIndex = 0;
	xsd__string ppData = "";	//As base64 data
	xsd__int ppCRC = 0;
};

class ns__Picture
{
	xsd__string pName = "";
	xsd__int pSize = 0;
	xsd__int pPartSize = 0;
	xsd__int pPartNo = 0;
	xsd__int pCRC = 0;
	std::vector<ns__PicturePart> pParts;
};

class ns__Device
{
	xsd__int dSerialNumber = 0;
	enum ns__DeviceModel dModel = SECTIONNER;
	xsd__string dName = "";
	xsd__string dNikeName = "";
	xsd__string dLocation = "";
	xsd__string dCity = "";
	xsd__int dSocket = 0;
	xsd__string dDNSAddress = "";	//DNS
	ns__IP dIP;
	xsd__int dPort = 0;
	xsd__dateTime dDateTime = 0; // Date time of device
	xsd__dateTime dDDateTime = 0; // Data Date time
	ns__Camera dCamera;
	ns__GPS dGPS;
	xsd__boolean dReset = false;
	xsd__int dSamplingTime = 0;
	xsd__double dTransPower = 0;
	xsd__double dTableCapacity = 0;
	xsd__double dPOK = 0;
	xsd__double dPTotal = 0;
	xsd__double dPR = 0;
	xsd__double dPS = 0;
	xsd__double dPT = 0;
	xsd__double dQR = 0;
	xsd__double dQS = 0;
	xsd__double dQT = 0;
	xsd__string dKeyName = "";
	ns__Picture dPicture;
	xsd__string dCustomCommand = "";
	std::vector<xsd__boolean> dRelays;
	std::vector<enum ns__RequestType> dRequests;
	std::vector<ns__SensorExtension> dSensors;
	xsd__int dSmsTerm = 60; // In minute
	xsd__boolean dSms = false;
	xsd__dateTime dSmsTime = 0;
	ns__ErrorCode dErr;
};

class ns__ReportRange
{
	xsd__dateTime rrStart = 0;
	xsd__dateTime rrEnd = 0;
};

class ns__ActivityLog
{
	ns__ReportRange alRange;
	xsd__string alData = "";
	ns__Device alDevice;
	ns__User alUser;
	ns__ErrorCode alErr;
};

class ns__UserLog
{
	ns__ReportRange ulRange;
	xsd__string ulData = "";
	ns__User ulUser;
};

class ns__DeviceStatusLog
{
	xsd__dateTime dslDateTime = 0;
	ns__Device dslDevice;
	ns__ErrorCode dslErr;
};

class ns__DeviceStatusLogList
{
	std::vector<ns__DeviceStatusLog> dsllStatus;
	ns__ErrorCode dsllErr;
};

// Send to 
class ns__DeviceStatus
{
	ns__ReportRange dsRange;
	std::vector<ns__Device> dsDevice;
	std::vector<std::string> dsErr;
};

class ns__ErrorLog
{
	xsd__dateTime elDateTime = 0;
	ns__Device elDevice;
	ns__ErrorCode elErr;
};

class ns__UserDevice
{
	ns__User udUser;
	std::vector<ns__Device> udDevs;
	ns__ErrorCode udErr;
};

class ns__DataBase
{
	xsd__string	dbServer = "";
	xsd__int bdPort = 0;
	xsd__string	dbUser = "";
	xsd__string	dbPassWord = "";
	ns__ErrorCode dbErr;
};

class ns__ServerOptions
{
	ns__DataBase soDataBase;
	xsd__int soRepeat = 0;
	ns__ErrorCode soErr;
};

class ns__CityLocation
{
	xsd__string clCity = "";
	std::vector<xsd__string> clLocations;
	ns__ErrorCode clErr;
};

class ns__AllCityLocatoins
{
	std::vector<ns__CityLocation> aclCityLocatoins;
	ns__ErrorCode aclErr;
};

//gsoap ns service method-protocol: GetDeviceStatus SOAP 
//gsoap ns service method-action: GetDeviceStatus "" 
xsd__int ns__GetDeviceStatus(ns__User requestUser, ns__Device requestDevice, ns__Device &responseDevice);
//gsoap ns service method-protocol: GetDeviceStatusSMS SOAP 
//gsoap ns service method-action: GetDeviceStatusSMS "" 
xsd__int ns__GetDeviceStatusSMS(ns__User requestUser, ns__Device requestDevice, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: GetAllDeviceStatus SOAP 
//gsoap ns service method-action: GetAllDeviceStatus "" 
xsd__int ns__GetAllDeviceStatus(ns__User requestUser, ns__UserDevice &responseUserDevices);
//gsoap ns service method-protocol: GetDevicePicture SOAP 
//gsoap ns service method-action: GetDevicePicture "" 
xsd__int ns__GetDevicePicture(ns__User requestUser, ns__Device requestDevice, ns__Device &responseDevice);
//gsoap ns service method-protocol: GetDevicePicturePart SOAP 
//gsoap ns service method-action: GetDevicePicturePart "" 
xsd__int ns__GetDevicePicturePart(ns__User requestUser, ns__Device requestDevice, ns__Device &responseDevice);
//gsoap ns service method-protocol: SetDeviceSetting SOAP 
//gsoap ns service method-action: SetDeviceSetting "" 
xsd__int ns__SetDeviceSetting(ns__User requestUser, ns__Device requestDevice, std::vector<ns__ErrorCode> &responseError);
//gsoap ns service method-protocol: SetDeviceSettingSMS SOAP 
//gsoap ns service method-action: SetDeviceSettingSMS "" 
xsd__int ns__SetDeviceSettingSMS(ns__User requestUser, ns__Device requestDevice, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: SetAllDeviceSetting SOAP 
//gsoap ns service method-action: SetAllDeviceSetting "" 
xsd__int ns__SetAllDeviceSetting(ns__User requestUser, std::vector<ns__Device> requestDevice, std::vector<ns__ErrorCode> &responseError);
//gsoap ns service method-protocol: DeleteDevices SOAP 
//gsoap ns service method-action: DeleteDevices "" 
xsd__int ns__DeleteDevices(ns__User requestUser, std::vector<ns__Device> requestDeviceList, std::vector<ns__ErrorCode> &responseError);

//gsoap ns service method-protocol: SetDeviceCalibration SOAP 
//gsoap ns service method-action: SetDeviceCalibration "" 
xsd__int ns__SetDeviceCalibration(ns__User requestUser, ns__Device requestDevice, ns__CalibrationList requestCalibrationList, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: GetDeviceCalibration SOAP 
//gsoap ns service method-action: GetDeviceCalibration "" 
xsd__int ns__GetDeviceCalibration(ns__User requestUser, ns__Device requestDevice, ns__CalibrationList &responsCalibrationList);

//gsoap ns service method-protocol: Login SOAP 
//gsoap ns service method-action: Login "" 
xsd__int ns__Login(ns__User requestUser, ns__User &responseUser);
//gsoap ns service method-protocol: GetUserDeviceList SOAP 
//gsoap ns service method-action: GetUserDeviceList "" 
xsd__int ns__GetUserDeviceList(ns__User requestUser, ns__UserDevice &responseUserDevices);

//gsoap ns service method-protocol: GetUsersList SOAP 
//gsoap ns service method-action: GetUsersList "" 
xsd__int ns__GetUsersList(ns__User requestUser, ns__UsersList &responseUsersList);
//gsoap ns service method-protocol: AddUser SOAP 
//gsoap ns service method-action: AddUser "" 
xsd__int ns__AddUser(ns__User requestUser, ns__User requestNewUser, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: UpdateUser SOAP 
//gsoap ns service method-action: UpdateUser "" 
xsd__int ns__UpdateUser(ns__User requestUser, ns__User requestNewUser, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: DeleteUser SOAP 
//gsoap ns service method-action: DeleteUser "" 
xsd__int ns__DeleteUser(ns__User requestUser, ns__User requestDeleteUser, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: AssignDeviceToUser SOAP 
//gsoap ns service method-action: AssignDeviceToUser "" 
xsd__int ns__AssignDeviceToUser(ns__User requestUser, ns__UserDevice requestUserDevice, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: DeleteDeviceFromUser SOAP 
//gsoap ns service method-action: DeleteDeviceFromUser "" 
xsd__int ns__DeleteDeviceFromUser(ns__User requestUser, ns__UserDevice requestUserDevice, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: GetUserDevices SOAP 
//gsoap ns service method-action: GetUserDevices "" 
xsd__int ns__GetUserDevices(ns__User requestUser, ns__User requestSelectedUser, ns__UserDevice &responseUserDevices);

//gsoap ns service method-protocol: SaveActivityLog SOAP 
//gsoap ns service method-action: SaveActivityLog "" 
xsd__int ns__SaveActivityLog(ns__User requestUser, ns__ActivityLog requestActivityLog, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: GetActivityLog SOAP 
//gsoap ns service method-action: GetActivityLog "" 
xsd__int ns__GetActivityLog(ns__User requestUser, ns__ReportRange requestReport, std::vector<ns__ActivityLog> &responseActivityLog);
//gsoap ns service method-protocol: SaveUserLog SOAP 
//gsoap ns service method-action: SaveUserLog "" 
xsd__int ns__SaveUserLog(ns__User requestUser, ns__UserLog requestActivityLog, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: GetUserLog SOAP 
//gsoap ns service method-action: GetUserLog "" 
xsd__int ns__GetUserLog(ns__User requestUser, ns__ReportRange requestReport, std::vector<ns__UserLog> &responseActivityLog);
//gsoap ns service method-protocol: SaveDeviceStatusLog SOAP 
//gsoap ns service method-action: SaveDeviceStatusLog "" 
xsd__int ns__SaveDeviceStatusLog(ns__User requestUser, ns__DeviceStatusLog requestStatusLog, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: GetDeviceStatusLog SOAP 
//gsoap ns service method-action: GetDeviceStatusLog "" 
xsd__int ns__GetDeviceStatusLog(ns__User requestUser, ns__DeviceStatus requestStatusLog, ns__DeviceStatusLogList &responseStatusLogList);

//gsoap ns service method-protocol: GetServerOptions SOAP 
//gsoap ns service method-action: GetServerOptions "" 
xsd__int ns__GetServerOptions(ns__User requestUser, ns__ServerOptions &serverOptions);
//gsoap ns service method-protocol: SetServerOptions SOAP 
//gsoap ns service method-action: SetServerOptions "" 
xsd__int ns__SetServerOptions(ns__User requestUser, ns__ServerOptions serverOptions, ns__ErrorCode &responseError);

//gsoap ns service method-protocol: GetCityAndLocations SOAP 
//gsoap ns service method-action: GetCityAndLocations "" 
xsd__int ns__GetCityAndLocations(ns__User requestUser, ns__AllCityLocatoins &responseCityLocation);
//gsoap ns service method-protocol: ChangeCityName SOAP 
//gsoap ns service method-action: ChangeCityName "" 
xsd__int ns__ChangeCityName(ns__User requestUser, xsd__string oldCityName, xsd__string newCityName, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: ChangeLocationName SOAP  
//gsoap ns service method-action: ChangeLocationName "" 
xsd__int ns__ChangeLocationName(ns__User requestUser, xsd__string oldCityName, xsd__string newCityName, xsd__string oldLocationName, xsd__string newLocationName, ns__ErrorCode &responseError);
//gsoap ns service method-protocol: ChangeDeviceName SOAP 
//gsoap ns service method-action: ChangeDeviceName "" 
xsd__int ns__ChangeDeviceName(ns__User requestUser, xsd__int deviceSerialNumber, xsd__string newCityName, xsd__string newLocationName, xsd__string newDeviceName, ns__ErrorCode &responseError);
#include "sectionner.nsmap"
#include "soapSectionnerService.h" /* generated with soapcpp2 -j calc.h */
#include "threads.h"  /* use gsoap/plugin/threads.h portable threads */

#include "Header.h"

#include "DeviceMap.h"
#include "ExecuteRequest.h"
#include "SConnection.h"

// Global Device List
std::map<int, DeviceMap> DeviceList;
std::map<int, DeviceMap> DeviceListClone;
CRITICAL_SECTION csThread;

std::string pictureSource;

struct arg_struct {
	SectionnerService *sectionner;
	SOAP_SOCKET client;
};

void start_socket_listener()
{
	SConnection socketObject;
	// Create socket
	socketObject.CreateSocket();
	// Start listen server to socket and continue
	socketObject.ThreadStartListen();
}

void start_socket_listener2()
{
	SConnection socketObject;
	// Create socket
	socketObject.CreateSocket2();
	// Start listen server to socket and continue
	socketObject.ThreadStartListen2();
}

void SetError(ns__ErrorCode &ErrorObject, ns__SettingLevel SettingLevelObject, const std::string ErrorMessage, const int ErrorNumber)
{
	ErrorObject.eType = SettingLevelObject;
	ErrorObject.eMsg = ErrorMessage;
	ErrorObject.eNo = ErrorNumber;
}

void *process_request(void *arg)
{
	struct arg_struct *args = static_cast<struct arg_struct *>(arg);
	THREAD_DETACH(THREAD_ID);
	Tools toolsObject;

	args->sectionner->serve();
	printf("Thread of socket %d connection from IP %d.%d.%d.%d served (%s)\n", static_cast<int>(args->client), (args->sectionner->soap->ip >> 24) & 0xFF, (args->sectionner->soap->ip >> 16) & 0xFF, (args->sectionner->soap->ip >> 8) & 0xFF, args->sectionner->soap->ip & 0xFF, (toolsObject.getCurrentDateTime()).c_str());
	args->sectionner->destroy(); /* clean up */
	delete args->sectionner;

	return nullptr;
}

int main(int argc, char **argv)
{
	InitializeCriticalSection(&(csThread));
	// Start server listen to devices
	start_socket_listener();
	start_socket_listener2();

	SectionnerService sectionner;
	sectionner.soap->namespaces = new struct Namespace;
	sectionner.soap->namespaces = namespaces;

	sectionner.soap->send_timeout = SEND_TIMEOUT; // 120 seconds max socket delay 
	sectionner.soap->recv_timeout = RECV_TIMEOUT; // 120 seconds max socket delay 
	//sectionner.soap->connect_timeout = CONNECT_TIMEOUT; // 120 seconds max socket delay 
	//sectionner.soap->transfer_timeout = TRANS_TIMEOUT; // 120 seconds max socket delay 
	//sectionner.soap->accept_timeout = ACCEPT_TIMEOUT; // 120 seconds max socket delay 
	//sectionner.soap->maxlength = -1;

	int port = SOAP_PORT; // first command-line arg is port 
	SOAP_SOCKET s;
	SOAP_SOCKET m = sectionner.bind(NULL, port, BACKLOG);
	if (soap_valid_socket(m))
	{
		printf("Socket connection successful %d\n", static_cast<int>(m));
		while (soap_valid_socket(s = sectionner.accept()))
		{
			THREAD_TYPE tid;
			printf("Thread accepts socket %d connection from IP %d.%d.%d.%d\n", static_cast<int>(s), (sectionner.soap->ip >> 24) & 0xFF, (sectionner.soap->ip >> 16) & 0xFF, (sectionner.soap->ip >> 8) & 0xFF, sectionner.soap->ip & 0xFF);

			struct arg_struct *args = new struct arg_struct;
			args->client = s;
			args->sectionner = sectionner.copy();
			while (THREAD_CREATE(&tid, (void*(*)(void*))process_request, args))
				Sleep(1000);
		}
	}
	sectionner.destroy(); /* clean up */
	return 0;
}

xsd__int SectionnerService::GetDeviceStatus(const ns__User& requestUser, const ns__Device& requestDevice, ns__Device &responseDevice)
{
	// Check received data format and values
	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;

	// Initialize response
	ns__Device device = requestDevice;
	responseDevice = device;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check device is in database
		if (erObject.CheckDevice(device) > 0)
		{
			// Check device is in list of user devices
			// and can view status
			int result = 1;
			if (user.uType != Admin)
			{
				result = erObject.IsDeviceForUser(user, device /*AllowType = 0 allow to read*/);
			}
			if (result > 0)
			{
				// Get device Status
				if (socketObject.GetDeviceStatus(responseDevice))
				{
					// Get other device information from database
					if (erObject.GetDeviceSetting(responseDevice) == 1)
					{
						responseDevice.dPOK = std::round(responseDevice.dTransPower * 0.75);
						responseDevice.dPTotal = std::round(responseDevice.dPR + responseDevice.dPS + responseDevice.dPT);

						time_t nowTime;
						time(&nowTime);
						responseDevice.dDateTime = nowTime;
						erObject.DeviceInfoToBase64(responseDevice);
					}
					else
					{
						// Database Error
						SetError(responseDevice.dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
					}
				}
				else
				{
					if (erObject.GetDeviceSetting(responseDevice) == 1)
					{
						erObject.DeviceInfoToBase64(responseDevice);
						// Get now
						time_t nowTime;
						time(&nowTime);

						if (nowTime > responseDevice.dSmsTime && ((nowTime - responseDevice.dSmsTime) / 60) < responseDevice.dSmsTerm)
						{
							responseDevice.dSms = false;
							SyntaxChecker parserObject;
							parserObject.DeviceInfo(responseDevice);
							int isCamera = 0;
							if (parserObject.CheckSTTSyntax(responseDevice.dCustomCommand, isCamera))
							{
								responseDevice = parserObject.DeviceInfo();

								if (isCamera == -1 && responseDevice.dModel != ALARM)
								{
									// Set sampling data
									socketObject.InitiateDeviceStatus(responseDevice);

									responseDevice.dPOK = round(responseDevice.dTransPower * 0.75);
									responseDevice.dPTotal = round(responseDevice.dPR + responseDevice.dPS + responseDevice.dPT);

									responseDevice.dDDateTime = responseDevice.dSmsTime;
									responseDevice.dDateTime = nowTime;
									socketObject.SaveDevieStatus(responseDevice, responseDevice.dCustomCommand);
								}

								socketObject.UpdateDeviceRow(responseDevice);
								socketObject.GetDeviceSamplingTime(responseDevice);
								responseDevice.dSms = true;

								// Database Error
								SetError(responseDevice.dErr, Warning, "", 0);
							}
							else
							{
								responseDevice.dSms = false;

								// Fill error in response
								SetError(responseDevice.dErr, Disable, "", 0);
							}
						}
						else
						{
							responseDevice.dSms = false;

							// Fill error in response
							SetError(responseDevice.dErr, Disable, "", 0);
						}
					}
					else
					{
						// Fill error in response
						SetError(responseDevice.dErr, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
					}
				}
			}
			else
			{
				if (result == -2)
				{
					// Permission Denied
					// Fill error in response
					SetError(responseDevice.dErr, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
				}
				else
				{
					// Database Error
					SetError(responseDevice.dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
		}
		else
		{
			// Fill error in response
			SetError(responseDevice.dErr, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseDevice.dErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetDeviceStatusSMS(const ns__User& requestUser, const ns__Device &requestDevice, ns__ErrorCode &responseError)
{
	// Check received data format and values
	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;

	// Initialize response
	responseError.eMsg = "";
	responseError.eNo = 0;

	ns__Device device = requestDevice;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check device is in database
		if (erObject.CheckDevice(device) > 0)
		{
			// Check device is in list of user devices
			// and can view status
			int result = 1;
			if (user.uType != Admin)
			{
				result = erObject.IsDeviceForUser(user, device /*AllowType = 0 allow to read*/);
			}
			if (result > 0)
			{
				// Get other device information from database
				if (erObject.SetDeviceStatusSms(device) != 1)
				{
					// Database Error
					SetError(responseError, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
			else
			{
				if (result == -2)
				{
					// Permission Denied
					// Fill error in response
					SetError(responseError, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
				}
				else
				{
					// Database Error
					SetError(responseError, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
		}
		else
		{
			// Fill error in response
			SetError(responseError, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetAllDeviceStatus(const ns__User& requestUser, ns__UserDevice &responseUserDevicesInfo)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.GetAllDeviceList(responseUserDevicesInfo.udDevs) != 1)
			{
				// No device found error
				SetError(responseUserDevicesInfo.udErr, Disable, NODEVICEFOUNDMESSAGE, NODEVICEFOUNDNUMBER);
			}
		}
		else
		{
			if (erObject.GetUserDeviceList(user, responseUserDevicesInfo.udDevs) != 1)
			{
				// No device found error
				SetError(responseUserDevicesInfo.udErr, Disable, NODEVICEFOUNDMESSAGE, NODEVICEFOUNDNUMBER);
			}
		}

		if (responseUserDevicesInfo.udErr.eMsg != NODEVICEFOUNDMESSAGE)
		{
			for (int i = 0; i < responseUserDevicesInfo.udDevs.size(); i++)
			{
				// Get device Status
				if (socketObject.GetDeviceStatus(responseUserDevicesInfo.udDevs[i]))
				{
					// Get other device information from database
					if (erObject.GetDeviceSetting(responseUserDevicesInfo.udDevs[i]) == 1)
					{
						erObject.DeviceInfoToBase64(responseUserDevicesInfo.udDevs[i]);
					}
					else
					{
						// Database Error
						SetError(responseUserDevicesInfo.udDevs[i].dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
					}
				}
				else
				{
					if (erObject.GetDeviceSetting(responseUserDevicesInfo.udDevs[i]) == 1)
					{
						erObject.DeviceInfoToBase64(responseUserDevicesInfo.udDevs[i]);
						// Get now
						time_t nowTime;
						time(&nowTime);

						if (nowTime > responseUserDevicesInfo.udDevs[i].dSmsTime && ((nowTime - responseUserDevicesInfo.udDevs[i].dSmsTime) / 60) < responseUserDevicesInfo.udDevs[i].dSmsTerm)
						{
							responseUserDevicesInfo.udDevs[i].dSms = false;
							SyntaxChecker parserObject;
							parserObject.DeviceInfo(responseUserDevicesInfo.udDevs[i]);
							int isCamera = 0;
							if (parserObject.CheckSTTSyntax(responseUserDevicesInfo.udDevs[i].dCustomCommand, isCamera))
							{
								responseUserDevicesInfo.udDevs[i] = parserObject.DeviceInfo();

								if (isCamera == -1 && responseUserDevicesInfo.udDevs[i].dModel != ALARM)
								{
									// Set sampling data
									socketObject.InitiateDeviceStatus(responseUserDevicesInfo.udDevs[i]);

									responseUserDevicesInfo.udDevs[i].dPOK = round(responseUserDevicesInfo.udDevs[i].dTransPower * 0.75);
									responseUserDevicesInfo.udDevs[i].dPTotal = round(responseUserDevicesInfo.udDevs[i].dPR + responseUserDevicesInfo.udDevs[i].dPS + responseUserDevicesInfo.udDevs[i].dPT);

									responseUserDevicesInfo.udDevs[i].dDDateTime = responseUserDevicesInfo.udDevs[i].dSmsTime;
									socketObject.SaveDevieStatus(responseUserDevicesInfo.udDevs[i], responseUserDevicesInfo.udDevs[i].dCustomCommand);
								}

								socketObject.UpdateDeviceRow(responseUserDevicesInfo.udDevs[i]);
								socketObject.GetDeviceSamplingTime(responseUserDevicesInfo.udDevs[i]);
								responseUserDevicesInfo.udDevs[i].dSms = true;

								// Database Error
								SetError(responseUserDevicesInfo.udDevs[i].dErr, Warning, "", 0);
							}
							else
							{
								responseUserDevicesInfo.udDevs[i].dSms = false;

								// Fill error in response
								SetError(responseUserDevicesInfo.udDevs[i].dErr, Disable, "", 0);
							}
						}
						else
						{
							responseUserDevicesInfo.udDevs[i].dSms = false;

							// Fill error in response
							SetError(responseUserDevicesInfo.udDevs[i].dErr, Disable, "", 0);
						}
					}
					else
					{
						// Fill error in response
						SetError(responseUserDevicesInfo.udDevs[i].dErr, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
					}
				}
			}
		}
	}
	else
	{
		// Fill error in response
		SetError(responseUserDevicesInfo.udErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetDevicePicture(const ns__User& requestUser, const ns__Device &requestDevice, ns__Device &responseDevice)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;
	// Initialize response
	responseDevice = requestDevice;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check device is in database
		if (erObject.CheckDevice(responseDevice) > 0)
		{
			responseDevice = requestDevice;
			// Check device is in list of user devices
			// and can view status
			int result = 1;
			if (user.uType != Admin)
			{
				result = erObject.IsDeviceForUser(user, responseDevice /*AllowType = 0 allow to read*/);
			}
			if (result > 0)
			{
				// Get device Status
				if (!(socketObject.GetDevicePicture(responseDevice)))
				{
					SetError(responseDevice.dErr, Disable, NOCONNECTEDMESSAGE, NOCONNECTEDNUMBER);
				}
			}
			else
			{
				if (result == -2)
				{
					// Permission Denied
					// Fill error in response
					SetError(responseDevice.dErr, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
				}
				else
				{
					// Database Error
					SetError(responseDevice.dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
		}
		else
		{
			// Fill error in response
			SetError(responseDevice.dErr, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseDevice.dErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetDevicePicturePart(const ns__User& requestUser, const ns__Device &requestDevice, ns__Device &responseDevice)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;
	// Initialize response
	responseDevice = requestDevice;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check device is in database
		if (erObject.CheckDevice(responseDevice) > 0)
		{
			responseDevice = requestDevice;
			// Check device is in list of user devices
			// and can view status
			int result = 1;
			if (user.uType != Admin)
			{
				result = erObject.IsDeviceForUser(user, responseDevice /*AllowType = 0 allow to read*/);
			}
			if (result > 0)
			{
				// Get device Status
				if (!(socketObject.GetDevicePicturePart(responseDevice)))
				{
					SetError(responseDevice.dErr, Disable, NOCONNECTEDMESSAGE, NOCONNECTEDNUMBER);
				}
			}
			else
			{
				if (result == -2)
				{
					// Permission Denied
					// Fill error in response
					SetError(responseDevice.dErr, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
				}
				else
				{
					// Database Error
					SetError(responseDevice.dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
		}
		else
		{
			// Fill error in response
			SetError(responseDevice.dErr, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseDevice.dErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::SetDeviceSetting(const ns__User& requestUser, const ns__Device &requestDevice, std::vector<ns__ErrorCode> &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;

	ns__ErrorCode errorCode;
	ns__Device device = requestDevice;
	erObject.DeviceInfoFromBase64(device);

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check device is in database
		if (erObject.CheckDevice(device) > 0)
		{
			int result;
			if (user.uType == Admin)
			{
				result = 1;
			}
			else
			{
				// Check device is in list of user devices
				// and can view status
				result = erObject.IsDeviceForUser(user, device, 1 /* AllowType = 1 allow to write*/);
			}
			if (result > 0)
			{
				// Set setting in device
				if (socketObject.SetDeviceStatus(device, responseError))
				{
					// Set setting in database
					if (!erObject.SetDeviceSetting(device))
					{
						// Database Error
						SetError(errorCode, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
						responseError.push_back(errorCode);
					}
				}
				else
				{
					// Fill error in response
					SetError(errorCode, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
					responseError.push_back(errorCode);
				}
			}
			else
			{
				if (result == -2)
				{
					// Permission Denied
					// Fill error in response
					SetError(errorCode, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
					responseError.push_back(errorCode);
				}
				else
				{
					// Database Error
					SetError(errorCode, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
					responseError.push_back(errorCode);
				}
			}
		}
		else
		{
			// Fill error in response
			SetError(errorCode, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
			responseError.push_back(errorCode);
		}
	}
	else
	{
		// Fill error in response
		SetError(errorCode, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
		responseError.push_back(errorCode);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::SetDeviceSettingSMS(const ns__User& requestUser, const ns__Device &requestDevice, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;
	ns__Device device = requestDevice;
	// Initialize response
	responseError.eMsg = "";
	responseError.eNo = 0;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check device is in database
		if (erObject.CheckDevice(device) > 0)
		{
			// Check device is in list of user devices
			// and can view status
			int result = 1;
			if (user.uType != Admin)
			{
				result = erObject.IsDeviceForUser(user, device /*AllowType = 0 allow to read*/);
			}
			if (result > 0)
			{
				// Get other device information from database
				if (erObject.SetDeviceSettingSms(device) != 1)
				{
					// Database Error
					SetError(responseError, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
			else
			{
				if (result == -2)
				{
					// Permission Denied
					// Fill error in response
					SetError(responseError, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
				}
				else
				{
					// Database Error
					SetError(responseError, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
				}
			}
		}
		else
		{
			// Fill error in response
			SetError(responseError, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::SetAllDeviceSetting(const ns__User& requestUser, std::vector<ns__Device> requestDevices, std::vector<ns__ErrorCode> &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;
	ns__ErrorCode errorTemp;
	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		for each (ns__Device requestDevice in requestDevices)
		{
			// Check device is in database
			if (erObject.CheckDevice(requestDevice) > 0)
			{
				// Check device is in list of user devices
				// and can view status
				int result = erObject.IsDeviceForUser(user, requestDevice, 1 /* AllowType = 1 allow to write*/);
				if (result > 0)
				{
					// Set setting in device
					if (socketObject.SetDeviceStatus(requestDevice, responseError))
					{
						// Set setting in database
						if (erObject.SetDeviceSetting(requestDevice))
						{

						}
						else
						{
							// Database Error
							SetError(errorTemp, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
						}
					}
					else
					{
						// Fill error in response
						SetError(errorTemp, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
					}
				}
				else
				{
					if (result == -2)
					{
						// Permission Denied
						// Fill error in response
						SetError(errorTemp, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
					}
					else
					{
						// Database Error
						SetError(errorTemp, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
					}
				}
			}
			else
			{
				// Fill error in response
				SetError(errorTemp, Disable, DEVICEEXISTMESSAGE, DEVICEEXISTNUMBER);
			}
			responseError.push_back(errorTemp);
		}
	}
	else
	{
		// Fill error in response
		SetError(errorTemp, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
		responseError.push_back(errorTemp);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::DeleteDevices(const ns__User& requestUser, std::vector<ns__Device> requestDeviceList, std::vector<ns__ErrorCode> &responseError)
{
	return SOAP_OK;
}

xsd__int SectionnerService::SetDeviceCalibration(const ns__User& requestUser, const ns__Device &requestDevice, const  ns__CalibrationList &requestCalibrationList, ns__ErrorCode &responseError)
{
	// Check received data format and values
	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;
	ns__Device device = requestDevice;
	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check user should be Admin or Control (no Monitor)
		if (user.uType == Admin || user.uType == Control)
		{
			if (!(erObject.SetCalibrationData(device, requestCalibrationList)))
			{
				SetError(responseError, Disable, SETCALIBRATIONMESSAGE, SETCALIBRATIONNUMBER);
			}
		}
		else
		{
			// Fill error in response
			SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetDeviceCalibration(const ns__User& requestUser, const ns__Device &requestDevice, ns__CalibrationList& responsCalibrationList)
{
	// Check received data format and values
	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;
	ns__Device device = requestDevice;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check user should be Admin or Control (no Monitor)
		if (user.uType == Admin || user.uType == Control)
		{
			if (!(erObject.GetCalibrationData(device, responsCalibrationList)))
			{
				SetError(responsCalibrationList.clErr, Disable, GETCALIBRATIONMESSAGE, GETCALIBRATIONNUMBER);
			}
		}
		else
		{
			// Fill error in response
			SetError(responsCalibrationList.clErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responsCalibrationList.clErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::Login(const ns__User& requestUser, ns__User &responseUserInfo)
{
	// Check received data format and values
	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	// Initialize
	responseUserInfo = requestUser;

	// User Authentication checking
	responseUserInfo.uId = erObject.AuthenticateUser(responseUserInfo);
	if (responseUserInfo.uId < 1)
	{
		// Fill error in response
		SetError(responseUserInfo.uErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetUserDeviceList(const ns__User& requestUser, ns__UserDevice &responseUserDevicesInfo)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.GetAllDeviceList(responseUserDevicesInfo.udDevs) != 1)
			{
				// No device found error
				SetError(responseUserDevicesInfo.udErr, Disable, NODEVICEFOUNDMESSAGE, NODEVICEFOUNDNUMBER);
			}
			else
			{
				for (int i = 0; i < responseUserDevicesInfo.udDevs.size(); i++)
				{
					erObject.DeviceInfoToBase64(responseUserDevicesInfo.udDevs[i]);
				}
			}
		}
		else
		{
			if (erObject.GetUserDeviceList(user, responseUserDevicesInfo.udDevs) != 1)
			{
				// No device found error
				SetError(responseUserDevicesInfo.udErr, Disable, NODEVICEFOUNDMESSAGE, NODEVICEFOUNDNUMBER);
			}
			else
			{
				for (int i = 0; i < responseUserDevicesInfo.udDevs.size(); i++)
				{
					erObject.DeviceInfoToBase64(responseUserDevicesInfo.udDevs[i]);
				}
			}
		}
	}
	else
	{
		// Fill error in response
		SetError(responseUserDevicesInfo.udErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}



xsd__int SectionnerService::GetUsersList(const ns__User& requestUser, ns__UsersList &responseUsersList)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseUsersList.ulUsers.clear();

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.GetUsersList(responseUsersList.ulUsers) <= 0)
			{
				// No device found error
				SetError(responseUsersList.ulErr, Disable, NOUSERFOUNDMESSAGE, NOUSERFOUNDNUMBER);
			}
		}
		else
		{
			SetError(responseUsersList.ulErr, Disable, USERPERMISSIONMESSAGE, USERPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseUsersList.ulErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::AddUser(const ns__User& requestUser, const ns__User &requestNewUserInfo, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseError.eMsg = "";
	responseError.eNo = 0;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.AddUser(requestNewUserInfo) > 0)
			{
			}
			else
			{
				// No device found error
				SetError(responseError, Disable, NOUSERFOUNDMESSAGE, NOUSERFOUNDNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, USERPERMISSIONMESSAGE, USERPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::UpdateUser(const ns__User& requestUser, const ns__User &requestNewUserInfo, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseError.eMsg = "";
	responseError.eNo = 0;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.UpdateUser(requestNewUserInfo) > 0)
			{
			}
			else
			{
				// No user found error
				SetError(responseError, Disable, NOUSERFOUNDMESSAGE, NOUSERFOUNDNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, USERPERMISSIONMESSAGE, USERPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::DeleteUser(const ns__User& requestUser, const ns__User &requestDeleteUserInfo, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseError.eMsg = "";
	responseError.eNo = 0;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.DeleteUser(requestDeleteUserInfo) > 0)
			{
			}
			else
			{
				// No device found error
				SetError(responseError, Disable, NOUSERFOUNDMESSAGE, NOUSERFOUNDNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, USERPERMISSIONMESSAGE, USERPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::AssignDeviceToUser(const ns__User& requestUser, const ns__UserDevice &userDeviceInfo, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseError.eMsg = "";
	responseError.eNo = 0;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.AssignDeviceToUser(userDeviceInfo) > 0)
			{
			}
			else
			{
				// No device found error
				SetError(responseError, Disable, NOUSERFOUNDMESSAGE, NOUSERFOUNDNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, USERPERMISSIONMESSAGE, USERPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::DeleteDeviceFromUser(const ns__User& requestUser, const ns__UserDevice &userDeviceInfo, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseError.eMsg = "";
	responseError.eNo = 0;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			if (erObject.DeleteDeviceFromUser(userDeviceInfo) > 0)
			{
			}
			else
			{
				// No device found error
				SetError(responseError, Disable, NOUSERFOUNDMESSAGE, NOUSERFOUNDNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, USERPERMISSIONMESSAGE, USERPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::GetUserDevices(const ns__User& requestUser, const  ns__User &requestSelectedUserInfo, ns__UserDevice &responseUserDevicesInfo)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	responseUserDevicesInfo.udUser = requestSelectedUserInfo;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin)
		{
			responseUserDevicesInfo.udUser = requestSelectedUserInfo;

			if (erObject.GetUserDeviceList(requestSelectedUserInfo, responseUserDevicesInfo.udDevs) != 1)
			{
				// No device found error
				SetError(responseUserDevicesInfo.udErr, Disable, NODEVICEFOUNDMESSAGE, NODEVICEFOUNDNUMBER);
			}
			else
			{
				for (int i = 0; i < responseUserDevicesInfo.udDevs.size(); i++)
				{
					// Get other device information from database
					if (erObject.GetDeviceSetting(responseUserDevicesInfo.udDevs[i]) == 1)
					{
						erObject.DeviceInfoToBase64(responseUserDevicesInfo.udDevs[i]);
					}
					else
					{
						// Database Error
						SetError(responseUserDevicesInfo.udDevs[i].dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
					}
				}
			}
		}
		else
		{
		}
	}
	else
	{
		// Fill error in response
		SetError(responseUserDevicesInfo.udErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}



xsd__int SectionnerService::SaveActivityLog(const ns__User& requestUser, const  ns__ActivityLog &requestActivityLogInfo, ns__ErrorCode &responseError)
{
	return SOAP_OK;
}

xsd__int SectionnerService::GetActivityLog(const ns__User& requestUser, const  ns__ReportRange &requestReportInfo, std::vector<ns__ActivityLog> &responseActivityLogInfo)
{
	return SOAP_OK;
}

xsd__int SectionnerService::SaveUserLog(const ns__User& requestUser, const ns__UserLog &requestActivityLogInfo, ns__ErrorCode &responseError)
{
	return SOAP_OK;
}

xsd__int SectionnerService::GetUserLog(const ns__User& requestUser, const ns__ReportRange &requestReportInfo, std::vector<ns__UserLog> &responseActivityLogInfo)
{
	return SOAP_OK;
}

xsd__int SectionnerService::SaveDeviceStatusLog(const ns__User& requestUser, const ns__DeviceStatusLog &requestStatusLog, ns__ErrorCode &responseError)
{
	return SOAP_OK;
}

xsd__int SectionnerService::GetDeviceStatusLog(const ns__User& requestUser, const  ns__DeviceStatus &requestStatusLog, ns__DeviceStatusLogList &responseStatusLogList)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;
	// Socket object
	SConnection socketObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		// Check user should be Admin or Control (no Monitor)
		if (user.uType == Admin || user.uType == Control)
		{
			if (erObject.GetDeviceStatusLog(requestStatusLog, responseStatusLogList) > 0)
			{
				// Base64 encoding data
				Base64 b64;
				for (int i = 0; i < responseStatusLogList.dsllStatus.size(); i++)
				{
					//erObject.DeviceInfoToBase64(responseStatusLogList.dsllStatus[i].dslDevice);
					socketObject.GetReport(responseStatusLogList.dsllStatus[i].dslDevice, responseStatusLogList.dsllStatus[i].dslErr.eMsg);

					/*if (erObject.GetDeviceSetting(responseStatusLogList.dsllStatus[i].dslDevice) == 1)
					{
						erObject.DeviceInfoToBase64(responseStatusLogList.dsllStatus[i].dslDevice);
					}
					else
					{
						// Database Error
						SetError(responseStatusLogList.dsllStatus[i].dslDevice.dErr, Disable, DATABASEERRORTMESSAGE, DATABASEERRORNUMBER);
					}*/
				}
			}
			else
			{
				// Fill error in response
				SetError(responseStatusLogList.dsllErr, Disable, NORECORDFOUNDMESSAGE, NORECORDFOUNDNUMBER);
			}
		}
		else
		{
			// Fill error in response
			SetError(responseStatusLogList.dsllErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseStatusLogList.dsllErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}



xsd__int SectionnerService::GetServerOptions(const ns__User& requestUser, ns__ServerOptions &serverOptions)
{
	return SOAP_OK;
}

xsd__int SectionnerService::SetServerOptions(const ns__User& requestUser, const  ns__ServerOptions &serverOptions, ns__ErrorCode &responseError)
{
	return SOAP_OK;
}



xsd__int SectionnerService::GetCityAndLocations(const ns__User& requestUser, ns__AllCityLocatoins &responseCityLocationInfo)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin || user.uType == Control)
		{
			if (erObject.GetDeviceCityLocationList(responseCityLocationInfo) < 1)
			{
				SetError(responseCityLocationInfo.aclErr, Disable, NOCITYLOCATIONMESSAGE, NOCITYLOCATIONNUMBER);
			}
			else
			{
				if (responseCityLocationInfo.aclCityLocatoins.size() > 0)
				{
					Base64 b64;
					for (int i = 0; i < responseCityLocationInfo.aclCityLocatoins.size(); i++)
					{
						// Encode to base 64 for unicode string
						responseCityLocationInfo.aclCityLocatoins[i].clCity = b64.Encode(responseCityLocationInfo.aclCityLocatoins[i].clCity);
						if (responseCityLocationInfo.aclCityLocatoins[i].clLocations.size() > 0)
						{
							for (int k = 0; k < responseCityLocationInfo.aclCityLocatoins[i].clLocations.size(); k++)
							{
								responseCityLocationInfo.aclCityLocatoins[i].clLocations[k] = b64.Encode(responseCityLocationInfo.aclCityLocatoins[i].clLocations[k]);
							}
						}
					}
				}
				else
				{
					SetError(responseCityLocationInfo.aclErr, Disable, NOITEMFOUNDMESSAGE, NOITEMFOUNDNUMBER);
				}
			}
		}
		else
		{
			SetError(responseCityLocationInfo.aclErr, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseCityLocationInfo.aclErr, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::ChangeCityName(const ns__User& requestUser, const  xsd__string &oldCityName, const  xsd__string &newCityName, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin || user.uType == Control)
		{
			Base64 b64;
			if (erObject.ChangeCityName(b64.Decode(oldCityName), b64.Decode(newCityName)) < 1)
			{
				SetError(responseError, Disable, UPDATECITYNAME, UPDATECITYNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::ChangeLocationName(const ns__User& requestUser, const  xsd__string &oldCityName, const xsd__string &newCityName, const  xsd__string &oldLocationName, const xsd__string &newLocationName, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin || user.uType == Control)
		{
			Base64 b64;
			if (erObject.ChangeLocationName(b64.Decode(oldCityName), b64.Decode(newCityName), b64.Decode(oldLocationName), b64.Decode(newLocationName)) < 1)
			{
				SetError(responseError, Disable, UPDATELOCATIONNAME, UPDATELOCATIONNUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}

xsd__int SectionnerService::ChangeDeviceName(const ns__User& requestUser, xsd__int deviceSerialNumber, const  xsd__string &newCityName, const  xsd__string &newLocationName, const  xsd__string &newDeviceName, ns__ErrorCode &responseError)
{
	// Check received data format and values

	// Create an object from ExecuteReauest class
	ExecuteRequest erObject;

	// User Authentication checking
	ns__User user = requestUser;
	user.uId = erObject.AuthenticateUser(user);
	if (user.uId > 0)
	{
		if (user.uType == Admin || user.uType == Control)
		{
			Base64 b64;
			if (erObject.ChangeDeviceName(deviceSerialNumber, b64.Decode(newCityName), b64.Decode(newLocationName), b64.Decode(newDeviceName)) < 1)
			{
				SetError(responseError, Disable, UPDATEDEVICENAMEMESSAGE, UPDATEDEVICENAMENUMBER);
			}
		}
		else
		{
			SetError(responseError, Disable, DEVICEPERMISSIONMESSAGE, DEVICEPERMISSIONNUMBER);
		}
	}
	else
	{
		// Fill error in response
		SetError(responseError, Disable, USERAUTHENTICATIONMESSAGE, USERAUTHENTICATIONNUMBER);
	}

	return SOAP_OK;
}
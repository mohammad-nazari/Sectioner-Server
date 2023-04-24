#include "..\Parser\SyntaxChecker.h"
#include "SConnection.h"
#include "ExecuteRequest.h"

extern std::map<int, DeviceMap> DeviceList;
extern std::map<int, DeviceMap> DeviceListClone;
extern CRITICAL_SECTION csThread;

SConnection::SConnection()
{
	this->_listenSocket = INVALID_SOCKET;
}

SConnection::~SConnection()
{
}

void SConnection::LogData(ns__Device DeviceObject, std::string Data)
{
	DBConnection dbConnection;

	dbConnection.SaveDeviceLog(DeviceObject, Data);
}

int SConnection::CreateSocket()
{
	WSADATA wsaData;

	struct addrinfo *result = nullptr;
	struct addrinfo hints;

	// Initialize WinSock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		//printf("WSAStartup failed with Error :  %d\n", iResult);
		return 0;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		//printf("getaddrinfo failed with Error :  %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Create a SOCKET for connecting to server
	this->_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->_listenSocket == INVALID_SOCKET)
	{
		//printf("socket failed with Error :  %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	// Setup the TCP listening socket
	iResult = bind(this->_listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR)
	{
		//printf("bind failed with Error :  %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	iResult = listen(this->_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	// Set time out
	int nTimeout = CONNECTIONTIMEOUT;
	iResult = setsockopt(this->_listenSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&nTimeout), sizeof(int));
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	iResult = setsockopt(this->_listenSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&nTimeout), sizeof(int));
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	int buffsize = 1024 * 1024;
	iResult = setsockopt(this->_listenSocket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&buffsize), sizeof(buffsize));
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	/*//std::cout << " On " << result->ai_addr << " : " << DEFAULT_PORT << std::endl;*/

	freeaddrinfo(result);

	return 1;
}

int SConnection::CloseSocket() const
{

	/* Clean up */
	closesocket(this->_listenSocket);
	WSACleanup();

	return 1;
}

int SConnection::ThreadStartListen()
{
	// Create thread
	std::thread threadObject(&SConnection::StartListen, *this);

	// Start thread and continue
	threadObject.detach();

	return 1;
}

//************************************
// Method:    StartListon
// FullName:  SConnection::StartListon
// Access:    public 
// Returns:   int
// Qualifier:
//
// Description: It is run as a thread async
//************************************
int SConnection::StartListen()
{
	/* Main loop */
	while (true)
	{
		int size = sizeof(struct sockaddr);
		struct sockaddr_in their_addr;

		ZeroMemory(&their_addr, sizeof(struct sockaddr));
		SOCKET newsock = accept(this->_listenSocket, reinterpret_cast<struct sockaddr*>(&their_addr), &size);
		if (newsock == INVALID_SOCKET)
		{
			perror("accept\n");
		}
		else
		{
			DeviceMap deviceMap(newsock);
			deviceMap._device.dSerialNumber = 10000001;
			// ReSharper disable once CppDeprecatedEntity
			Splitter spObj(inet_ntoa(their_addr.sin_addr), ".");
			deviceMap._device.dIP.ip1 = this->_toolsObject.toInteger(spObj[0]);
			deviceMap._device.dIP.ip2 = this->_toolsObject.toInteger(spObj[1]);
			deviceMap._device.dIP.ip3 = this->_toolsObject.toInteger(spObj[2]);
			deviceMap._device.dIP.ip4 = this->_toolsObject.toInteger(spObj[3]);
			deviceMap._device.dPort = ntohs(their_addr.sin_port);
			deviceMap._device.dSocket = newsock;

			// Generate add run thread and continue
			this->ThreadHandleDevice(deviceMap);
		}
	}
}

//************************************
// Method:    StartListon
// FullName:  SConnection::StartListon
// Access:    public 
// Returns:   int
// Qualifier:
//
// Description: It is run as a thread async
//************************************
int SConnection::StartListen2()
{
	/* Main loop */
	while (true)
	{
		int size = sizeof(struct sockaddr);
		struct sockaddr_in their_addr;

		ZeroMemory(&their_addr, sizeof(struct sockaddr));
		SOCKET newsock = accept(this->_listenSocket, reinterpret_cast<struct sockaddr*>(&their_addr), &size);
		if (newsock == INVALID_SOCKET)
		{
			perror("accept\n");
		}
		else
		{
			DeviceMap deviceMap(newsock);
			deviceMap._device.dSerialNumber = 10000001;
			// ReSharper disable once CppDeprecatedEntity
			Splitter spObj(inet_ntoa(their_addr.sin_addr), ".");
			deviceMap._device.dIP.ip1 = this->_toolsObject.toInteger(spObj[0]);
			deviceMap._device.dIP.ip2 = this->_toolsObject.toInteger(spObj[1]);
			deviceMap._device.dIP.ip3 = this->_toolsObject.toInteger(spObj[2]);
			deviceMap._device.dIP.ip4 = this->_toolsObject.toInteger(spObj[3]);
			deviceMap._device.dPort = ntohs(their_addr.sin_port);
			deviceMap._device.dSocket = newsock;
			this->ReadData(newsock, deviceMap._device.dCustomCommand);
			std::cout << "Command (" << deviceMap._device.dSerialNumber << "): " << deviceMap._device.dCustomCommand << std::endl;
			
			// Generate add run thread and continue
			this->ThreadHandleDevice2(deviceMap);
		}
	}
}

int SConnection::ThreadHandleDevice(DeviceMap& DeviceObject)
{
	// Create a thread
	std::thread threadObject(&SConnection::HandleDevice, this, DeviceObject);

	// Run thread and continue
	threadObject.detach();

	return 1;
}

int SConnection::HandleDevice(DeviceMap& DeviceMapObject) const
{
	ns__Device device = DeviceMapObject._device;
	if (this->IsDevice(DeviceMapObject) > 0)
	{
		/*DeviceMap DeviceMapObject;
		DeviceMapObject._device.dSerialNumber = DeviceMapObjectIn._device.dSerialNumber;
		AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Clone);*/
		bool isCommand = false;
		device.dSerialNumber = DeviceMapObject._device.dSerialNumber;
		Base64 base64Object;
		SyntaxChecker parserObject;
		parserObject.DeviceInfo(DeviceMapObject._device);

		int bytesSendReceived;

		std::string strMessageSend;
		std::string strMessageReceived = "";

		int tryCounter = 0;
		int const tryNumber = TRYCOUNTER;

		time_t samplingTime = time(nullptr);

		if (DeviceMapObject._isCamera == true || DeviceMapObject._isAlarm == true)
		{
			strMessageSend = SERVER_LIVE_OK_STRING;
			this->_toolsObject.replaceOne(strMessageSend, "[%ss]", this->_toolsObject.toString(device.dSerialNumber), 0);
			this->_toolsObject.replaceOne(strMessageSend, "[%s]", this->_toolsObject.toString(device.dIP.ip1) + "." + this->_toolsObject.toString(device.dIP.ip2) + "." + this->_toolsObject.toString(device.dIP.ip3) + "." + this->_toolsObject.toString(device.dIP.ip4), 0);
		}
		else
		{
			strMessageSend = SERVER_STT_STRING;
			this->_toolsObject.replaceOne(strMessageSend, "[%ss]", this->_toolsObject.toString(DeviceMapObject._device.dSerialNumber), 0);
		}
		do
		{
			////====////std::cout << "Request(" << this->_toolsObject.getCurrentDateTime() << "): " << strMessageSend << std::endl;
			bytesSendReceived = this->WriteData(device.dSocket, strMessageSend.c_str());
			//LogData(DeviceMapObject._device, "Request(" + this->_toolsObject.getCurrentDateTime() + "): " + strMessageSend);

			if (bytesSendReceived != SOCKET_ERROR)
			{
				bytesSendReceived = this->ReadData(device.dSocket, strMessageReceived);

				////====////std::cout << "Message(" << this->_toolsObject.getCurrentDateTime() << "): " << strMessageReceived << std::endl;
				//LogData(DeviceMapObject._device, "Message(" + this->_toolsObject.getCurrentDateTime() + "): " + strMessageReceived);

				if (bytesSendReceived != SOCKET_ERROR)
				{
					bool isSet;
					if (parserObject.AnalyzeRecievedData(strMessageReceived, isSet))
					{
						if (this->ParsDeviceData(strMessageReceived, parserObject, DeviceMapObject, tryCounter) == 0)
						{
							// It is STT command result
							tryCounter++;
							bytesSendReceived = tryCounter < tryNumber ? 1 : SOCKET_ERROR;
						}
						else
						{
							tryCounter = 0;
							if (isCommand == true)
							{
								DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result = strMessageReceived;
								// Update result value
								AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Set_CNF_Result);

								// Fire event
								SetEvent(DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hReadWriteResult);
								std::cout << "Message(" << this->_toolsObject.getCurrentDateTime() << "): " << strMessageReceived << std::endl;

								isCommand = false;
							}
						}
						if (tryCounter == 0)
						{
							// Idle for some times
							HANDLE idleThread = DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hIdleThread;
							int timeOut = IDLETIME;
							// Wait until a command enter or time out
							DWORD waitEventIdle = WaitForSingleObject(idleThread, timeOut);

							// Wait until next command com in
							// or time go out
							HANDLE checkCNF = DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hReadWriteCommand;
							timeOut = WAITCOMMANDTIME;
							// Wait until a command enter or time out
							DWORD waitEventResult = WaitForSingleObject(checkCNF, timeOut);
							// CNF command received 
							if (waitEventResult == WAIT_OBJECT_0)
							{
								ResetEvent(DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hReadWriteCommand);
								// Check CNF commands and do them first
								AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Get_CNF);
							}

							bytesSendReceived = 1;
							strMessageSend = SERVER_LIVE_OK_STRING;
							if (DeviceMapObject._requestMap[DeviceMapObject._isCamera]._command != "")
							{
								strMessageSend = DeviceMapObject._requestMap[DeviceMapObject._isCamera]._command;

								std::cout << "(" << this->_toolsObject.getCurrentDateTime() << ") A command received for device (" << DeviceMapObject._device.dSerialNumber << "): ";
								std::cout << strMessageSend << std::endl;
								isCommand = true;
							}
							else if (/*((time(nullptr) - samplingTime) > DeviceMapObject._device.dSamplingTime) &&*/ DeviceMapObject._isCamera == false && DeviceMapObject._isAlarm == false) // It is not camera or alarm device
							{
								// After this time should send STT command
								strMessageSend = SERVER_STT_STRING;
								samplingTime = time(nullptr);
							}
							else if (DeviceMapObject._isAlarm == true && DeviceMapObject._device.dModel == ALARM)
							{
								// Check all device for alarms
								// Door and Gas bit
								this->AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Clone_ALL);

								bool sendAlarm = false;
								for (std::map<int, DeviceMap>::iterator it = DeviceListClone.begin(); it != DeviceListClone.end() && sendAlarm == false; ++it)
								{
									if (it->second._device.dModel == SECTIONNER)
									{
										if (it->second._device.dSensors.size() > 0)
										{
											for (unsigned int i = 0; i < it->second._device.dSensors.size() && sendAlarm == false; i++)
											{
												if (it->second._device.dSensors[i].seName == DIGITALINPUT)
												{
													if (it->second._device.dSensors[i].seVal.size() > 2)
													{
														sendAlarm |= it->second._device.dSensors[i].seVal[2] != 1;
													}
													if (it->second._device.dSensors[i].seVal.size() > 4)
													{
														sendAlarm |= it->second._device.dSensors[i].seVal[4] != 1;
													}
												}
											}
										}
									}
								}

								strMessageSend = SERVER_ALARM_STRING;
								this->_toolsObject.replaceOne(strMessageSend, "[%s]", (sendAlarm == false ? "0" : "1"), 0);
							}
							else
							{
								this->_toolsObject.replaceOne(strMessageSend, "[%s]", this->_toolsObject.toString(device.dIP.ip1) + "." + this->_toolsObject.toString(device.dIP.ip2) + "." + this->_toolsObject.toString(device.dIP.ip3) + "." + this->_toolsObject.toString(device.dIP.ip4), 0);
							}

							this->_toolsObject.replaceOne(strMessageSend, "[%ss]", this->_toolsObject.toString(DeviceMapObject._device.dSerialNumber), 0);
						}
					}
					else
					{
						tryCounter++;
						bytesSendReceived = tryCounter < tryNumber ? 1 : SOCKET_ERROR;
					}
				}
				else
				{
					tryCounter++;
					bytesSendReceived = tryCounter < tryNumber ? 1 : SOCKET_ERROR;
					//LogData(DeviceMapObject._device, "Error Code(" + this->_toolsObject.getCurrentDateTime() + "): " + this->_toolsObject.toString(WSAGetLastError()));
				}
			}
			else
			{
				tryCounter++;
				bytesSendReceived = tryCounter < tryNumber ? 1 : SOCKET_ERROR;
			}
		} while (bytesSendReceived != SOCKET_ERROR || tryCounter < tryNumber);

		/*
		*	Thread end
		*	and device should be deleted from list
		*/
		if (this->AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Delete))
		{
			////====////std::cout << "\nDevice (" << DeviceMapObject._device.dSerialNumber << ") Deleted From List" << std::endl;
			//LogData(DeviceMapObject._device, "\nDevice (" + Tools::toString(DeviceMapObject._device.dSerialNumber) + ") Deleted From List");
		}
		else
		{
			////====////std::cout << "\nError in deleting device (" << DeviceMapObject._device.dSerialNumber << ") from list" << std::endl;
			//LogData(DeviceMapObject._device, "\nError in deleting device (" + Tools::toString(DeviceMapObject._device.dSerialNumber) + ") from list");
		}
	}
	try
	{
		closesocket(device.dSocket);
		////====////std::cout << "Socket (" << device.dSocket << ") Closed successfully" << std::endl;
	}
	catch (std::exception e)
	{
		////====////std::cout << "Socket (" << device.dSocket << ") Closed unsuccessfully" << std::endl;
	}
	_endthread();

	return 1;
}

int SConnection::HandleDevice2(DeviceMap& DeviceMapObject) const
{
	std::string strMessage = DeviceMapObject._device.dCustomCommand;

	this->RemoveUnusedChars(strMessage);
	Splitter sp(strMessage, "#");
	sp.reset(sp[0], ",");

	if (sp.size() == 5 && this->_toolsObject.toInteger(this->_toolsObject.toUpper(sp[3])) == OUTRELAY)
	{
		DeviceMapObject._device.dRequests.push_back(ns__RequestType::OUTRELAY);
		for each (char var in sp[4])
		{
			DeviceMapObject._device.dRelays.push_back(var == '0' ? false : true);
		}
	}
	else if (sp.size() == 4 && this->_toolsObject.toInteger(this->_toolsObject.toUpper(sp[3])) == STT)
	{
		DeviceMapObject._device.dRequests.push_back(ns__RequestType::STT);
	}
	DeviceMapObject._device.dName = sp[1];
	DeviceMapObject._device.dNikeName = sp[2];
	DeviceMapObject._device.dSerialNumber = this->_toolsObject.toInteger(this->_toolsObject.toUpper(sp[0]));

	std::vector<ns__ErrorCode> ErrorCode;
	this->SetDeviceStatus(DeviceMapObject._device, ErrorCode);
	WriteData(DeviceMapObject._device.dSocket, "DON#");
	closesocket(DeviceMapObject._device.dSocket);
	_endthread();

	return 1;
}

int SConnection::IsDevice(DeviceMap& DeviceMapObject) const
{
	Base64 coder;
	DeviceMapObject._isCamera = false;
	DeviceMapObject._isAlarm = false;
	std::string strMessage = SERVER_NAME_STRING;

	////====////std::cout << "Request(" << this->_toolsObject.getCurrentDateTime() << "): " << strMessage << std::endl;

	int bytesSendReceived = this->WriteData(DeviceMapObject._device.dSocket, strMessage.c_str());
	if (bytesSendReceived == SOCKET_ERROR)
	{
		return -1;
	}

	bytesSendReceived = this->ReadData(DeviceMapObject._device.dSocket, strMessage);
	////====////std::cout << "Message(" << this->_toolsObject.getCurrentDateTime() << "): " << strMessage << std::endl;

	if (bytesSendReceived == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAECONNRESET)
		{
			return -1;
		}
	}
	if (bytesSendReceived != 0)
	{
		this->RemoveUnusedChars(strMessage);
		Splitter sp(strMessage, "#");
		sp.reset(sp[0], ",");

		if (sp.size() == 3 && this->_toolsObject.toUpper(sp[2]) == "NAM")
		{
			if (this->_toolsObject.toUpper(sp[1]) == "CAM")
			{
				DeviceMapObject._isCamera = true;

				DeviceMapObject._device.dCamera.cSocket = DeviceMapObject._device.dSocket;
				DeviceMapObject._device.dCamera.cIP = DeviceMapObject._device.dIP;
				DeviceMapObject._device.dCamera.cPort = DeviceMapObject._device.dPort;

				DeviceMapObject._device.dSocket = 0;
				DeviceMapObject._device.dIP.ip1 = 0;
				DeviceMapObject._device.dIP.ip2 = 0;
				DeviceMapObject._device.dIP.ip3 = 0;
				DeviceMapObject._device.dIP.ip4 = 0;
				DeviceMapObject._device.dPort = 0;
			}
			else if (this->_toolsObject.toUpper(sp[1]) == "DEVM")
			{
				DeviceMapObject._device.dModel = MANAGER;
			}
			else if (this->_toolsObject.toUpper(sp[1]) == "DEVS")
			{
				DeviceMapObject._device.dModel = SECTIONNER;
			}
			else if (this->_toolsObject.toUpper(sp[1]) == "DALM")
			{
				DeviceMapObject._device.dModel = ALARM;
				DeviceMapObject._isAlarm = true;
			}
			else
			{
				return -1;
			}
			DeviceMapObject._device.dSerialNumber = atoi(sp[0].c_str());
			this->AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Add);
			this->CreateDeviceRow(DeviceMapObject._device);
			return DeviceMapObject._device.dSerialNumber;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		// Connection closed
		return -1;
	}
}

int SConnection::ParsDeviceData(std::string strMessageReceived, SyntaxChecker parserObject, DeviceMap& DeviceMapObject, int& tryCounter) const
{
	int isCamera = 0;
	parserObject.DeviceInfo(DeviceMapObject._device);

	if (parserObject.CheckSTTSyntax(strMessageReceived, isCamera))
	{
		DeviceMapObject._device = parserObject.DeviceInfo();

		time(&(DeviceMapObject._device.dDDateTime)); // Current date time

		tryCounter = 0;

		ExecuteRequest erObject;
		if (erObject.GetDeviceSetting(DeviceMapObject._device) == 1)
		{
			erObject.DeviceInfoToBase64(DeviceMapObject._device);
		}
		else
		{
			this->CreateDeviceRow(DeviceMapObject._device);
		}

		if (isCamera == -1 && DeviceMapObject._device.dModel != ALARM)
		{
			// Set sampling data
			this->InitiateDeviceStatus(DeviceMapObject._device);

			DeviceMapObject._device.dPOK = round(DeviceMapObject._device.dTransPower * 0.75);
			DeviceMapObject._device.dPTotal = round(DeviceMapObject._device.dPR + DeviceMapObject._device.dPS + DeviceMapObject._device.dPT);

			this->SaveDevieStatus(DeviceMapObject._device, strMessageReceived);
		}
		this->AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Update);
		this->UpdateDeviceRow(DeviceMapObject._device);
		this->GetDeviceSamplingTime(DeviceMapObject._device);

		return 1;
	}
	return 0;
}

int SConnection::WriteData(SOCKET Socket, const std::string strMessage)
{
	// Base64
	//std::string MessageData = Encode(strMessage);
	std::string MessageData = strMessage;

	int bytesSendReceived = send(Socket, MessageData.c_str(), static_cast<int>(MessageData.size()), 0);
	//Sleep(SLEEPTIMEVALUE);

	return bytesSendReceived;
}

int SConnection::ReadData(SOCKET Socket, std::string& strMessage)
{
	strMessage = "";
	char buf[MAX_DATA] = { '\0' };
	int bytesSendReceived;
	int bytesReceivedFromClientMsgSum = 1;
	strMessage.clear();
	do
	{
		memset(buf, 0, MAX_DATA);  //clear the variable
		bytesSendReceived = recv(Socket, buf, MAX_DATA - 1, 0);
		bytesReceivedFromClientMsgSum += bytesSendReceived;
		// Base64
		strMessage += buf;
	} while (bytesSendReceived > 0 && strMessage.find('#') == std::string::npos);

	return bytesReceivedFromClientMsgSum;
}

int SConnection::UpdateDeviceRow(ns__Device& deviceInfo)
{
	DBConnection dbObject;

	unsigned long id = dbObject.UpdateDeviceRow(deviceInfo);

	return id;
}

int SConnection::ThreadHandleDevice2(DeviceMap deviceMap)
{
	// Create a thread
	std::thread threadObject(&SConnection::HandleDevice2, this, deviceMap);

	// Run thread and continue
	threadObject.detach();

	return 1;
}

int SConnection::GetDeviceSamplingTime(ns__Device & deviceInfo)
{
	DBConnection dbObject;

	unsigned long id = dbObject.GetDeviceSamplingTime(deviceInfo);

	return id;
}

int SConnection::CreateDeviceRow(ns__Device& deviceInfo) const
{
	DBConnection dbObject;
	ns__Device deviceTemp = deviceInfo;

	if (dbObject.GetDeviceSetting(deviceTemp) == 0)
	{
		ns__CalibrationList calibrationList;
		if (deviceTemp.dModel != ALARM)
		{
			this->GenerateCalibrationData(deviceInfo, calibrationList);
		}
		std::string calibrationData = this->_toolsObject.GetCalibrationDataFromSOAP(calibrationList);
		dbObject.CreateDeviceRow(deviceInfo, calibrationData);
	}
	else
	{
		dbObject.UpdateDeviceRow(deviceInfo);
	}

	return 1;
}

int SConnection::ExistDeviceRow(ns__Device& deviceInfo) const
{
	DBConnection dbObject;
	return dbObject.CheckSerialNumber(this->_toolsObject.toString(deviceInfo.dSerialNumber));
}

int SConnection::GetDeviceStatus(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapInfo;
	deviceMapInfo._device = DeviceObject;

	int result = this->AddUpdateDeletSearchDeviceInList(deviceMapInfo, Device_Clone);
	DeviceObject = deviceMapInfo._device;

	return result;
}

int SConnection::GetDevicePicture(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapObject;
	deviceMapObject._device = DeviceObject;
	deviceMapObject._isCamera = true;

	// Generate string sending to device
	std::string strCommand = REQUEST_PICTURE_STRING;
	this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
	deviceMapObject._requestMap[deviceMapObject._isCamera]._command = strCommand;

	//ns__Device deviceObject;
	if (this->AddUpdateDeletSearchDeviceInList(deviceMapObject, Device_Search))
	{
		if (DeviceObject.dCamera.cPort != 0)
		{
			strCommand = this->ApplyCommand(deviceMapObject);
			SyntaxChecker parserObject;
			if (parserObject.CheckPICSyntax(strCommand))
			{
				DeviceObject.dPicture = parserObject.DeviceInfo().dPicture;

				return 1;
			}
			else
			{
				DeviceObject.dErr.eMsg = parserObject.ErrorData();
				DeviceObject.dErr.eNo = 1;
				return 0;
			}
		}
		else
		{
			DeviceObject.dErr.eMsg = "Camera is disabled";
			DeviceObject.dErr.eNo = 1;
			return 0;
		}
	}

	return 0;
}

int SConnection::GetDevicePicturePart(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapObject;
	deviceMapObject._device = DeviceObject;
	deviceMapObject._isCamera = true;

	// Generate string sending to device
	std::string strCommand = REQUEST_PART_STRING;
	this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(DeviceObject.dPicture.pParts[0].ppIndex), 0);
	deviceMapObject._requestMap[deviceMapObject._isCamera]._command = strCommand;

	//ns__Device deviceObject;
	if (this->AddUpdateDeletSearchDeviceInList(deviceMapObject, Device_Search))
	{
		if (DeviceObject.dCamera.cPort != 0)
		{
			strCommand = this->ApplyCommand(deviceMapObject);
			SyntaxChecker parserObject;

			if (parserObject.CheckPartSyntax(strCommand))
			{
				DeviceObject.dPicture = parserObject.DeviceInfo().dPicture;

				/*Base64 base64;
				DeviceObject.dPicture.pParts[0].partData = base64.Encode(DeviceObject.dPicture.pParts[0].partData);*/

				return 1;
			}
			else
			{
				DeviceObject.dErr.eMsg = parserObject.ErrorData();
				DeviceObject.dErr.eNo = 1;
				return 0;
			}
		}
		else
		{
			DeviceObject.dErr.eMsg = "Camera is disabled";
			DeviceObject.dErr.eNo = 1;
			return 0;
		}
	}

	return 0;
}

int SConnection::SetDeviceStatus(ns__Device& DeviceObject, std::vector<ns__ErrorCode>& ErrorCode) const
{
	if (DeviceObject.dRequests.size() > 0)
	{
		DeviceMap deviceMapInfo;
		deviceMapInfo._device = DeviceObject;
		ns__ErrorCode errorCode;

		//ns__Device deviceObject;
		if (this->AddUpdateDeletSearchDeviceInList(deviceMapInfo, Device_Search))
		{
			for each (ns__RequestType var in DeviceObject.dRequests)
			{
				switch (var)
				{
				case DATETIME:
				{
					if (this->SetDateTime(DeviceObject) < 1)
					{
						this->_toolsObject.SetErrorData(errorCode, DATETIMEMESSAGE, DATETIMENUMBER, Critical);
						ErrorCode.push_back(errorCode);
					}
					break;
				}
				case OUTRELAY:
				{
					if (this->SetRelay(DeviceObject) < 1)
					{
						this->_toolsObject.SetErrorData(errorCode, RELAYMESSAGE, RELAYNUMBER, Critical);
						ErrorCode.push_back(errorCode);
					}
					break;
				}
				case RESET:
				{
					if (this->SetReset(DeviceObject) < 1)
					{
						this->_toolsObject.SetErrorData(errorCode, RESETMESSAGE, RESETNUMBER, Critical);
						ErrorCode.push_back(errorCode);
					}
					break;
				}
				case STT:
				{
					if (this->SetStt(DeviceObject) < 1)
					{
						this->_toolsObject.SetErrorData(errorCode, RESETMESSAGE, RESETNUMBER, Critical);
						ErrorCode.push_back(errorCode);
					}
					break;
				}
				default:
					break;
				}
			}
		}
		else
		{
			this->_toolsObject.SetErrorData(errorCode, DEVICECONNECTIONMESSAGE, DEVICECONNECTIONNUMBER, Critical);
			ErrorCode.push_back(errorCode);
		}
	}
	return 1;
}

std::string SConnection::ApplyCommand(DeviceMap& DeviceMapObject) const
{
	// Save CNF command in device object to progress in device
	this->AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Set_CNF);
	// Fire event
	SetEvent(DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hReadWriteCommand);

	// While CNF not progressed and result not returned
	// Wait until a command enter or time out
	HANDLE checkCNF = DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hReadWriteResult;
	int timeOut = WAITINGTIMEOUT;
	DWORD waitEventResult = WaitForSingleObject(checkCNF, timeOut);
	if (waitEventResult == WAIT_OBJECT_0)
	{
		ResetEvent(DeviceMapObject._requestMap[DeviceMapObject._isCamera]._hReadWriteResult);
		// Get CNF result
		this->AddUpdateDeletSearchDeviceInList(DeviceMapObject, Device_Get_CNF_Result);

		std::cout << "CNF result for device (" << DeviceMapObject._device.dSerialNumber << ") is : " << DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result << std::endl;
	}
	return DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result;
}

bool SConnection::GetCNFResult(ns__Device& DeviceObject, std::string CNFResult) const
{
	std::string strCommand = "";
	int replaceIndex = 0;
	Splitter sp(CNFResult, ",");
	for each (ns__RequestType request in DeviceObject.dRequests)
	{
		switch (request)
		{
		case SAMPLING:
		{
			strCommand = REQUEST_SET_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(DeviceObject.dSamplingTime), replaceIndex);
			break;
		}
		case DATETIME:
		{
			localtime(&(DeviceObject.dDateTime));
			strCommand = REQUEST_TIME_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.Time_tToString(DeviceObject.dDateTime, "%y-%m-%d %H:%M:%S"), replaceIndex);
			break;
		}
		case OUTRELAY:
		{
			ConverterDecimal converter;
			strCommand = REQUEST_OUT_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(converter.Binary_Decimal_Array(DeviceObject.dRelays)), replaceIndex);
			break;
		}
		case PARTS:
		{
			strCommand = REQUEST_PART_STRING;
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
			replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(DeviceObject.dPicture.pParts[0].ppIndex), replaceIndex);
			break;
		}
		case PICTURE:
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
	return true;
}

std::map<std::string, std::string> SConnection::GetSensorsName(std::string Input)
{
	return std::map<std::string, std::string>();
}

int SConnection::AddUpdateDeletSearchDeviceInList(DeviceMap& DeviceMapObject, Device_AddRemoveUpdateSearch CommandType)
{
	int result = 0;

	int i = DeviceMapObject._device.dSerialNumber;
	switch (CommandType)
	{
	case Device_Add: // Add
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&(csThread));
			// Is camera
			if (DeviceMapObject._isCamera == true)
			{
				DeviceList[i]._device.dCamera = DeviceMapObject._device.dCamera;
			}
			else
			{
				DeviceMapObject._device.dCamera = DeviceList[i]._device.dCamera;
				DeviceMapObject._device.dPicture = DeviceList[i]._device.dPicture;
				DeviceList[i]._device = DeviceMapObject._device;
			}
			DeviceMapObject = DeviceList[i];
			result = -1;


			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&(csThread));
		}
		else
		{
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&(csThread));
			DeviceList.insert(std::pair<int, DeviceMap>(i, DeviceMapObject));
			result = 1;

			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&(csThread));
		}
		break;
	}
	case Device_Update: // Update
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&(csThread));
			// Is camera
			if (DeviceMapObject._isCamera == true)
			{
				DeviceList[i]._device.dCamera = DeviceMapObject._device.dCamera;
			}
			else
			{
				DeviceMapObject._device.dCamera = DeviceList[i]._device.dCamera;
				DeviceMapObject._device.dPicture = DeviceList[i]._device.dPicture;
				DeviceList[i]._device = DeviceMapObject._device;
			}
			result = 1;

			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&(csThread));
		}
		break;
	}
	case Device_Delete: // Delete
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&(csThread));
			// Is camera
			if (DeviceMapObject._isCamera == 1 && DeviceList[i]._device.dCamera.cSocket == DeviceMapObject._device.dCamera.cSocket)
			{
				DeviceList[i]._device.dCamera.cSocket = 0;
				DeviceList[i]._device.dCamera.cPort = 0;
				DeviceList[i]._device = DeviceList[i]._device;
				if (DeviceList[i]._device.dSocket == 0)
				{
					DeviceList.erase(i);
				}
			}
			else if (DeviceList[i]._device.dSocket == DeviceMapObject._device.dSocket)
			{
				DeviceList[i]._device.dSocket = 0;
				DeviceList[i]._device.dPort = 0;
				DeviceList[i]._device = DeviceList[i]._device;
				if (DeviceList[i]._device.dCamera.cSocket == 0)
				{
					DeviceList.erase(i);
				}
			}
			result = 1;

			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&(csThread));
		}
		break;
	}
	case Device_Search: // Search
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			result = 1;
		}
		break;
	}
	case Device_Set_CNF: // Set CNF
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			DeviceList[i]._requestMap[DeviceMapObject._isCamera]._command = DeviceMapObject._requestMap[DeviceMapObject._isCamera]._command;
			DeviceList[i]._requestMap[DeviceMapObject._isCamera]._result = "";
			DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result = "";
			result = 1;
		}
		break;
	}
	case Device_Set_CNF_Result: // Set CNF result
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			DeviceList[i]._requestMap[DeviceMapObject._isCamera]._result = DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result;
			DeviceList[i]._requestMap[DeviceMapObject._isCamera]._command = "";
			DeviceMapObject._requestMap[DeviceMapObject._isCamera]._command = "";
			result = 1;
		}
		break;
	}
	case Device_Get_CNF: // Get CNF
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			DeviceMapObject._requestMap[DeviceMapObject._isCamera]._command = DeviceList[i]._requestMap[DeviceMapObject._isCamera]._command;
			DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result = "";
			result = 1;
		}
		break;
	}
	case Device_Get_CNF_Result: // Get CNF Result
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			DeviceMapObject._requestMap[DeviceMapObject._isCamera]._result = DeviceList[i]._requestMap[DeviceMapObject._isCamera]._result;
			DeviceMapObject._requestMap[DeviceMapObject._isCamera]._command = "";
			result = 1;
		}
		break;
	}
	case Device_Clone: // Search
	{
		if (DeviceList.find(i) != DeviceList.end())
		{
			DeviceMapObject = DeviceList[i];
			result = 1;
		}
		break;
	}
	case Device_Clone_ALL: // Search
	{
		DeviceListClone.clear();
		DeviceListClone = DeviceList;
		result = 1;
		break;
	}
	}

	return result;
}

void SConnection::InitiateDeviceStatus(ns__Device& DeviceObject) const
{
	double voltage[3] = { 0,0,0 };
	double ampere[3] = { 0,0,0 };
	double cosQ[3] = { 0,0,0 };

	std::vector<double> ampereSensor(4);

	DBConnection dbObject;
	std::string calibrationData;
	dbObject.GetDeviceCalibration(DeviceObject, calibrationData);
	ns__CalibrationList calibrationObject = this->_toolsObject.GetCalibrationDataFromXML(calibrationData);

	try
	{
		for (int i = 0; i < DeviceObject.dSensors.size(); i++)
		{
			if (DeviceObject.dSensors[i].seName == ACVOLTAGE || DeviceObject.dSensors[i].seName == ACAMPERE)
			{
				ns__Calibration calibrationTemp;
				std::vector<ns__Calibration> calibrationListTemp;
				if (DeviceObject.dSensors[i].seName == ACVOLTAGE)
				{
					calibrationListTemp = calibrationObject.clVoltage;
					for (size_t index = calibrationListTemp.size(); index < 36; index++)
					{
						calibrationTemp.cOffset = 0;
						calibrationTemp.cZero = 0;
						calibrationTemp.cMin = 0;
						calibrationTemp.cSpan = 320;
						calibrationTemp.cMax = 240;
						calibrationListTemp.push_back(calibrationTemp);
					}
				}
				else if (DeviceObject.dSensors[i].seName == ACAMPERE)
				{
					calibrationListTemp = calibrationObject.clAmpere;
					for (size_t index = calibrationListTemp.size(); index < 36; index++)
					{
						calibrationTemp.cOffset = 0;
						calibrationTemp.cZero = 20;
						calibrationTemp.cMin = 0;
						calibrationTemp.cSpan = 200;
						calibrationTemp.cMax = 250;
						calibrationListTemp.push_back(calibrationTemp);
					}
				}
				int len = DeviceObject.dSensors[i].seVal.size();
				if (len > 32)
				{
					len = 32;
				}
				for (int j = 0; j < len; j++)
				{
					if (DeviceObject.dSensors[i].seVal[j] >= 0)
					{
						int index = j;
						if (DeviceObject.dSensors[i].seName == ACAMPERE && DeviceObject.dModel == MANAGER)
						{
							index = j + 4;
						}
						DeviceObject.dSensors[i].seVal[j] = this->CalculateCalibratedValue(DeviceObject.dSensors[i].seVal[j], calibrationListTemp[index].cMin, calibrationListTemp[index].cMax, calibrationListTemp[index].cZero, calibrationListTemp[index].cSpan);

						if (j < 3)
						{
							if (DeviceObject.dSensors[i].seName == ACVOLTAGE)
							{
								voltage[j] = DeviceObject.dSensors[i].seVal[j];
							}
							else if (DeviceObject.dSensors[i].seName == ACAMPERE)
							{
								ampere[j] = DeviceObject.dSensors[i].seVal[j];
							}
						}
						if (DeviceObject.dSensors[i].seName == ACAMPERE && DeviceObject.dModel == MANAGER)
						{
							ampereSensor[j % 4] += DeviceObject.dSensors[i].seVal[j];
						}
					}
				}
				if (DeviceObject.dSensors[i].seName == ACAMPERE && DeviceObject.dModel == MANAGER)
				{
					DeviceObject.dSensors[i].seVal.insert(DeviceObject.dSensors[i].seVal.begin(), ampereSensor.begin(), ampereSensor.end());
				}
			}
			else if (DeviceObject.dSensors[i].seName == COSQ)
			{
				ns__Calibration calibrationTemp;
				std::vector<ns__Calibration> calibrationListTemp = calibrationObject.clCosq;
				for (size_t index = calibrationListTemp.size(); index < 3; index++)
				{
					calibrationTemp.cOffset = 0;
					calibrationTemp.cZero = 0;
					calibrationTemp.cMin = 0;
					calibrationTemp.cSpan = 1;
					calibrationTemp.cMax = 1;
					calibrationListTemp.push_back(calibrationTemp);
				}
				int len = DeviceObject.dSensors[i].seVal.size();
				if (len > 3)
				{
					len = 3;
				}
				for (int j = 0; j < len; j++)
				{
					DeviceObject.dSensors[i].seVal[j] = DeviceObject.dSensors[i].seVal[j] - calibrationListTemp[j].cOffset;
					if (DeviceObject.dSensors[i].seVal[j] > 0 && DeviceObject.dSensors[i].seVal[j] <= 320)
					{
						DeviceObject.dSensors[i].seVal[j] = COS(DeviceObject.dSensors[i].seVal[j] * 360 / 640);
					}
					else
					{
						DeviceObject.dSensors[i].seVal[j] = 0;
					}
					cosQ[j] = DeviceObject.dSensors[i].seVal[j];
				}
			}
		}

		DeviceObject.dPR = (voltage[0] * ampere[0] * cosQ[0] / 1.72);
		DeviceObject.dPS = (voltage[1] * ampere[1] * cosQ[1] / 1.72);
		DeviceObject.dPT = (voltage[2] * ampere[2] * cosQ[2] / 1.72);

		DeviceObject.dQR = (voltage[0] * ampere[0] * sin(acos(cosQ[0])) / 1.72);
		DeviceObject.dQS = (voltage[1] * ampere[1] * sin(acos(cosQ[1])) / 1.72);
		DeviceObject.dQT = (voltage[2] * ampere[2] * sin(acos(cosQ[2])) / 1.72);

		if (DeviceObject.dModel == SECTIONNER && DeviceObject.dSensors.size() > 0)
		{
			ns__SensorExtension sensorEx;
			ns__Sensor sensor;
			sensorEx.seName = DIGITALEXIST;
			sensor.sVal = DeviceObject.dPR + DeviceObject.dPS + DeviceObject.dPT;
			sensorEx.seVal.push_back(sensor.sVal);
			sensor.sVal = DeviceObject.dPR;
			sensorEx.seVal.push_back(sensor.sVal);
			sensor.sVal = DeviceObject.dPS;
			sensorEx.seVal.push_back(sensor.sVal);
			sensor.sVal = DeviceObject.dPT;
			sensorEx.seVal.push_back(sensor.sVal);
			DeviceObject.dSensors.push_back(sensorEx);

			sensorEx.seVal.clear();
			sensorEx.seName = DIGITALOUTPUT;
			sensor.sVal = DeviceObject.dQR + DeviceObject.dQS + DeviceObject.dQT;
			sensorEx.seVal.push_back(sensor.sVal);
			sensor.sVal = DeviceObject.dQR;
			sensorEx.seVal.push_back(sensor.sVal);
			sensor.sVal = DeviceObject.dQS;
			sensorEx.seVal.push_back(sensor.sVal);
			sensor.sVal = DeviceObject.dQT;
			sensorEx.seVal.push_back(sensor.sVal);
			DeviceObject.dSensors.push_back(sensorEx);
		}
	}
	catch (const std::exception&)
	{

	}
}

double SConnection::CalculateCalibratedValue(double& SensorValue, double MinimumValue, double MaximumValue, double ZeroValue, double SpanValue)
{
	double calibratedValue = 0;
	try
	{
		double gradient = 0;
		if (double(SpanValue - ZeroValue) != 0)
		{
			gradient = (double(MaximumValue - MinimumValue)) / (double(SpanValue - ZeroValue));
		}
		calibratedValue = (gradient * (SensorValue - ZeroValue)) + MinimumValue;
		if (calibratedValue < 0)
		{
			calibratedValue = 0;
		}
	}
	catch (const std::exception&)
	{

	}
	return calibratedValue;
}

//************************************
// Method:    GenerateCalibrationData
// FullName:  SConnection::GenerateCalibrationData
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: ns__Device DeviceObject
//************************************
bool SConnection::GenerateCalibrationData(ns__Device DeviceObject, ns__CalibrationList& calibrationList)
{
	// for all sensors generate
	// save to database

	ns__Calibration calibrationTemp;
	for (size_t i = 0; i < 32; i++)
	{
		calibrationTemp.cOffset = 0;
		calibrationTemp.cMin = 0;
		calibrationTemp.cZero = 0;
		calibrationTemp.cMax = 240;
		calibrationTemp.cSpan = 320;

		calibrationList.clVoltage.push_back(calibrationTemp);
	}
	for (size_t i = 0; i < 32; i++)
	{
		calibrationTemp.cOffset = 0;
		calibrationTemp.cMin = 0;
		calibrationTemp.cZero = 20;
		calibrationTemp.cMax = 35;
		calibrationTemp.cSpan = 350;

		calibrationList.clAmpere.push_back(calibrationTemp);
	}
	for (size_t i = 0; i < 3; i++)
	{
		calibrationTemp.cOffset = 0;
		calibrationTemp.cMin = 0;
		calibrationTemp.cZero = 20;
		calibrationTemp.cMax = 35;
		calibrationTemp.cSpan = 350;

		calibrationList.clCosq.push_back(calibrationTemp);
	}
	return false;
}

int SConnection::SaveDevieStatus(ns__Device& DeviceObject, std::string PureData) const
{
	std::string strStr = this->_toolsObject.GetDeviceDataFromSOAP(DeviceObject);
	DBConnection dbObject;
	return dbObject.SaveDeviceStatus(DeviceObject, strStr, PureData);

	/*DBConnection dbObject;
	return dbObject.SaveDeviceStatus(DeviceObject, DeviceStatusString);*/
}

int SConnection::SetSampling(ns__Device& DeviceObject)
{
	/*std::string strCommand = REQUEST_SET_STRING;
	int replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
	replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(DeviceObject.dSamplingTime), 0);

	////====////std::cout << strCommand << std::endl;
	strCommand = this->ApplyCommand(DeviceObject, strCommand);
	////====////std::cout << "CNF Result is: " << strCommand << std::endl;

	// Split received data (device serial number and result with > seperator)
	this->RemoveUnusedChars(strCommand);
	Splitter sp(strCommand, "#");
	sp.reset(sp[0], ",");

	// Three section is OK
	if(sp.size() == 3)
	{
		if(this->_toolsObject.toInteger(sp[0]) == DeviceObject.dSerialNumber && this->_toolsObject.toUpper(sp[1]) == "SET")
		{
			if(this->_toolsObject.is_number(sp[2]))
			{
				DeviceObject.dSamplingTime = this->_toolsObject.toInteger(sp[2]);
				return 1;
			}
			else
			{
				DeviceObject.dErr.eMsg = "Error";
				DeviceObject.dErr.eNo = 1;
			}
		}
		else
		{
			DeviceObject.dErr.eMsg = "Error";
			DeviceObject.dErr.eNo = 1;
		}
	}
	else
	{
		DeviceObject.dErr.eMsg = "Error";
		DeviceObject.dErr.eNo = 1;
	}*/

	return 0;
}

int SConnection::GetReport(ns__Device & DeviceObject, std::string ReportData) const
{
	DeviceObject = this->_toolsObject.GetDeviceDataFromXML(ReportData);
	return 1;
}

bool SConnection::CheckSensors(ns__Device& DeviceObject, std::string Input, ns__SensorName SensorName, std::string SensorNikeName /*= ""*/) const
{
	ns__Sensor Sensor;
	if (this->_toolsObject.is_double(Input))
	{
		Sensor.sNkName = SensorNikeName;
		Sensor.sVal = this->_toolsObject.toInteger(Input);
		Sensor.sType = Multi;

		// Add sensor to list
		this->AddToSensorsList(DeviceObject, Sensor, SensorName);

		return true;
	}
	if (Input == "-")
	{
		Sensor.sNkName = SensorNikeName;
		Sensor.sVal = -100;
		Sensor.sType = Multi;

		// Add sensor to list
		this->AddToSensorsList(DeviceObject, Sensor, SensorName);

		return true;
	}
	return false;
}

bool SConnection::AddToSensorsList(ns__Device& DeviceObject, ns__Sensor Sensor, ns__SensorName SensorName)
{
	for (int i = 0; i < DeviceObject.dSensors.size(); i++)
	{
		if (SensorName == DeviceObject.dSensors[i].seName)
		{
			DeviceObject.dSensors[i].seVal.push_back(Sensor.sVal);
			return true;
		}
	}
	ns__SensorExtension sensorExtensionTemp;
	sensorExtensionTemp.seVal.clear();
	sensorExtensionTemp.seName = SensorName;
	sensorExtensionTemp.seVal.push_back(Sensor.sVal);
	DeviceObject.dSensors.push_back(sensorExtensionTemp);

	return false;
}

int SConnection::CreateSocket2()
{
	WSADATA wsaData;

	struct addrinfo *result = nullptr;
	struct addrinfo hints;

	// Initialize WinSock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		//printf("WSAStartup failed with Error :  %d\n", iResult);
		return 0;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(nullptr, DEFAULT_PORT2, &hints, &result);
	if (iResult != 0)
	{
		//printf("getaddrinfo failed with Error :  %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Create a SOCKET for connecting to server
	this->_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->_listenSocket == INVALID_SOCKET)
	{
		//printf("socket failed with Error :  %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	// Setup the TCP listening socket
	iResult = bind(this->_listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR)
	{
		//printf("bind failed with Error :  %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	iResult = listen(this->_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	// Set time out
	int nTimeout = CONNECTIONTIMEOUT;
	iResult = setsockopt(this->_listenSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&nTimeout), sizeof(int));
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	iResult = setsockopt(this->_listenSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&nTimeout), sizeof(int));
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	int buffsize = 1024 * 1024;
	iResult = setsockopt(this->_listenSocket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&buffsize), sizeof(buffsize));
	if (iResult == SOCKET_ERROR)
	{
		//printf("listen failed with Error :  %d\n", WSAGetLastError());
		closesocket(this->_listenSocket);
		WSACleanup();
		return 0;
	}

	/*//std::cout << " On " << result->ai_addr << " : " << DEFAULT_PORT << std::endl;*/

	freeaddrinfo(result);

	return 1;
}

int SConnection::ThreadStartListen2()
{
	// Create thread
	std::thread threadObject(&SConnection::StartListen2, *this);

	// Start thread and continue
	threadObject.detach();

	return 1;
}

int SConnection::SetDateTime(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapObject;
	deviceMapObject._device = DeviceObject;
	deviceMapObject._isCamera = false;

	std::string strCommand = REQUEST_TIME_STRING;
	int replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);
	this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.Time_tToString(DeviceObject.dDateTime, "%y-%m-%d %H:%M:%S"), replaceIndex);

	////====////std::cout << strCommand << std::endl;

	deviceMapObject._requestMap[deviceMapObject._isCamera]._command = strCommand;
	strCommand = this->ApplyCommand(deviceMapObject);
	////====////std::cout << "CNF Result is: " << strCommand << std::endl;


	// Split received data (device serial number and result with > seperator)
	this->RemoveUnusedChars(strCommand);
	Splitter sp(strCommand, "#");
	sp.reset(sp[0], ",");

	// Two section is OK
	if (sp.size() == 3 && this->_toolsObject.toInteger(sp[0]) == DeviceObject.dSerialNumber && this->_toolsObject.toUpper(sp[1]) == "DAT")
	{
		SyntaxChecker parserObject;
		if (parserObject.CheckDateTime(sp[2]))
		{
			DeviceObject.dDateTime = parserObject.DeviceInfo().dDateTime;
			return 1;
		}
		else
		{
			DeviceObject.dErr.eMsg = "Error";
			DeviceObject.dErr.eNo = 1;
		}
	}
	else
	{
		DeviceObject.dErr.eMsg = "Error";
		DeviceObject.dErr.eNo = 1;
	}

	return 0;
}

int SConnection::SetRelay(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapObject;
	deviceMapObject._device = DeviceObject;
	deviceMapObject._isCamera = false;

	ConverterDecimal converter;
	std::string strCommand = REQUEST_OUT_STRING;
	int replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);

	size_t relaysCount = DeviceObject.dRelays.size();
	floor(DeviceObject.dRelays.size() / 8);
	for (int i = 0; i < 4; i++)
	{
		std::vector<bool> parts;
		for (int j = 0; j < 8; j++)
		{
			if (i * 8 + j < relaysCount)
			{
				parts.push_back(DeviceObject.dRelays[i * 8 + j]);
			}
			else
			{
				parts.push_back(false);
			}
		}
		replaceIndex = this->_toolsObject.replaceOne(strCommand, "[%s]", this->_toolsObject.toString(converter.Binary_Decimal_Array(parts)), replaceIndex);
	}

	////====////std::cout << strCommand << std::endl;
	//LogData(DeviceObject, strCommand);

	if (DeviceObject.dSms == true)
	{
		DBConnection dbObject;
		DeviceObject.dCustomCommand = strCommand;
		return dbObject.SetDeviceSettingSms(DeviceObject);
	}

	deviceMapObject._requestMap[deviceMapObject._isCamera]._command = strCommand;
	strCommand = this->ApplyCommand(deviceMapObject);
	////====////std::cout << "CNF Result is: " << strCommand << std::endl;
	//LogData(DeviceObject, "CNF Result is: " + strCommand);

	// Split received data (device serial number and result with > seperator)
	this->RemoveUnusedChars(strCommand);
	Splitter sp(strCommand, "#");
	sp.reset(sp[0], ",");

	SyntaxChecker parserObject;
	// Two section is OK
	int tryCounter = 0;
	if (this->ParsDeviceData(strCommand, parserObject, deviceMapObject, tryCounter))
	{
		DeviceObject.dSensors = parserObject.DeviceInfo().dSensors;
		return 1;
	}
	/*if (sp.size() == 6 && this->_toolsObject.toInteger(sp[0]) == DeviceObject.dSerialNumber && this->_toolsObject.toUpper(sp[1]) == "OUT")
	{
		for (size_t i = 2; i < 6; i++)
		{
			if (parserObject.CheckSensorsBit(sp[i], ns__SensorName::RELAY, 8))
			{
				DeviceObject.dSensors = parserObject.DeviceInfo().dSensors;
				return 1;
			}
			else
			{
				DeviceObject.dErr.eMsg = "Error";
				DeviceObject.dErr.eNo = 1;
			}
		}
	}
	else
	{
		DeviceObject.dErr.eMsg = "Error";
		DeviceObject.dErr.eNo = 1;
	}*/

	return 0;
}

int SConnection::SetReset(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapObject;
	deviceMapObject._device = DeviceObject;
	deviceMapObject._isCamera = false;

	std::string strCommand = REQUEST_RESET_STRING;
	this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);

	////====////std::cout << strCommand << std::endl;
	deviceMapObject._requestMap[deviceMapObject._isCamera]._command = strCommand;

	strCommand = this->ApplyCommand(deviceMapObject);

	////====////std::cout << "CNF Result is: " << strCommand << std::endl;

	// Split received data (device serial number and result with > seperator)
	this->RemoveUnusedChars(strCommand);
	Splitter sp(strCommand, "#");
	sp.reset(sp[0], ",");

	// Two section is OK
	if (sp.size() == 2 && this->_toolsObject.toInteger(sp[0]) == DeviceObject.dSerialNumber && this->_toolsObject.toUpper(sp[1]) == "RST")
	{
		return 1;
	}
	else
	{
		DeviceObject.dErr.eMsg = "Error";
		DeviceObject.dErr.eNo = 1;
	}

	return 0;
}

int SConnection::SetStt(ns__Device& DeviceObject) const
{
	DeviceMap deviceMapObject;
	deviceMapObject._device = DeviceObject;
	deviceMapObject._isCamera = false;

	std::string strCommand = SERVER_STT_STRING;
	this->_toolsObject.replaceOne(strCommand, "[%ss]", this->_toolsObject.toString(DeviceObject.dSerialNumber), 0);

	////====////std::cout << strCommand << std::endl;
	deviceMapObject._requestMap[deviceMapObject._isCamera]._command = strCommand;

	strCommand = this->ApplyCommand(deviceMapObject);

	////====////std::cout << "CNF Result is: " << strCommand << std::endl;

	// Split received data (device serial number and result with > seperator)
	this->RemoveUnusedChars(strCommand);
	Splitter sp(strCommand, "#");
	sp.reset(sp[0], ",");

	SyntaxChecker parserObject;
	// Two section is OK
	int tryCounter = 0;
	if (this->ParsDeviceData(strCommand, parserObject, deviceMapObject, tryCounter))
	{
		DeviceObject.dSensors = parserObject.DeviceInfo().dSensors;
		return 1;
	}

	// Two section is OK
	/*if (sp.size() == 2 && this->_toolsObject.toInteger(sp[0]) == DeviceObject.dSerialNumber && this->_toolsObject.toUpper(sp[1]) == "RST")
	{
		return 1;
	}
	else
	{
		DeviceObject.dErr.eMsg = "Error";
		DeviceObject.dErr.eNo = 1;
	}*/

	return 0;
}

void SConnection::RemoveUnusedChars(std::string& Input) const
{
	this->_toolsObject.replaceOne(Input, "\r\n", "", 0);
	this->_toolsObject.replaceOne(Input, "\n", "", 0);
	this->_toolsObject.replaceOne(Input, "\r", "", 0);
	Input.erase(remove(Input.begin(), Input.end(), '\r'), Input.end());
	Input.erase(remove(Input.begin(), Input.end(), '\n'), Input.end());
}

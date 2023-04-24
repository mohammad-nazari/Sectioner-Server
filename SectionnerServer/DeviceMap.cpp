#include "DeviceMap.h"

inline DeviceMap::DeviceMap(SOCKET newsock) : _isCamera(false), _isAlarm(false)
{
	RequestData req(newsock);

	this->_requestMap.insert(std::pair<bool, RequestData>(true, req));
	this->_requestMap.insert(std::pair<bool, RequestData>(false, req));
}

DeviceMap::~DeviceMap()
{
	try
	{
		this->_requestMap.clear();
	}
	catch (...)
	{

	}
}

RequestData::RequestData(SOCKET newsock)
{
	this->_command = "";
	this->_result = "";
	this->_hReadWriteCommand = new HANDLE;
	this->_hReadWriteCommand = CreateEvent(nullptr, true, false, TEXT("SetCommand"));
	this->_hReadWriteResult = new HANDLE;
	this->_hReadWriteResult = CreateEvent(nullptr, true, false, TEXT("SetResult"));
	this->_hIdleThread = new HANDLE;
	this->_hIdleThread = CreateEvent(nullptr, true, false, TEXT("SetResult"));
}

RequestData::~RequestData()
{
	if (this->_hReadWriteCommand != INVALID_HANDLE_VALUE && this->_hReadWriteCommand != nullptr)
	{
		//CloseHandle(this->_hReadWriteCommand);
	}
	if (this->_hReadWriteResult != INVALID_HANDLE_VALUE && this->_hReadWriteResult != nullptr)
	{
		//CloseHandle(this->_hReadWriteResult);
	}
	if (this->_hIdleThread != INVALID_HANDLE_VALUE && this->_hIdleThread != nullptr)
	{
		//CloseHandle(this->_hIdleThread);
	}
}

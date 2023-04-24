#pragma once
#ifndef _DEVICEMAP_H_
#define _DEVICEMAP_H_

#include "soapsectionnerService.h"
#include "Header.h"

class RequestData
{
public:
	RequestData(SOCKET newsock = 0);
	~RequestData();

	/*std::string Command() const
	{
		return this->_command;
	}
	void Command(std::string val)
	{
		this->_command = val;
	}
	std::string Result() const
	{
		return this->_result;
	}
	void Result(std::string val)
	{
		this->_result = val;
	}
	HANDLE HReadWriteCommand() const
	{
		return this->_hReadWriteCommand;
	}
	void HReadWriteCommand(HANDLE val)
	{
		this->_hReadWriteCommand = val;
	}
	HANDLE HReadWriteResult() const
	{
		return this->_hReadWriteResult;
	}
	void HReadWriteResult(HANDLE val)
	{
		this->_hReadWriteResult = val;
	}*/
	std::string _command;
	std::string _result;
	HANDLE _hReadWriteCommand;
	HANDLE _hReadWriteResult;
	HANDLE _hIdleThread;
};

class __declspec(dllexport)DeviceMap
{
public:
	DeviceMap(SOCKET newsock = 0);
	~DeviceMap();

	std::map<bool, RequestData> _requestMap;
	ns__Device _device;
	CRITICAL_SECTION _csDeviceList;
	bool _isCamera;
	bool _isAlarm;
};
#endif

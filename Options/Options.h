#pragma once
#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include "OptionsH.h"
#include "../SectionnerServer/Header.h"

class __declspec(dllexport)Options
{
public:
	Options(void);
	~Options(void);

	bool ReadOptions(ns__ServerOptions& ServerOptions);
	bool SaveOptions(ns__ServerOptions& ServerOptions);

	void DestroySoap(struct soap* pSoap);
private:
	ns__ServerOptions serverOptions;
	std::string OptionsFileName;
	CRITICAL_SECTION optionsCriticalSection;
};
#endif // End _OPTIONS_H_

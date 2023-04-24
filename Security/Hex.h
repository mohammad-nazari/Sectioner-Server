#pragma once
#ifndef _HEX_H
#define _HEX_H
#include <iostream>

class __declspec(dllexport)Hex
{
public:
	Hex();
	~Hex();
	static std::string HexToString(std::string Input);
};

#endif // !1


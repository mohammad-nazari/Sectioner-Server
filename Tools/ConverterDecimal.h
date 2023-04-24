#pragma once
#ifndef _CONVERTERDECIMAL_H_
#define _CONVERTERDECIMAL_H_

#include "../SectionnerServer/Header.h"

class __declspec(dllexport)ConverterDecimal
{
public:
	ConverterDecimal();
	~ConverterDecimal();

	static long ToBinary(int number);
	static std::vector<bool> ToBinaryArray(int number, int Length);
	static double ToHex(int number);
	static double ToOctal(int number);
	static int Decimal_Binary(int n);
	int Binary_Decimal(int n) const;
	int Binary_Decimal_Array(std::vector<bool> ArrayList) const;
};
#endif


#include "Hex.h"



Hex::Hex()
{
}


Hex::~Hex()
{
}

std::string Hex::HexToString(std::string Input)
{
	int len = Input.length();
	std::string newString;
	for(int i = 0; i < len; i += 2)
	{
		std::string byte = Input.substr(i, 2);
		char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
		newString.push_back(chr);
	}

	return newString;
}

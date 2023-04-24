#pragma once

#ifndef _BASE64_H_
#define _BASE64_H_

#include "../SectionnerServer/Header.h"

class __declspec(dllexport)Base64
{
public:
	Base64();
	~Base64();

	std::string Encode(std::string Input);
	std::string Decode(std::string Input) const;
private:
	std::string base64_chars;
	static inline bool is_base64( unsigned char c );
	std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	std::string base64_decode(std::string const& encoded_string) const;
};

#endif // _BASE64_H_
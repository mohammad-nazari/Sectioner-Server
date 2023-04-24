#pragma once

#ifndef _SPLITTER_H_
#define _SPLITTER_H_
#include <vector>
#include <iostream>

//! Maintains a collection of substrings that are
//! delimited by a string of one or more characters
class __declspec(dllexport)Splitter {
private:
	//! Contains the split tokens
	std::vector<std::string> _tokens;
public:
	//! Subscript type for use with operator[]
	typedef std::vector<std::string>::size_type size_type;
public:

	Splitter(const std::string& src, const std::string& delim);

	std::string& operator[] (size_type i);

	size_type size() const;

	void reset(const std::string& src, const std::string& delim);
};

class __declspec(dllexport)WSplitter {
	//! Contains the split tokens
	std::vector<std::wstring> _wtokens;
public:
	//! Subscript type for use with operator[]
	typedef std::vector<std::wstring>::size_type wsize_type;
public:

	WSplitter(const std::wstring& src, const std::wstring& delim);

	std::wstring& operator[] (wsize_type i);

	wsize_type wsize() const;

	void wreset(const std::wstring& src, const std::wstring& delim);
};
#endif
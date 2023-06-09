
//  EnumString - A utility to provide stringizing support for C++ enums
//  Author: Francis Xavier Joseph Pulikotil
//
//  This code is free software: you can do whatever you want with it,
//	although I would appreciate if you gave credit where it's due.
//
//  This code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef ENUMSTRING_HEADER
#define ENUMSTRING_HEADER

#include <string>
#include <map>
#include <cassert>

// Helper macros

#define Begin_Enum_String(EnumerationName)                                      \
    template <> struct EnumString<EnumerationName> :                            \
        public EnumStringBase< EnumString<EnumerationName>, EnumerationName >   \
															    {                                                                           \
        static void RegisterEnumerators()
//      {

#define Enum_String(EnumeratorName)                                             \
            RegisterEnumerator( EnumeratorName, #EnumeratorName );
//      }

#define End_Enum_String                                                         \
															    }

// The EnumString base class
template <class DerivedType, class EnumType>
class EnumStringBase
{
	// Types
protected:
	typedef std::map<std::string, EnumType> AssocMap;

protected:
	// Constructor / Destructor
	explicit EnumStringBase();
	~EnumStringBase();

private:
	// Copy Constructor / Assignment Operator
	EnumStringBase(const EnumStringBase &);
	const EnumStringBase &operator =(const EnumStringBase &);

	// Functions
private:
	static AssocMap &GetMap();

protected:
	// Use this helper function to register each enumerator
	// and its string representation.
	static void RegisterEnumerator(const EnumType e, const std::string &eStr);

public:
	// Converts from an enumerator to a string.
	// Returns an empty string if the enumerator was not registered.
	static const std::string &From(const EnumType e);

	// Converts from a string to an enumerator.
	// Returns true if the conversion is successful; false otherwise.
	static const bool To(EnumType &e, const std::string &str);
};

// The EnumString class
// Note: Specialize this class for each enumeration, and implement
//       the RegisterEnumerators() function.
template <class EnumType>
struct EnumString: public EnumStringBase < EnumString<EnumType>, EnumType >
{
	static void RegisterEnumerators();
};

// Function definitions

template <class DerivedType, class EnumType>
typename EnumStringBase<DerivedType, EnumType>::AssocMap &EnumStringBase<DerivedType, EnumType>::GetMap()
{
	// A static map of associations from strings to enumerators
	static AssocMap assocMap;
	static bool     bFirstAccess = true;

	// If this is the first time we're accessing the map, then populate it.
	if(bFirstAccess)
	{
		bFirstAccess = false;
		DerivedType::RegisterEnumerators();
		assert(!assocMap.empty());
	}

	return assocMap;
}

template <class DerivedType, class EnumType>
void EnumStringBase<DerivedType, EnumType>::RegisterEnumerator(const EnumType e, const std::string &eStr)
{
	const bool bRegistered = GetMap().insert(typename AssocMap::value_type(eStr, e)).second;
	assert(bRegistered);
	(void)sizeof(bRegistered); // This is to avoid the pesky 'unused variable' warning in Release Builds.
}
template <class DerivedType, class EnumType>
const std::string & EnumStringBase<DerivedType, EnumType>::From(const EnumType e)
{
	for(;;) // Code block
	{
		// Search for the enumerator in our map
		typename AssocMap::const_iterator i;
		for(i = GetMap().begin(); i != GetMap().end(); ++i)
			if((*i).second == e)
				break;

		// If we didn't find it, we can't do this conversion
		if(i == GetMap().end())
			break;

		// Keep searching and see if we find another one with the same value
		typename AssocMap::const_iterator j(i);
		for(++j; j != GetMap().end(); ++j)
			if((*j).second == e)
				break;

		// If we found another one with the same value, we can't do this conversion
		if(j != GetMap().end())
			break;

		// We found exactly one string which matches the required enumerator
		return (*i).first;
	}

	// We couldn't do this conversion; return an empty string.
	static const std::string dummy;
	return dummy;
}

template <class DerivedType, class EnumType>
const bool EnumStringBase<DerivedType, EnumType>::To(EnumType &e, const std::string &str)
{
	// Search for the string in our map.
	const typename AssocMap::const_iterator itr(GetMap().find(str));

	// If we have it, then return the associated enumerator.
	if(itr != GetMap().end())
	{
		e = (*itr).second;
		return true;
	}

	// We don't have it; the conversion failed.
	return false;
}
#endif

#include "Tools.h"

Tools::Tools()
{

}


Tools::~Tools()
{
}


// trim from start
std::string Tools::VectorStringToString(std::string const &v)
{
	try {
		std::stringstream ss;
		for (size_t i = 0; i < v.size(); ++i)
		{
			ss << v[i];
		}
		return ss.str();
	}
	catch (const std::exception&)
	{
		return{};
	}
}

// trim from start
std::string Tools::ltrim(std::string const &str) const
{
	try
	{
		std::string s = str;
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

// trim from end
std::string Tools::rtrim(std::string const &str) const
{
	try
	{
		std::string s = str;
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

// trim from both ends
std::string Tools::trim(std::string const &str) const
{
	try
	{
		std::string s = str;
		return ltrim(rtrim(s));

	}
	catch (const std::exception&)
	{
		return{};
	}
}

std::vector<double> Tools::ConvertStringToBinaryList(unsigned int val)
{
	std::vector<double> listTemp;
	try
	{
		unsigned int mask = 1 << (sizeof(int) * 8 - 1);

		for (int i = 0; i < 8; i++)
		{
			if ((val & mask) == 0)
				listTemp.push_back(0);
			else
				listTemp.push_back(1);
		}

		return listTemp;
	}
	catch (const std::exception&)
	{
		return listTemp;
	}
}

void Tools::SetErrorData(ns__ErrorCode& ErrorCode, std::string ErrorMessage, int ErrorNumber, ns__SettingLevel ErrorType /*= ns__SettingLevel::Critical*/)
{
	ErrorCode.eMsg = ErrorMessage;
	ErrorCode.eNo = ErrorNumber;
	ErrorCode.eType = ErrorType;
}

ns__CalibrationList Tools::GetCalibrationDataFromXML(std::string CalibrationData) const
{
	ns__CalibrationList calibrationListTemp;
	// Create and initialize soap
	// gSOAP runtime context
	struct soap soap1;

	this->InitiateSoap(&soap1);

	//
	// Load XML (De serialize)
	//

	std::istringstream stringStreamValue(CalibrationData);
	soap1.is = &stringStreamValue;

	// calls soap_begin_recv, soap_get__gt__Library and soap_end_recv
	if (soap_read_ns__CalibrationList(&soap1, &calibrationListTemp) != SOAP_OK)
	{
		//std::cout << "soap_read failed" << std::endl;
		this->DestroySoap(&soap1);

		return calibrationListTemp;
	}

	this->DestroySoap(&soap1);

	return calibrationListTemp;
}

std::string Tools::GetCalibrationDataFromSOAP(ns__CalibrationList CalibrationList) const
{
	// Create and initialize soap
	// gSOAP runtime context
	struct soap soap1;

	this->InitiateSoap(&soap1);

	std::ostringstream stringStreamValue;
	soap1.os = &stringStreamValue;

	// calls soap_begin_send, soap_serialize, soap_put and soap_end_send
	if (soap_write_ns__CalibrationList(&soap1, &CalibrationList) != SOAP_OK)
	{
		//std::cout << "soap_write failed" << std::endl;
		this->DestroySoap(&soap1);

		return{};
	}

	this->DestroySoap(&soap1);
	std::string strStr = stringStreamValue.str();

	return strStr;
}

ns__Device Tools::GetDeviceDataFromXML(std::string DeviceData) const
{
	ns__Device deviceTemp;
	// Create and initialize soap
	// gSOAP runtime context
	struct soap soap1;

	this->InitiateSoap(&soap1);

	//
	// Load XML (De serialize)
	//

	std::istringstream stringStreamValue(DeviceData);
	soap1.is = &stringStreamValue;

	// calls soap_begin_recv, soap_get__gt__Library and soap_end_recv
	if (soap_read_ns__Device(&soap1, &deviceTemp) != SOAP_OK)
	{
		//std::cout << "soap_read failed" << std::endl;
		this->DestroySoap(&soap1);

		return deviceTemp;
	}

	this->DestroySoap(&soap1);

	return deviceTemp;
}

std::string Tools::GetDeviceDataFromSOAP(ns__Device DeviceObject) const
{
	// Create and initialize soap
	// gSOAP runtime context
	struct soap soap1;

	this->InitiateSoap(&soap1);

	std::ostringstream stringStreamValue;
	soap1.os = &stringStreamValue;

	// calls soap_begin_send, soap_serialize, soap_put and soap_end_send
	if (soap_write_ns__Device(&soap1, &DeviceObject) != SOAP_OK)
	{
		//ServerOptions << "soap_write failed" << std::endl;
		this->DestroySoap(&soap1);

		return{};
	}

	this->DestroySoap(&soap1);
	std::string strStr = stringStreamValue.str();
	return strStr;
}

void Tools::replaceAll(std::string& str, const std::string& oldStr, const std::string& newStr)
{
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != std::string::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

size_t Tools::replaceOne(std::string& str, const std::string& oldStr, const std::string& newStr, size_t position)
{
	position = str.find(oldStr, position);
	if (position != std::string::npos)
	{
		str.replace(position, oldStr.length(), newStr);
		position += newStr.length();
	}
	return position;
}

size_t Tools::findOne(std::string str, const std::string& findStr, size_t position)
{
	position = str.find(findStr, position);
	if (position != std::string::npos)
	{
		return true;
	}
	return false;
}

std::string Tools::toUpper(std::string& str)
{
	std::string result;

	replaceOne(str, "\r\n", "", 0);

	for (unsigned int i = 0; i < str.length(); ++i)
	{
		result += toupper(str.at(i));
	}

	return result;
}

std::string Tools::toLower(std::string& str)
{
	std::string result;

	replaceOne(str, "\r\n", "", 0);

	for (unsigned int i = 0; i < str.length(); ++i)
	{
		result += tolower(str.at(i));
	}

	return result;
}

//************************************
// Method:    is_number
// FullName:  Tools::is_number
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: const std::string & str
// First check string is in integer format
// else is in float format
// else return false
//************************************
bool Tools::is_number(const std::string& str) const
{
	try
	{
		std::string s = this->trim(str);

		if (s[0] == '-' || s[0] == '+')
		{
			s = s.substr(1);
		}
		std::string::const_iterator it = s.begin();
		while (it != s.end() && isdigit(*it)) ++it;

		if ((!s.empty() && it == s.end()) == false)
		{
			std::istringstream iss(str);
			float f;
			iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
								  // Check the entire string was consumed and if either failbit or badbit is set
			return (iss.eof() && !iss.fail());
		}
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Tools::is_number2(const std::string& str)
{
	try
	{
		if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) return false;

		char * p;
		strtol(str.c_str(), &p, 10);

		return (*p == 0);
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Tools::is_number3(const std::string& str)
{
	try
	{
		std::string::const_iterator it = str.begin();
		while (it != str.end() && isdigit(*it)) ++it;
		return !str.empty() && it == str.end();
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Tools::is_numberWithLen(const std::string& str, int lenght) const
{
	try
	{
		std::string s = this->trim(str);
		if (s.size() == lenght)
		{
			return is_number(str);
		}
		return false;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Tools::is_double(std::string const& str) const
{
	try
	{
		std::string s = this->trim(str);
		std::istringstream ss(s);
		double d;
		return (ss >> d) && (ss >> std::ws).eof();
	}
	catch (const std::exception&)
	{
		return false;
	}
}

int Tools::toInteger(std::string const& Input) const
{
	try
	{
		int tmp = std::stoi(this->trim(Input));

		return tmp;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

long long int Tools::toLongLong(std::string const& Input) const
{
	try
	{
		long long int tmp = std::stoll(this->trim(Input));

		return tmp;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

std::string Tools::toString(int Input)
{
	try
	{
		std::string str = std::to_string(Input);

		return str;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

std::string Tools::toString(int Input, int Length)
{
	try
	{
		std::stringstream ss;
		ss << std::setw(Length) << std::setfill('0') << Input;
		std::string str = ss.str();

		return str;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

std::string Tools::toString(long long int Input) const
{
	try
	{
		std::string str = std::to_string(Input);

		return str;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

std::string Tools::toString(long long int Input, int Length)
{
	try
	{
		std::stringstream ss;
		ss << std::setw(Length) << std::setfill('0') << Input;
		std::string str = ss.str();

		return str;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

std::string Tools::toStringDouble(double Input, int PrecisionLength)
{
	try
	{
		std::ostringstream out;
		out << std::setprecision(PrecisionLength) << Input;
		return out.str();
	}
	catch (const std::exception&)
	{
		return{};
	}
}

int Tools::getDigitLenth(long int InputNumber) const
{
	int nDigits = floor(log10(abs(InputNumber))) + 1;

	return nDigits;
}

bool Tools::checkFormat(std::string Input, std::string DataFormat) const
{
	std::regex regObject(DataFormat);
	bool result = regex_match(Input, regObject);

	return result;
}

bool Tools::isInString(std::string Input, std::string ParentString)
{
	bool result = ParentString.find(Input);

	return result;
}

/*
std::string Tools::SimCardTypeToString(ns__SimcardType SimCardType)
{
	return EnumString<ns__SimcardType>::From(SimCardType);
}

bool Tools::SimCardTypeFromString(ns__SimcardType &SimcardType, std::string SimCardTypeName)
{
	bool result = EnumString<ns__SimcardType>::To(SimcardType, SimCardTypeName);
	return result;
}*/

std::string Tools::UserTypeToString(ns__UserType UserType)
{
	try
	{
		return EnumString<ns__UserType>::From(UserType);
	}
	catch (const std::exception&)
	{
		return{};
	}
}

bool Tools::UserTypeFromString(ns__UserType &UserType, std::string UserTypeName)
{
	try
	{
		bool result = EnumString<ns__UserType>::To(UserType, UserTypeName);
		return result;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

std::string Tools::SensorTypeToString(ns__SensorType SensorType)
{
	try
	{
		return EnumString<ns__SensorType>::From(SensorType);
	}
	catch (const std::exception&)
	{
		return{};
	}
}

bool Tools::SensorTypeFromString(ns__SensorType &SensorType, std::string SensorTypeName)
{
	try
	{
		bool result = EnumString<ns__SensorType>::To(SensorType, SensorTypeName);
		return result;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

std::string Tools::SensorNameToString(ns__SensorName SensorName)
{
	try
	{
		return EnumString<ns__SensorName>::From(SensorName);
	}
	catch (const std::exception&)
	{
		return{};
	}
}

bool Tools::SensorNameFromString(ns__SensorName &SensorName, std::string SensorNameName)
{
	bool result = EnumString<ns__SensorName>::To(SensorName, SensorNameName);
	return result;
}

std::string Tools::RequestTypeToString(ns__RequestType RequestType)
{
	try
	{
		return EnumString<ns__RequestType>::From(RequestType);
	}
	catch (const std::exception&)
	{
		return{};
	}
}

bool Tools::RequestTypeFromString(ns__RequestType &RequestType, std::string RequestTypeName)
{
	bool result = EnumString<ns__RequestType>::To(RequestType, RequestTypeName);
	return result;
}

std::string Tools::DeviceModelToString(ns__DeviceModel DeviceModel)
{
	try
	{
		return EnumString<ns__DeviceModel>::From(DeviceModel);
	}
	catch (const std::exception&)
	{
		return{};
	}
}

bool Tools::DeviceModelFromString(ns__DeviceModel & DeviceModel, std::string DeviceModelName)
{
	bool result = EnumString<ns__DeviceModel>::To(DeviceModel, DeviceModelName);
	return result;
}

std::string Tools::getCurrentDateTime()
{
	try
	{
		time_t rawtime;
		char buffer[80];

		time(&rawtime);
		struct tm * timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
		std::string str(buffer);

		return str;
	}
	catch (const std::exception&)
	{
		return{};
	}
}

struct tm Tools::StringToTMStruct(std::string Input) const
{
	struct tm tmTemp;

	try
	{
		this->strptime(Input.c_str(), "%Y-%m-%d %H:%M:%S", &tmTemp);

		return tmTemp;
	}
	catch (const std::exception&)
	{
		return tmTemp;
	}
}

time_t Tools::StringToTime_t(std::string Input, std::string FormatDateTime /*= "%Y-%m-%d %H:%M:%S"*/) const
{
	struct tm tmTemp;
	this->strptime(Input.c_str(), FormatDateTime.c_str(), &tmTemp);
	time_t t = mktime(&tmTemp);

	return t-(60*60);
}

char * Tools::strptime(const char *buf, const char *fmt, struct tm *tm)
{
	try
	{
		tm->tm_sec = 0;		//< seconds (0 - 60)
		tm->tm_min = 0;		//< minutes (0 - 59)
		tm->tm_hour = 0;	//< hours (0 - 23)
		tm->tm_mday = 1;	//< day of month (1 - 31)
		tm->tm_mon = 1;		//< month of year (0 - 11)
		tm->tm_year = 1970;	//< year - 1900
		tm->tm_wday = 0;	//< day of week (Sunday = 0)
		tm->tm_yday = 0;	//< day of year (0 - 365)
		tm->tm_isdst = 0;	//< is summer time in effect?

		char c;
		size_t len = 0;
		int i, split_year = 0;

		const char *bp = buf;

		while ((c = *fmt) != '\0')
		{
			/* Clear `alternate' modifier prior to new conversion. */
			int alt_format = 0;

			/* Eat up white-space. */
			if (isspace(c))
			{
				while (isspace(*bp))
					bp++;

				fmt++;
				continue;
			}

			if ((c = *fmt++) != '%')
				goto literal;


		again:        switch (c = *fmt++)
		{
		case '%': /* "%%" is converted to "%". */
			literal :
				if (c != *bp++)
					return{};
			break;

			/*
			* "Alternative" modifiers. Just set the appropriate flag
			* and start over again.
			*/
		case 'E': /* "%E?" alternative conversion modifier. */
			LEGAL_ALT(0);
			alt_format |= ALT_E;
			goto again;

		case 'O': /* "%O?" alternative conversion modifier. */
			LEGAL_ALT(0);
			alt_format |= ALT_O;
			goto again;

			/*
			* "Complex" conversion rules, implemented through recursion.
			*/
		case 'c': /* Date and time, using the locale's format. */
			LEGAL_ALT(ALT_E);
			if (!(bp == strptime(bp, "%x %X", tm)))
				return{};
			break;

		case 'D': /* The date as "%m/%d/%y". */
			LEGAL_ALT(0);
			if (!(bp == strptime(bp, "%m/%d/%y", tm)))
				return{};
			break;

		case 'R': /* The time as "%H:%M". */
			LEGAL_ALT(0);
			if (!(bp == strptime(bp, "%H:%M", tm)))
				return{};
			break;

		case 'r': /* The time in 12-hour clock representation. */
			LEGAL_ALT(0);
			if (!(bp == strptime(bp, "%I:%M:%S %p", tm)))
				return{};
			break;

		case 'T': /* The time as "%H:%M:%S". */
			LEGAL_ALT(0);
			if (!(bp == strptime(bp, "%H:%M:%S", tm)))
				return{};
			break;

		case 'X': /* The time, using the locale's format. */
			LEGAL_ALT(ALT_E);
			if (!(bp == strptime(bp, "%H:%M:%S", tm)))
				return{};
			break;

		case 'x': /* The date, using the locale's format. */
			LEGAL_ALT(ALT_E);
			if (!(bp == strptime(bp, "%m/%d/%y", tm)))
				return{};
			break;

			/*
			* "Elementary" conversion rules.
			*/
		case 'A': /* The day of week, using the locale's form. */
		case 'a':
			LEGAL_ALT(0);
			for (i = 0; i < 7; i++)
			{
				/* Full name. */
				len = strlen(day[i]);
				if (strncasecmp(const_cast<char *>(day[i]), const_cast<char *>(bp), len) == 0)
					break;

				/* Abbreviated name. */
				len = strlen(abday[i]);
				if (strncasecmp(const_cast<char *>(abday[i]), const_cast<char *>(bp), len) == 0)
					break;
			}

			/* Nothing matched. */
			if (i == 7)
				return{};

			tm->tm_wday = i;
			bp += len;
			break;

		case 'B': /* The month, using the locale's form. */
		case 'b':
		case 'h':
			LEGAL_ALT(0);
			for (i = 0; i < 12; i++)
			{
				/* Full name. */

				len = strlen(mon[i]);
				if (strncasecmp(const_cast<char *>(mon[i]), const_cast<char *>(bp), len) == 0)
					break;

				/* Abbreviated name. */
				len = strlen(abmon[i]);
				if (strncasecmp(const_cast<char *>(abmon[i]), const_cast<char *>(bp), len) == 0)
					break;
			}

			/* Nothing matched. */
			if (i == 12)
				return{};

			tm->tm_mon = i;
			bp += len;
			break;

		case 'C': /* The century number. */
			LEGAL_ALT(ALT_E);
			if (!(conv_num(&bp, &i, 0, 99)))
				return{};

			if (split_year)
			{
				tm->tm_year = (tm->tm_year % 100) + (i * 100);
			}
			else
			{
				tm->tm_year = i * 100;
				split_year = 1;
			}
			break;

		case 'd': /* The day of month. */
		case 'e':
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_mday, 1, 31)))
				return{};
			break;

		case 'k': /* The hour (24-hour clock representation). */
			LEGAL_ALT(0);
			/* FALLTHROUGH */
		case 'H':
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_hour, 0, 23)))
				return{};
			break;

		case 'l': /* The hour (12-hour clock representation). */
			LEGAL_ALT(0);
			/* FALLTHROUGH */
		case 'I':
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_hour, 1, 12)))
				return{};
			if (tm->tm_hour == 12)
				tm->tm_hour = 0;
			break;

		case 'j': /* The day of year. */
			LEGAL_ALT(0);
			if (!(conv_num(&bp, &i, 1, 366)))
				return{};
			tm->tm_yday = i - 1;
			break;

		case 'M': /* The minute. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_min, 0, 59)))
				return{};
			break;

		case 'm': /* The month. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &i, 1, 12)))
				return{};
			tm->tm_mon = i - 1;
			break;

			//            case 'p': /* The locale's equivalent of AM/PM. */
			//                LEGAL_ALT(0);
			//                /* AM? */
			//                if (strcasecmp(am_pm[0], bp) == 0) 
			//                {
			//                    if (tm->tm_hour > 11)
			//                        return {};
			//
			//                    bp += strlen(am_pm[0]);
			//                    break;
			//                }
			//                /* PM? */
			//                else if (strcasecmp(am_pm[1], bp) == 0) 
			//                {
			//                    if (tm->tm_hour > 11)
			//                        return {};
			//
			//                    tm->tm_hour += 12;
			//                    bp += strlen(am_pm[1]);
			//                    break;
			//                }
			//
			//                /* Nothing matched. */
			//                return {};

		case 'S': /* The seconds. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_sec, 0, 61)))
				return{};
			break;

		case 'U': /* The week of year, beginning on sunday. */
		case 'W': /* The week of year, beginning on monday. */
			LEGAL_ALT(ALT_O);
			/*
			* XXX This is bogus, as we can not assume any valid
			* information present in the tm structure at this
			* point to calculate a real value, so just check the
			* range for now.
			*/
			if (!(conv_num(&bp, &i, 0, 53)))
				return{};
			break;

		case 'w': /* The day of week, beginning on sunday. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_wday, 0, 6)))
				return{};
			break;

		case 'Y': /* The year. */
			LEGAL_ALT(ALT_E);
			if (!(conv_num(&bp, &i, 0, 9999)))
				return{};

			tm->tm_year = i - 1900;
			break;

		case 'y': /* The year within 100 years of the epoch. */
			LEGAL_ALT(ALT_E | ALT_O);
			if (!(conv_num(&bp, &i, 0, 99)))
				return{};

			if (split_year)
			{
				tm->tm_year = ((tm->tm_year / 100) * 100) + i;
				break;
			}
			split_year = 1;
			if (i <= 68)
				tm->tm_year = i + 2000 - TM_YEAR_BASE;
			else
				tm->tm_year = i + 1900 - TM_YEAR_BASE;
			break;

			/*
			* Miscellaneous conversions.
			*/
		case 'n': /* Any kind of white-space. */
		case 't':
			LEGAL_ALT(0);
			while (isspace(*bp))
				bp++;
			break;


		default: /* Unknown/unsupported conversion. */
			return{};
		}


		}

		/* LINTED functional specification */
		return const_cast<char *>(bp);
	}
	catch (const std::exception&)
	{
		return "";
	}
}

std::string Tools::Time_tToString(time_t Input, std::string Format/*= "%Y-%m-%d %H:%M:%S"*/)
{
	try
	{
		char buffer[100];

		struct tm * timeinfo = localtime(&Input);

		if (strftime(buffer, 100, Format.c_str(), timeinfo))
		{
			return buffer;
		}
	}
	catch (const std::exception&)
	{
		return{};
	}
	return{};
}

std::string Tools::TMStructToString(struct tm Input, std::string Format /*= "%Y-%m-%d %H:%M:%S"*/)
{
	try
	{
		char buffer[100];

		if (strftime(buffer, 100, Format.c_str(), &Input))
		{
			return buffer;
		}
	}
	catch (const std::exception&)
	{
		return{};
	}
	return{};
}

int Tools::conv_num(const char **buf, int *dest, int llim, int ulim)
{
	try
	{
		int result = 0;

		/* The limit also determines the number of valid digits. */
		int rulim = ulim;

		if (**buf < '0' || **buf > '9')
			return (0);

		do
		{
			result *= 10;
			result += *(*buf)++ - '0';
			rulim /= 10;
		} while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');

		if (result < llim || result > ulim)
			return (0);

		*dest = result;
		return (1);
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

int Tools::strncasecmp(char *s1, char *s2, size_t n)
{
	try
	{
		if (n == 0)
			return 0;

		while (n-- != 0 && tolower(*s1) == tolower(*s2))
		{
			if (n == 0 || *s1 == '\0' || *s2 == '\0')
				break;
			s1++;
			s2++;
		}

		return tolower(*reinterpret_cast<unsigned char *>(s1)) - tolower(*reinterpret_cast<unsigned char *>(s2));
	}
	catch (const std::exception&)
	{
		return 0;
	}
}



void Tools::InitiateSoap(struct soap* pSoap)
{
	// initialize runtime context
	soap_init(pSoap);

	// Set input mode
	soap_imode(pSoap, SOAP_ENC_XML);

	// reset deserializers; start new (de)serialization phase
	soap_begin(pSoap);

	//
	// Serialize and Save to file
	//

	soap_set_omode(pSoap, SOAP_XML_INDENT);
}

//************************************
// Method:    DestroySoap
// FullName:  Options::DestroySoap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: struct soap * pSoap
//
// Description: Terminate and destroy soap
//************************************
void Tools::DestroySoap(struct soap* pSoap)
{
	// remove deserialized class instances (C++ objects) 
	soap_destroy(pSoap);

	// clean up and remove deserialized data 
	soap_end(pSoap);

	// detach context (last use and no longer in scope)
	soap_done(pSoap);
}
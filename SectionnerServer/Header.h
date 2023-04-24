#pragma once
#ifndef _HEADER_H_
#define	_HEADER_H_

#pragma warning(disable: 4146) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4624) // warning C4624: 'llvm::AugmentedUse' : destructor could not be generated because a base class destructor is inaccessible
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
#pragma warning(disable: 4800) // warning C4800: 'const unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4996) // warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_Sclan::SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#pragma warning(disable: 4244) // warning C4244: 'return' : conversion from 'uint64_t' to 'unsigned int', possible loss of data

#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <process.h>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <regex>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fstream>
#include <wchar.h>
#include <iomanip>
#include <thread>
#include <map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <exception>
#include <ctype.h>

typedef unsigned char uchar;

// Timer
#define CONNECTIONTIMEOUT	60000
#define SLEEPTIMEVALUE		1000
#define RECEIVETIME			60000
#define WAITCOMMANDTIME		60000
#define WAITINGTIMEOUT		60000
#define IDLETIME			3000
#define TRYCOUNTER			5

// Number of threads to alive
#define BACKLOG 1    // Max. request backlog

// SOAP timeout data
#define SEND_TIMEOUT		60
#define RECV_TIMEOUT		60
#define CONNECT_TIMEOUT		3
#define ACCEPT_TIMEOUT		30
#define TRANS_TIMEOUT		30

// Sensors 
#define SENSORSNAMEMULTI	"TEM>TEMPERATURE,HUM>HUMIDITY,ACV>ACVOLTAGE,ACA>ACAMPERE,DCV>DCVOLTAGE,DCA>DCAMPERE"
#define SENSORSNAMEMONO		"DGI>DIGITALINPUT,DGO>DIGITALOUTPUT,DGE>DIGITALEXIST,REL>RELAY"

#define DEFAULT_PORT	"50001"		/* Port to listen on */
#define DEFAULT_PORT2	"50000"		/* Port to listen on */

#define DATABASESERVER		"localhost"
#define DATABASEUSER		"bespo"
#define DATABASEPASSWORD	"GfwDq724CNSJxwUT"
#define DATABASENAME		"sectionnerdb"

#define MAX_DATA		4096	// Maximum bytes that could be sent or received
#define BASE64ENCODING	0		// Make base64 encoding
#define ENCRIPTIONDATA	0		// Make encrypt data
#define MAX_THR			100	// Max. threads to serve requests
#define SOAP_PORT		9090	// Port connect to server gsoap

// Database Query commands
#define SELECTDEVICEIP			"SELECT * FROM `device` WHERE `DevIP`='%s'"
#define SELECTDEVICEINFO		"SELECT * FROM `device` WHERE `DevSerialNumber` = '%s'"
#define SELECTDEVICEINFOALL		"SELECT * FROM `device`"
#define SELECTDEVICESERIAL		"SELECT * FROM `device` WHERE `DevSerialNumber`='%s'"
#define SELECTCITYNAME			"SELECT DISTINCT `DevCity` FROM `device`"
#define SELECTLOCATIONNAME		"SELECT DISTINCT `DevLocation` FROM `device` WHERE `DevCity`='%s'"

#define INSERTDEVICE			"INSERT INTO `device`(`DevSerialNumber`,`DevName`, `DevCity`, `DevLocation`, `DevSocketNumber`, `DevIP`, `DevPort`, `DevInstallDate`, `DevStatus`, `DevSimcardNumber`, `DevSimcardType`) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"

#define UPDATEDEVICESETTING		"UPDATE `device` SET `DevName`='%s',`DevCity`='%s',`DevLocation`='%s' WHERE `DevSerialNumber`='%s'"

#define UPDATECITYNAME			"UPDATE `device` SET `DevCity`='%s' WHERE `DevCity`='%s'"
#define UPDATELOCATIONNAME		"UPDATE `device` SET `DevLocation`='%s', `DevCity`='%s' WHERE `DevCity`='%s' and `DevLocation`='%s'"
#define UPDATEDEVICENAME		"UPDATE `device` SET `DevName`='%s', `DevLocation`='%s', `DevCity`='%s' WHERE `DevSerialNumber`='%s'"
#define SETDEVICESTATUSSMS		"UPDATE `device` SET `DevSmsReq`=%s WHERE `DevSerialNumber`='%s'"
#define SETDEVICESETTINGSMS		"UPDATE `device` SET `DevSmsOut`=%s,`DevSmsSet`='%s' WHERE `DevSerialNumber`='%s'"
#define REMOVEDEVICESMS			"UPDATE `device` SET `DevSmsData`= '' WHERE `DevSerialNumber`='%s'"

#define UPDATEDEVICE			"UPDATE `device` SET `DevSocketNumber`='%s',`DevIP`='%s',`DevPort`='%s',`DevStatus`='%s' WHERE `DevSerialNumber`='%s'"

#define UPDATEDEVICECLIBRATION	"UPDATE `device` SET `DevCalibration`='%s' WHERE `DevSerialNumber`='%s'"

#define SELECTUSER				"SELECT * FROM `users` WHERE `UserName`='%s' and `Password` = '%s'"
#define SELECTUSERWITHNAME		"SELECT * FROM `users` WHERE `UserName`='%s'"
#define SELECTALLUSERS			"SELECT * FROM `users`"
#define INSERTUSER				"INSERT INTO `users`(`UserName`, `Password`, `UserType`, `UserFirstName`, `UserLastName`) VALUES ('%s','%s','%s','%s','%s')"
#define UPDATEUSER				"UPDATE `users` SET `UserName`='%s',`Password`='%s',`UserType`='%s',`UserFirstName`='%s',`UserLastName`='%s'"
#define DELETEUSER				"DELETE FROM `users` WHERE `UserID`=%s"

#define INSERTUSERDEVICE		"INSERT INTO `userdevice`(`UserId`, `DeviceId`, `ViewAct`, `UpdateAct`, `DeleteAct`) VALUES (%s,%s,%s,%s,%s)"
#define SELECTUSERDEVICE		"SELECT * FROM `userdevice` WHERE `UserId` = %s and `DeviceId` = %s"
#define SELECTUSERDEVICELIST	"SELECT * FROM `userdevice` WHERE `UserId` = %s"
#define DELETEUSERDEVICE		"DELETE FROM `userdevice` WHERE `UserId`=%s and `DeviceId`=%s"

#define INSERTLOGSTATUS			"INSERT INTO `logstatus`(`DevSerialNumber`, `StatusData`, `LogDate`) VALUES ('%s','%s','%s')"
#define SELECTLOGSTATUS			"SELECT `LogID`, `DevSerialNumber`, `StatusData`, `LogDate` FROM `logstatus` WHERE `DevSerialNumber` = '%s' and `LogDate` BETWEEN '%s' and '%s'"

#define INSERTLOGPICTURE		"INSERT INTO `logpicture` (`DevSerialNumber`, `DevPicture`, `LogDate`) VALUES ('%s','%s','%s')"
#define SELECTLOGPICTURE		"SELECT `logpicture`.*, `device`.* FROM `logpicture` LEFT JOIN `device` ON `logpicture`.`DevSerialNumber` = `device`.`DevSerialNumber` WHERE `logpicture`.`LogDate` BETWEEN '%s' AND '%s' AND `device`.`DevSerialNumber` IN(%s)"

#define SELECTDEVICESTATUSLOG "SELECT `logstatus`.* FROM `logstatus` WHERE `logstatus`.`LogDate` BETWEEN '%s' AND '%s' AND `logstatus`.`DevSerialNumber` IN(%s)"
/*#define SELECTDEVICESTATUSLOG "SELECT `logstatus`.`DevSerialNumber`,`logstatus`.`StatusData`,`logstatus`.`LogDate` FROM `logstatus` WHERE `logstatus`.`LogDate` BETWEEN '%s' AND '%s' AND `logstatus`.`DevSerialNumber` IN(%s)"*/

#define SELECTDEVICESTATUSLOGALL "SELECT `logstatus`.*, `device`.* FROM `logstatus` LEFT JOIN `device` ON `logstatus`.`DevSerialNumber` = `device`.`DevSerialNumber` WHERE `logstatus`.`LogDate` BETWEEN '%s' AND '%s' AND `device`.`DevSerialNumber` IN(%s)"

// Patterns for regular expression
#define DATETIMEFORMAT "^(?ni:(?=\d)((?'year'((1[6-9])|([2-9]\d))\d\d)(?'sep'[/.-])(?'month'0?[1-9]|1[012])\2(?'day'((?<!(\2((0?[2469])|11)\2))31)|(?<!\2(0?2)\2)(29|30)|((?<=((1[6-9]|[2-9]\d)(0[48]|[2468][048]|[13579][26])|(16|[2468][048]|[3579][26])00)\2\3\2)29)|((0?[1-9])|(1\d)|(2[0-8])))(?:(?=\x20\d)\x20|$))?((?<time>((0?[1-9]|1[012])(:[0-5]\d){0,2}(\x20[AP]M))|([01]\d|2[0-3])(:[0-5]\d){1,2}))?)$"

// Request Errors
#define DEVICEPERMISSIONMESSAGE		"You dont have any permission to this device"
#define DEVICEPERMISSIONNUMBER		5100

#define DEVICEEXISTMESSAGE			"Unknown device"
#define DEVICEEXISTNUMBER			5110

#define USERAUTHENTICATIONMESSAGE	"Unknown user"
#define USERAUTHENTICATIONNUMBER	5120

#define DEVICECONNECTIONMESSAGE		"Error in connect to device"
#define DEVICECONNECTIONNUMBER		5130

#define UNKNOWNRESULTMESSAGE		"Unknown result received from device"
#define UNKNOWNRESULTNUMBER			5140

#define DATABASEERRORTMESSAGE		"Unknown result received from databse"
#define DATABASEERRORNUMBER			5150

#define NODEVICEFOUNDMESSAGE		"No device found for this user"
#define NODEVICEFOUNDNUMBER			5160

#define NOCONNECTEDMESSAGE			"No Connected"
#define NOCONNECTEDNUMBER			5170

#define NOCITYLOCATIONMESSAGE		"Could not get device city and location list"
#define NOCITYLOCATIONNUMBER		5180

#define UPDATECITYMESSAGE			"Could not update city name"
#define UPDATECITYNUMBER			5190

#define UPDATELOCATIONMESSAGE		"Could not update location name"
#define UPDATELOCATIONNUMBER		5200

#define UPDATEDEVICENAMEMESSAGE		"Could not update device name"
#define UPDATEDEVICENAMENUMBER		5210

#define NOITEMFOUNDMESSAGE			"No Item found"
#define NOITEMFOUNDNUMBER			5220

#define USERPERMISSIONMESSAGE		"You dont have any permission to do this operation"
#define USERPERMISSIONNUMBER		5230

#define NOUSERFOUNDMESSAGE			"No user found"
#define NOUSERFOUNDNUMBER			5240

#define NORECORDFOUNDMESSAGE		"No record found"
#define NORECORDFOUNDNUMBER			5250

#define DATETIMEMESSAGE				"Could not update device date and time"
#define DATETIMENUMBER				5260

#define RELAYMESSAGE				"Could not change device relays"
#define RELAYNUMBER					5270

#define RESETMESSAGE				"Could not reset device"
#define RESETNUMBER					5280

#define SETCALIBRATIONMESSAGE		"Could not set device calibration"
#define SETCALIBRATIONNUMBER		5290

#define GETCALIBRATIONMESSAGE		"Could not get device calibration"
#define GETCALIBRATIONNUMBER		5290



/*
* We do not implement alternate representations. However, we always
* check whether a given modifier is allowed for a certain conversion.
*/
#define ALT_E          0x01
#define ALT_O          0x02
//#define LEGAL_ALT(x)       { if (alt_format & ~(x)) return (0); }
#define LEGAL_ALT(x)       { ; }
#define TM_YEAR_BASE   (1970)

static const char *day[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
	"Friday", "Saturday"
};
static const char *abday[7] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
static const char *mon[12] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December"
};
static const char *abmon[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
static const char *am_pm[2] = {
	"AM", "PM"
};

enum Device_AddRemoveUpdateSearch
{
	Device_Add,
	Device_Update,
	Device_Delete,
	Device_Search,
	Device_Set_CNF,
	Device_Set_CNF_Result,
	Device_Get_CNF,
	Device_Get_CNF_Result,
	Device_Clone,
	Device_Clone_ALL
};

#define REQUEST_SET_STRING			"[%ss],@SMP,[%s],#\r\n"							//set sampling value
#define RESPONSE_SET_STRING			"[%ss],SMP,[%s]#\r\n"							//response

#define REQUEST_TIME_STRING			"[%ss],@DAT,[%s],#\r\n"							//set date and time value
#define RESPONSE_TIME_STRING		"[%ss],DAT,[%s]#\r\n"							//response

#define REQUEST_RESET_STRING		"[%ss],@RST,#\r\n"								//set date and time value
#define RESPONSE_RESET_STRING		"[%ss],RST#\r\n"								//response

#define REQUEST_OUT_STRING			"[%ss],@OUT,[%s],[%s],[%s],[%s],#\r\n"			//set relay status
#define RESPONSE_OUT_STRING			"[%ss],OUT,[%s],[%s],[%s],[%s]#\r\n"			//response

#define REQUEST_PICTURE_STRING		"[%ss],@PIC,#\r\n"								//request for take a picture sending to server
#define RESPONSE_PICTURE_STRING		"[%ss],PIC,[%s],[%s],[%s],[%s]#\r\n"			//sending picture size,part size, part numbers and picture CRC to server

#define REQUEST_PART_STRING			"[%ss],@PRT,[%s],#\r\n"							//request specific picture part from device sending to server
#define RESPONSE_PART_STRING		"[%ss],PRT,[%s],[%s],[%s]#\r\n"					//part received from device

#define REQUEST_ALARM_STRING		"[%ss],@ALM,[%s],#\r\n"							//set alarm 0 Off or 1 On
#define RESPONSE_ALARM_STRING		"[%ss],ALM,[%s]#\r\n"							//response

#define	SERVER_LIVE_OK_STRING		"[%ss],@LIVE,[%s],#\r\n"						//Server identify device by its IP
#define	DEVICE_LIVE_OK_STRING		"[%ss],LIVE,[%s],[%s],[%s],[%s]#\r\n"			//device send this packet to server and report

#define	SERVER_ALARM_STRING			"[%ss],@ALM,[%s],#\r\n"							//Server send alarm to device
#define	DEVICE_ALARM_STRING			"[%ss],ALM,[%s]#\r\n"							//device send this packet to server

#define	SERVER_NAME_STRING			"@NAM,#\r\n"									//in the first time server get device name
#define	DEVICE_NAME_STRING			"[%ss],[DEVM|DEVS|CAM],NAM#\r\n"				//device send its name to the server at the first time

#define	SERVER_STT_STRING			"[%ss],@STT,#\r\n"								//Server send to get device status
#define	DEVICE_SECTIONNER_STRING	"[%ss],DATS,[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]#\r\n"
#define	DEVICE_MANAGER_STRING		"[%ss],DATM,[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]#\r\n"

// JSON for default calibrations
#define DEFAULTJSON					"{\"SensorCalibration\" : [{\"Name\" : \"TEMPERATURE\",\"Zero\" : -50.0,\"Span\" : 150.0,\"Min\" : -50.0,\"Max\" : 150.0},{\"Name\" : \"HUMIDITY\",\"Zero\" : 0.0,\"Span\" : 100.0,\"Min\" : 0.0,\"Max\" : 100.0},{\"Name\" : \"ACVOLTAGE\",\"Zero\" : 0.0,\"Span\" : 230.0,\"Min\" : 0.0,\"Max\" : 230.0},{\"Name\" : \",\"Zero\" : 0.0,\"Span\" : 15.0,\"Min\" : 0.0,\"Max\" : 15.0},{\"Name\" : \"DCVOLTAGE\",\"Zero\" : 12.0,\"Span\" : 60.0,\"Min\" : 12.0,\"Max\" : 60.0},{\"Name\" : \",\"Zero\" : 0.0,\"Span\" : 20.0,\"Min\" : 0.0,\"Max\" : 20.0},{\"Name\" : \"COSQ\",\"Zero\" : 0.0,\"Span\" : 5.0,\"Min\" : 0.0,\"Max\" : 5.0}]}"

#define PI 3.14159265

#define SIN(param){sin (param * PI / 180)}
#define COS(param){cos (param * PI / 180)}
#define TAN(param){tan (param * PI / 180)}
#define CTAN(param){1 / tan (param * PI / 180)}

#endif	// _HEADER_H_


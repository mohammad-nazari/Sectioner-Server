/* OptionsStub.h
   Generated by gSOAP 2.8.32 for F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h

gSOAP XML Web services tools
Copyright (C) 2000-2016, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include <string.h>
#include <vector>
#include <vector>

#ifndef OptionsStub_H
#define OptionsStub_H
#include "stdsoap2.h"
#if GSOAP_VERSION != 20832
# error "GSOAP VERSION 20832 MISMATCH IN GENERATED CODE VERSUS LIBRARY CODE: PLEASE REINSTALL PACKAGE"
#endif


/******************************************************************************\
 *                                                                            *
 * Enumeration Types                                                          *
 *                                                                            *
\******************************************************************************/


/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:19 */
#ifndef SOAP_TYPE_ns__SensorName
#define SOAP_TYPE_ns__SensorName (20)
/* ns:SensorName */
enum ns__SensorName {
	GAS = 0,
	TEMPERATURE = 1,
	HUMIDITY = 2,
	H2S = 3,
	PRESSER = 4,
	ACVOLTAGE = 5,
	ACVOLTAGE1 = 6,
	ACVOLTAGE2 = 7,
	ACVOLTAGE3 = 8,
	ACAMPERE = 9,
	ACAMPERE1 = 10,
	ACAMPERE2 = 11,
	ACAMPERE3 = 12,
	DCVOLTAGE = 13,
	DCAMPERE = 14,
	UNKNOWN = 15
};
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes, Structs and Unions                                                *
 *                                                                            *
\******************************************************************************/

class ns__Sensor;	/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:38 */
class ns__DataBaseInfo;	/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:48 */
class ns__Messages;	/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:57 */
class ns__ServerOptions;	/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:65 */

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:38 */
#ifndef SOAP_TYPE_ns__Sensor
#define SOAP_TYPE_ns__Sensor (21)
/* complex XSD type 'ns:Sensor': */
class SOAP_CMAC ns__Sensor {
      public:
        /// Optional element 'sensorName' of XSD type 'ns:SensorName'
        enum ns__SensorName sensorName;	///< default = (enum ns__SensorName)1
        /// Optional element 'sensorCalibration' of XSD type 'xsd:int'
        int sensorCalibration;	///< default = 5
        /// Optional element 'sensorMinimumValue' of XSD type 'xsd:int'
        int sensorMinimumValue;	///< default = 0
        /// Optional element 'sensorMaximumValue' of XSD type 'xsd:int'
        int sensorMaximumValue;	///< default = 100
        /// Optional element 'sensorMaximumThreshold' of XSD type 'xsd:int'
        int sensorMaximumThreshold;	///< default = 10
        /// Optional element 'sensorMinimumThreshold' of XSD type 'xsd:int'
        int sensorMinimumThreshold;	///< default = 10
      public:
        /// Return unique type id SOAP_TYPE_ns__Sensor
        virtual int soap_type(void) const { return SOAP_TYPE_ns__Sensor; }
        /// (Re)set members to default values
        virtual void soap_default(struct soap*);
        /// Serialize object to prepare for SOAP 1.1/1.2 encoded output (or with SOAP_XML_GRAPH) by analyzing its (cyclic) structures
        virtual void soap_serialize(struct soap*) const;
        /// Output object in XML, compliant with SOAP 1.1 encoding style, return error code or SOAP_OK
        virtual int soap_put(struct soap*, const char *tag, const char *type) const;
        /// Output object in XML, with tag and optional id attribute and xsi:type, return error code or SOAP_OK
        virtual int soap_out(struct soap*, const char *tag, int id, const char *type) const;
        /// Get object from XML, compliant with SOAP 1.1 encoding style, return pointer to object or NULL on error
        virtual void *soap_get(struct soap*, const char *tag, const char *type);
        /// Get object from XML, with matching tag and type (NULL matches any tag and type), return pointer to object or NULL on error
        virtual void *soap_in(struct soap*, const char *tag, const char *type);
        /// Return a new object of type ns__Sensor, default initialized and not managed by a soap context
        virtual ns__Sensor *soap_alloc(void) const { return SOAP_NEW(ns__Sensor); }
      public:
        /// Constructor with initializations
        ns__Sensor()
        {
          sensorName = (enum ns__SensorName)1;
          sensorCalibration = 5;
          sensorMinimumValue = 0;
          sensorMaximumValue = 100;
          sensorMaximumThreshold = 10;
          sensorMinimumThreshold = 10;
        }
        virtual ~ns__Sensor() { }
        /// Friend allocator used by soap_new_ns__Sensor(struct soap*, int)
        friend SOAP_FMAC1 ns__Sensor * SOAP_FMAC2 Options_instantiate_ns__Sensor(struct soap*, int, const char*, const char*, size_t*);
};
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:48 */
#ifndef SOAP_TYPE_ns__DataBaseInfo
#define SOAP_TYPE_ns__DataBaseInfo (22)
/* complex XSD type 'ns:DataBaseInfo': */
class SOAP_CMAC ns__DataBaseInfo {
      public:
        /// Optional element 'dataBaseInfoServerName' of XSD type 'xsd:string'
        std::string dataBaseInfoServerName;	///< default = "localhost"
        /// Optional element 'dataBaseInfoPort' of XSD type 'xsd:int'
        int dataBaseInfoPort;	///< default = 3306
        /// Optional element 'dataBaseInfoUserName' of XSD type 'xsd:string'
        std::string dataBaseInfoUserName;	///< default = "root"
        /// Optional element 'dataBaseInfoPassWord' of XSD type 'xsd:string'
        std::string dataBaseInfoPassWord;	///< default = "123456"
        /// Optional element 'databaseDatabeseName' of XSD type 'xsd:string'
        std::string databaseDatabeseName;	///< default = "protectordb"
      public:
        /// Return unique type id SOAP_TYPE_ns__DataBaseInfo
        virtual int soap_type(void) const { return SOAP_TYPE_ns__DataBaseInfo; }
        /// (Re)set members to default values
        virtual void soap_default(struct soap*);
        /// Serialize object to prepare for SOAP 1.1/1.2 encoded output (or with SOAP_XML_GRAPH) by analyzing its (cyclic) structures
        virtual void soap_serialize(struct soap*) const;
        /// Output object in XML, compliant with SOAP 1.1 encoding style, return error code or SOAP_OK
        virtual int soap_put(struct soap*, const char *tag, const char *type) const;
        /// Output object in XML, with tag and optional id attribute and xsi:type, return error code or SOAP_OK
        virtual int soap_out(struct soap*, const char *tag, int id, const char *type) const;
        /// Get object from XML, compliant with SOAP 1.1 encoding style, return pointer to object or NULL on error
        virtual void *soap_get(struct soap*, const char *tag, const char *type);
        /// Get object from XML, with matching tag and type (NULL matches any tag and type), return pointer to object or NULL on error
        virtual void *soap_in(struct soap*, const char *tag, const char *type);
        /// Return a new object of type ns__DataBaseInfo, default initialized and not managed by a soap context
        virtual ns__DataBaseInfo *soap_alloc(void) const { return SOAP_NEW(ns__DataBaseInfo); }
      public:
        /// Constructor with initializations
        ns__DataBaseInfo()
        {
          dataBaseInfoServerName = "localhost";
          dataBaseInfoPort = 3306;
          dataBaseInfoUserName = "root";
          dataBaseInfoPassWord = "123456";
          databaseDatabeseName = "protectordb";
        }
        virtual ~ns__DataBaseInfo() { }
        /// Friend allocator used by soap_new_ns__DataBaseInfo(struct soap*, int)
        friend SOAP_FMAC1 ns__DataBaseInfo * SOAP_FMAC2 Options_instantiate_ns__DataBaseInfo(struct soap*, int, const char*, const char*, size_t*);
};
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:57 */
#ifndef SOAP_TYPE_ns__Messages
#define SOAP_TYPE_ns__Messages (23)
/* complex XSD type 'ns:Messages': */
class SOAP_CMAC ns__Messages {
      public:
        /// Required element 'messagesDescription' of XSD type 'xsd:string'
        std::string messagesDescription;
        /// Required element 'messagesString' of XSD type 'xsd:string'
        std::string messagesString;
        /// Required element 'messagesName' of XSD type 'xsd:string'
        std::string messagesName;
        /// Required element 'messagesCodeNumber' of XSD type 'xsd:int'
        int messagesCodeNumber;
      public:
        /// Return unique type id SOAP_TYPE_ns__Messages
        virtual int soap_type(void) const { return SOAP_TYPE_ns__Messages; }
        /// (Re)set members to default values
        virtual void soap_default(struct soap*);
        /// Serialize object to prepare for SOAP 1.1/1.2 encoded output (or with SOAP_XML_GRAPH) by analyzing its (cyclic) structures
        virtual void soap_serialize(struct soap*) const;
        /// Output object in XML, compliant with SOAP 1.1 encoding style, return error code or SOAP_OK
        virtual int soap_put(struct soap*, const char *tag, const char *type) const;
        /// Output object in XML, with tag and optional id attribute and xsi:type, return error code or SOAP_OK
        virtual int soap_out(struct soap*, const char *tag, int id, const char *type) const;
        /// Get object from XML, compliant with SOAP 1.1 encoding style, return pointer to object or NULL on error
        virtual void *soap_get(struct soap*, const char *tag, const char *type);
        /// Get object from XML, with matching tag and type (NULL matches any tag and type), return pointer to object or NULL on error
        virtual void *soap_in(struct soap*, const char *tag, const char *type);
        /// Return a new object of type ns__Messages, default initialized and not managed by a soap context
        virtual ns__Messages *soap_alloc(void) const { return SOAP_NEW(ns__Messages); }
      public:
        /// Constructor with initializations
        ns__Messages()
        {
          messagesCodeNumber = (int)0;
        }
        virtual ~ns__Messages() { }
        /// Friend allocator used by soap_new_ns__Messages(struct soap*, int)
        friend SOAP_FMAC1 ns__Messages * SOAP_FMAC2 Options_instantiate_ns__Messages(struct soap*, int, const char*, const char*, size_t*);
};
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:65 */
#ifndef SOAP_TYPE_ns__ServerOptions
#define SOAP_TYPE_ns__ServerOptions (24)
/* complex XSD type 'ns:ServerOptions': */
class SOAP_CMAC ns__ServerOptions {
      public:
        /// Required element 'optionsDataBaseInfo' of XSD type 'ns:DataBaseInfo'
        ns__DataBaseInfo optionsDataBaseInfo;
        /// Optional element 'optionsTimeRepeat' of XSD type 'xsd:int'
        int optionsTimeRepeat;	///< default = 3
        /// Optional element 'optionsSensorValues' of XSD type 'ns:Sensor'
        std::vector<ns__Sensor> optionsSensorValues;
        /// Optional element 'optionsMessages' of XSD type 'ns:Messages'
        std::vector<ns__Messages> optionsMessages;
      public:
        /// Return unique type id SOAP_TYPE_ns__ServerOptions
        virtual int soap_type(void) const { return SOAP_TYPE_ns__ServerOptions; }
        /// (Re)set members to default values
        virtual void soap_default(struct soap*);
        /// Serialize object to prepare for SOAP 1.1/1.2 encoded output (or with SOAP_XML_GRAPH) by analyzing its (cyclic) structures
        virtual void soap_serialize(struct soap*) const;
        /// Output object in XML, compliant with SOAP 1.1 encoding style, return error code or SOAP_OK
        virtual int soap_put(struct soap*, const char *tag, const char *type) const;
        /// Output object in XML, with tag and optional id attribute and xsi:type, return error code or SOAP_OK
        virtual int soap_out(struct soap*, const char *tag, int id, const char *type) const;
        /// Get object from XML, compliant with SOAP 1.1 encoding style, return pointer to object or NULL on error
        virtual void *soap_get(struct soap*, const char *tag, const char *type);
        /// Get object from XML, with matching tag and type (NULL matches any tag and type), return pointer to object or NULL on error
        virtual void *soap_in(struct soap*, const char *tag, const char *type);
        /// Return a new object of type ns__ServerOptions, default initialized and not managed by a soap context
        virtual ns__ServerOptions *soap_alloc(void) const { return SOAP_NEW(ns__ServerOptions); }
      public:
        /// Constructor with initializations
        ns__ServerOptions()
        {
          optionsTimeRepeat = 3;
        }
        virtual ~ns__ServerOptions() { }
        /// Friend allocator used by soap_new_ns__ServerOptions(struct soap*, int)
        friend SOAP_FMAC1 ns__ServerOptions * SOAP_FMAC2 Options_instantiate_ns__ServerOptions(struct soap*, int, const char*, const char*, size_t*);
};
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:73 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (27)
/* SOAP_ENV__Header: */
struct SOAP_ENV__Header {
      public:
        /** Return unique type id SOAP_TYPE_SOAP_ENV__Header */
        int soap_type() const { return SOAP_TYPE_SOAP_ENV__Header; }
        /** Constructor with member initializations */
        SOAP_ENV__Header()
        {
        }
        /** Friend allocator used by soap_new_SOAP_ENV__Header(struct soap*, int) */
        friend SOAP_FMAC1 SOAP_ENV__Header * SOAP_FMAC2 Options_instantiate_SOAP_ENV__Header(struct soap*, int, const char*, const char*, size_t*);
};
#endif
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:73 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (28)
/* Type SOAP_ENV__Code is a recursive data type, (in)directly referencing itself through its (base or derived class) members */
/* SOAP_ENV__Code: */
struct SOAP_ENV__Code {
      public:
        /** Optional element 'SOAP-ENV:Value' of XSD type 'xsd:QName' */
        char *SOAP_ENV__Value;
        /** Optional element 'SOAP-ENV:Subcode' of XSD type 'SOAP-ENV:Code' */
        struct SOAP_ENV__Code *SOAP_ENV__Subcode;
      public:
        /** Return unique type id SOAP_TYPE_SOAP_ENV__Code */
        int soap_type() const { return SOAP_TYPE_SOAP_ENV__Code; }
        /** Constructor with member initializations */
        SOAP_ENV__Code()
        {
          SOAP_ENV__Value = (char *)0;
          SOAP_ENV__Subcode = (struct SOAP_ENV__Code *)0;
        }
        /** Friend allocator used by soap_new_SOAP_ENV__Code(struct soap*, int) */
        friend SOAP_FMAC1 SOAP_ENV__Code * SOAP_FMAC2 Options_instantiate_SOAP_ENV__Code(struct soap*, int, const char*, const char*, size_t*);
};
#endif
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:73 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (30)
/* SOAP_ENV__Detail: */
struct SOAP_ENV__Detail {
      public:
        char *__any;
        /** Any type of element 'fault' assigned to fault with its SOAP_TYPE_T assigned to __type */
        /** Do not create a cyclic data structure throught this member unless SOAP encoding or SOAP_XML_GRAPH are used for id-ref serialization */
        int __type;
        void *fault;
      public:
        /** Return unique type id SOAP_TYPE_SOAP_ENV__Detail */
        int soap_type() const { return SOAP_TYPE_SOAP_ENV__Detail; }
        /** Constructor with member initializations */
        SOAP_ENV__Detail()
        {
          __any = (char *)0;
          __type = 0;
          fault = NULL;
        }
        /** Friend allocator used by soap_new_SOAP_ENV__Detail(struct soap*, int) */
        friend SOAP_FMAC1 SOAP_ENV__Detail * SOAP_FMAC2 Options_instantiate_SOAP_ENV__Detail(struct soap*, int, const char*, const char*, size_t*);
};
#endif
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:73 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (33)
/* SOAP_ENV__Reason: */
struct SOAP_ENV__Reason {
      public:
        /** Optional element 'SOAP-ENV:Text' of XSD type 'xsd:string' */
        char *SOAP_ENV__Text;
      public:
        /** Return unique type id SOAP_TYPE_SOAP_ENV__Reason */
        int soap_type() const { return SOAP_TYPE_SOAP_ENV__Reason; }
        /** Constructor with member initializations */
        SOAP_ENV__Reason()
        {
          SOAP_ENV__Text = (char *)0;
        }
        /** Friend allocator used by soap_new_SOAP_ENV__Reason(struct soap*, int) */
        friend SOAP_FMAC1 SOAP_ENV__Reason * SOAP_FMAC2 Options_instantiate_SOAP_ENV__Reason(struct soap*, int, const char*, const char*, size_t*);
};
#endif
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:73 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (34)
/* SOAP_ENV__Fault: */
struct SOAP_ENV__Fault {
      public:
        /** Optional element 'faultcode' of XSD type 'xsd:QName' */
        char *faultcode;
        /** Optional element 'faultstring' of XSD type 'xsd:string' */
        char *faultstring;
        /** Optional element 'faultactor' of XSD type 'xsd:string' */
        char *faultactor;
        /** Optional element 'detail' of XSD type 'SOAP-ENV:Detail' */
        struct SOAP_ENV__Detail *detail;
        /** Optional element 'SOAP-ENV:Code' of XSD type 'SOAP-ENV:Code' */
        struct SOAP_ENV__Code *SOAP_ENV__Code;
        /** Optional element 'SOAP-ENV:Reason' of XSD type 'SOAP-ENV:Reason' */
        struct SOAP_ENV__Reason *SOAP_ENV__Reason;
        /** Optional element 'SOAP-ENV:Node' of XSD type 'xsd:string' */
        char *SOAP_ENV__Node;
        /** Optional element 'SOAP-ENV:Role' of XSD type 'xsd:string' */
        char *SOAP_ENV__Role;
        /** Optional element 'SOAP-ENV:Detail' of XSD type 'SOAP-ENV:Detail' */
        struct SOAP_ENV__Detail *SOAP_ENV__Detail;
      public:
        /** Return unique type id SOAP_TYPE_SOAP_ENV__Fault */
        int soap_type() const { return SOAP_TYPE_SOAP_ENV__Fault; }
        /** Constructor with member initializations */
        SOAP_ENV__Fault()
        {
          faultcode = (char *)0;
          faultstring = (char *)0;
          faultactor = (char *)0;
          detail = (struct SOAP_ENV__Detail *)0;
          SOAP_ENV__Code = (struct SOAP_ENV__Code *)0;
          SOAP_ENV__Reason = (struct SOAP_ENV__Reason *)0;
          SOAP_ENV__Node = (char *)0;
          SOAP_ENV__Role = (char *)0;
          SOAP_ENV__Detail = (struct SOAP_ENV__Detail *)0;
        }
        /** Friend allocator used by soap_new_SOAP_ENV__Fault(struct soap*, int) */
        friend SOAP_FMAC1 SOAP_ENV__Fault * SOAP_FMAC2 Options_instantiate_SOAP_ENV__Fault(struct soap*, int, const char*, const char*, size_t*);
};
#endif
#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/


/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:7 */
#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:7 */
#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:11 */
#ifndef SOAP_TYPE_xsd__string
#define SOAP_TYPE_xsd__string (9)
typedef std::string xsd__string;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:12 */
#ifndef SOAP_TYPE_xsd__int
#define SOAP_TYPE_xsd__int (10)
typedef int xsd__int;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:13 */
#ifndef SOAP_TYPE_xsd__boolean
#define SOAP_TYPE_xsd__boolean (12)
typedef bool xsd__boolean;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:14 */
#ifndef SOAP_TYPE_xsd__double
#define SOAP_TYPE_xsd__double (14)
typedef double xsd__double;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:15 */
#ifndef SOAP_TYPE_xsd__dateTime
#define SOAP_TYPE_xsd__dateTime (16)
typedef time_t xsd__dateTime;
#endif

/* F:\Projects\WebService\GeneratedCode\Sectionner\source\DataBinding\source.h:16 */
#ifndef SOAP_TYPE_xsd__long
#define SOAP_TYPE_xsd__long (19)
typedef LONG64 xsd__long;
#endif

/******************************************************************************\
 *                                                                            *
 * Serializable Types                                                         *
 *                                                                            *
\******************************************************************************/


/* char has binding name 'byte' for type 'xsd:byte' */
#ifndef SOAP_TYPE_byte
#define SOAP_TYPE_byte (3)
#endif

/* xsd__int has binding name 'xsd__int' for type 'xsd:int' */
#ifndef SOAP_TYPE_xsd__int
#define SOAP_TYPE_xsd__int (10)
#endif

/* int has binding name 'int' for type 'xsd:int' */
#ifndef SOAP_TYPE_int
#define SOAP_TYPE_int (1)
#endif

/* long has binding name 'long' for type 'xsd:long' */
#ifndef SOAP_TYPE_long
#define SOAP_TYPE_long (17)
#endif

/* xsd__long has binding name 'xsd__long' for type 'xsd:long' */
#ifndef SOAP_TYPE_xsd__long
#define SOAP_TYPE_xsd__long (19)
#endif

/* LONG64 has binding name 'LONG64' for type 'xsd:long' */
#ifndef SOAP_TYPE_LONG64
#define SOAP_TYPE_LONG64 (18)
#endif

/* xsd__double has binding name 'xsd__double' for type 'xsd:double' */
#ifndef SOAP_TYPE_xsd__double
#define SOAP_TYPE_xsd__double (14)
#endif

/* double has binding name 'double' for type 'xsd:double' */
#ifndef SOAP_TYPE_double
#define SOAP_TYPE_double (13)
#endif

/* xsd__dateTime has binding name 'xsd__dateTime' for type 'xsd:dateTime' */
#ifndef SOAP_TYPE_xsd__dateTime
#define SOAP_TYPE_xsd__dateTime (16)
#endif

/* time_t has binding name 'dateTime' for type 'xsd:dateTime' */
#ifndef SOAP_TYPE_dateTime
#define SOAP_TYPE_dateTime (15)
#endif

/* enum ns__SensorName has binding name 'ns__SensorName' for type 'ns:SensorName' */
#ifndef SOAP_TYPE_ns__SensorName
#define SOAP_TYPE_ns__SensorName (20)
#endif

/* xsd__boolean has binding name 'xsd__boolean' for type 'xsd:boolean' */
#ifndef SOAP_TYPE_xsd__boolean
#define SOAP_TYPE_xsd__boolean (12)
#endif

/* bool has binding name 'bool' for type 'xsd:boolean' */
#ifndef SOAP_TYPE_bool
#define SOAP_TYPE_bool (11)
#endif

/* ns__ServerOptions has binding name 'ns__ServerOptions' for type 'ns:ServerOptions' */
#ifndef SOAP_TYPE_ns__ServerOptions
#define SOAP_TYPE_ns__ServerOptions (24)
#endif

/* ns__Messages has binding name 'ns__Messages' for type 'ns:Messages' */
#ifndef SOAP_TYPE_ns__Messages
#define SOAP_TYPE_ns__Messages (23)
#endif

/* ns__DataBaseInfo has binding name 'ns__DataBaseInfo' for type 'ns:DataBaseInfo' */
#ifndef SOAP_TYPE_ns__DataBaseInfo
#define SOAP_TYPE_ns__DataBaseInfo (22)
#endif

/* ns__Sensor has binding name 'ns__Sensor' for type 'ns:Sensor' */
#ifndef SOAP_TYPE_ns__Sensor
#define SOAP_TYPE_ns__Sensor (21)
#endif

/* xsd__string has binding name 'xsd__string' for type 'xsd:string' */
#ifndef SOAP_TYPE_xsd__string
#define SOAP_TYPE_xsd__string (9)
#endif

/* std::string has binding name 'std__string' for type 'xsd:string' */
#ifndef SOAP_TYPE_std__string
#define SOAP_TYPE_std__string (8)
#endif

/* struct SOAP_ENV__Fault has binding name 'SOAP_ENV__Fault' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (34)
#endif

/* struct SOAP_ENV__Reason has binding name 'SOAP_ENV__Reason' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (33)
#endif

/* struct SOAP_ENV__Detail has binding name 'SOAP_ENV__Detail' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (30)
#endif

/* struct SOAP_ENV__Code has binding name 'SOAP_ENV__Code' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (28)
#endif

/* struct SOAP_ENV__Header has binding name 'SOAP_ENV__Header' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (27)
#endif

/* struct SOAP_ENV__Reason * has binding name 'PointerToSOAP_ENV__Reason' for type '' */
#ifndef SOAP_TYPE_PointerToSOAP_ENV__Reason
#define SOAP_TYPE_PointerToSOAP_ENV__Reason (36)
#endif

/* struct SOAP_ENV__Detail * has binding name 'PointerToSOAP_ENV__Detail' for type '' */
#ifndef SOAP_TYPE_PointerToSOAP_ENV__Detail
#define SOAP_TYPE_PointerToSOAP_ENV__Detail (35)
#endif

/* struct SOAP_ENV__Code * has binding name 'PointerToSOAP_ENV__Code' for type '' */
#ifndef SOAP_TYPE_PointerToSOAP_ENV__Code
#define SOAP_TYPE_PointerToSOAP_ENV__Code (29)
#endif

/* _XML has binding name '_XML' for type '' */
#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
#endif

/* _QName has binding name '_QName' for type 'xsd:QName' */
#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
#endif

/* char * has binding name 'string' for type 'xsd:string' */
#ifndef SOAP_TYPE_string
#define SOAP_TYPE_string (4)
#endif

/* std::vector<ns__Messages>  has binding name 'std__vectorTemplateOfns__Messages' for type 'ns:Messages' */
#ifndef SOAP_TYPE_std__vectorTemplateOfns__Messages
#define SOAP_TYPE_std__vectorTemplateOfns__Messages (26)
#endif

/* std::vector<ns__Sensor>  has binding name 'std__vectorTemplateOfns__Sensor' for type 'ns:Sensor' */
#ifndef SOAP_TYPE_std__vectorTemplateOfns__Sensor
#define SOAP_TYPE_std__vectorTemplateOfns__Sensor (25)
#endif

/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of OptionsStub.h */

/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef data_TYPES_H
#define data_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace wire { namespace data {


class NoData {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  NoData() {
  }

  virtual ~NoData() throw() {}


  bool operator == (const NoData & /* rhs */) const
  {
    return true;
  }
  bool operator != (const NoData &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const NoData & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(NoData &a, NoData &b);

typedef struct _SessionStart__isset {
  _SessionStart__isset() : properties(false), blender_version(false), blender_subversion(false), os(false), os_version(false), resolution_x(false), resolution_y(false), gui(false) {}
  bool properties;
  bool blender_version;
  bool blender_subversion;
  bool os;
  bool os_version;
  bool resolution_x;
  bool resolution_y;
  bool gui;
} _SessionStart__isset;

class SessionStart {
 public:

  static const char* ascii_fingerprint; // = "156EF29B76E759BBEFED8A8D03EDB600";
  static const uint8_t binary_fingerprint[16]; // = {0x15,0x6E,0xF2,0x9B,0x76,0xE7,0x59,0xBB,0xEF,0xED,0x8A,0x8D,0x03,0xED,0xB6,0x00};

  SessionStart() : blender_version(0), blender_subversion(0), os(), os_version(), resolution_x(0), resolution_y(0), gui(0) {
  }

  virtual ~SessionStart() throw() {}

  std::map<std::string, std::string>  properties;
  int16_t blender_version;
  int16_t blender_subversion;
  std::string os;
  std::string os_version;
  int16_t resolution_x;
  int16_t resolution_y;
  bool gui;

  _SessionStart__isset __isset;

  void __set_properties(const std::map<std::string, std::string> & val) {
    properties = val;
    __isset.properties = true;
  }

  void __set_blender_version(const int16_t val) {
    blender_version = val;
    __isset.blender_version = true;
  }

  void __set_blender_subversion(const int16_t val) {
    blender_subversion = val;
    __isset.blender_subversion = true;
  }

  void __set_os(const std::string& val) {
    os = val;
    __isset.os = true;
  }

  void __set_os_version(const std::string& val) {
    os_version = val;
    __isset.os_version = true;
  }

  void __set_resolution_x(const int16_t val) {
    resolution_x = val;
    __isset.resolution_x = true;
  }

  void __set_resolution_y(const int16_t val) {
    resolution_y = val;
    __isset.resolution_y = true;
  }

  void __set_gui(const bool val) {
    gui = val;
    __isset.gui = true;
  }

  bool operator == (const SessionStart & rhs) const
  {
    if (__isset.properties != rhs.__isset.properties)
      return false;
    else if (__isset.properties && !(properties == rhs.properties))
      return false;
    if (__isset.blender_version != rhs.__isset.blender_version)
      return false;
    else if (__isset.blender_version && !(blender_version == rhs.blender_version))
      return false;
    if (__isset.blender_subversion != rhs.__isset.blender_subversion)
      return false;
    else if (__isset.blender_subversion && !(blender_subversion == rhs.blender_subversion))
      return false;
    if (__isset.os != rhs.__isset.os)
      return false;
    else if (__isset.os && !(os == rhs.os))
      return false;
    if (__isset.os_version != rhs.__isset.os_version)
      return false;
    else if (__isset.os_version && !(os_version == rhs.os_version))
      return false;
    if (__isset.resolution_x != rhs.__isset.resolution_x)
      return false;
    else if (__isset.resolution_x && !(resolution_x == rhs.resolution_x))
      return false;
    if (__isset.resolution_y != rhs.__isset.resolution_y)
      return false;
    else if (__isset.resolution_y && !(resolution_y == rhs.resolution_y))
      return false;
    if (__isset.gui != rhs.__isset.gui)
      return false;
    else if (__isset.gui && !(gui == rhs.gui))
      return false;
    return true;
  }
  bool operator != (const SessionStart &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SessionStart & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(SessionStart &a, SessionStart &b);

typedef struct _RNAPropertyData__isset {
  _RNAPropertyData__isset() : valueBoolean(false), listBoolean(false), valueInt(false), listInt(false), valueDouble(false), listDouble(false), valueString(false), valueEnum(false), listEnum(false), valuePointer(false), collection(false) {}
  bool valueBoolean;
  bool listBoolean;
  bool valueInt;
  bool listInt;
  bool valueDouble;
  bool listDouble;
  bool valueString;
  bool valueEnum;
  bool listEnum;
  bool valuePointer;
  bool collection;
} _RNAPropertyData__isset;

class RNAPropertyData {
 public:

  static const char* ascii_fingerprint; // = "EB13F12900F9242F2C7663FCDE00EC35";
  static const uint8_t binary_fingerprint[16]; // = {0xEB,0x13,0xF1,0x29,0x00,0xF9,0x24,0x2F,0x2C,0x76,0x63,0xFC,0xDE,0x00,0xEC,0x35};

  RNAPropertyData() : valueBoolean(0), valueInt(0), valueDouble(0), valueString(), valueEnum(), valuePointer() {
  }

  virtual ~RNAPropertyData() throw() {}

  bool valueBoolean;
  std::vector<bool>  listBoolean;
  int32_t valueInt;
  std::vector<int32_t>  listInt;
  double valueDouble;
  std::vector<double>  listDouble;
  std::string valueString;
  std::string valueEnum;
  std::set<std::string>  listEnum;
  std::string valuePointer;
  std::vector<std::string>  collection;

  _RNAPropertyData__isset __isset;

  void __set_valueBoolean(const bool val) {
    valueBoolean = val;
    __isset = _RNAPropertyData__isset();
    __isset.valueBoolean = true;
  }

  void __set_listBoolean(const std::vector<bool> & val) {
    listBoolean = val;
    __isset = _RNAPropertyData__isset();
    __isset.listBoolean = true;
  }

  void __set_valueInt(const int32_t val) {
    valueInt = val;
    __isset = _RNAPropertyData__isset();
    __isset.valueInt = true;
  }

  void __set_listInt(const std::vector<int32_t> & val) {
    listInt = val;
    __isset = _RNAPropertyData__isset();
    __isset.listInt = true;
  }

  void __set_valueDouble(const double val) {
    valueDouble = val;
    __isset = _RNAPropertyData__isset();
    __isset.valueDouble = true;
  }

  void __set_listDouble(const std::vector<double> & val) {
    listDouble = val;
    __isset = _RNAPropertyData__isset();
    __isset.listDouble = true;
  }

  void __set_valueString(const std::string& val) {
    valueString = val;
    __isset = _RNAPropertyData__isset();
    __isset.valueString = true;
  }

  void __set_valueEnum(const std::string& val) {
    valueEnum = val;
    __isset = _RNAPropertyData__isset();
    __isset.valueEnum = true;
  }

  void __set_listEnum(const std::set<std::string> & val) {
    listEnum = val;
    __isset = _RNAPropertyData__isset();
    __isset.listEnum = true;
  }

  void __set_valuePointer(const std::string& val) {
    valuePointer = val;
    __isset = _RNAPropertyData__isset();
    __isset.valuePointer = true;
  }

  void __set_collection(const std::vector<std::string> & val) {
    collection = val;
    __isset = _RNAPropertyData__isset();
    __isset.collection = true;
  }

  bool operator == (const RNAPropertyData & rhs) const
  {
    if (__isset.valueBoolean != rhs.__isset.valueBoolean)
      return false;
    else if (__isset.valueBoolean && !(valueBoolean == rhs.valueBoolean))
      return false;
    if (__isset.listBoolean != rhs.__isset.listBoolean)
      return false;
    else if (__isset.listBoolean && !(listBoolean == rhs.listBoolean))
      return false;
    if (__isset.valueInt != rhs.__isset.valueInt)
      return false;
    else if (__isset.valueInt && !(valueInt == rhs.valueInt))
      return false;
    if (__isset.listInt != rhs.__isset.listInt)
      return false;
    else if (__isset.listInt && !(listInt == rhs.listInt))
      return false;
    if (__isset.valueDouble != rhs.__isset.valueDouble)
      return false;
    else if (__isset.valueDouble && !(valueDouble == rhs.valueDouble))
      return false;
    if (__isset.listDouble != rhs.__isset.listDouble)
      return false;
    else if (__isset.listDouble && !(listDouble == rhs.listDouble))
      return false;
    if (__isset.valueString != rhs.__isset.valueString)
      return false;
    else if (__isset.valueString && !(valueString == rhs.valueString))
      return false;
    if (__isset.valueEnum != rhs.__isset.valueEnum)
      return false;
    else if (__isset.valueEnum && !(valueEnum == rhs.valueEnum))
      return false;
    if (__isset.listEnum != rhs.__isset.listEnum)
      return false;
    else if (__isset.listEnum && !(listEnum == rhs.listEnum))
      return false;
    if (__isset.valuePointer != rhs.__isset.valuePointer)
      return false;
    else if (__isset.valuePointer && !(valuePointer == rhs.valuePointer))
      return false;
    if (__isset.collection != rhs.__isset.collection)
      return false;
    else if (__isset.collection && !(collection == rhs.collection))
      return false;
    return true;
  }
  bool operator != (const RNAPropertyData &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RNAPropertyData & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RNAPropertyData &a, RNAPropertyData &b);

typedef struct _RNAProperty__isset {
  _RNAProperty__isset() : identifier(false), type(false), length(false), data(false) {}
  bool identifier;
  bool type;
  bool length;
  bool data;
} _RNAProperty__isset;

class RNAProperty {
 public:

  static const char* ascii_fingerprint; // = "765F2F039979508BE12A0AD81BFAEBA2";
  static const uint8_t binary_fingerprint[16]; // = {0x76,0x5F,0x2F,0x03,0x99,0x79,0x50,0x8B,0xE1,0x2A,0x0A,0xD8,0x1B,0xFA,0xEB,0xA2};

  RNAProperty() : identifier(), type(0), length(0) {
  }

  virtual ~RNAProperty() throw() {}

  std::string identifier;
  int16_t type;
  int16_t length;
  RNAPropertyData data;

  _RNAProperty__isset __isset;

  void __set_identifier(const std::string& val) {
    identifier = val;
    __isset.identifier = true;
  }

  void __set_type(const int16_t val) {
    type = val;
    __isset.type = true;
  }

  void __set_length(const int16_t val) {
    length = val;
    __isset.length = true;
  }

  void __set_data(const RNAPropertyData& val) {
    data = val;
    __isset.data = true;
  }

  bool operator == (const RNAProperty & rhs) const
  {
    if (__isset.identifier != rhs.__isset.identifier)
      return false;
    else if (__isset.identifier && !(identifier == rhs.identifier))
      return false;
    if (__isset.type != rhs.__isset.type)
      return false;
    else if (__isset.type && !(type == rhs.type))
      return false;
    if (__isset.length != rhs.__isset.length)
      return false;
    else if (__isset.length && !(length == rhs.length))
      return false;
    if (__isset.data != rhs.__isset.data)
      return false;
    else if (__isset.data && !(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const RNAProperty &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RNAProperty & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RNAProperty &a, RNAProperty &b);

typedef struct _Object__isset {
  _Object__isset() : selected(false), active(false), name(false), type(false), baseAddress(false), objectAddress(false), parentType(false), parentAddress(false) {}
  bool selected;
  bool active;
  bool name;
  bool type;
  bool baseAddress;
  bool objectAddress;
  bool parentType;
  bool parentAddress;
} _Object__isset;

class Object {
 public:

  static const char* ascii_fingerprint; // = "53C1A71B52080E404A9A108B17CCAC80";
  static const uint8_t binary_fingerprint[16]; // = {0x53,0xC1,0xA7,0x1B,0x52,0x08,0x0E,0x40,0x4A,0x9A,0x10,0x8B,0x17,0xCC,0xAC,0x80};

  Object() : selected(0), active(0), name(), type(0), baseAddress(), objectAddress(), parentType(0), parentAddress() {
  }

  virtual ~Object() throw() {}

  bool selected;
  bool active;
  std::string name;
  int16_t type;
  std::string baseAddress;
  std::string objectAddress;
  int16_t parentType;
  std::string parentAddress;

  _Object__isset __isset;

  void __set_selected(const bool val) {
    selected = val;
    __isset.selected = true;
  }

  void __set_active(const bool val) {
    active = val;
    __isset.active = true;
  }

  void __set_name(const std::string& val) {
    name = val;
    __isset.name = true;
  }

  void __set_type(const int16_t val) {
    type = val;
    __isset.type = true;
  }

  void __set_baseAddress(const std::string& val) {
    baseAddress = val;
    __isset.baseAddress = true;
  }

  void __set_objectAddress(const std::string& val) {
    objectAddress = val;
    __isset.objectAddress = true;
  }

  void __set_parentType(const int16_t val) {
    parentType = val;
    __isset.parentType = true;
  }

  void __set_parentAddress(const std::string& val) {
    parentAddress = val;
    __isset.parentAddress = true;
  }

  bool operator == (const Object & rhs) const
  {
    if (__isset.selected != rhs.__isset.selected)
      return false;
    else if (__isset.selected && !(selected == rhs.selected))
      return false;
    if (__isset.active != rhs.__isset.active)
      return false;
    else if (__isset.active && !(active == rhs.active))
      return false;
    if (__isset.name != rhs.__isset.name)
      return false;
    else if (__isset.name && !(name == rhs.name))
      return false;
    if (__isset.type != rhs.__isset.type)
      return false;
    else if (__isset.type && !(type == rhs.type))
      return false;
    if (__isset.baseAddress != rhs.__isset.baseAddress)
      return false;
    else if (__isset.baseAddress && !(baseAddress == rhs.baseAddress))
      return false;
    if (__isset.objectAddress != rhs.__isset.objectAddress)
      return false;
    else if (__isset.objectAddress && !(objectAddress == rhs.objectAddress))
      return false;
    if (__isset.parentType != rhs.__isset.parentType)
      return false;
    else if (__isset.parentType && !(parentType == rhs.parentType))
      return false;
    if (__isset.parentAddress != rhs.__isset.parentAddress)
      return false;
    else if (__isset.parentAddress && !(parentAddress == rhs.parentAddress))
      return false;
    return true;
  }
  bool operator != (const Object &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Object & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Object &a, Object &b);

typedef struct _Context__isset {
  _Context__isset() : windowName(false), windowAddress(false), screenName(false), screenAddress(false), spaceType(false), spaceAddress(false), regionType(false), regionAddress(false), dataMode(false), visibleObjects(false) {}
  bool windowName;
  bool windowAddress;
  bool screenName;
  bool screenAddress;
  bool spaceType;
  bool spaceAddress;
  bool regionType;
  bool regionAddress;
  bool dataMode;
  bool visibleObjects;
} _Context__isset;

class Context {
 public:

  static const char* ascii_fingerprint; // = "D778B00BF825EAEB25F05F885CF943A6";
  static const uint8_t binary_fingerprint[16]; // = {0xD7,0x78,0xB0,0x0B,0xF8,0x25,0xEA,0xEB,0x25,0xF0,0x5F,0x88,0x5C,0xF9,0x43,0xA6};

  Context() : windowName(), windowAddress(), screenName(), screenAddress(), spaceType(0), spaceAddress(), regionType(0), regionAddress(), dataMode() {
  }

  virtual ~Context() throw() {}

  std::string windowName;
  std::string windowAddress;
  std::string screenName;
  std::string screenAddress;
  int16_t spaceType;
  std::string spaceAddress;
  int16_t regionType;
  std::string regionAddress;
  std::string dataMode;
  std::vector<Object>  visibleObjects;

  _Context__isset __isset;

  void __set_windowName(const std::string& val) {
    windowName = val;
    __isset.windowName = true;
  }

  void __set_windowAddress(const std::string& val) {
    windowAddress = val;
    __isset.windowAddress = true;
  }

  void __set_screenName(const std::string& val) {
    screenName = val;
    __isset.screenName = true;
  }

  void __set_screenAddress(const std::string& val) {
    screenAddress = val;
    __isset.screenAddress = true;
  }

  void __set_spaceType(const int16_t val) {
    spaceType = val;
    __isset.spaceType = true;
  }

  void __set_spaceAddress(const std::string& val) {
    spaceAddress = val;
    __isset.spaceAddress = true;
  }

  void __set_regionType(const int16_t val) {
    regionType = val;
    __isset.regionType = true;
  }

  void __set_regionAddress(const std::string& val) {
    regionAddress = val;
    __isset.regionAddress = true;
  }

  void __set_dataMode(const std::string& val) {
    dataMode = val;
    __isset.dataMode = true;
  }

  void __set_visibleObjects(const std::vector<Object> & val) {
    visibleObjects = val;
    __isset.visibleObjects = true;
  }

  bool operator == (const Context & rhs) const
  {
    if (__isset.windowName != rhs.__isset.windowName)
      return false;
    else if (__isset.windowName && !(windowName == rhs.windowName))
      return false;
    if (__isset.windowAddress != rhs.__isset.windowAddress)
      return false;
    else if (__isset.windowAddress && !(windowAddress == rhs.windowAddress))
      return false;
    if (__isset.screenName != rhs.__isset.screenName)
      return false;
    else if (__isset.screenName && !(screenName == rhs.screenName))
      return false;
    if (__isset.screenAddress != rhs.__isset.screenAddress)
      return false;
    else if (__isset.screenAddress && !(screenAddress == rhs.screenAddress))
      return false;
    if (__isset.spaceType != rhs.__isset.spaceType)
      return false;
    else if (__isset.spaceType && !(spaceType == rhs.spaceType))
      return false;
    if (__isset.spaceAddress != rhs.__isset.spaceAddress)
      return false;
    else if (__isset.spaceAddress && !(spaceAddress == rhs.spaceAddress))
      return false;
    if (__isset.regionType != rhs.__isset.regionType)
      return false;
    else if (__isset.regionType && !(regionType == rhs.regionType))
      return false;
    if (__isset.regionAddress != rhs.__isset.regionAddress)
      return false;
    else if (__isset.regionAddress && !(regionAddress == rhs.regionAddress))
      return false;
    if (__isset.dataMode != rhs.__isset.dataMode)
      return false;
    else if (__isset.dataMode && !(dataMode == rhs.dataMode))
      return false;
    if (__isset.visibleObjects != rhs.__isset.visibleObjects)
      return false;
    else if (__isset.visibleObjects && !(visibleObjects == rhs.visibleObjects))
      return false;
    return true;
  }
  bool operator != (const Context &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Context & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Context &a, Context &b);

typedef struct _WmOp__isset {
  _WmOp__isset() : operatorId(false), properties(false), pythonRepresentation(false), screenshotHash(false), context(false), repeat(false) {}
  bool operatorId;
  bool properties;
  bool pythonRepresentation;
  bool screenshotHash;
  bool context;
  bool repeat;
} _WmOp__isset;

class WmOp {
 public:

  static const char* ascii_fingerprint; // = "197B25015B7E7BAF7A3570AFE4019C02";
  static const uint8_t binary_fingerprint[16]; // = {0x19,0x7B,0x25,0x01,0x5B,0x7E,0x7B,0xAF,0x7A,0x35,0x70,0xAF,0xE4,0x01,0x9C,0x02};

  WmOp() : operatorId(), pythonRepresentation(), screenshotHash(), repeat(0) {
  }

  virtual ~WmOp() throw() {}

  std::string operatorId;
  std::vector<RNAProperty>  properties;
  std::string pythonRepresentation;
  std::string screenshotHash;
  Context context;
  bool repeat;

  _WmOp__isset __isset;

  void __set_operatorId(const std::string& val) {
    operatorId = val;
    __isset.operatorId = true;
  }

  void __set_properties(const std::vector<RNAProperty> & val) {
    properties = val;
    __isset.properties = true;
  }

  void __set_pythonRepresentation(const std::string& val) {
    pythonRepresentation = val;
    __isset.pythonRepresentation = true;
  }

  void __set_screenshotHash(const std::string& val) {
    screenshotHash = val;
    __isset.screenshotHash = true;
  }

  void __set_context(const Context& val) {
    context = val;
    __isset.context = true;
  }

  void __set_repeat(const bool val) {
    repeat = val;
    __isset.repeat = true;
  }

  bool operator == (const WmOp & rhs) const
  {
    if (__isset.operatorId != rhs.__isset.operatorId)
      return false;
    else if (__isset.operatorId && !(operatorId == rhs.operatorId))
      return false;
    if (__isset.properties != rhs.__isset.properties)
      return false;
    else if (__isset.properties && !(properties == rhs.properties))
      return false;
    if (__isset.pythonRepresentation != rhs.__isset.pythonRepresentation)
      return false;
    else if (__isset.pythonRepresentation && !(pythonRepresentation == rhs.pythonRepresentation))
      return false;
    if (__isset.screenshotHash != rhs.__isset.screenshotHash)
      return false;
    else if (__isset.screenshotHash && !(screenshotHash == rhs.screenshotHash))
      return false;
    if (__isset.context != rhs.__isset.context)
      return false;
    else if (__isset.context && !(context == rhs.context))
      return false;
    if (__isset.repeat != rhs.__isset.repeat)
      return false;
    else if (__isset.repeat && !(repeat == rhs.repeat))
      return false;
    return true;
  }
  bool operator != (const WmOp &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const WmOp & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(WmOp &a, WmOp &b);

typedef struct _WmTabletData__isset {
  _WmTabletData__isset() : active(false), pressure(false), xtilt(false), ytilt(false) {}
  bool active;
  bool pressure;
  bool xtilt;
  bool ytilt;
} _WmTabletData__isset;

class WmTabletData {
 public:

  static const char* ascii_fingerprint; // = "ED111A57916E19AA931FCB07344CD9A6";
  static const uint8_t binary_fingerprint[16]; // = {0xED,0x11,0x1A,0x57,0x91,0x6E,0x19,0xAA,0x93,0x1F,0xCB,0x07,0x34,0x4C,0xD9,0xA6};

  WmTabletData() : active(0), pressure(0), xtilt(0), ytilt(0) {
  }

  virtual ~WmTabletData() throw() {}

  int32_t active;
  double pressure;
  double xtilt;
  double ytilt;

  _WmTabletData__isset __isset;

  void __set_active(const int32_t val) {
    active = val;
    __isset.active = true;
  }

  void __set_pressure(const double val) {
    pressure = val;
    __isset.pressure = true;
  }

  void __set_xtilt(const double val) {
    xtilt = val;
    __isset.xtilt = true;
  }

  void __set_ytilt(const double val) {
    ytilt = val;
    __isset.ytilt = true;
  }

  bool operator == (const WmTabletData & rhs) const
  {
    if (__isset.active != rhs.__isset.active)
      return false;
    else if (__isset.active && !(active == rhs.active))
      return false;
    if (__isset.pressure != rhs.__isset.pressure)
      return false;
    else if (__isset.pressure && !(pressure == rhs.pressure))
      return false;
    if (__isset.xtilt != rhs.__isset.xtilt)
      return false;
    else if (__isset.xtilt && !(xtilt == rhs.xtilt))
      return false;
    if (__isset.ytilt != rhs.__isset.ytilt)
      return false;
    else if (__isset.ytilt && !(ytilt == rhs.ytilt))
      return false;
    return true;
  }
  bool operator != (const WmTabletData &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const WmTabletData & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(WmTabletData &a, WmTabletData &b);

typedef struct _WmEv__isset {
  _WmEv__isset() : type(false), value(false), x(false), y(false), mval1(false), mval2(false), character(false), prevtype(false), prevval(false), prevx(false), prevy(false), prevclicktime(false), prevclickx(false), prevclicky(false), shift(false), ctrl(false), alt(false), oskey(false), keymodifier(false), check_click(false), keymap_idname(false), tablet_data(false) {}
  bool type;
  bool value;
  bool x;
  bool y;
  bool mval1;
  bool mval2;
  bool character;
  bool prevtype;
  bool prevval;
  bool prevx;
  bool prevy;
  bool prevclicktime;
  bool prevclickx;
  bool prevclicky;
  bool shift;
  bool ctrl;
  bool alt;
  bool oskey;
  bool keymodifier;
  bool check_click;
  bool keymap_idname;
  bool tablet_data;
} _WmEv__isset;

class WmEv {
 public:

  static const char* ascii_fingerprint; // = "2A2900F8FA20170DD20F40524CFF055F";
  static const uint8_t binary_fingerprint[16]; // = {0x2A,0x29,0x00,0xF8,0xFA,0x20,0x17,0x0D,0xD2,0x0F,0x40,0x52,0x4C,0xFF,0x05,0x5F};

  WmEv() : type(0), value(0), x(0), y(0), mval1(0), mval2(0), character(), prevtype(0), prevval(0), prevx(0), prevy(0), prevclicktime(0), prevclickx(0), prevclicky(0), shift(0), ctrl(0), alt(0), oskey(0), keymodifier(0), check_click(0), keymap_idname() {
  }

  virtual ~WmEv() throw() {}

  int16_t type;
  int16_t value;
  int32_t x;
  int32_t y;
  int32_t mval1;
  int32_t mval2;
  std::string character;
  int16_t prevtype;
  int16_t prevval;
  int32_t prevx;
  int32_t prevy;
  double prevclicktime;
  int32_t prevclickx;
  int32_t prevclicky;
  int16_t shift;
  int16_t ctrl;
  int16_t alt;
  int16_t oskey;
  int16_t keymodifier;
  int16_t check_click;
  std::string keymap_idname;
  WmTabletData tablet_data;

  _WmEv__isset __isset;

  void __set_type(const int16_t val) {
    type = val;
    __isset.type = true;
  }

  void __set_value(const int16_t val) {
    value = val;
    __isset.value = true;
  }

  void __set_x(const int32_t val) {
    x = val;
    __isset.x = true;
  }

  void __set_y(const int32_t val) {
    y = val;
    __isset.y = true;
  }

  void __set_mval1(const int32_t val) {
    mval1 = val;
    __isset.mval1 = true;
  }

  void __set_mval2(const int32_t val) {
    mval2 = val;
    __isset.mval2 = true;
  }

  void __set_character(const std::string& val) {
    character = val;
    __isset.character = true;
  }

  void __set_prevtype(const int16_t val) {
    prevtype = val;
    __isset.prevtype = true;
  }

  void __set_prevval(const int16_t val) {
    prevval = val;
    __isset.prevval = true;
  }

  void __set_prevx(const int32_t val) {
    prevx = val;
    __isset.prevx = true;
  }

  void __set_prevy(const int32_t val) {
    prevy = val;
    __isset.prevy = true;
  }

  void __set_prevclicktime(const double val) {
    prevclicktime = val;
    __isset.prevclicktime = true;
  }

  void __set_prevclickx(const int32_t val) {
    prevclickx = val;
    __isset.prevclickx = true;
  }

  void __set_prevclicky(const int32_t val) {
    prevclicky = val;
    __isset.prevclicky = true;
  }

  void __set_shift(const int16_t val) {
    shift = val;
    __isset.shift = true;
  }

  void __set_ctrl(const int16_t val) {
    ctrl = val;
    __isset.ctrl = true;
  }

  void __set_alt(const int16_t val) {
    alt = val;
    __isset.alt = true;
  }

  void __set_oskey(const int16_t val) {
    oskey = val;
    __isset.oskey = true;
  }

  void __set_keymodifier(const int16_t val) {
    keymodifier = val;
    __isset.keymodifier = true;
  }

  void __set_check_click(const int16_t val) {
    check_click = val;
    __isset.check_click = true;
  }

  void __set_keymap_idname(const std::string& val) {
    keymap_idname = val;
    __isset.keymap_idname = true;
  }

  void __set_tablet_data(const WmTabletData& val) {
    tablet_data = val;
    __isset.tablet_data = true;
  }

  bool operator == (const WmEv & rhs) const
  {
    if (__isset.type != rhs.__isset.type)
      return false;
    else if (__isset.type && !(type == rhs.type))
      return false;
    if (__isset.value != rhs.__isset.value)
      return false;
    else if (__isset.value && !(value == rhs.value))
      return false;
    if (__isset.x != rhs.__isset.x)
      return false;
    else if (__isset.x && !(x == rhs.x))
      return false;
    if (__isset.y != rhs.__isset.y)
      return false;
    else if (__isset.y && !(y == rhs.y))
      return false;
    if (__isset.mval1 != rhs.__isset.mval1)
      return false;
    else if (__isset.mval1 && !(mval1 == rhs.mval1))
      return false;
    if (__isset.mval2 != rhs.__isset.mval2)
      return false;
    else if (__isset.mval2 && !(mval2 == rhs.mval2))
      return false;
    if (__isset.character != rhs.__isset.character)
      return false;
    else if (__isset.character && !(character == rhs.character))
      return false;
    if (__isset.prevtype != rhs.__isset.prevtype)
      return false;
    else if (__isset.prevtype && !(prevtype == rhs.prevtype))
      return false;
    if (__isset.prevval != rhs.__isset.prevval)
      return false;
    else if (__isset.prevval && !(prevval == rhs.prevval))
      return false;
    if (__isset.prevx != rhs.__isset.prevx)
      return false;
    else if (__isset.prevx && !(prevx == rhs.prevx))
      return false;
    if (__isset.prevy != rhs.__isset.prevy)
      return false;
    else if (__isset.prevy && !(prevy == rhs.prevy))
      return false;
    if (__isset.prevclicktime != rhs.__isset.prevclicktime)
      return false;
    else if (__isset.prevclicktime && !(prevclicktime == rhs.prevclicktime))
      return false;
    if (__isset.prevclickx != rhs.__isset.prevclickx)
      return false;
    else if (__isset.prevclickx && !(prevclickx == rhs.prevclickx))
      return false;
    if (__isset.prevclicky != rhs.__isset.prevclicky)
      return false;
    else if (__isset.prevclicky && !(prevclicky == rhs.prevclicky))
      return false;
    if (__isset.shift != rhs.__isset.shift)
      return false;
    else if (__isset.shift && !(shift == rhs.shift))
      return false;
    if (__isset.ctrl != rhs.__isset.ctrl)
      return false;
    else if (__isset.ctrl && !(ctrl == rhs.ctrl))
      return false;
    if (__isset.alt != rhs.__isset.alt)
      return false;
    else if (__isset.alt && !(alt == rhs.alt))
      return false;
    if (__isset.oskey != rhs.__isset.oskey)
      return false;
    else if (__isset.oskey && !(oskey == rhs.oskey))
      return false;
    if (__isset.keymodifier != rhs.__isset.keymodifier)
      return false;
    else if (__isset.keymodifier && !(keymodifier == rhs.keymodifier))
      return false;
    if (__isset.check_click != rhs.__isset.check_click)
      return false;
    else if (__isset.check_click && !(check_click == rhs.check_click))
      return false;
    if (__isset.keymap_idname != rhs.__isset.keymap_idname)
      return false;
    else if (__isset.keymap_idname && !(keymap_idname == rhs.keymap_idname))
      return false;
    if (__isset.tablet_data != rhs.__isset.tablet_data)
      return false;
    else if (__isset.tablet_data && !(tablet_data == rhs.tablet_data))
      return false;
    return true;
  }
  bool operator != (const WmEv &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const WmEv & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(WmEv &a, WmEv &b);

typedef struct _Data__isset {
  _Data__isset() : noData(false), sessionStart(false), wmOp(false), wmEv(false) {}
  bool noData;
  bool sessionStart;
  bool wmOp;
  bool wmEv;
} _Data__isset;

class Data {
 public:

  static const char* ascii_fingerprint; // = "56CBE0FB4F81B69A34DFE99FE9AA5500";
  static const uint8_t binary_fingerprint[16]; // = {0x56,0xCB,0xE0,0xFB,0x4F,0x81,0xB6,0x9A,0x34,0xDF,0xE9,0x9F,0xE9,0xAA,0x55,0x00};

  Data() {
  }

  virtual ~Data() throw() {}

  NoData noData;
  SessionStart sessionStart;
  WmOp wmOp;
  WmEv wmEv;

  _Data__isset __isset;

  void __set_noData(const NoData& val) {
    noData = val;
    __isset = _Data__isset();
    __isset.noData = true;
  }

  void __set_sessionStart(const SessionStart& val) {
    sessionStart = val;
    __isset = _Data__isset();
    __isset.sessionStart = true;
  }

  void __set_wmOp(const WmOp& val) {
    wmOp = val;
    __isset = _Data__isset();
    __isset.wmOp = true;
  }

  void __set_wmEv(const WmEv& val) {
    wmEv = val;
    __isset = _Data__isset();
    __isset.wmEv = true;
  }

  bool operator == (const Data & rhs) const
  {
    if (__isset.noData != rhs.__isset.noData)
      return false;
    else if (__isset.noData && !(noData == rhs.noData))
      return false;
    if (__isset.sessionStart != rhs.__isset.sessionStart)
      return false;
    else if (__isset.sessionStart && !(sessionStart == rhs.sessionStart))
      return false;
    if (__isset.wmOp != rhs.__isset.wmOp)
      return false;
    else if (__isset.wmOp && !(wmOp == rhs.wmOp))
      return false;
    if (__isset.wmEv != rhs.__isset.wmEv)
      return false;
    else if (__isset.wmEv && !(wmEv == rhs.wmEv))
      return false;
    return true;
  }
  bool operator != (const Data &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Data & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Data &a, Data &b);

}} // namespace

#endif

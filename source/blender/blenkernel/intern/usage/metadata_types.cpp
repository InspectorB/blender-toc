/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "metadata_types.h"

#include <algorithm>

namespace wire { namespace metadata {

const char* NoMetadata::ascii_fingerprint = "99914B932BD37A50B983C5E7C90AE93B";
const uint8_t NoMetadata::binary_fingerprint[16] = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

uint32_t NoMetadata::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    xfer += iprot->skip(ftype);
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t NoMetadata::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("NoMetadata");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(NoMetadata &a, NoMetadata &b) {
  using ::std::swap;
  (void) a;
  (void) b;
}

const char* OnlyUser::ascii_fingerprint = "148F3AAAC1D9859963D5E800D187BF26";
const uint8_t OnlyUser::binary_fingerprint[16] = {0x14,0x8F,0x3A,0xAA,0xC1,0xD9,0x85,0x99,0x63,0xD5,0xE8,0x00,0xD1,0x87,0xBF,0x26};

uint32_t OnlyUser::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->user);
          this->__isset.user = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t OnlyUser::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("OnlyUser");

  if (this->__isset.user) {
    xfer += oprot->writeFieldBegin("user", ::apache::thrift::protocol::T_I64, 1);
    xfer += oprot->writeI64(this->user);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(OnlyUser &a, OnlyUser &b) {
  using ::std::swap;
  swap(a.user, b.user);
  swap(a.__isset, b.__isset);
}

const char* SessionKey::ascii_fingerprint = "66E694018C17E5B65A59AE8F55CCA3CD";
const uint8_t SessionKey::binary_fingerprint[16] = {0x66,0xE6,0x94,0x01,0x8C,0x17,0xE5,0xB6,0x5A,0x59,0xAE,0x8F,0x55,0xCC,0xA3,0xCD};

uint32_t SessionKey::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->sessionKey);
          this->__isset.sessionKey = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t SessionKey::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("SessionKey");

  if (this->__isset.sessionKey) {
    xfer += oprot->writeFieldBegin("sessionKey", ::apache::thrift::protocol::T_STRING, 1);
    xfer += oprot->writeString(this->sessionKey);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(SessionKey &a, SessionKey &b) {
  using ::std::swap;
  swap(a.sessionKey, b.sessionKey);
  swap(a.__isset, b.__isset);
}

const char* Metadata::ascii_fingerprint = "BC6E462F30B0AE22ED9A244AB3806973";
const uint8_t Metadata::binary_fingerprint[16] = {0xBC,0x6E,0x46,0x2F,0x30,0xB0,0xAE,0x22,0xED,0x9A,0x24,0x4A,0xB3,0x80,0x69,0x73};

uint32_t Metadata::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->noMetadata.read(iprot);
          this->__isset.noMetadata = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->onlyUser.read(iprot);
          this->__isset.onlyUser = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->sessionKey.read(iprot);
          this->__isset.sessionKey = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Metadata::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Metadata");

  if (this->__isset.noMetadata) {
    xfer += oprot->writeFieldBegin("noMetadata", ::apache::thrift::protocol::T_STRUCT, 1);
    xfer += this->noMetadata.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.onlyUser) {
    xfer += oprot->writeFieldBegin("onlyUser", ::apache::thrift::protocol::T_STRUCT, 2);
    xfer += this->onlyUser.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.sessionKey) {
    xfer += oprot->writeFieldBegin("sessionKey", ::apache::thrift::protocol::T_STRUCT, 3);
    xfer += this->sessionKey.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Metadata &a, Metadata &b) {
  using ::std::swap;
  swap(a.noMetadata, b.noMetadata);
  swap(a.onlyUser, b.onlyUser);
  swap(a.sessionKey, b.sessionKey);
  swap(a.__isset, b.__isset);
}

}} // namespace
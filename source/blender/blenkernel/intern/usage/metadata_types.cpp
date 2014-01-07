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

const char* Metadata::ascii_fingerprint = "9C63F070D776FB08873B5382A3AF4B1B";
const uint8_t Metadata::binary_fingerprint[16] = {0x9C,0x63,0xF0,0x70,0xD7,0x76,0xFB,0x08,0x87,0x3B,0x53,0x82,0xA3,0xAF,0x4B,0x1B};

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
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Metadata &a, Metadata &b) {
  using ::std::swap;
  swap(a.noMetadata, b.noMetadata);
  swap(a.onlyUser, b.onlyUser);
  swap(a.__isset, b.__isset);
}

}} // namespace

/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef TocService_H
#define TocService_H

#include <thrift/TDispatchProcessor.h>
#include "service_types.h"

namespace wire {

class TocServiceIf {
 public:
  virtual ~TocServiceIf() {}
  virtual void sendMessage(const Message& message) = 0;
  virtual void sendScreenshot(const Screenshot& screenshot) = 0;
  virtual void ping() = 0;
};

class TocServiceIfFactory {
 public:
  typedef TocServiceIf Handler;

  virtual ~TocServiceIfFactory() {}

  virtual TocServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(TocServiceIf* /* handler */) = 0;
};

class TocServiceIfSingletonFactory : virtual public TocServiceIfFactory {
 public:
  TocServiceIfSingletonFactory(const boost::shared_ptr<TocServiceIf>& iface) : iface_(iface) {}
  virtual ~TocServiceIfSingletonFactory() {}

  virtual TocServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(TocServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<TocServiceIf> iface_;
};

class TocServiceNull : virtual public TocServiceIf {
 public:
  virtual ~TocServiceNull() {}
  void sendMessage(const Message& /* message */) {
    return;
  }
  void sendScreenshot(const Screenshot& /* screenshot */) {
    return;
  }
  void ping() {
    return;
  }
};

typedef struct _TocService_sendMessage_args__isset {
  _TocService_sendMessage_args__isset() : message(false) {}
  bool message;
} _TocService_sendMessage_args__isset;

class TocService_sendMessage_args {
 public:

  static const char* ascii_fingerprint; // = "A0548005B5D090108E275D21A4A1861A";
  static const uint8_t binary_fingerprint[16]; // = {0xA0,0x54,0x80,0x05,0xB5,0xD0,0x90,0x10,0x8E,0x27,0x5D,0x21,0xA4,0xA1,0x86,0x1A};

  TocService_sendMessage_args() {
  }

  virtual ~TocService_sendMessage_args() throw() {}

  Message message;

  _TocService_sendMessage_args__isset __isset;

  void __set_message(const Message& val) {
    message = val;
    __isset.message = true;
  }

  bool operator == (const TocService_sendMessage_args & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const TocService_sendMessage_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TocService_sendMessage_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class TocService_sendMessage_pargs {
 public:

  static const char* ascii_fingerprint; // = "A0548005B5D090108E275D21A4A1861A";
  static const uint8_t binary_fingerprint[16]; // = {0xA0,0x54,0x80,0x05,0xB5,0xD0,0x90,0x10,0x8E,0x27,0x5D,0x21,0xA4,0xA1,0x86,0x1A};


  virtual ~TocService_sendMessage_pargs() throw() {}

  const Message* message;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TocService_sendMessage_result__isset {
  _TocService_sendMessage_result__isset() : unavailable(false), unknownToken(false), incorrectlyFormattedMessage(false) {}
  bool unavailable;
  bool unknownToken;
  bool incorrectlyFormattedMessage;
} _TocService_sendMessage_result__isset;

class TocService_sendMessage_result {
 public:

  static const char* ascii_fingerprint; // = "2E72786EE0F17DA00FC5BDED362FE255";
  static const uint8_t binary_fingerprint[16]; // = {0x2E,0x72,0x78,0x6E,0xE0,0xF1,0x7D,0xA0,0x0F,0xC5,0xBD,0xED,0x36,0x2F,0xE2,0x55};

  TocService_sendMessage_result() {
  }

  virtual ~TocService_sendMessage_result() throw() {}

  Unavailable unavailable;
  UnknownToken unknownToken;
  IncorrectlyFormattedMessage incorrectlyFormattedMessage;

  _TocService_sendMessage_result__isset __isset;

  void __set_unavailable(const Unavailable& val) {
    unavailable = val;
    __isset.unavailable = true;
  }

  void __set_unknownToken(const UnknownToken& val) {
    unknownToken = val;
    __isset.unknownToken = true;
  }

  void __set_incorrectlyFormattedMessage(const IncorrectlyFormattedMessage& val) {
    incorrectlyFormattedMessage = val;
    __isset.incorrectlyFormattedMessage = true;
  }

  bool operator == (const TocService_sendMessage_result & rhs) const
  {
    if (!(unavailable == rhs.unavailable))
      return false;
    if (!(unknownToken == rhs.unknownToken))
      return false;
    if (!(incorrectlyFormattedMessage == rhs.incorrectlyFormattedMessage))
      return false;
    return true;
  }
  bool operator != (const TocService_sendMessage_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TocService_sendMessage_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TocService_sendMessage_presult__isset {
  _TocService_sendMessage_presult__isset() : unavailable(false), unknownToken(false), incorrectlyFormattedMessage(false) {}
  bool unavailable;
  bool unknownToken;
  bool incorrectlyFormattedMessage;
} _TocService_sendMessage_presult__isset;

class TocService_sendMessage_presult {
 public:

  static const char* ascii_fingerprint; // = "2E72786EE0F17DA00FC5BDED362FE255";
  static const uint8_t binary_fingerprint[16]; // = {0x2E,0x72,0x78,0x6E,0xE0,0xF1,0x7D,0xA0,0x0F,0xC5,0xBD,0xED,0x36,0x2F,0xE2,0x55};


  virtual ~TocService_sendMessage_presult() throw() {}

  Unavailable unavailable;
  UnknownToken unknownToken;
  IncorrectlyFormattedMessage incorrectlyFormattedMessage;

  _TocService_sendMessage_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _TocService_sendScreenshot_args__isset {
  _TocService_sendScreenshot_args__isset() : screenshot(false) {}
  bool screenshot;
} _TocService_sendScreenshot_args__isset;

class TocService_sendScreenshot_args {
 public:

  static const char* ascii_fingerprint; // = "DACC98DD9466187070BB70575828B8E6";
  static const uint8_t binary_fingerprint[16]; // = {0xDA,0xCC,0x98,0xDD,0x94,0x66,0x18,0x70,0x70,0xBB,0x70,0x57,0x58,0x28,0xB8,0xE6};

  TocService_sendScreenshot_args() {
  }

  virtual ~TocService_sendScreenshot_args() throw() {}

  Screenshot screenshot;

  _TocService_sendScreenshot_args__isset __isset;

  void __set_screenshot(const Screenshot& val) {
    screenshot = val;
    __isset.screenshot = true;
  }

  bool operator == (const TocService_sendScreenshot_args & rhs) const
  {
    if (!(screenshot == rhs.screenshot))
      return false;
    return true;
  }
  bool operator != (const TocService_sendScreenshot_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TocService_sendScreenshot_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class TocService_sendScreenshot_pargs {
 public:

  static const char* ascii_fingerprint; // = "DACC98DD9466187070BB70575828B8E6";
  static const uint8_t binary_fingerprint[16]; // = {0xDA,0xCC,0x98,0xDD,0x94,0x66,0x18,0x70,0x70,0xBB,0x70,0x57,0x58,0x28,0xB8,0xE6};


  virtual ~TocService_sendScreenshot_pargs() throw() {}

  const Screenshot* screenshot;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TocService_sendScreenshot_result__isset {
  _TocService_sendScreenshot_result__isset() : unavailable(false), unknownToken(false), incorrectlyFormattedMessage(false) {}
  bool unavailable;
  bool unknownToken;
  bool incorrectlyFormattedMessage;
} _TocService_sendScreenshot_result__isset;

class TocService_sendScreenshot_result {
 public:

  static const char* ascii_fingerprint; // = "2E72786EE0F17DA00FC5BDED362FE255";
  static const uint8_t binary_fingerprint[16]; // = {0x2E,0x72,0x78,0x6E,0xE0,0xF1,0x7D,0xA0,0x0F,0xC5,0xBD,0xED,0x36,0x2F,0xE2,0x55};

  TocService_sendScreenshot_result() {
  }

  virtual ~TocService_sendScreenshot_result() throw() {}

  Unavailable unavailable;
  UnknownToken unknownToken;
  IncorrectlyFormattedMessage incorrectlyFormattedMessage;

  _TocService_sendScreenshot_result__isset __isset;

  void __set_unavailable(const Unavailable& val) {
    unavailable = val;
    __isset.unavailable = true;
  }

  void __set_unknownToken(const UnknownToken& val) {
    unknownToken = val;
    __isset.unknownToken = true;
  }

  void __set_incorrectlyFormattedMessage(const IncorrectlyFormattedMessage& val) {
    incorrectlyFormattedMessage = val;
    __isset.incorrectlyFormattedMessage = true;
  }

  bool operator == (const TocService_sendScreenshot_result & rhs) const
  {
    if (!(unavailable == rhs.unavailable))
      return false;
    if (!(unknownToken == rhs.unknownToken))
      return false;
    if (!(incorrectlyFormattedMessage == rhs.incorrectlyFormattedMessage))
      return false;
    return true;
  }
  bool operator != (const TocService_sendScreenshot_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TocService_sendScreenshot_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TocService_sendScreenshot_presult__isset {
  _TocService_sendScreenshot_presult__isset() : unavailable(false), unknownToken(false), incorrectlyFormattedMessage(false) {}
  bool unavailable;
  bool unknownToken;
  bool incorrectlyFormattedMessage;
} _TocService_sendScreenshot_presult__isset;

class TocService_sendScreenshot_presult {
 public:

  static const char* ascii_fingerprint; // = "2E72786EE0F17DA00FC5BDED362FE255";
  static const uint8_t binary_fingerprint[16]; // = {0x2E,0x72,0x78,0x6E,0xE0,0xF1,0x7D,0xA0,0x0F,0xC5,0xBD,0xED,0x36,0x2F,0xE2,0x55};


  virtual ~TocService_sendScreenshot_presult() throw() {}

  Unavailable unavailable;
  UnknownToken unknownToken;
  IncorrectlyFormattedMessage incorrectlyFormattedMessage;

  _TocService_sendScreenshot_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class TocService_ping_args {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  TocService_ping_args() {
  }

  virtual ~TocService_ping_args() throw() {}


  bool operator == (const TocService_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const TocService_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TocService_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class TocService_ping_pargs {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~TocService_ping_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TocService_ping_result__isset {
  _TocService_ping_result__isset() : unavailable(false) {}
  bool unavailable;
} _TocService_ping_result__isset;

class TocService_ping_result {
 public:

  static const char* ascii_fingerprint; // = "771E7EF40B572D2BFAB12C49547ADCBF";
  static const uint8_t binary_fingerprint[16]; // = {0x77,0x1E,0x7E,0xF4,0x0B,0x57,0x2D,0x2B,0xFA,0xB1,0x2C,0x49,0x54,0x7A,0xDC,0xBF};

  TocService_ping_result() {
  }

  virtual ~TocService_ping_result() throw() {}

  Unavailable unavailable;

  _TocService_ping_result__isset __isset;

  void __set_unavailable(const Unavailable& val) {
    unavailable = val;
    __isset.unavailable = true;
  }

  bool operator == (const TocService_ping_result & rhs) const
  {
    if (!(unavailable == rhs.unavailable))
      return false;
    return true;
  }
  bool operator != (const TocService_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TocService_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TocService_ping_presult__isset {
  _TocService_ping_presult__isset() : unavailable(false) {}
  bool unavailable;
} _TocService_ping_presult__isset;

class TocService_ping_presult {
 public:

  static const char* ascii_fingerprint; // = "771E7EF40B572D2BFAB12C49547ADCBF";
  static const uint8_t binary_fingerprint[16]; // = {0x77,0x1E,0x7E,0xF4,0x0B,0x57,0x2D,0x2B,0xFA,0xB1,0x2C,0x49,0x54,0x7A,0xDC,0xBF};


  virtual ~TocService_ping_presult() throw() {}

  Unavailable unavailable;

  _TocService_ping_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class TocServiceClient : virtual public TocServiceIf {
 public:
  TocServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  TocServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void sendMessage(const Message& message);
  void send_sendMessage(const Message& message);
  void recv_sendMessage();
  void sendScreenshot(const Screenshot& screenshot);
  void send_sendScreenshot(const Screenshot& screenshot);
  void recv_sendScreenshot();
  void ping();
  void send_ping();
  void recv_ping();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class TocServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<TocServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (TocServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_sendMessage(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_sendScreenshot(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  TocServiceProcessor(boost::shared_ptr<TocServiceIf> iface) :
    iface_(iface) {
    processMap_["sendMessage"] = &TocServiceProcessor::process_sendMessage;
    processMap_["sendScreenshot"] = &TocServiceProcessor::process_sendScreenshot;
    processMap_["ping"] = &TocServiceProcessor::process_ping;
  }

  virtual ~TocServiceProcessor() {}
};

class TocServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  TocServiceProcessorFactory(const ::boost::shared_ptr< TocServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< TocServiceIfFactory > handlerFactory_;
};

class TocServiceMultiface : virtual public TocServiceIf {
 public:
  TocServiceMultiface(std::vector<boost::shared_ptr<TocServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~TocServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<TocServiceIf> > ifaces_;
  TocServiceMultiface() {}
  void add(boost::shared_ptr<TocServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void sendMessage(const Message& message) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendMessage(message);
    }
    ifaces_[i]->sendMessage(message);
  }

  void sendScreenshot(const Screenshot& screenshot) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendScreenshot(screenshot);
    }
    ifaces_[i]->sendScreenshot(screenshot);
  }

  void ping() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping();
    }
    ifaces_[i]->ping();
  }

};

} // namespace

#endif
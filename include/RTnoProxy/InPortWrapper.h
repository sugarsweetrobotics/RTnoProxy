#ifndef INPORT_WRAPPER_HEADER_INCLUDED
#define INPORT_WRAPPER_HEADER_INCLUDED

#include <stdint.h>

#include <string>

class InPortWrapperBase {
 protected:
  std::string m_TypeCode;
  RTC::InPortBase* m_pPort;
  std::string m_Name;
 public:
  RTC::InPortBase& GetPort() { return *m_pPort; }
  
  operator RTC::InPortBase&(){return *m_pPort;}
  
  std::string& GetName() {return m_Name;}
  std::string& GetTypeCode() {return m_TypeCode;}
 public:
  InPortWrapperBase(const std::string& name){m_Name = name;}
  virtual ~InPortWrapperBase(){}
  
 public:
  virtual int32_t isNew(void) = 0;
  virtual int32_t Read(void) = 0;
  virtual int32_t getLength(void) = 0;
  virtual int32_t getTypesize(void) = 0;
  virtual int32_t getTypeSizeInArduino(void) = 0;
  virtual int32_t Get(void* dst, unsigned int size) = 0;
};


template<class DataType, typename VType>
class InPortWrapper : public InPortWrapperBase {
 private:
  DataType m_Value;
  
 public:
 InPortWrapper(const std::string& name) : InPortWrapperBase(name) {
    m_TypeCode = CORBA_Util::toTypename<DataType>();
    //m_TypeCode = toTypename<DataType>();
    m_pPort  = new RTC::InPort<DataType>(name.c_str(), m_Value);
  }
  
  virtual ~InPortWrapper() {
    delete m_pPort;
  }
  
 public:
  
  int32_t isNew(void) {
    return ((RTC::InPort<DataType>*)m_pPort)->isNew();
  }

  int32_t Read(void) {
    ((RTC::InPort<DataType>*)m_pPort)->read();
    //    std::cout << "Value is " << m_Value.data << std::endl;
      return 1;
  }


  int32_t getLength(void) {return 1;}
  int32_t getTypesize() {return sizeof(VType);}

  int32_t getTypeSizeInArduino(void) {
    if(m_TypeCode == "TimedDouble") {
      return 4;
    }
    return sizeof(VType);
  }

  int32_t Get(void* dst, uint32_t size) {
    VType* pbuf = (VType*)dst;
    return Get(pbuf, size);
  }
 private:
  int32_t Get(VType* dst, uint32_t size) {
    if(size != 1) return -1;
    if(m_TypeCode == "TimedDouble") {
      *((float*)dst) = m_Value.data;
    }else {
      *dst = m_Value.data;
    }
    return 0;
  }
};

template<class DataType, class VType>
class SeqInPortWrapper : public InPortWrapperBase {
private:
  DataType* m_pValue;
 public:
  
 public:
 SeqInPortWrapper(const std::string& name) : InPortWrapperBase(name)
  {
    m_TypeCode = CORBA_Util::toTypename<DataType>();
    //m_TypeCode = toTypename<DataType>();
    m_pValue = new DataType();
    m_pPort  = new RTC::InPort<DataType>(name.c_str(), *m_pValue);
  }
  
  virtual ~SeqInPortWrapper() {
      delete m_pValue;
      delete m_pPort;
    }
  
public:

  int32_t isNew(void) {
    return ((RTC::InPort<DataType>*)m_pPort)->isNew();
  }
  
  int32_t Read(void) {
    ((RTC::InPort<DataType>*)m_pPort)->read();
    return m_pValue->data.length();
  }

  int32_t getLength(void) {
    return m_pValue->data.length();
  }
  
  int32_t getTypesize() {
    return sizeof(VType);
  }
  
  int32_t getTypeSizeInArduino(void) {
    if(m_TypeCode == "TimedDoubleSeq") {
      return 4;
    }
    return sizeof(VType);
  }

  int32_t Get(void* dst, uint32_t size) {
    VType* pbuf = (VType*)dst;
    return Get(pbuf, size);
  }
  
 private:
  int32_t Get(VType* dst, uint32_t size) {
    if(m_TypeCode == "TimedDoubleSeq") {
      float *pDst = (float*)dst;
      for(unsigned int i = 0;i < size;i++) {
	pDst[i] = m_pValue->data[i];
      }
    }else {
      for(unsigned int i = 0;i < size;i++) {
	dst[i] = m_pValue->data[i];
      }
    }
    return 0;
  }
};





#endif

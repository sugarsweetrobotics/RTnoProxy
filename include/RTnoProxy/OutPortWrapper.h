#ifndef OUTPORT_WRAPPER_HEADER_OUTCLUDED
#define OUTPORT_WRAPPER_HEADER_OUTCLUDED

#include <stdint.h>
#include <string>

class OutPortWrapperBase {
protected:
  std::string m_TypeCode;
  RTC::OutPortBase* m_pPort;
  std::string m_Name;
 public:
  RTC::OutPortBase& GetPort() { return *m_pPort; }
  
  operator RTC::OutPortBase&(){return *m_pPort;}
  
  std::string& GetName() {return m_Name;}
  std::string& GetTypeCode() {return m_TypeCode;}
 public:
  OutPortWrapperBase(const std::string& name){m_Name = name;}
  virtual ~OutPortWrapperBase(){}
  
 public:
  virtual int32_t getTypesize() = 0;
  virtual int32_t getTypeSizeInArduino() = 0;
  virtual int32_t Write(void* data, uint32_t size) = 0;
};


template<class DataType, typename VType>
class OutPortWrapper : public OutPortWrapperBase {
 private:
  DataType m_Value;
  
 public:
 OutPortWrapper(const std::string& name) : OutPortWrapperBase(name) {
    //m_TypeCode = toTypename<DataType>();
    m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::OutPort<DataType>(name.c_str(), m_Value);
  }
  
  virtual ~OutPortWrapper() {
    delete m_pPort;
  }
  
 public:
  
  int32_t getTypesize() {return sizeof(VType);}
  int32_t getTypeSizeInArduino() {
    if(m_TypeCode == "TimedDouble") {
      return 4;
    } else {
      return getTypesize();
    }
  }

  int32_t Write(void* data, uint32_t size) {
    VType *buf = (VType*)data;
    return Write(buf, size);
  }

 private:  
  int32_t Write(VType* data, uint32_t size)
  {
    if(size != 1) return -1;
    if(m_TypeCode == "TimedDouble") {
      m_Value.data = *((float*)data);
    }else {
      m_Value.data = *data;
    }
    return ((RTC::OutPort<DataType>*)m_pPort)->write();
  }
};

template<class DataType, class VType>
class SeqOutPortWrapper : public OutPortWrapperBase {
private:
  DataType m_Value;
  
 public:
 SeqOutPortWrapper(const std::string& name) : OutPortWrapperBase(name) {
    //m_TypeCode = toTypename<DataType>();
    m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::OutPort<DataType>(name.c_str(), m_Value);
  }
  
  virtual ~SeqOutPortWrapper() {
    delete m_pPort;
  }
  
 public:
  int32_t getTypesize() {return sizeof(VType);}
  int32_t getTypeSizeInArduino() {
    if(m_TypeCode == "TimedDoubleSeq") {
      return 4;
    } else {
      return getTypesize();
    }
  }

  int32_t Write(void* data, uint32_t size) {
    VType* buf = (VType*)data;
    return Write(buf, size);	
  }

 private:  
  int32_t Write(VType* value, uint32_t size) {
    m_Value.data.length(size);
    if(m_TypeCode == "TimedDoubleSeq") {
      float* pData = (float*)value;
      for(unsigned int i = 0;i < size;i++) {
	m_Value.data[i] = pData[i];
      }
    } else {
      memcpy(&(m_Value.data[0]), value, size*sizeof(VType));
      //for(unsigned int i = 0;i < m_Value.data.length();i++) {
      //}
    }
    return ((RTC::OutPort<DataType>*)m_pPort)->write();
  }
};





#endif

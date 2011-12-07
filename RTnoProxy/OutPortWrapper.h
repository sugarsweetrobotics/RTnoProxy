#ifndef OUTPORT_WRAPPER_HEADER_OUTCLUDED
#define OUTPORT_WRAPPER_HEADER_OUTCLUDED

#include "rtm/Typename.h"
#include "rtm/OutPort.h"
#include "rtm/idl/BasicDataType.hh"

class OutPortWrapperBase
{
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
  OutPortWrapperBase(const char* name){m_Name = name;}
  virtual ~OutPortWrapperBase(){}
  
 public:
  virtual int getTypesize() = 0;
  virtual int getTypeSizeInArduino() = 0;
  virtual int Write(void* data, unsigned int size) = 0;
};


template<class DataType, typename VType>
class OutPortWrapper : public OutPortWrapperBase
{
 private:
  DataType m_Value;
  
 public:
  
 public:
 OutPortWrapper(const char* name) : OutPortWrapperBase(name)
  {
    m_TypeCode = toTypename<DataType>();
    //	  m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::OutPort<DataType>(name, m_Value);
  }
  
  ~OutPortWrapper()
    {
      delete m_pPort;
    }
  
 public:
  
  int getTypesize() {return sizeof(VType);}
  int getTypeSizeInArduino() {
    if(m_TypeCode == "TimedDouble") {
      return 4;
    } else {
      return getTypesize();
    }
  }

  int Write(void* data, unsigned int size) {
    VType *buf = (VType*)data;
    return Write(buf, size);
  }

 private:  
  int Write(VType* data, unsigned int size)
  {
    //    std::cout << "Writing Data (" << *(float*)data << ", size = " << size << std::endl;
    if(size != 1) return -1;
    if(m_TypeCode == "TimedDouble") {
      //      std::cout << "---Write(Double) (" << *(float*)data << ")" << std::endl;
      m_Value.data = *((float*)data);
    }else {
      m_Value.data = *data;
      //      std::cout << "---Write (" << *data << ")" << std::endl;
    }
    return ((RTC::OutPort<DataType>*)m_pPort)->write();
  }
};

template<class DataType, class VType>
class SeqOutPortWrapper : public OutPortWrapperBase
{
private:
  DataType m_Value;
 public:
  
 public:
 SeqOutPortWrapper(const char* name) : OutPortWrapperBase(name) {
    m_TypeCode = toTypename<DataType>();
    //m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::OutPort<DataType>(name, m_Value);
  }
  
  ~SeqOutPortWrapper()
    {
      delete m_pPort;
    }
  
 public:
  int getTypesize() {return sizeof(VType);}
  int getTypeSizeInArduino() {
    if(m_TypeCode == "TimedDoubleSeq") {
      return 4;
    } else {
      return getTypesize();
    }
  }

  int Write(void* data, unsigned int size) {
    VType* buf = (VType*)data;
    return Write(buf, size);	
  }

 private:  
  int Write(VType* value, unsigned int size)
  {
    m_Value.data.length(size);
    if(m_TypeCode == "TimedDoubleSeq") {
      float* pData = (float*)value;
      //      std::cout << "---Write(DoubleSeq size = " << size <<  ") (";
      for(unsigned int i = 0;i < size;i++) {
	m_Value.data[i] = pData[i];
	//	std::cout << m_Value.data[i] << ", ";
      }
      //      std::cout << ")" << std::endl;
    } else {
      memcpy(&(m_Value.data[0]), value, size*sizeof(VType));
      //      std::cout << "---Write(";
      for(unsigned int i = 0;i < m_Value.data.length();i++) {
	//	std::cout << m_Value.data[i] << ", ";
      }
      //      std::cout << std::endl;
    }
    return ((RTC::OutPort<DataType>*)m_pPort)->write();
  }
};





#endif

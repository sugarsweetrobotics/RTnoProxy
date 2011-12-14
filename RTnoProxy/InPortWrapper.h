#ifndef INPORT_WRAPPER_HEADER_INCLUDED
#define INPORT_WRAPPER_HEADER_INCLUDED


class InPortWrapperBase
{
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
	InPortWrapperBase(const char* name){m_Name = name;}
	virtual ~InPortWrapperBase(){}

public:
	virtual int isNew(void) = 0;
	virtual int Read(void) = 0;
	virtual int getLength(void) = 0;
	virtual int getTypesize(void) = 0;
	virtual int getTypeSizeInArduino(void) = 0;
	virtual int Get(void* dst, unsigned int size) = 0;
};


template<class DataType, typename VType>
class InPortWrapper : public InPortWrapperBase
{
 private:
  DataType m_Value;
  
 public:
 InPortWrapper(const char* name) : InPortWrapperBase(name) {
	 m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::InPort<DataType>(name, m_Value);
  }
  
  ~InPortWrapper()
    {
      delete m_pPort;
    }
  
 public:
  
  int isNew(void) {
    return ((RTC::InPort<DataType>*)m_pPort)->isNew();
  }

  int Read(void) {
    ((RTC::InPort<DataType>*)m_pPort)->read();
    //    std::cout << "Value is " << m_Value.data << std::endl;
      return 1;
  }


  int getLength(void) {return 1;}
  int getTypesize() {return sizeof(VType);}

  int getTypeSizeInArduino(void) {
    if(m_TypeCode == "TimedDouble") {
      return 4;
    }
	 return sizeof(VType);
  }

  int Get(void* dst, unsigned int size) {
    VType* pbuf = (VType*)dst;
    return Get(pbuf, size);
  }
 private:
  int Get(VType* dst, unsigned int size) {
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
class SeqInPortWrapper : public InPortWrapperBase
{
private:
  DataType* m_pValue;
 public:
  
 public:
 SeqInPortWrapper(const char* name) : InPortWrapperBase(name)
  {
	  m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pValue = new DataType();
    m_pPort  = new RTC::InPort<DataType>(name, *m_pValue);
  }
  
  ~SeqInPortWrapper()
    {
      delete m_pValue;
      delete m_pPort;
    }
  
public:

  int isNew(void) {
    return ((RTC::InPort<DataType>*)m_pPort)->isNew();
  }
  
  int Read(void) {
    ((RTC::InPort<DataType>*)m_pPort)->read();
    return m_pValue->data.length();
  }

  int getLength(void) {
    return m_pValue->data.length();
  }
  
  int getTypesize() {
    return sizeof(VType);
  }
  
  int getTypeSizeInArduino(void) {
    if(m_TypeCode == "TimedDoubleSeq") {
      return 4;
    }
    return sizeof(VType);
  }

  int Get(void* dst, unsigned int size) {
    VType* pbuf = (VType*)dst;
    return Get(pbuf, size);
  }
  
 private:
  int Get(VType* dst, unsigned int size) {
    if(m_TypeCode == "TimedDoubleSeq") {
      float *pDst = (float*)dst;
      for(unsigned int i = 0;i < size;i++) {
	pDst[i] = m_pValue->data[i];
      }
    }else {
	  for(int i = 0;i < size;i++) {
		  dst[i] = m_pValue->data[i];
	  }
    }
    return 0;
  }
};





#endif

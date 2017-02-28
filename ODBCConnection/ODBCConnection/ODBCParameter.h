#pragma once
#include "stdafx.h"

enum ODBCColumnType {DBC_STRING,   DBC_DATE,
                     DBC_DATETIME, DBC_DOUBLE,
                     DBC_INTEGER,  DBC_BINARY,
                     DBC_DEFAULT};

class ODBCParameter
{
public:
    ODBCParameter();
    ~ODBCParameter();
    void initialize      (SQLPOINTER     pData,      const int      pSize, 
                          std::string    pParamType, ODBCColumnType pODBCDataType);
    void setParamType    (std::string    pParamtype);
    void setAppDataType  (ODBCColumnType pDataType);
    void setSqlDataType  (ODBCColumnType pDataType);
    void setDecimalPrec  (SQLINTEGER     pPrecision);
    void setDecimalScale (SQLSMALLINT    pScale);
    void setPCBValue     (SQLINTEGER*    pValue);
    void setDataAddress  (SQLPOINTER     pData);
    void setDataLength   (const int      pSize);
    void setDecimalPrec();
    void setDecimalScale();
    void setPCBValue();

    SQLSMALLINT getParamType()   { return mParamType;             }
    SQLSMALLINT getAppDataType() { return mAppDataType;           }
    SQLSMALLINT getSqlDataType() { return mSqlDataType;           }
    SQLINTEGER  getDecimalPrec() { return mDecimalPrec;           }
    SQLSMALLINT getDecimalScale(){ return mDecimalScale;          }
    SQLLEN      getDataLength()  { return mDataLen;               }
    SQLPOINTER  getDataAddress() { return mDataAddress;           }
    SQLINTEGER* getPCBValue()    { return (SQLINTEGER*)mPCBValue; }

private:
    SQLSMALLINT mParamType;     //Input, Output, Input/Output
    SQLSMALLINT mAppDataType;   //Type of data app side
    SQLSMALLINT mSqlDataType;   //Type of data sql side
    SQLINTEGER  mDecimalPrec;   //Decimal Precision of Data
    SQLSMALLINT mDecimalScale;  //Decimal Scale
    SQLPOINTER  mDataAddress;   //Memory location of Data
    SQLLEN      mDataLen;       //Length of Data
    SQLINTEGER  mPCBValue;
};

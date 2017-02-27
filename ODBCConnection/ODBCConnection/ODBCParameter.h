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
    void initialize(SQLPOINTER pData, const int pSize, std::string pParamType, ODBCColumnType pODBCDataType);
    void setParamType(std::string type);
    void setAppDataType(ODBCColumnType pDataType);
    void setSqlDataType(ODBCColumnType pDataType);
    void setDataAddress(SQLPOINTER pData);
    void setDataLength(const int pSize);

    SQLSMALLINT getParamType()   { return mParamType; }
    SQLSMALLINT getAppDataType() { return mAppDataType; }
    SQLSMALLINT getSqlDataType() { return mSqlDataType; }
    SQLLEN      getDataLength()  { return mDataLen; }
    SQLPOINTER  getDataAddress() { return mDataAddress; }

private:
    SQLSMALLINT mParamType;     //Input, Output, Input/Output
    SQLSMALLINT mAppDataType;   //Type of data app side
    SQLSMALLINT mSqlDataType;   //Type of data sql side
    SQLPOINTER  mDataAddress;   //Memory location of Data
    SQLLEN      mDataLen;       //Length of Data
};


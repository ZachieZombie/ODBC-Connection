#pragma once
#include "stdafx.h"

enum ODBCColumnType {DBC_STRING,   DBC_DATE,
                     DBC_DATETIME, DBC_DOUBLE,
                     DBC_INTEGER,  DBC_BINARY};

class ODBCParameter
{
public:
    ODBCParameter();
    ODBCParameter::ODBCParameter(ODBCColumnType pType, unsigned int pWidth,
                                 std::string ParamType);
    ~ODBCParameter();
    void initialize();
    void setType(const ODBCColumnType type);
    void setParamType(const std::string paramType);
    void setWidth(const unsigned int width);
    void setNull(const bool pSetNull);
    void unbind();
    char* getBindAddress()         {return (char*)&mData;}
    const int   getBindSize()            {return mWidth;}
    const ODBCColumnType getType()       {return mType;}
    const unsigned int getWidth()        {return mWidth;}
    const bool isNull()                  {return mIndicator == SQL_NULL_DATA;}
    SQLINTEGER* getIndAddress()    {return &mIndicator;}
    const unsigned int getDecDigits()    {return mWidth;}
    const std::string getStringValue();

    char           mData[256];

private:
    void refreshString();
    unsigned int   mDigits;
    unsigned int   mWidth;
    std::string    mParamType;
    ODBCColumnType mType;
    SQLINTEGER     mIndicator;
    int            mBinds;
};


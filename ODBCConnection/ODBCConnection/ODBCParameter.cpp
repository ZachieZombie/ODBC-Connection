#include "stdafx.h"
#include "ODBCParameter.h"




ODBCParameter::ODBCParameter()
{
    mParamType    = NULL;
    mAppDataType  = NULL;
    mSqlDataType  = NULL;
    mDecimalPrec  = NULL;
    mDecimalScale = NULL;
    mDataAddress  = NULL;
    mDataLen      = NULL;
    mPCBValue     = NULL;
}



void ODBCParameter::initialize(SQLPOINTER pData, const int pSize, std::string pParamType, ODBCColumnType pODBCDataType)
{
    setDataLength(pSize);
    setDataAddress(pData);
    setParamType(pParamType);
    setSqlDataType(pODBCDataType);
    setAppDataType(pODBCDataType);
    setDecimalPrec();
    setDecimalScale();
    setPCBValue();
}



ODBCParameter::~ODBCParameter()
{
}



void ODBCParameter::setParamType(std::string pParamType)
{
    if (pParamType == "Input")
    {
        mParamType = SQL_PARAM_INPUT;
    }
    else if (pParamType == "Output")
    {
        mParamType = SQL_PARAM_OUTPUT;
    }
    else if (pParamType == "Input/Output")
    {
        mParamType = SQL_PARAM_INPUT_OUTPUT;
    }
    else
        mParamType = SQL_PARAM_ERROR;
}



void ODBCParameter::setAppDataType(ODBCColumnType pDataType)
{
    switch (pDataType)
    {
        case DBC_STRING:
            mAppDataType = SQL_C_CHAR;
            break;
        case DBC_DOUBLE:
            mAppDataType = SQL_C_DOUBLE;
            break;
        case DBC_INTEGER:
            mAppDataType = SQL_C_SLONG;
            break;
        case DBC_DEFAULT:
            mAppDataType = SQL_DEFAULT;
            break;
        default:
            mAppDataType = SQL_DEFAULT;
    }
}



void ODBCParameter::setSqlDataType(ODBCColumnType pDataType)
{
    switch (pDataType)
    {
    case DBC_STRING:
        if (mDataLen > 2048)
        {
            mSqlDataType = SQL_LONGVARCHAR;
        }
        else
            mSqlDataType = SQL_VARCHAR;
        break;
    case DBC_DOUBLE:
        mSqlDataType = SQL_DOUBLE;
        break;
    case DBC_INTEGER:
        mSqlDataType = SQL_INTEGER;
        break;
    case DBC_DEFAULT:
        mSqlDataType = SQL_VARCHAR;
        break;
    default:
        mSqlDataType = SQL_VARCHAR;
    }
}



void ODBCParameter::setDataAddress(SQLPOINTER pData)
{
    mDataAddress = pData;
}



void ODBCParameter::setDataLength(const int pSize)
{
    mDataLen = pSize;
}



void ODBCParameter::setDecimalPrec()
{
    if (mSqlDataType == SQL_VARCHAR)
    {
        mDecimalPrec = mDataLen;
    }
    else if (mSqlDataType == SQL_DOUBLE)
    {
        mDecimalPrec = 15;
    }
    else if (mSqlDataType == SQL_INTEGER)
    {
        mDecimalPrec = 10;
    }
}



void ODBCParameter::setDecimalScale()
{
    if (mSqlDataType == SQL_VARCHAR)
    {
        mDecimalScale = 0;
    }
    else if (mSqlDataType == SQL_DOUBLE)
    {
        mDecimalScale = 4;
    }
    else if (mSqlDataType == SQL_INTEGER)
    {
        mDecimalScale = 0;
    }
}



void ODBCParameter::setPCBValue()
{
    if (mAppDataType == SQL_C_CHAR)
    {
        mPCBValue = SQL_NTS;
    }
    mPCBValue = 0;
}



void ODBCParameter::setDecimalPrec(SQLINTEGER pPrecision)
{
    mDecimalPrec = pPrecision;
}



void ODBCParameter::setDecimalScale(SQLSMALLINT pScale)
{
    mDecimalScale = pScale;
}



void ODBCParameter::setPCBValue(SQLINTEGER* pValue)
{
    mPCBValue = *pValue;
}
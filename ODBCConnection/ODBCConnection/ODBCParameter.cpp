#include "stdafx.h"
#include "ODBCParameter.h"


ODBCParameter::ODBCParameter()
{
    mParamType   = NULL;
    mAppDataType = NULL;
    mSqlDataType = NULL;
    mDataAddress = NULL;
    mDataLen     = NULL;
}



void ODBCParameter::initialize(SQLPOINTER pData, const int pSize, std::string pParamType, ODBCColumnType pODBCDataType)
{
    setDataLength(pSize);
    setDataAddress(pData);
    setParamType(pParamType);
    setSqlDataType(pODBCDataType);
    setAppDataType(pODBCDataType);
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
        mSqlDataType = SQL_VARCHAR;
        break;
    case DBC_DEFAULT:
        mSqlDataType = SQL_DEFAULT;
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
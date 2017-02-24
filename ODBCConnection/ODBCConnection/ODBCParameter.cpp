#include "stdafx.h"
#include "ODBCParameter.h"


ODBCParameter::ODBCParameter()
{
    initialize();
}



ODBCParameter::ODBCParameter(ODBCColumnType pType, unsigned int pWidth, 
                             std::string pParamType)
{
    initialize();
    setType(pType);
    setWidth(pWidth);
    setParamType(pParamType);
}



ODBCParameter::~ODBCParameter()
{

}




void ODBCParameter::initialize()
{
    mParamType = DBC_STRING;
    mDigits = 4;
    mIndicator = SQL_NTS;
    mWidth = 0;
    mBinds = 0;
    memset(&mData, 0, sizeof(mData));
}




void ODBCParameter::setType(const ODBCColumnType type)
{
    mType = type;
}




void ODBCParameter::setParamType(const std::string paramType)
{
    mParamType = paramType;
}




void ODBCParameter::setWidth(const unsigned int width)
{
    mWidth = width;
}



void ODBCParameter::setNull(const bool pSetNull)
{
    if (pSetNull)
    {
        mIndicator = SQL_NULL_DATA;

        switch (mType)
        {
            /*case DBC_DOUBLE:
                mData = 0.0;
                break;
            case DBC_INTEGER:
                mData = 0;
                break;*/
            case DBC_DATE:
            case DBC_DATETIME:
            case DBC_STRING:
            case DBC_BINARY:
                mData[0] = '\0';
                break;
            default:
                memset(mData, 0, sizeof(mData));
                break;
        }
    }

    else
    {
        mIndicator = SQL_NTS;
    }
}



void ODBCParameter::unbind()
{
    mBinds--;
}
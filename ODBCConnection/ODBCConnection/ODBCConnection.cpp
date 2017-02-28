#include "stdafx.h"
#include "ODBCConnection.h"


ODBCConnection::ODBCConnection()
{
    mHdbc = NULL;
    mHenv = NULL;
    mConnectionTimeout = 0;
    mLoginTimeout = 5;
    mIsConnected = FALSE;
    allocSQL();
}


ODBCConnection::~ODBCConnection()
{
    close();
    mConnectionTimeout = 0;
    mLoginTimeout = 0;
    mIsConnected = FALSE;
}



bool ODBCConnection::open(char* ptrDsnStr, char* ptrUserStr,
    char* ptrPassStr)
{
    if (mConnectionTimeout > 0)
    {
        SQLSetConnectAttr(mHdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)mConnectionTimeout, 0);
    }

    SQLSetConnectAttr(mHdbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)mLoginTimeout, 0);

    mRetCode = SQLConnect(mHdbc, (SQLCHAR*)ptrDsnStr, SQL_NTS,
        (SQLCHAR*)ptrUserStr, SQL_NTS,
        (SQLCHAR*)ptrPassStr, SQL_NTS);

    if ((mRetCode == SQL_SUCCESS) || (mRetCode == SQL_SUCCESS_WITH_INFO))
    {
        mIsConnected = true;
    }
    else
    {
        mIsConnected = false;
    }

    return mIsConnected;
}


bool ODBCConnection::connectDriver(char* ptrConStr, char* ptrConStrOut, HWND hWnd,
enum driverCompletion driverCon)
{
    SQLSMALLINT pcbConnStrOut;

    if ((driverCon == sqlPrompt) && (hWnd == NULL))
    {
        return false;
    }

    if (mConnectionTimeout > 0)
    {
        SQLSetConnectAttr(mHdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)mConnectionTimeout, 0);
    }

    SQLSetConnectAttr(mHdbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)mLoginTimeout, 0);

    mRetCode = SQLDriverConnect(mHdbc, hWnd, (SQLCHAR*)ptrConStr, SQL_NTS,
        (SQLCHAR*)ptrConStrOut, sizeof(ptrConStrOut),
        &pcbConnStrOut,
        (SQLUSMALLINT)driverCon);

    if ((mRetCode == SQL_SUCCESS) || (mRetCode == SQL_SUCCESS_WITH_INFO))
    {
        mIsConnected = true;
    }
    else
    {
        mIsConnected = false;
    }

    return mIsConnected;
}



void ODBCConnection::allocSQL()
{
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mHenv);
    SQLSetEnvAttr(mHenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, mHenv, &mHdbc);
}



void ODBCConnection::freeSQL()
{
    SQLFreeHandle(SQL_HANDLE_DBC, mHdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, mHenv);
    mHdbc = NULL;
    mHenv = NULL;
}



void ODBCConnection::close()
{
    mIsConnected = false;
    if (mHdbc == NULL)
        return;
    SQLDisconnect(mHdbc);
    freeSQL();
}



std::string ODBCConnection::retrieveError(std::string fn)
{
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT length;
    SQLRETURN ret;
    std::string error;

    error = "The driver reported the following diagnostics while running(";
    error += fn + "): ";

    ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS)
    {
        ret = SQLGetDiagRec(SQL_HANDLE_ENV, mHenv, ++i, 
                            state, &native, text, sizeof(text), &length);
        if (SQL_SUCCEEDED(ret))
        {
            std::string tempError;
            std::string buffer;
            buffer.assign((char*)state);
            tempError += "Code " + buffer + " - #" + std::to_string(native) + "\n";
            buffer.clear();
            tempError.append((char*)text);
            error += tempError;
        }
    }
    return error;
}
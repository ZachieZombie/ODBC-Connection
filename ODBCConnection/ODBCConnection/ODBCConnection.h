#pragma once
#include "stdafx.h"

#define MAX_BUFFER 256
#define MAX_COL_NAME_LEN 32

class ODBCConnection
{
public:
    SQLHDBC mHdbc;

    enum driverCompletion
    {
        sqlNoPrompt = SQL_DRIVER_NOPROMPT,
        sqlPrompt = SQL_DRIVER_PROMPT
    };

    ODBCConnection();
    ~ODBCConnection();

    bool open(char* ptrDsnStr, char* ptrUserStr = NULL,
        char* ptrStrPass = NULL);
    bool connectDriver(char* ptrConStr, char* ptrConStrOut = NULL,
        HWND hWnd = NULL, enum driverCompletion driverCon = sqlNoPrompt);
    void setConnectionTimeout(long nSeconds);
    void setLoginTimeout(long nSeconds);
    long getConnectionTimeout();
    bool isConnected();
    void close();
    std::string retrieveError(std::string fn);

protected:
    SQLHENV mHenv;
    SQLRETURN mRetCode;
    long    mConnectionTimeout;
    long    mLoginTimeout;
    bool    mIsConnected;
    void freeSQL();
    void allocSQL();
};



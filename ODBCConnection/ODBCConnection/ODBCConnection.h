#pragma once
#include "stdafx.h"

#define MAX_BUFFER 256
#define MAX_COL_NAME_LEN 32

/*
    Class: ODBCConnection

    This class connects via ODBC to a given DSN with username and
        passwords being optional. For this class to function properly
        the DSN must be configured correctly on the user's computer.

    Variables:
        Public:
            mHdbc - This variables stores the connection information
                    and is public so that it can be shared with other
                    classes
        Private:
            mHenv - This variable holds the SQL Environment info needed
                    for the connection
            mRetCode - This variable catches errors on SQL function calls
            mConnectionTimeout - (Optional) this variable holds the time to
                                  wait before giving up on connection
            mLoginTimeout - (Optional) this variable holds the time to wait
                             before giving up trying to login
            mIsConnected - Variable to specify if connection is established
            mError - Variable to hold the last error the class encountered
*/

class ODBCConnection
{
public:
    SQLHDBC mHdbc;
    enum driverCompletion
    {
        sqlNoPrompt = SQL_DRIVER_NOPROMPT,
        sqlPrompt   = SQL_DRIVER_PROMPT
    };
    ODBCConnection();
    ~ODBCConnection();
    bool open(const char* ptrDsnStr, const char* ptrUserStr = NULL,
        const char* ptrStrPass = NULL);
    bool connectDriver(char* ptrConStr, char* ptrConStrOut = NULL,
        HWND hWnd = NULL, enum driverCompletion driverCon = sqlNoPrompt);
    void setConnectionTimeout(long nSeconds);
    void setLoginTimeout(long nSeconds);
    long getConnectionTimeout();
    bool isConnected();
    void close();
    std::string getLastError(){ return mError; }
    std::string retrieveError(std::string pFn, SQLSMALLINT pHandleType);

protected:
    SQLHENV     mHenv;
    SQLRETURN   mRetCode;
    long        mConnectionTimeout;
    long        mLoginTimeout;
    bool        mIsConnected;
    std::string mError;
    void freeSQL();
    void allocSQL();
};



#include "stdafx.h"
#include "ODBCStoredProc.h"


ODBCStoredProc::ODBCStoredProc()
{
    mHdbc = NULL;
    mHstmt = NULL;
}



ODBCStoredProc::ODBCStoredProc(ODBCConnection* pDb)
{
    mHdbc = pDb->mHdbc;
    mHstmt = NULL;
    allocStatement();
}


ODBCStoredProc::~ODBCStoredProc()
{
    close();
    mHstmt = NULL;
}



void ODBCStoredProc::allocStatement()
{
    SQLAllocHandle(SQL_HANDLE_STMT, mHdbc, &mHstmt);
}



bool ODBCStoredProc::run(char* ptrSqlStr)
{
    retCode = SQLExecDirect(mHstmt, (SQLCHAR*)ptrSqlStr, SQL_NTS);

    if (!((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO)))
    {
        return false;
    }

    SQLFetch(mHstmt);
    return true;
}



void ODBCStoredProc::close()
{
    if (mHstmt != NULL)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, mHstmt);
    }
    mHstmt = NULL;
}



bool ODBCStoredProc::bindParameter(unsigned int pIndex, ODBCParameter &pParam)
{
    retCode = SQLBindParameter(mHstmt,
                               pIndex,
                               pParam.getParamType(),
                               pParam.getAppDataType(),
                               pParam.getSqlDataType(),
                               pParam.getDecimalPrec(),
                               pParam.getDecimalScale(),
                               pParam.getDataAddress(),
                               pParam.getDataLength(),
                               pParam.getPCBValue());

    if ((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO))
    {
        return true;
    }
    return false;
}



std::string ODBCStoredProc::retrieveError(std::string fn)
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
        ret = SQLGetDiagRec(SQL_HANDLE_STMT, mHstmt, ++i,
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
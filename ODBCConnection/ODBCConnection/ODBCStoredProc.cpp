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
    mCon.close();
    close();
    mHstmt = NULL;
}



/* 
   Initialize Connection to be used to run the stored procedure 
       Returns true: Successful Connection
       Returns false: Unsuccessful Connection
   If connection fails the SQL Driver diagnostic information is retrieved
*/
bool ODBCStoredProc::initialize(std::string pDSN, std::string pUser, std::string pPass)
{
    setDSN (pDSN);
    setUser(pUser);
    setPass(pPass);
    connectODBC();

    if (!mODBCConnected)
    {
        return false;
    }
    mHdbc = mCon.mHdbc;
    allocStatement();
    return true;
}



/*
    Set Stored Procedure Call 
        Syntax: {return = call [stored].[proc].[name] (Input/Output Variables)
        Example: {call [dw].[dataloads].[dbo].[ustp_EmailManagement] ('Admin: Machine Status', ?, ?)}
*/
void ODBCStoredProc::setProcCall(std::string pCall)
{
    mProcCall = pCall;
}



/*
    Set DSN Connection Name
*/
void ODBCStoredProc::setDSN(std::string pDSN)
{
    mDSN = pDSN;
}



/*
    Set DSN Connection Username
*/
void ODBCStoredProc::setUser(std::string pUser)
{
    mUser = pUser;
}



/*
    Set DSN Connection Password
*/
void ODBCStoredProc::setPass(std::string pPass)
{
    mPass = pPass;
}



/*
    Allocate SQL Statement handle
*/
void ODBCStoredProc::allocStatement()
{
    SQLAllocHandle(SQL_HANDLE_STMT, mHdbc, &mHstmt);
}



/* runProcedure(std::string)
    Run Stored Procedure Call
        Syntax: {return = call [stored].[proc].[name] (Input/Output Variables)
            Return value from stored procedure needs to be an int
            Input/Output variables can be hard coded (input) or bound params (in/out)
        Example: {call [dw].[dataloads].[dbo].[ustp_EmailManagement] ('Admin: Machine Status', ?, ?)}
            In the example call "'Admin: Machine Status'" is a hard coded string input parameter and
            the two ? marks are bound parameters.
*/
bool ODBCStoredProc::runProcedure(std::string ptrSqlStr)
{
    retCode = SQLExecDirect(mHstmt, (SQLCHAR*)ptrSqlStr.c_str(), SQL_NTS);

    if (!((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO)))
    {
        mError.clear();
        mError = retrieveError("Run Procedure");
        return false;
    }
    SQLFetch(mHstmt);
    return true;
}



/* runProcedure()
    Default Run Stored Procedure Call. 
        Uses procedure call set by the setProcCall function.
*/
bool ODBCStoredProc::runProcedure()
{
    retCode = SQLExecDirect(mHstmt, (SQLCHAR*)mProcCall.c_str(), SQL_NTS);

    if (!((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO)))
    {
        mError.clear();
        mError = retrieveError("Run Procedure");
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



/* bindParameter(unsigned int, ODBCParameter)
    Bind parameter to SQL statement handle
        Returns true: successfully bound parameter to statement
        Returns false: bind was not successful
    If returning false diagnostic information is retrieved from the SQL driver
*/
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
    else
    {
        mError.clear();
        mError = retrieveError("Bind Parameter");
        return false;
    }
}




void ODBCStoredProc::connectODBC()
{
    if (!mCon.open(mDSN.c_str(), mUser.c_str(), mPass.c_str()))
    {
        mError.clear();
        mError = mCon.getLastError();
        mCon.close();
        mODBCConnected = false;
    }
    else
        mODBCConnected = true;
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

    error = "The driver reported the following diagnostics while running (";
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
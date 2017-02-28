#pragma once
#include "stdafx.h"
#include "ODBCConnection.h"
#include "ODBCParameter.h"

class ODBCStoredProc
{
public:
    ODBCStoredProc();
    ODBCStoredProc(ODBCConnection* pdb);
    ~ODBCStoredProc();

    bool initialize(std::string pDSN, std::string pUser, std::string pPass);
    void connectODBC();
    bool isODBCConnected();

    bool runProcedure(std::string ptrSqlStr);
    bool runProcedure();

    bool bindParameter(unsigned int pIndex, ODBCParameter &pParam);
    void close();


    void setProcCall(std::string pCall);
    void setDSN     (std::string pDSN);
    void setUser    (std::string pUser);
    void setPass    (std::string pPass);


    std::string retrieveError(std::string fn);
    std::string getLastError() { return mError; }
private:
    ODBCConnection mCon;
    SQLHDBC        mHdbc;
    SQLHSTMT       mHstmt;
    SQLRETURN      retCode;
    std::string    mProcCall;
    std::string    mDSN;
    std::string    mUser;
    std::string    mPass;
    std::string    mError;
    bool           mODBCConnected;
    void allocStatement();
};


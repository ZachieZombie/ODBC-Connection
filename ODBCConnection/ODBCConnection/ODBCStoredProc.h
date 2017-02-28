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
    bool run(char* ptrSqlStr);
    bool bindParameter(unsigned int pIndex, ODBCParameter &pParam);
    SQLHSTMT mHstmt;
    void close();
    std::string retrieveError(std::string fn);
private:
    SQLHDBC mHdbc;
    SQLRETURN retCode;
    void allocStatement();
};


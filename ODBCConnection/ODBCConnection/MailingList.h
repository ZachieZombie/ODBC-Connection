#pragma once
#include "stdafx.h"
#include "ODBCStoredProc.h"

class MailingList
{
public:
    MailingList();
    ~MailingList();
    void initialize();
    void reset();
    std::string getLists(std::string pList, std::string &pTo, std::string &pCC);
    bool runProcedure();
private:
    SQLCHAR        mToList[1000];
    SQLCHAR        mCCList[1000];
    ODBCStoredProc mMail;
    ODBCParameter  mTo;
    ODBCParameter  mCC;
    std::string    mList;
    std::string    mCall;
    std::string    mError;
    bool           mODBCIsConnected;
};


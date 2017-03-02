#include "stdafx.h"
#include "MailingList.h"


MailingList::MailingList()
{
    initialize();
}



MailingList::~MailingList()
{
}



void MailingList::initialize()
{
    if (!mMail.initialize("DEVMASSETS", "", ""))
    {
        mError.clear();
        mError = mMail.getLastError();
        mODBCIsConnected = false;
        return;
    }
    mODBCIsConnected = true;

    mTo.initialize(&mToList, sizeof(mToList), "Output", DBC_STRING);
    mCC.initialize(&mCCList, sizeof(mCCList), "Output", DBC_STRING);

    mMail.bindParameter(1, mTo);
    mMail.bindParameter(2, mCC);
}



std::string MailingList::getLists(std::string pList, std::string &pTo, std::string &pCC)
{
    if (!mODBCIsConnected)
    {
        return mError;
    }
    mList = pList;

    runProcedure();

    pTo.assign((char*)mToList);
    pCC.assign((char*)mToList);

    return "";
}



bool MailingList::runProcedure()
{
    mCall = "{call [dw].[dataloads].[dbo].[ustp_EmailManagement] ('";
    mCall += mList;
    mCall += "', ?, ?)}";

    if (!mMail.runProcedure(mCall))
    {
        mError.clear();
        mError = mMail.getLastError();
        return false;
    }

    return true;
}



void MailingList::reset()
{
    mToList[0] = '\0';
    mCCList[0] = '\0';

    mList.clear();
    mCall.clear();
    mError.clear();
}
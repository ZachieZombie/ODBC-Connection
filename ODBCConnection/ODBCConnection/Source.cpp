//Entry Point for the Application

#include "stdafx.h"
#include "ODBCConnection.h"
#include "ODBCRecordset.h"
#include "ODBCParameter.h"
#include "ODBCStoredProc.h"
#include "MailingList.h"

int main(int argc, char* argv[])
{
    //ODBCConnection con;
    //if (!con.open("DEVMASSETS", "", ""))
    //{
    //    con.close();
    //    return -1;
    //}

    //ODBCStoredProc spa = ODBCStoredProc(&con);

    ODBCStoredProc spa;
    if (!spa.initialize("DEVMASSETS", "", ""))
    {
        std::string error = spa.getLastError();
        return -1;
    }
    spa.setProcCall("{call [dw].[dataloads].[dbo].[ustp_EmailManagement] ('Admin: Machine Status', ?, ?)}");
    ODBCParameter ToList;
    ODBCParameter CCList;
    SQLCHAR ToBuffer[1000];
    SQLCHAR CCBuffer[1000];
    ToList.initialize(&ToBuffer, sizeof(ToBuffer), "Output", DBC_STRING);
    CCList.initialize(&CCBuffer, sizeof(CCBuffer), "Output", DBC_STRING);
    spa.bindParameter(1, ToList);
    spa.bindParameter(2, CCList);
    spa.runProcedure();


    //ODBCStoredProc spb = ODBCStoredProc(&con);
    ODBCStoredProc spb;
    if (!spb.initialize("DEVMASSETS", "", ""))
    {
        std::string error = spb.getLastError();
        return -2;
    }
    ODBCParameter eq;
    ODBCParameter id;
    SQLDOUBLE eBuffer = 0;
    int idBuffer = 57;
    eq.initialize(&eBuffer,  sizeof(eBuffer), "Output", DBC_DOUBLE);
    id.initialize(&idBuffer, sizeof(idBuffer), "Input", DBC_INTEGER);
    spb.bindParameter(1, id);
    spb.bindParameter(2, eq);
    spb.runProcedure("{call [massets].[dbo].[ustp_ISEquityPercent] (?,?)}");

    MailingList list;
    std::string To;
    std::string CC;
    std::string MLError = list.getLists("Admin: Machine Status", To, CC);

    return 0;
    //ODBCRecordset records = ODBCRecordset(&con);
    //SQLCHAR ToList [1000];
    //SQLLEN cbToList = sizeof(ToList);
    //SQLCHAR CCList [1000];
    //SQLLEN cbCCList = sizeof(CCList);
    //SQLRETURN retCode;
    //retCode = SQLBindParameter(records.mHstmt, 
    //                             1, 
    //                             SQL_PARAM_OUTPUT, 
    //                             SQL_C_CHAR, 
    //                             SQL_VARCHAR,
    //                             cbToList,
    //                             0, 
    //                             &ToList, 
    //                             cbToList,
    //                             0);
    //retCode = SQLBindParameter(records.mHstmt,
    //                             2,
    //                             SQL_PARAM_OUTPUT,
    //                             SQL_C_CHAR,
    //                             SQL_VARCHAR,
    //                             cbCCList,
    //                             0,
    //                             &CCList,
    //                             cbCCList,
    //                             0);


    //SQLRETURN retCode;
    //    retCode = SQLBindParameter(records.mHstmt,
    //                                1,
    //                                ToList.getParamType(),
    //                                ToList.getAppDataType(),
    //                                SQL_VARCHAR,
    //                                ToList.getDataLength(),
    //                                0,
    //                                ToList.getDataAddress(),
    //                                ToList.getDataLength(),
    //                                0);
    //    retCode = SQLBindParameter(records.mHstmt,
    //                                2,
    //                                CCList.getParamType(),
    //                                CCList.getAppDataType(),
    //                                SQL_VARCHAR,
    //                                sizeof(CCBuffer),
    //                                0,
    //                                &CCBuffer,
    //                                sizeof(CCBuffer),
    //                                0);
    //records.open("{call [dw].[dataloads].[dbo].[ustp_EmailManagement] ('Admin: Machine Status', ?, ?)}");
    //records.open("{? = call [dbo].[ustp_ISEquityPercent] (?)}");
    //records.open("Select * from MAUser");
    //exec [dbo].[ustp_ISEquityPercent] @id = 363

    //char dataStr[256];
    //wchar_t wDataStr[256];

    //while (!records.isEoF())
    //{
    //    int niType;
    //    long lData;
    //    double dData;
    //    int niLen;
    //    struct tm time;

    //    records.getFieldAttributes(0, NULL, niType, niLen);

    //    memset(dataStr, 1, sizeof(dataStr));
    //    records.getFieldValue(1, dataStr);

    //    records.moveNext();
    //}
}
//Entry Point for the Application

#include "stdafx.h"
#include "ODBCConnection.h"
#include "ODBCRecordset.h"
#include "ODBCParameter.h"

int main(int argc, char* argv[])
{
    ODBCConnection con;
    if (!con.open("DEVMASSETS", "", ""))
    {
        con.close();
        return -1;
    }
    ODBCRecordset records = ODBCRecordset(&con);

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

    ODBCParameter ToList;
    ODBCParameter CCList;
    SQLCHAR ToBuffer[1000];
    SQLCHAR CCBuffer[1000];

    ToList.initialize(&ToBuffer, sizeof(ToBuffer), "Output", DBC_STRING);
    CCList.initialize(&CCBuffer, sizeof(CCBuffer), "Output", DBC_STRING);

    SQLRETURN retCode;

        retCode = SQLBindParameter(records.mHstmt,
                                    1,
                                    ToList.getParamType(),
                                    ToList.getAppDataType(),
                                    SQL_VARCHAR,
                                    ToList.getDataLength(),
                                    0,
                                    ToList.getDataAddress(),
                                    ToList.getDataLength(),
                                    0);

        retCode = SQLBindParameter(records.mHstmt,
                                    2,
                                    CCList.getParamType(),
                                    CCList.getAppDataType(),
                                    SQL_VARCHAR,
                                    sizeof(CCBuffer),
                                    0,
                                    &CCBuffer,
                                    sizeof(CCBuffer),
                                    0);



    records.open("{call [dw].[dataloads].[dbo].[ustp_EmailManagement] ('Admin: Machine Status', ?, ?)}");
    //records.open("Select * from MAUser");

    char dataStr[256];
    wchar_t wDataStr[256];

    while (!records.isEoF())
    {
        int niType;
        long lData;
        double dData;
        int niLen;
        struct tm time;

        records.getFieldAttributes(0, NULL, niType, niLen);

        memset(dataStr, 1, sizeof(dataStr));
        records.getFieldValue(1, dataStr);

        records.moveNext();
    }


    return 0;
}
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
    ODBCParameter paramA = ODBCParameter(DBC_STRING, 256, "Output");
    ODBCParameter paramB = ODBCParameter(DBC_STRING, 256, "Output");

    records.bindParameter(1, paramA);
    records.bindParameter(2, paramB);
    records.open("{call dw.dataloads.dbo.ustp_EmailManagement('Admin: Machine Status', ?, ?)");
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
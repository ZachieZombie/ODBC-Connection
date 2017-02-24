#include "stdafx.h"
#include "ODBCRecordset.h"


ODBCRecordset::ODBCRecordset(ODBCConnection* pDb)
{
    mHdbc  = pDb->mHdbc;
    mHstmt = NULL;
    mIsEOF = FALSE;
    mIsBOF = FALSE;
    allocStatement();
}


ODBCRecordset::~ODBCRecordset()
{
    close();
    mHstmt = NULL;
    mIsEOF = FALSE;
    mIsBOF = FALSE;
}



void ODBCRecordset::allocStatement()
{
    SQLAllocHandle(SQL_HANDLE_STMT, mHdbc, &mHstmt);
}



bool ODBCRecordset::isEoF()
{
    return mIsEOF;
}



bool ODBCRecordset::isBoF()
{
    return mIsBOF;
}



bool ODBCRecordset::open(char* ptrSqlStr)
{
    retCode = SQLExecDirect(mHstmt, (SQLCHAR*)ptrSqlStr, SQL_NTS);

    if (!((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO)))
    {
        return false;
    }

    SQLFetch(mHstmt);
    return true;
}



bool ODBCRecordset::getFieldValue(int niField, wchar_t* ptrDataStr)
{
    SQLINTEGER value;
    int niLength = getFieldLength(niField) + 1;

    retCode = SQLGetData(mHstmt, (SQLUSMALLINT)(niField + 1), SQL_C_WCHAR,
                         ptrDataStr, niLength, &value);
    
    return ((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO));
}



bool ODBCRecordset::getFieldValue(char* ptrFieldNameStr, wchar_t* ptrDataStr)
{
    return getFieldValue(getFieldIndex(ptrFieldNameStr), ptrDataStr);
}



bool ODBCRecordset::getFieldValue(int niField, char* ptrDataStr)
{
    SQLINTEGER value;
    int niLength = getFieldLength(niField) + 1;

    retCode = SQLGetData(mHstmt, (SQLUSMALLINT)(niField + 1), SQL_C_CHAR,
        ptrDataStr, niLength, &value);

    return ((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO));
}



bool ODBCRecordset::getFieldValue(char* ptrFieldNameStr, char* ptrDataStr)
{
    return getFieldValue(getFieldIndex(ptrFieldNameStr), ptrDataStr);
}



bool ODBCRecordset::getFieldValue(int niField, long* lDataPtr)
{
    SQLINTEGER value;
    int niLength = getFieldLength(niField) + 1;

    retCode = SQLGetData(mHstmt, (SQLUSMALLINT)(niField + 1), SQL_C_LONG,
                         lDataPtr, niLength, &value);

    return ((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO));
}



bool ODBCRecordset::getFieldValue(char* ptrFieldNameStr, long* lDataPtr)
{
    return getFieldValue(getFieldIndex(ptrFieldNameStr), lDataPtr);
}



bool ODBCRecordset::getFieldValue(int niField, double* dDataPtr)
{
    SQLINTEGER value;
    int niLength = getFieldLength(niField) + 1;

    retCode = SQLGetData(mHstmt, (SQLUSMALLINT)(niField + 1), SQL_C_DOUBLE,
        dDataPtr, niLength, &value);

    return ((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO));
}



bool ODBCRecordset::getFieldValue(char* ptrFieldNameStr, double* dDataPtr)
{
    return getFieldValue(getFieldIndex(ptrFieldNameStr), dDataPtr);
}



bool ODBCRecordset::getFieldValue(int niField, struct tm* time)
{
    SQLINTEGER value;
    int niLength = getFieldLength(niField) + 1;
    SQL_TIMESTAMP_STRUCT* timeSQL = new SQL_TIMESTAMP_STRUCT;

    retCode = SQLGetData(mHstmt, (SQLUSMALLINT)(niField + 1), SQL_C_TYPE_TIMESTAMP,
        timeSQL, niLength, &value);

    if (!((retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO)))
    {
        return false;
    }

    time->tm_year = timeSQL->year;
    time->tm_mon = timeSQL->month - 1; //Adjust to make January 0
    time->tm_mday = timeSQL->day;
    time->tm_hour = timeSQL->hour;
    time->tm_min = timeSQL->minute;
    time->tm_sec = timeSQL->second;
    return true;
}



bool ODBCRecordset::getFieldValue(char* ptrFieldNameStr, struct tm* time)
{
    return getFieldValue(getFieldIndex(ptrFieldNameStr), time);
}



bool ODBCRecordset::getFieldName(int niField, char* ptrFieldNameStr)
{
    int niType;
    int niLength;

    return getFieldAttributes(niField, ptrFieldNameStr, niType, niLength);
}



int  ODBCRecordset::getFieldIndex(char* ptrFieldNameStr)
{
    SQLSMALLINT niCols;
    int niCol = 1;
    char colNameStr[MAX_COL_NAME_LEN];
    SQLSMALLINT niColNameLen;
    SQLSMALLINT iSQLType;
    SQLSMALLINT iScale;
    SQLSMALLINT iNullable;
    SQLUINTEGER iColDef;

    SQLNumResultCols(mHstmt, &niCols);

    while (niCol < niCols)
    {
        memset(colNameStr, 0, 32 * sizeof(char));

        SQLDescribeCol(mHstmt, niCol, (SQLCHAR*)colNameStr, MAX_COL_NAME_LEN,
                      &niColNameLen, &iSQLType, &iColDef, &iScale, &iNullable);

        if (strcmp(colNameStr, ptrFieldNameStr) == 0)
        {
            return niCol - 1;
        }

        niCol++;
    }
    return -1;
}



bool ODBCRecordset::moveFirst()
{
    retCode = SQLFetchScroll(mHstmt, SQL_FETCH_FIRST, 0);

    mIsBOF = (retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO);

    if (mIsBOF)
    {
        mIsEOF = false;
    }

    return mIsBOF;
}



bool ODBCRecordset::moveNext()
{
    retCode = SQLFetchScroll(mHstmt, SQL_FETCH_NEXT, 0);

    mIsEOF = (retCode == SQL_NO_DATA);
    mIsBOF = false;

    return (retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO);
}



bool ODBCRecordset::movePrev()
{
    retCode = SQLFetchScroll(mHstmt, SQL_FETCH_PRIOR, 0);

    mIsBOF = (retCode == SQL_NO_DATA);
    mIsEOF = false;

    return (retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO);
}



bool ODBCRecordset::moveLast()
{
    retCode = SQLFetchScroll(mHstmt, SQL_FETCH_LAST, 0);

    mIsEOF = (retCode == SQL_SUCCESS) || (retCode == SQL_SUCCESS_WITH_INFO);

    if (mIsEOF)
    {
        mIsBOF = false;
    }

    return mIsEOF;
}



void ODBCRecordset::close()
{
    if (mHstmt != NULL)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, mHstmt);
    }

    mHstmt = NULL;
}



long ODBCRecordset::getFieldLength(int niField)
{
    SQLSMALLINT iSQLType;
    SQLSMALLINT iScale;
    SQLSMALLINT iNullable;
    SQLUINTEGER iColDef;

    SQLDescribeCol(mHstmt, niField + 1, NULL, 0, 0, &iSQLType, &iColDef, 
                   &iScale, &iNullable);

    return iColDef;
}



bool ODBCRecordset::getFieldAttributes(int niField, char* ptrFieldNameStr, int& niType,
                                       int& niLength)
{
    SQLSMALLINT iColNameLen;
    SQLSMALLINT iSQLType;
    SQLSMALLINT iScale;
    SQLSMALLINT iNullable;
    SQLUINTEGER iColDef;

    retCode = SQLDescribeCol(mHstmt, niField + 1, (SQLCHAR*)ptrFieldNameStr, MAX_COL_NAME_LEN,
                             &iColNameLen, &iSQLType, &iColDef, &iScale, &iNullable);

    niType = iSQLType;
    niLength = iColDef;

    return ((retCode == SQL_SUCCESS) || (SQL_SUCCESS_WITH_INFO));
}



int ODBCRecordset::getFieldCount()
{
    SQLSMALLINT niFieldCount = 0;
    SQLNumResultCols(mHstmt, &niFieldCount);

    return niFieldCount;
}



bool ODBCRecordset::bindParameter(unsigned int pIndex, ODBCParameter pParam)
{
    const unsigned int width = pParam.getWidth();
    const int bindSize = pParam.getBindSize();
    char* bindAddress = pParam.getBindAddress();
    SQLINTEGER* indAddress = pParam.getIndAddress();

    switch (pParam.getType())
    {
        case DBC_STRING:
            if (bindSize < 256)
            {
                pParam.setWidth(256);
            }

            if (bindSize > 2048)
            {
                retCode = SQLBindParameter(mHstmt, pIndex,
                                           SQL_PARAM_OUTPUT, SQL_C_CHAR,
                                           SQL_LONGVARCHAR, bindSize, 0,
                                           bindAddress, bindSize, indAddress);
            }
            else
            {
                retCode = SQLBindParameter(mHstmt, pIndex,
                                           SQL_PARAM_OUTPUT, SQL_C_CHAR,
                                           SQL_LONGVARCHAR, bindSize, 0,
                                           bindAddress, bindSize, indAddress);
            }
            break;
        case DBC_INTEGER:
            break;
    }
    return true;
}
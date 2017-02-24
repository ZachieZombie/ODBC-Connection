#pragma once
#include "stdafx.h"
#include "ODBCConnection.h"
#include "ODBCParameter.h"

class ODBCRecordset
{
    public:
        enum datatypeEnum
        {
            typeChar =          SQL_CHAR,
            typeVarChar =       SQL_VARCHAR,
            typeLongVarChar =   SQL_LONGVARCHAR,
            typeWChar =         SQL_WCHAR,
            typeWVarChar =      SQL_WVARCHAR,
            typeWLongVarChar =  SQL_WLONGVARCHAR,
            typeDecimal =       SQL_DECIMAL,
            typeNumeric =       SQL_NUMERIC,
            typeSmallint =      SQL_SMALLINT,
            typeInteger =       SQL_INTEGER,
            typeReal =          SQL_REAL,
            typeFloat =         SQL_FLOAT,
            typeDouble =        SQL_DOUBLE,
            typeBit =           SQL_BIT,
            typeTinyint =       SQL_TINYINT,
            typeBigInt =        SQL_BIGINT,
            typeBinary =        SQL_BINARY,
            typeVarBinary =     SQL_VARBINARY,
            typeLongVarBinary = SQL_LONGVARBINARY,
            typeDate =          SQL_TYPE_DATE,
            typeTime =          SQL_TYPE_TIME,
            typeTimeStamp =     SQL_TYPE_TIMESTAMP,
            typeIntervalMonth = SQL_INTERVAL_MONTH,
            typeIntervalYear =  SQL_INTERVAL_YEAR,
            typeIntervalYearToMonth =    SQL_INTERVAL_YEAR_TO_MONTH,
            typeIntervalDay =            SQL_INTERVAL_DAY,
            typeIntervalHour =           SQL_INTERVAL_HOUR,
            typeIntervalMinute =         SQL_INTERVAL_MINUTE,
            typeIntervalSecond =         SQL_INTERVAL_SECOND,
            typeIntervalDayToHour =      SQL_INTERVAL_DAY_TO_HOUR,
            typeIntervalDayToMinute =    SQL_INTERVAL_DAY_TO_MINUTE,
            typeIntervalDayToSecond =    SQL_INTERVAL_DAY_TO_SECOND,
            typeIntervalHourToMinute =   SQL_INTERVAL_HOUR_TO_MINUTE,
            typeIntervalHourToSecond =   SQL_INTERVAL_HOUR_TO_SECOND,
            typeIntervalMinuteToSecond = SQL_INTERVAL_MINUTE_TO_SECOND,
            typeGUID = SQL_GUID
        };

        ODBCRecordset(ODBCConnection* pdb);
        ~ODBCRecordset();
        bool open(char* ptrSqlStr);
        long getFieldLength(int niField);
        int  getFieldIndex(char* ptrFieldName);
        bool getFieldName(int niField, char* ptrFieldNameStr);
        bool getFieldAttributes(int niField, char* ptrFieldNameStr, int& niType, int& niLength);
        int  getFieldCount();
        bool getFieldValue(int niField, char* ptrDataStr);
        bool getFieldValue(char* ptrFieldNameStr, char* ptrDataStr);
        bool getFieldValue(int niField, wchar_t* ptrDataStr);
        bool getFieldValue(char* ptrFieldNameStr, wchar_t* ptrDataStr);
        bool getFieldValue(int niField, long* lDataPtr);
        bool getFieldValue(char* ptrFieldNameStr, long* lDataPtr);
        bool getFieldValue(int niField, double* dDataPtr);
        bool getFieldValue(char* ptrFieldNameStr, double* dDataPtr);
        bool getFieldValue(int niField, struct tm* time);
        bool getFieldValue(char* ptrFieldNameStr, struct tm* time);
        bool bindParameter(unsigned int pIndex, ODBCParameter pParam);
        bool moveFirst();
        bool moveNext();
        bool movePrev();
        bool moveLast();
        bool isEoF();
        bool isBoF();
        void close();
    protected:
        bool mIsEOF;
        bool mIsBOF;
        SQLHDBC mHdbc;
        SQLHSTMT mHstmt;
        SQLRETURN retCode;
        void allocStatement();
};


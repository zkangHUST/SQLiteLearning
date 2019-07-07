#include<stdio.h>
#include "sqlite3.h"
int main()
{
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    int err;
    err = sqlite3_open("test.db", &db);
    if (err != SQLITE_OK) {
        sqlite3_close(db);
        fprintf(stderr, "could not open test DB\n");
        return err;
    }
    err = sqlite3_prepare(db, "select SID from Students order by SID", -1, &stmt, 0);
    if (err != SQLITE_OK) {
        fprintf(stderr, "could not execute select\n");
        return err;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int i = sqlite3_column_int(stmt, 0);
        printf("SID = %d\n", i);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return SQLITE_OK;
}
#include"sqlite3.h"
#include<stdio.h>
static int callback(void* notused, int argc, char** agrv, char** colname);
int main(int argc, char **argv)
{
    sqlite3* db = NULL;
    char* zErrMsg = NULL;   
    int rc;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s DATABASE-FILE-NAME SQL-STATEMENT\n", argv[0]);
        return -1;
    }
    rc = sqlite3_open(argv[1], &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -2;
    }
    rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    return rc;
}

int callback(void* notused, int argc, char** argv, char** colname)
{
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", colname[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

#include<stdio.h>
#include <pthread.h>
#include"sqlite3.h"
void* myInsert(void *arg);
int main()
{
    pthread_t t[10];
    int i;
    for (i=0; i < 10; i++) {
        pthread_create(&t[i], 0, myInsert, (void*)i); /* Pass the value of i */
    }
    /* wait for all threads to finish */
    for (i=0; i<10; i++) {
        pthread_join(t[i], 0);
    }
        return 0;
}

void* myInsert(void *arg)
{
    sqlite3* db = 0;
    sqlite3_stmt* stmt = 0;
    int val = (int)arg;
    char SQL[100];
    int rc;
    rc = sqlite3_open("test.db", &db); /* Open a database named MyDB */
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Thread[%d] fails to open the database\n", val);
        goto errorRet;
    }
        /*   Create the SQL string. If you were using string values,
        you would need to use sqlite3_prepare() and sqlite3_bind_*
        to avoid an SQL injection vulnerability. However %d
        guarantees an integer value, so this use of sprintf is
        safe.
        */
        sprintf(SQL, "insert or replace into Students values(%d)", val * 10);
        /* Prepare the insert statement */
        rc = sqlite3_prepare(db, SQL, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Thread[%d] fails to prepare SQL: %s ->return code %d\n", val, SQL, rc);
            goto errorRet;
        }
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Thread[%d] fails to execute SQL: %s -> return code %d\n", val, SQL, rc);
        } else {
            printf("Thread[%d] successfully executes SQL: %s\n", val, SQL);
        }
        errorRet:
            sqlite3_close(db);
            return (void*)rc;
}


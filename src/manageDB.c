#include "header.h"

int CreateTable()
{
    sqlite3 *db;
    char *Error = 0;
    int rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    const char *userTable = "CREATE TABLE IF NOT EXISTS users ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "uname TEXT NOT NULL UNIQUE,"
                            "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, userTable, NULL, 0, &Error);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", Error);
        sqlite3_free(Error);
        sqlite3_close(db);
        return 1;
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }

    const char *recordsTable = "CREATE TABLE IF NOT EXISTS records ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "userId INTEGER,"
                               "name TEXT NOT NULL,"
                               "country TEXT NOT NULL,"
                               "phone TEXT,"
                               "accountType TEXT NOT NULL,"
                               "accountNbr INTEGER NOT NULL UNIQUE,"
                               "amount INTEGER,"
                               "time TEXT,"
                               "FOREIGN KEY (userId) REFERENCES users(id) ON DELETE CASCADE);";

    rc = sqlite3_exec(db, recordsTable, NULL, 0, &Error);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", Error);
        sqlite3_free(Error);
        sqlite3_close(db);
        return 1;
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }

    sqlite3_close(db);
    return 0;
}
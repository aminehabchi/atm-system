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
                               "amount DOUBLE,"
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
int getPassword(struct User u)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, uname, password FROM users where uname=?;";

    // Open database
    if (sqlite3_open("database.db", &db) != SQLITE_OK)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, u.name, -1, SQLITE_STATIC);
    // Execute statement and loop through results
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *pass = (const char *)sqlite3_column_text(stmt, 2);

        if (strcmp(u.password, pass) == 0)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return id;
        }
    }

    // Finalize statement and close database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
}
int checkUserIfExist(sqlite3 *db, char name[50])
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, uname, password FROM users where uname=?;";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    // Execute statement and loop through results
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *pass = (const char *)sqlite3_column_text(stmt, 2);
        sqlite3_finalize(stmt);
        return id;
    }
    sqlite3_finalize(stmt);
    return -1;
}
int checkAccontIfExist(sqlite3 *db, int accountNbr)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT accountNbr FROM records where accountNbr=?;";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    // sqlite3_bind_text(stmt, 1, accountNbr, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 1, accountNbr);
    // Execute statement and loop through results
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }
    sqlite3_finalize(stmt);
    return -1;
}
int InsertAccInfo(sqlite3 *db, struct User u, struct Record r)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO records(userId,name,country,phone,accountType,accountNbr,time,amount) VALUES (?,?,?,?,?,?,?,?);";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, u.id);
    sqlite3_bind_text(stmt, 2, u.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, r.phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, r.accountType, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, r.accountNbr);
    sqlite3_bind_text(stmt, 7, r.time, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 8, r.amount);

    while (sqlite3_step(stmt) == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}
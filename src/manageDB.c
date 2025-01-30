#include "header.h"

int CreateTable()
{
    sqlite3 *db;
    char *Error = 0;
    int rc = sqlite3_open("../data/database.db", &db);
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

        return 1;
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }

    return 0;
}
int getPassword(struct User u, sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, uname, password FROM users where uname=?;";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));

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
            return id;
        }
    }

    // Finalize statement and close database
    sqlite3_finalize(stmt);
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

    if (sqlite3_step(stmt) == SQLITE_DONE || sqlite3_step(stmt) == SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return -1;
}
int checkAccountIfExist(sqlite3 *db, int userId, int accountNbr)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM records WHERE userId=? AND accountNbr=?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1; // Error during preparation
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, accountNbr);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        return 2; // account exists
    }
    else if (rc == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 1; // account does not exist
    }
    else
    {
        printf("Error during step: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        exit(1);
        return -1; // Execution error
    }
}

int InsertAccInfo(sqlite3 *db, struct User u, struct Record r)
{
    sqlite3_stmt *stmt;
    char *sql = "INSERT INTO records(userId,name,country,phone,accountType,accountNbr,time,amount) VALUES (?,?,?,?,?,?,?,?);";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, u.id);
    sqlite3_bind_text(stmt, 2, u.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, r.phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, r.accountType, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, r.accountNbr);
    sqlite3_bind_text(stmt, 7, r.time, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 8, r.amount);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}
int registerInfo(char a[50], char pass[50], sqlite3 *db)
{
    if (checkUserIfExist(db, a) == 0)
    {
        sqlite3_stmt *stmt;
        const char *sql = "INSERT INTO users (uname, password) VALUES (?, ?);";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));

            return -1;
        }
        sqlite3_bind_text(stmt, 1, a, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Insertion failed: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("User inserted successfully.\n");
        }

        sqlite3_finalize(stmt);

        return checkUserIfExist(db, a);
    }
    return -1;
}

double getAmount(int id, int accountId, sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT amount FROM records WHERE accountNbr=? AND userId=?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, accountId);
    sqlite3_bind_int(stmt, 2, id);

    double accntAmount = 0.0;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        accntAmount = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return accntAmount;
}

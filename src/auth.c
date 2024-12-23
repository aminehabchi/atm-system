#include "header.h"

char *USERS = "./data/users.txt";
void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

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

int registerMenu(char a[50], char pass[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User to Register:");
    scanf("%s", a);
    while (getchar() != '\n')
        ;
    printf("\n\n\n\n\n\t\t\t\tEnter the password to Register:");
    scanf("%s", pass);
     while (getchar() != '\n')
        ;
    sqlite3 *db;
    if (sqlite3_open("database.db", &db) != SQLITE_OK)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    if (checkUserIfExist(db, a) == -1)
    {
        sqlite3_stmt *stmt;
        const char *sql = "INSERT INTO users (uname, password) VALUES (?, ?);";
        // Prepare SELECT statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
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
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        
        return checkUserIfExist(db, a);
    }

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
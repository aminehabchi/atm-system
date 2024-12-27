#include "header.h"

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;
    char *buffer = NULL;
    system("clear");
    while (buffer == NULL)
    {
        printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(a, buffer);
    free(buffer);
    buffer = NULL;
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

    while (buffer == NULL)
    {
        printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
        buffer = scanString(50, isPrintble);
    }
    strcpy(pass, buffer);
    free(buffer);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

int registerMenu(char a[50], char pass[50], sqlite3 *db)
{
    char *buffer = NULL;
    //***************************** */
    while (buffer == NULL)
    {
        system("clear");
        printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User to Register:");
        // scanf("%s", a);
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(a, buffer);
    free(buffer);
    buffer = NULL;
    /******************************************* */

    while (buffer == NULL)
    {
        printf("\n\n\n\n\n\t\t\t\tEnter the password to Register:");
        buffer = scanString(50, isPrintble);
    }
    strcpy(pass, buffer);
    free(buffer);

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

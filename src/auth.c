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

    return registerInfo(a, pass, db);
}


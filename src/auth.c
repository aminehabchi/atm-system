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
    FILE *fp;
    struct User userChecker;
    fp = fopen("../data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        printf("%d\n", userChecker.id);
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            printf("%s\n", userChecker.password);

            return userChecker.id;
        }
    }

    fclose(fp);
    return -1;
}

int registerMenu(char a[50], char pass[50])
{
    FILE *fp;
    fp = fopen("../data/users.txt", "a+");

    system("clear");

    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User to Register:");
    scanf("%s", a);
    int id = checkInfoIfExist(fp, a);
    if (id == -1)
    {
        return -1;
    }

    printf("\n\n\n\n\n\t\t\t\tEnter the password to Register:");
    scanf("%s", pass);

    saveInfo(fp, a, pass, id);

    fclose(fp);
    return id;
}
int checkInfoIfExist(FILE *ptr, char a[50])
{
    int id = 0;

    struct User userChecker;
    while (fscanf(ptr, "%d %49s %49s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, a) == 0)
        {
            return -1;
        }
        id++;
    }
    return id;
}
void saveInfo(FILE *ptr, char a[50], char pass[50], int id)
{
    fprintf(ptr, "%d %s %s\n", id, a, pass);
};

#include "header.h"
int isAlphaNemric(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if (!isalnum(s[i]) && s[i] != '\n')
        {
            return -1;
        }
        i++;
    }
    return 1;
}
int isPrintble(char *s)
{
    int i;
    while (s[i] != '\0')
    {
        if (s[i] >= ' ' && s[i] <= 'z' && s[i] != '\n')
        {
            i++;
        }
        else
        {
            return -1;
        }
    }
    return 1;
}

int isNums(char *s)
{
    int i;
    while (s[i] != '\0')
    {
        if (s[i] >= '0' && s[i] <= '9' && s[i] != '\n')
        {
            i++;
        }
        else
        {
            return -1;
        }
    }
    return 1;
}

char *scanString(int maxlen, int f(char *s))
{
    char *line = NULL;
    size_t len = 0; // Buffer size
    size_t read;    // Number of characters read

    read = getline(&line, &len, stdin);

    if (read == -1 || read > maxlen || isAlphaNemric(line) == -1)
    {
        free(line);
        return NULL;
    }

    return line;
}
int indexOfChar(char *str, char ch)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ch)
        {
            return i;
        }
    }
    return -1;
}
int scanInt()
{
    char *line = NULL;
    size_t len = 0; // Buffer size
    size_t read;    // Number of characters read

    read = getline(&line, &len, stdin);

    // 10 is length(2147483647)
    if (read == -1 || read > 10 || isNums(line) == -1)
    {
        free(line);
        return 0;
    }

    int i = indexOfChar(line, '\n');
    if (i != -1)
    {
        line[i] = '\0';
    }
    int r = atoi(line);
    return r;
}

int isDouble(char *s)
{
    int i = 0;
    int p = 0;

    while (s[i] != '\0')
    {
        if (s[i] == '.')
        {
            p++;
            if (p > 1)
            {
                return -1; // More than one decimal point
            }
        }
        else if (s[i] >= '0' && s[i] <= '9')
        {
          
        }
        else
        {
            return -1; // Invalid character
        }
        i++;
    }

    return 1;
}

double scanDouble()
{
    char *line = NULL;
    size_t len = 0; // Buffer size
    size_t read;    // Number of characters read
    read = getline(&line, &len, stdin);
    // 10 is length(2147483647)
    if (read == -1 || read > 10)
    {
        free(line);
        return 0;
    }
    int i = indexOfChar(line, '\n');
    if (i != -1)
    {
        line[i] = '\0';
    }
    if (isDouble(line) == -1)
    {
        free(line);
        return 0;
    }
    return atof(line);
}

void printAcountInfo(struct Record r)
{
    printf("Account number:%d\n", r.accountNbr);
    printf("Deposit Date:%s\n", r.time);
    printf("country:%s\n", r.country);
    printf("Phone number:%s\n", r.phone);
    printf("Amountdeposited: $%f\n", r.amount);
    printf("Type Of Account:%s\n\n", r.accountType);
}

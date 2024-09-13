#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wordlength(char *str)
{
    int len = 0;
    while (str != '\0')
    {
        len++;
        str++;
    }
    return len;
}

int main()
{
    FILE *wordlist;
    if ((wordlist = fopen("", "r")) == NULL)
    {
        printf("Error opening file");
    }
    char word[16];
    while (fgets(word, 16, wordlist) != NULL)
    {
    }
}
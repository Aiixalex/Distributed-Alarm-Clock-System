#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeToFile(char *fileName, char *str)
{
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open export file.\n");
        exit(EXIT_FAILURE);
    }

    // Write to data to the file using fprintf()
    int charWritten = fprintf(pFile, "%s", str);
    if (charWritten <= 0)
    {
        printf("ERROR WRITING DATA");
        exit(EXIT_FAILURE);
    }

    fclose(pFile);
}

void readFromFileToScreen(char *fileName, char *buf, int buf_size)
{
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Read string (line)
    fgets(buf, buf_size, pFile);

    // Close
    fclose(pFile);
}

char* concatStrs(char* str1, char* str2, char* str3, char* res)
{
    strcpy(res, str1);
    strcat(res, str2);
    strcat(res, str3);
    return res;
}